#include "Constants.h"

#include <cmath>

#include <QLocale>
#include <QSettings>
#include <QVector>

namespace Constants
{
QString getUpdaterName() { return QStringLiteral("VersionCheck"); }

QString getExeFileSuffix() { return QStringLiteral(".exe"); }

QString getTemporaryFileSuffix() { return QStringLiteral(".tmp"); }

QString getConfigurationFileName() { return QStringLiteral("config"); }

QString getDatasetDefinitionFilename()
{
    return QStringLiteral("definition.xml");
}

QString getDatasetDataFilename() { return QStringLiteral("data.csv"); }

QString getDatasetStringsFilename() { return QStringLiteral("strings.txt"); }

QString getDatasetExtension() { return QStringLiteral(".vbx"); }

static QVector<QString> initProgressBarNames()
{
    QVector<QString> progressTitles(static_cast<int>(BarTitle::END));
    progressTitles[static_cast<int>(BarTitle::LOADING)] =
        QObject::tr("Loading");
    progressTitles[static_cast<int>(BarTitle::SAVING)] = QObject::tr("Saving");
    progressTitles[static_cast<int>(BarTitle::ANALYSING)] =
        QObject::tr("Analysing");
    return progressTitles;
}

QString getProgressBarTitle(BarTitle barTitle)
{
    static const QVector<QString> progressTitles{initProgressBarNames()};
    return progressTitles[static_cast<int>(barTitle)];
}

QString getDatasetNameRegExp() { return QStringLiteral("[\\w\\s-]+"); }

}  // namespace Constants
