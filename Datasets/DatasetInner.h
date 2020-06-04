#ifndef DATASETINNER_H
#define DATASETINNER_H

#include "Dataset.h"

#include <Qt5Quazip/quazip.h>

class QuaZipFile;

/**
 * @brief Dataset class for inner format.
 */
class DatasetInner : public Dataset
{
public:
    explicit DatasetInner(const QString& name, QObject* parent = nullptr);

    ~DatasetInner() override = default;

protected:
    std::tuple<bool, QVector<QVector<QVariant>>> getSample() override;

    std::tuple<bool, QVector<QVector<QVariant>>> getAllData() override;

    bool analyze() override;

    void closeZip() override;

private:
    bool openZip();

    bool openQuaZipFile(QuaZipFile& zipFile);

    /// Load definition, strings and sample data.
    bool load();

    void retrieveColumnsFromXml(const QDomElement& root);

    bool fromXml(QByteArray& definitionContent);

    bool loadXmlFile(QByteArray& definitionContent, QuaZip& zip);

    bool loadStrings(QuaZip& zip);

    std::tuple<bool, QVector<QVector<QVariant>>> fillData(QuaZip& zip,
                                                          bool fillSamplesOnly);

    void updateProgress(unsigned int currentRow, unsigned int rowCount,
                        unsigned int& lastEmittedPercent);

    void addElementToContainer(const ColumnType columnFormat,
                               const QString& element,
                               QVector<QVector<QVariant>>& dataContainer,
                               const unsigned int lineCounter,
                               const unsigned int columnToFill) const;

    QVariant getDefaultVariantForFormat(const ColumnType format) const;

    QVector<QVector<QVariant>> prepareContainerForData(
        bool fillSamplesOnly) const;

    QuaZip zip_;

    const QString datasetsDir_;
};

#endif  // DATASETINNER_H
