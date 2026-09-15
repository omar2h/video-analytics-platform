#include <vap/analytics/analytics_session.hpp>

#include "analytics_mailbox.hpp"
#include "analytics_result_mailbox.hpp"
#include "analytics_worker.hpp"

#include <QMetaObject>
#include <QThread>
#include <QPointer>

#include <exception>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <utility>
#include <limits>

namespace vap
{

class AnalyticsSession::Impl
{
public:
    Impl(
        AnalyticsSession& owner,
        AnalyticsSession::DetectorFactory detectorFactory)
        : m_owner(owner)
        , m_detectorFactory(std::move(detectorFactory))
    {
        if (!m_detectorFactory)
        {
            throw std::invalid_argument(
                "Analytics requires a detector factory.");
        }

        m_worker = std::jthread(
            [this](std::stop_token stopToken)
            {
                run(stopToken);
            });
    }

    ~Impl()
    {
        m_worker.request_stop();

        if (m_worker.joinable())
            m_worker.join();
    }

    std::uint64_t enable(std::uint64_t streamRunId);
    void disable();

    [[nodiscard]]
    bool isEnabled() const;

    [[nodiscard]]
    bool submitFrame(AnalyticsFrame frame);

    [[nodiscard]]
    std::optional<AnalyticsResult> takeLatestResult();


private:
    void run(std::stop_token stopToken);

    void publishResult(AnalyticsResult result);

    void reportFailure(
        AnalyticsFrameIdentity identity,
        QString message);

    // Caller must hold m_stateMutex.
    [[nodiscard]]
    bool accepts(
        const AnalyticsFrameIdentity& identity) const noexcept
    {
        return m_enabled
               && identity.streamRunId == m_streamRunId
               && identity.analyticsGeneration == m_generation;
    }

private:
    AnalyticsSession& m_owner;
    AnalyticsSession::DetectorFactory m_detectorFactory;

    mutable std::mutex m_stateMutex;

    bool m_enabled{false};
    std::uint64_t m_streamRunId{};
    std::uint64_t m_generation{};

    detail::AnalyticsFrameMailbox m_frames;
    detail::AnalyticsResultMailbox m_results;

