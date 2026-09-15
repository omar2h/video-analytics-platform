#pragma once

#include <vap/analytics/analytics_frame.hpp>
#include <vap/analytics/analytics_result.hpp>
#include <vap/analytics/detection/i_object_detector.hpp>

#include <QObject>
#include <QString>

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>

namespace vap
{

class AnalyticsSession final : public QObject
{
    Q_OBJECT

public:
    using DetectorFactory = std::function<std::unique_ptr<IObjectDetector>()>;

    explicit AnalyticsSession(
        DetectorFactory detectorFactory,
        QObject* parent = nullptr);

    ~AnalyticsSession() override;

    // GUI thread only.
    // Starts a new activation and returns its generation.
    [[nodiscard]]
    std::uint64_t enable(std::uint64_t streamRunId);

    // GUI thread only.
    // Invalidates the activation and clears pending work/results.
    void disable();

    // GUI thread only.
    // True means frame submission is enabled, not necessarily
    // that model loading has completed.
    [[nodiscard]]
    bool isEnabled() const;

    // Thread-safe while this session is alive.
    // Accepts only frames matching the active run and generation.
    [[nodiscard]]
    bool submitFrame(AnalyticsFrame frame);

    // GUI thread only.
    // Takes the latest result for the current activation.
    [[nodiscard]]
    std::optional<AnalyticsResult> takeLatestResult();

signals:
    // Emitted on the GUI thread.
    void enabledChanged();

    // Notification only; retrieve data with takeLatestResult().
    void resultAvailable();

    // Current activation failed and has been disabled.
    void errorOccurred(const QString& message);

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace vap
