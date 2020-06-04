#include "DatasetSpreadsheet.h"

#include <ImportSpreadsheet.h>

#include "Shared/Logger.h"

DatasetSpreadsheet::DatasetSpreadsheet(const QString& name,
                                       const QString& zipFileName,
                                       QObject* parent)
    : Dataset(name, parent), zipFile_(zipFileName)
{
}

DatasetSpreadsheet::~DatasetSpreadsheet() {}

bool DatasetSpreadsheet::analyze()
{
    importer_->setNameForEmptyColumn(QObject::tr("no name"));
    QObject::connect(importer_.get(),
                     &ImportSpreadsheet::progressPercentChanged, this,
                     &Dataset::loadingPercentChanged);

    if (!getSheetList() || !loadSpecificData() ||
        !getColumnTypes(getSheetName()) || !getHeadersList(getSheetName()))
        return false;

    columnsCount_ = importer_->getColumnCount(getSheetName()).second;
    rowsCount_ = importer_->getRowCount(getSheetName()).second;

    valid_ = true;

    return true;
}

const QString& DatasetSpreadsheet::getSheetName()
{
    return sheetNames_.constFirst();
}

std::tuple<bool, QVector<QVector<QVariant>>> DatasetSpreadsheet::getSample()
{
    auto [success, data] = getDataFromZip(getSheetName(), true);
    if (!success)
        return {false, {}};

    updateSampleDataStrings(data);
    return {true, data};
}

std::tuple<bool, QVector<QVector<QVariant>>> DatasetSpreadsheet::getAllData()
{
    if (!isValid())
        return {false, {}};

    QVector<QVector<QVariant>> data;
    std::tie(valid_, data) = getDataFromZip(getSheetName(), false);
    return {valid_, data};
}

void DatasetSpreadsheet::closeZip()
{
    zipFile_.close();
    importer_ = nullptr;
}

bool DatasetSpreadsheet::getSheetList()
{
    bool success{false};
    std::tie(success, sheetNames_) = importer_->getSheetNames();
    if (!success)
        LOG(LogTypes::IMPORT_EXPORT, importXlsx_.getLastError());
    return success;
}

bool DatasetSpreadsheet::getHeadersList(const QString& sheetName)
{
    bool success{false};
    std::tie(success, headerColumnNames_) =
        importer_->getColumnNames(sheetName);
    if (!success)
        LOG(LogTypes::IMPORT_EXPORT, importXlsx_.getLastError());
    return success;
}

bool DatasetSpreadsheet::getColumnTypes(const QString& sheetName)
{
    bool success{false};
    std::tie(success, columnTypes_) = importer_->getColumnTypes(sheetName);
    if (!success)
    {
        LOG(LogTypes::IMPORT_EXPORT, importOds_.getLastError());
        return false;
    }
    return true;
}

std::tuple<bool, QVector<QVector<QVariant>>> DatasetSpreadsheet::getDataFromZip(
    const QString& sheetName, bool fillSamplesOnly)
{
    QVector<QVector<QVariant>> data;
    bool success{false};
    if (fillSamplesOnly)
    {
        std::tie(success, data) = importer_->getLimitedData(
            sheetName, {}, SAMPLE_SIZE < rowCount() ? SAMPLE_SIZE : rowCount());
    }
    else
    {
        QVector<unsigned int> excludedColumns;
        for (unsigned int i = 0; i < columnCount(); ++i)
            if (!activeColumns_.at(i))
                excludedColumns.append(i);
        std::tie(success, data) =
            importer_->getData(sheetName, excludedColumns);
    }

    if (!success)
    {
        LOG(LogTypes::IMPORT_EXPORT, importXlsx_.getLastError());
        return {false, {}};
    }

    if (!fillSamplesOnly)
    {
        Q_ASSERT(rowCount() == static_cast<unsigned int>(data.size()));
        LOG(LogTypes::IMPORT_EXPORT,
            "Loaded file having " + QString::number(rowsCount_) + " rows.");
    }

    return {true, data};
}
