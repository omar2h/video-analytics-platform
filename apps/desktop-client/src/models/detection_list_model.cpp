#include "detection_list_model.hpp"

#include <QThread>

#include <limits>
#include <stdexcept>
#include <utility>

namespace vap
{

DetectionListModel::DetectionListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int DetectionListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return static_cast<int>(m_detections.size());
}

QVariant DetectionListModel::data(
    const QModelIndex& index,
    int role) const
{
    if (!index.isValid() ||
        index.model() != this ||
        index.column() != 0 ||
        index.row() < 0 ||
        index.row() >= m_detections.size())
    {
        return {};
    }

    const auto& detection = m_detections.at(index.row());

    switch (role)
    {
    case BoxXRole:
        return detection.x;
    case BoxYRole:
        return detection.y;
    case BoxWidthRole:
        return detection.width;
    case BoxHeightRole:
        return detection.height;
    case LabelRole:
        return detection.label;
    case ConfidenceRole:
        return detection.confidence;
    default:
        return {};
    }
}

QHash<int, QByteArray> DetectionListModel::roleNames() const
{
    return {
        {BoxXRole, "boxX"},
        {BoxYRole, "boxY"},
        {BoxWidthRole, "boxWidth"},
        {BoxHeightRole, "boxHeight"},
        {LabelRole, "label"},
        {ConfidenceRole, "confidence"}
    };
}

void DetectionListModel::setDetections(
    QList<DetectionOverlayItem> detections)
{
    Q_ASSERT(QThread::currentThread() == thread());

    if (detections.size() > std::numeric_limits<int>::max())
    {
        throw std::length_error("Too many detection rows.");
    }

    beginResetModel();
    m_detections = std::move(detections);
    endResetModel();
}

void DetectionListModel::clear()
{
    Q_ASSERT(QThread::currentThread() == thread());

    if (!m_detections.isEmpty())
        setDetections({});
}

} // namespace vap
