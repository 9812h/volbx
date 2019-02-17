#include <QSplitter>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

#include "Common/Configuration.h"
#include "Datasets/DatasetDefinitionSpreadsheet.h"
#include "Datasets/DatasetDefinitionOds.h"
#include "Datasets/DatasetDefinitionXlsx.h"
#include "Common/Constants.h"

#include "DatasetDefinitionVisualization.h"
#include "DatasetsListBrowser.h"
#include "ColumnsPreview.h"
#include "SpreadsheetsImportTab.h"
#include "ui_SpreadsheetsImportTab.h"

SpreadsheetsImportTab::SpreadsheetsImportTab(QWidget *parent) :
    ImportTab(parent),
    ui(new Ui::SpreadsheetsImportTab)
{
    ui->setupUi(this);

    auto visualization = new DatasetDefinitionVisualization(this);

    auto splitter2 = new QSplitter(Qt::Vertical, this);
    splitter2->addWidget(visualization);
    auto columnsPreview = new ColumnsPreview(this);
    splitter2->addWidget(columnsPreview);
    QList<int> sizes;
    sizes << 300 << 150;
    splitter2->setSizes(sizes);

    ui->verticalLayout->addWidget(splitter2);

    const int rowHeight = fontMetrics().height()*1.5;
    columnsPreview->verticalHeader()->setDefaultSectionSize(rowHeight);

    visualization->setEnabled(false);
    columnsPreview->setEnabled(false);

    connect(visualization,
            SIGNAL(currentColumnNeedSync(int)),
            columnsPreview,
            SLOT(selectCurrentColumn(int)));

    connect(columnsPreview,
            SIGNAL(currentColumnNeedSync(int)),
            visualization,
            SLOT(selectCurrentColumn(int)));

    ui->sheetCombo->hide();
}

SpreadsheetsImportTab::~SpreadsheetsImportTab()
{
    delete ui;
}

void SpreadsheetsImportTab::on_openFileButton_clicked()
{
    QString fileName =
        QFileDialog::getOpenFileName(this,
                                     tr("Open file"),
                                     Configuration::getInstance().getImportFilePath(),
                                     tr("Spreadsheets (*.xlsx *.ods )"));

    if ( true == fileName.isEmpty() )
    {
        return;
    }

    QFileInfo fileInfo(fileName);
    if ( false == fileInfo.exists() || false == fileInfo.isReadable() )
    {
        QMessageBox::information(this,
                                 tr("Access error"),
                                 tr("Can not access file."));
        return;
    }

    Configuration::getInstance().setImportFilePath(fileInfo.absolutePath());

    ui->fileNameLineEdit->setText(fileName);

    DatasetDefinitionSpreadsheet* datasetDefinition = nullptr;

    //Remove all not allowed characters from file name.
    QString regexpString = QString(Constants::datasetNameRegExp_).replace("[", "[^");
    QString datasetName = fileInfo.completeBaseName().remove(QRegExp(regexpString));

    if ( true == datasetName.isEmpty() )
    {
        datasetName = tr("Dataset");
    }

    if ( 0 == fileInfo.suffix().toLower().compare("ods") )
    {
        datasetDefinition = new DatasetDefinitionOds(datasetName, fileName);
    }
    else
    {
        if ( 0 == fileInfo.suffix().toLower().compare("xlsx") )
        {
            datasetDefinition = new DatasetDefinitionXlsx(datasetName, fileName);
        }
        else
        {
            QMessageBox::information(this,
                                     tr("Wrong file"),
                                     tr("File type is not supported."));
            emit definitionIsReady(false);
            return;
        }
    }

    datasetDefinition->init();

    DatasetDefinitionVisualization* visualization =
        findChild<DatasetDefinitionVisualization*>();
    if( nullptr == visualization )
    {
        return;
    }
    visualization->setDatasetDefiniton(datasetDefinition);
    visualization->setEnabled(true);

    ColumnsPreview* columnsPreview = findChild<ColumnsPreview*>();
    if( nullptr == columnsPreview )
    {
        return;
    }
    columnsPreview->setDatasetDefinitionSampleInfo(datasetDefinition);
    columnsPreview->setEnabled(true);

    emit definitionIsReady(true);
}

DatasetDefinition* SpreadsheetsImportTab::getDatasetDefinition()
{
    DatasetDefinitionVisualization* definition =
        findChild<DatasetDefinitionVisualization*>();
    return definition->getDatasetDefinition();
}