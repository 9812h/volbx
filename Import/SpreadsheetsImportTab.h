#ifndef SPREADSHEETSIMPORTTAB_H
#define SPREADSHEETSIMPORTTAB_H

#include "ImportTab.h"

namespace Ui
{
class SpreadsheetsImportTab;
}  // namespace Ui

class DatasetSpreadsheet;

class SpreadsheetsImportTab : public ImportTab
{
    Q_OBJECT
public:
    explicit SpreadsheetsImportTab(QWidget* parent = nullptr);

    ~SpreadsheetsImportTab() override;

    SpreadsheetsImportTab& operator=(const SpreadsheetsImportTab& other) =
        delete;
    SpreadsheetsImportTab(const SpreadsheetsImportTab& other) = delete;

    SpreadsheetsImportTab& operator=(SpreadsheetsImportTab&& other) = delete;
    SpreadsheetsImportTab(SpreadsheetsImportTab&& other) = delete;

    std::unique_ptr<Dataset> getDataset() override;

private Q_SLOTS:
    void openFileButtonClicked();

private:
    void analyzeFile(std::unique_ptr<DatasetSpreadsheet>& dataset);

    Ui::SpreadsheetsImportTab* ui;
};

#endif  // SPREADSHEETSIMPORTTAB_H