    // Declared last so it is destroyed before the state it uses.
    std::jthread m_worker;
};

std::uint64_t AnalyticsSession::Impl::enable(std::uint64_t streamRunId)
{
    Q_ASSERT(QThread::currentThread() == m_owner.thread());

    std::uint64_t generation;
    bool enabledChanged;

    {
        std::lock_guard lock(m_stateMutex);

        if (m_generation ==
            std::numeric_limits<std::uint64_t>::max())
        {
            throw std::overflow_error(
                "Analytics generation counter exhausted.");
        }

        generation = ++m_generation;
        m_streamRunId = streamRunId;

        enabledChanged = !m_enabled;
        m_enabled = true;

        m_frames.clear();
        m_results.clear();
    }

    if (enabledChanged)
        emit m_owner.enabledChanged();

    return generation;
}
void AnalyticsSession::Impl::disable()
{
    Q_ASSERT(QThread::currentThread() == m_owner.thread());

    bool enabledChanged;

    {
        std::lock_guard lock(m_stateMutex);

        enabledChanged = m_enabled;
        m_enabled = false;

        m_frames.clear();
        m_results.clear();
    }

    if (enabledChanged)
        emit m_owner.enabledChanged();
}

[[nodiscard]]
bool AnalyticsSession::Impl::isEnabled() const
{
    Q_ASSERT(QThread::currentThread() == m_owner.thread());

    std::lock_guard lock(m_stateMutex);
    return m_enabled;
}

[[nodiscard]]
bool AnalyticsSession::Impl::submitFrame(AnalyticsFrame frame)
{
    if (frame.image.isNull() ||
        frame.image.format() != QImage::Format_RGB888)
    {
        return false;
    }

    std::lock_guard lock(m_stateMutex);

    if (!accepts(frame.identity))
        return false;

    m_frames.submit(std::move(frame));
    return true;
}

[[nodiscard]]
std::optional<AnalyticsResult> AnalyticsSession::Impl::takeLatestResult()
{
    Q_ASSERT(QThread::currentThread() == m_owner.thread());

    std::lock_guard lock(m_stateMutex);

    auto result = m_results.takeLatest();

    if (!result || !accepts(result->identity))
        return std::nullopt;

    return result;
}

void AnalyticsSession::Impl::run(std::stop_token stopToken)
{
    // Created, used, and destroyed on this worker thread.
    std::unique_ptr<IObjectDetector> detector;

    while (!stopToken.stop_requested())
    {
        auto frame = m_frames.waitAndTake(stopToken);

        if (!frame)
            break;

        {
            std::lock_guard lock(m_stateMutex);

            if (!accepts(frame->identity))
                continue;
        }

        try
        {
            if (!detector)
            {
                detector = m_detectorFactory();

                if (!detector)
                {
                    throw std::runtime_error(
                        "Detector factory returned null.");
                }
            }

            if (stopToken.stop_requested())
                break;

            // Model loading may have taken time. Check again.
            {
                std::lock_guard lock(m_stateMutex);

                if (!accepts(frame->identity))
                    continue;
            }

            auto result =
                detail::processAnalyticsFrame(*frame, *detector);

            if (stopToken.stop_requested())
                break;

            publishResult(std::move(result));
        }
        catch (const std::exception& error)
        {
            detector.reset();

            reportFailure(
                frame->identity,
                QString::fromUtf8(error.what()));
        }
        catch (...)
        {
            detector.reset();

            reportFailure(
                frame->identity,
                QStringLiteral("Unknown analytics failure."));
        }
    }
}

void AnalyticsSession::Impl::publishResult(AnalyticsResult result)
{
    std::lock_guard lock(m_stateMutex);

    if (!accepts(result.identity))
        return;

    if (!m_results.publish(std::move(result)))
        return;

    QMetaObject::invokeMethod(
        &m_owner,
        [this]
        {
            bool shouldNotify;

            {
                std::lock_guard stateLock(m_stateMutex);

                const bool hasResult =
                    m_results.acknowledgeNotification();

                shouldNotify = m_enabled && hasResult;
            }

            if (shouldNotify)
                emit m_owner.resultAvailable();
        },
        Qt::QueuedConnection);
}

void AnalyticsSession::Impl::reportFailure(AnalyticsFrameIdentity identity, QString message)
{
    std::lock_guard lock(m_stateMutex);

    if (!accepts(identity))
        return;

    m_enabled = false;
    m_frames.clear();
    m_results.clear();

    QMetaObject::invokeMethod(
        &m_owner,
        [this, identity, message = std::move(message)]
        {
            {
                std::lock_guard stateLock(m_stateMutex);

                if (m_streamRunId != identity.streamRunId ||
                    m_generation != identity.analyticsGeneration ||
                    m_enabled)
                {
                    return;
                }
            }

            QPointer<AnalyticsSession> ownerGuard(&m_owner);

            emit m_owner.enabledChanged();

            // A connected slot could destroy the session.
            if (!ownerGuard)
                return;

            // A connected slot could also enable a new activation.
            {
                std::lock_guard stateLock(m_stateMutex);

                if (m_streamRunId != identity.streamRunId ||
                    m_generation != identity.analyticsGeneration ||
                    m_enabled)
                {
                    return;
                }
            }

            emit m_owner.errorOccurred(message);
        },
        Qt::QueuedConnection);
}

AnalyticsSession::AnalyticsSession(
    DetectorFactory detectorFactory,
    QObject* parent)
    : QObject(parent)
    , m_impl(std::make_unique<Impl>(
          *this,
          std::move(detectorFactory)))
{
}

AnalyticsSession::~AnalyticsSession()
{
    Q_ASSERT(QThread::currentThread() == thread());

    // Stop and join while the QObject owner still exists.
    m_impl.reset();
}

std::uint64_t AnalyticsSession::enable(std::uint64_t streamRunId)
{
    return m_impl->enable(streamRunId);
}

void AnalyticsSession::disable()
{
    m_impl->disable();
}

bool AnalyticsSession::isEnabled() const
{
    return m_impl->isEnabled();
}

bool AnalyticsSession::submitFrame(AnalyticsFrame frame)
{
    return m_impl->submitFrame(std::move(frame));
}

std::optional<AnalyticsResult> AnalyticsSession::takeLatestResult()
{
    return m_impl->takeLatestResult();
}

} // namespace vap
