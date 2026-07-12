#include <gtest/gtest.h>

#include <QFile>

#include <vap/database/database.hpp>
#include <vap/camera/repositories/sqlite_camera_repository.hpp>

namespace vap
{
constexpr auto DatabasePath = "test_video_analytics.db";
class SQLiteCameraRepositoryTest : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    std::unique_ptr<Database> m_database;
    std::unique_ptr<SQLiteCameraRepository> m_repository;
};

void SQLiteCameraRepositoryTest::SetUp()
{
    m_database = std::make_unique<Database>(DatabasePath);

    ASSERT_TRUE(m_database->open());

    m_repository = std::make_unique<SQLiteCameraRepository>(*m_database);
}

void SQLiteCameraRepositoryTest::TearDown()
{
    m_repository.reset();
    m_database.reset();

    QFile::remove(DatabasePath);
}

TEST_F(SQLiteCameraRepositoryTest, CamerasInitiallyEmpty)
{
    EXPECT_TRUE(m_repository->cameras().isEmpty());
}

TEST_F(SQLiteCameraRepositoryTest, AddCameraPersistsCamera)
{
    Camera camera;

    camera.id = "1";
    camera.name = "Front Door";
    camera.config.url = "rtsp://192.168.1.10/live";

    m_repository->addCamera(camera);
    const QList<Camera> cameras = m_repository->cameras();

    ASSERT_EQ(cameras.size(), 1);

    EXPECT_EQ(cameras.first().id, camera.id);
    EXPECT_EQ(cameras.first().name, camera.name);
    EXPECT_EQ(cameras.first().config.url, camera.config.url);
}

TEST_F(SQLiteCameraRepositoryTest, UpdateCameraUpdatesStoredCamera)
{
    Camera camera;

    camera.id = "1";
    camera.name = "Front Door";
    camera.config.url = "rtsp://192.168.1.10/live";

    m_repository->addCamera(camera);

    camera.name = "Parking";
    camera.config.url = "rtsp://192.168.1.20/live";

    m_repository->updateCamera(camera);

    const QList<Camera> cameras = m_repository->cameras();

    ASSERT_EQ(cameras.size(), 1);

    EXPECT_EQ(cameras.first().id, camera.id);
    EXPECT_EQ(cameras.first().name, "Parking");
    EXPECT_EQ(cameras.first().config.url, "rtsp://192.168.1.20/live");
}

TEST_F(SQLiteCameraRepositoryTest, RemoveCameraDeletesStoredCamera)
{
    Camera camera;

    camera.id = "1";
    camera.name = "Front Door";
    camera.config.url = "rtsp://192.168.1.10/live";

    m_repository->addCamera(camera);

    m_repository->removeCamera(camera.id);

    const QList<Camera> cameras = m_repository->cameras();

    EXPECT_TRUE(cameras.isEmpty());
}

TEST_F(SQLiteCameraRepositoryTest, CamerasReturnsAllStoredCameras)
{
    QList<Camera> cameras =
        {
            {
                "1",
                "Lobby Camera",
                {"rtsp://lobby"}
            },

            {
                "2",
                "Parking Camera",
                {"rtsp://parking"}
            },

            {
                "3",
                "Warehouse Camera",
                {"rtsp://warehouse"}
            }
        };
    for(const auto& camera : cameras)
    {
        m_repository->addCamera(camera);
    }

    QList<Camera> storedCameras = m_repository->cameras();

    ASSERT_EQ(storedCameras.size(), 3);

    EXPECT_EQ(cameras[0].id, "1");
    EXPECT_EQ(cameras[1].id, "2");
    EXPECT_EQ(cameras[2].id, "3");
}

}
