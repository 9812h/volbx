#pragma once

#include <QStyledItemDelegate>

/**
 * @brief Delegate for numeric type.
 */
class NumericDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit NumericDelegate(QObject* parent = nullptr);

    ~NumericDelegate() override = default;

    QString displayText(const QVariant& value,
                        const QLocale& locale) const override;
};
