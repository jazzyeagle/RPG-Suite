#ifndef RGCURRENCYCOMPONENT_H
#define RGCURRENCYCOMPONENT_H

#include <QtGui>

class rgCurrencyComponent : public QWidget
{
    Q_OBJECT
public:
    rgCurrencyComponent(QWidget *parent, QStringList list);
    rgCurrencyComponent(QWidget *parent, QStringList list, QString value);
    QString value();
    void setValue(QString value);

private:
    QSpinBox *amount;
    QComboBox *currency;
};

#endif // RGCURRENCYCOMPONENT_H
