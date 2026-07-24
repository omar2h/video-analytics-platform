#pragma once

#include <QObject>
#include <memory>

#include <src/models/camera_list_model.hpp>
#include <vap/camera/validation/camera_validation.hpp>

namespace vap
{
class ICameraApplicationService;
class StreamingManager;
class StreamStatistics;
class StreamingSession;
class CameraManagementViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(
        CameraListModel* cameraModel
        READ cameraModel CONSTANT)

    Q_PROPERTY(
        int selectedIndex
        READ selectedIndex
        WRITE setSelectedIndex
        NOTIFY selectedIndexChanged)

    Q_PROPERTY(
        QString selectedCameraName
        READ selectedCameraName
        NOTIFY selectedCameraChanged)

    Q_PROPERTY(
        QString selectedCameraUrl
        READ selectedCameraUrl
        NOTIFY selectedCameraChanged)

    Q_PROPERTY(
        int selectedCameraState
        READ selectedCameraState
        NOTIFY selectedCameraStateChanged)

    Q_PROPERTY(QString selectedCameraCodec
        READ selectedCameraCodec
        NOTIFY selectedCameraStatisticsChanged)

    Q_PROPERTY(QString selectedCameraResolution
        READ selectedCameraResolution
        NOTIFY selectedCameraStatisticsChanged)

    Q_PROPERTY(quint64 selectedCameraFramesDecoded
        READ selectedCameraFramesDecoded
        NOTIFY selectedCameraStatisticsChanged)

    Q_PROPERTY(quint64 selectedCameraPacketsReceived
        READ selectedCameraPacketsReceived
        NOTIFY selectedCameraStatisticsChanged)

    Q_PROPERTY(double selectedCameraFps
        READ selectedCameraFps
        NOTIFY selectedCameraStatisticsChanged)

    Q_PROPERTY(double selectedCameraBitrateMbps
        READ selectedCameraBitrateMbps
        NOTIFY selectedCameraStatisticsChanged)

    Q_PROPERTY(
        QString cameraName
        READ cameraName
        WRITE setCameraName
        NOTIFY cameraNameChanged)

    Q_PROPERTY(
        QString cameraUrl
        READ cameraUrl
        WRITE setCameraUrl
        NOTIFY cameraUrlChanged)

    Q_PROPERTY(
        QString validationMessage
        READ validationMessage
        NOTIFY validationErrorChanged)

public:
    explicit CameraManagementViewModel(ICameraApplicationService* cameraApplicationService,
                                       StreamingManager* streamingManager, QObject* parent = nullptr);

    CameraListModel* cameraModel() const;

    int selectedIndex() const;
    void setSelectedIndex(int index);

    QString selectedCameraName() const;
    QString selectedCameraUrl() const;

    QString cameraName() const;
    void setCameraName(const QString& name);

    QString cameraUrl() const;
    void setCameraUrl(const QString& url);

    CameraValidationError validationError() const;
    void setValidationError(CameraValidationError error);
    QString validationMessage() const;

    int selectedCameraState() const;

    QString selectedCameraCodec() const;
    QString selectedCameraResolution() const;
    quint64 selectedCameraFramesDecoded() const;
    quint64 selectedCameraPacketsReceived() const;
    double selectedCameraFps() const;
    double selectedCameraBitrateMbps() const;

    Q_INVOKABLE void addCamera();
    Q_INVOKABLE void connectSelectedCamera();
    Q_INVOKABLE void stopSelectedCamera();
    Q_INVOKABLE void deleteSelectedCamera();
    Q_INVOKABLE void updateSelectedCamera();
    Q_INVOKABLE void beginAddCamera();

signals:
    void selectedIndexChanged();
    void selectedCameraChanged();

    void cameraNameChanged();
    void cameraUrlChanged();

    void validationErrorChanged();

    void selectedCameraStateChanged();

    void selectedCameraStatisticsChanged();

    void focusCameraNameRequested();

private:
    void reloadCameras();
    void loadSelectedCameraIntoForm();
    void clearForm();

    [[nodiscard]]
    StreamingSession* selectedSession() const;
    [[nodiscard]]
    const StreamStatistics *currentStatistics() const;

    const Camera* selectedCamera() const;

    ICameraApplicationService* m_cameraApplicationService{};
    StreamingManager* m_streamingManager;

    std::unique_ptr<CameraListModel> m_cameraModel;

    QList<Camera> m_cameras;

    int m_selectedIndex{-1};

    QString m_cameraName;
    QString m_cameraUrl;
    CameraValidationError m_validationError{
        CameraValidationError::None
    };

    QMetaObject::Connection m_sessionStateConnection;
    QMetaObject::Connection m_sessionStatisticsConnection;

};

}
