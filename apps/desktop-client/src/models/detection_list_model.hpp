#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QString>

namespace vap
{

struct DetectionOverlayItem
{
    double x{};
    double y{};
    double width{};
    double height{};
    QString label;
    float confidence{};
};

class DetectionListModel final : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Role
    {
        BoxXRole = Qt::UserRole + 1,
        BoxYRole,
        BoxWidthRole,
        BoxHeightRole,
        LabelRole,
        ConfidenceRole
    };

    explicit DetectionListModel(QObject* parent = nullptr);

    int rowCount(
        const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(
        const QModelIndex& index,
        int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setDetections(QList<DetectionOverlayItem> detections);
    void clear();

private:
    QList<DetectionOverlayItem> m_detections;
};

} // namespace vap
