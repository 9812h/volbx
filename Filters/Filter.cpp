#include <QDebug>

#include "Common/Constants.h"
#include "Shared/Application.h"

#include "Filter.h"

Filter::Filter(QString title, int column, QWidget *parent) :
    QGroupBox(title, parent), column_(column)
{
    setCheckable(true);
    connect(this, SIGNAL(toggled(bool)), this, SLOT(setChecked(bool)));
    setStyleSheet(Application::getGroupBoxStyle());
}

Filter::~Filter()
{

}

void Filter::setChecked(bool checked)
{
    QGroupBox::setChecked(checked);
    QList<QWidget *> widgets = findChildren<QWidget*>();

    foreach(QWidget* current, widgets)
    {
        current->setVisible(checked);
    }
}