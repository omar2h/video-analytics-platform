#pragma once

#include <QMetaType>
#include <QString>

namespace vap
{

struct RecordingConfiguration
{
    QString outputPath{};
    QString cameraName{}; // optional metadata
};

}
