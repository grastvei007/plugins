#include "path.h"

#include <QDir>

namespace path{

QString fromConfigDir(const QString &filename)
{
    QString path = QDir::homePath() + QDir::separator() + ".config" + QDir::separator() + "june";
    path += QDir::separator() + filename;
    return path;
}


} //end namespace
