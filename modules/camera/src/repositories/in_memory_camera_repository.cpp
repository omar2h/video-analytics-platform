#include <vap/camera/repositories/in_memory_camera_repository.hpp>

namespace vap
{

InMemoryCameraRepository::InMemoryCameraRepository()
{
    m_cameras =
        {
            {
                "lobby",
                "Lobby Camera",
                {"rtsp://lobby"}
            },

            {
                "parking",
                "Parking Camera",
                {"rtsp://parking"}
            },

            {
                "warehouse",
                "Warehouse Camera",
                {"rtsp://warehouse"}
            }
        };
}

QList<Camera> InMemoryCameraRepository::cameras() const
{
    return m_cameras;
}

void InMemoryCameraRepository::addCamera(const Camera &camera)
{
    m_cameras.append(camera);
}

bool InMemoryCameraRepository::updateCamera(const Camera &camera)
{
    auto it = std::find_if(
        m_cameras.begin(),
        m_cameras.end(),
        [&camera](const Camera& existing)
        {
            return existing.id == camera.id;
        });
    if(it != m_cameras.end()) {
        *it = camera;
        return true;
    }
    return false;
}

void InMemoryCameraRepository::removeCamera(const CameraId &cameraId)
{
    auto it = std::find_if(
        m_cameras.begin(),
        m_cameras.end(),
        [&cameraId](const Camera& existing)
        {
            return existing.id == cameraId;
        });
    if(it != m_cameras.end())
        m_cameras.erase(it);
}

}
