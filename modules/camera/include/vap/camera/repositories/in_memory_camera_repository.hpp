#pragma once

#include <vap/camera/repositories/i_camera_repository.hpp>

namespace vap
{

class InMemoryCameraRepository : public ICameraRepository
{
public:
    InMemoryCameraRepository();

    QList<Camera> cameras() const override;

private:
    QList<Camera> m_cameras;
};

}
