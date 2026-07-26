#pragma once

#include <QMetaType>
#include <QString>

namespace vap
{

struct RecordingConfiguration
{
    QString outputPath;
};

}

Q_DECLARE_METATYPE(vap::RecordingConfiguration)
