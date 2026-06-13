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

}
