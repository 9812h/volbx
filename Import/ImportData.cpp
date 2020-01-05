#include <QSplitter>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPushButton>

#include "Common/Constants.h"
#include "Datasets/DatasetDefinitionInner.h"
#include "Datasets/DatasetDefinitionOds.h"
#include "Datasets/DatasetDefinitionXlsx.h"
#include "Common/Configuration.h"

#include "ImportData.h"
#include "ui_ImportData.h"
#include "DatasetImportTab.h"
#include "SpreadsheetsImportTab.h"

ImportData::ImportData(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ImportData),
    buttonBox_(nullptr)
{
    ui->setupUi(this);

    auto enableOpenButton = [ = ](bool activate)
    {
        buttonBox_->button(QDialogButtonBox::Open)->setEnabled(activate);
    };

    // Create tabs.
    auto datasetsTab = new DatasetImportTab(ui->tabWidget);
    connect(datasetsTab, &ImportTab::definitionIsReady, enableOpenButton);
    ui->tabWidget->insertTab(static_cast<int>(IMPORT_TYPE_INNER),
                             datasetsTab,
                             tr("Datasets"));

    auto spreadsheetsTab = new SpreadsheetsImportTab(ui->tabWidget);
    connect(spreadsheetsTab, &ImportTab::definitionIsReady, enableOpenButton);
    ui->tabWidget->insertTab(static_cast<int>(IMPORT_TYPE_SPREADSHEET),
                             spreadsheetsTab,
                             tr("Spreadsheets"));

    // If no datasets, than switch to spreadsheets tab.
    if (datasetsTab->datasetsAreAvailable())
    {
        ui->tabWidget->setCurrentWidget(datasetsTab);
    }
    else
    {
        ui->tabWidget->setCurrentWidget(spreadsheetsTab);
    }

    buttonBox_ =
        new QDialogButtonBox(QDialogButtonBox::Open | QDialogButtonBox::Cancel,
                             Qt::Horizontal,
                             this);

    connect(buttonBox_, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox_, &QDialogButtonBox::rejected, this, &QDialog::reject);

    ui->verticalLayout->addWidget(buttonBox_);

    buttonBox_->button(QDialogButtonBox::Open)->setEnabled(false);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

ImportData::~ImportData()
{
    delete ui;
}

DatasetDefinition* ImportData::getSelectedDataset()
{
    auto tab = dynamic_cast<ImportTab*>(ui->tabWidget->currentWidget());
    return tab->getDatasetDefinition();
}

ImportData::ImportDataType ImportData::getImportDataType() const
{
    return static_cast<ImportDataType>(ui->tabWidget->currentIndex());
}
