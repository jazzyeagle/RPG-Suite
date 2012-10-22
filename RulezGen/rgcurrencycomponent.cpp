#include "rgcurrencycomponent.h"


// Generic constructor:  QStringList required to get the currency symbols from the rulebook.
rgCurrencyComponent::rgCurrencyComponent(QWidget *parent, QStringList list) : QWidget(parent) {
    amount = new QSpinBox(this);
    currency = new QComboBox(this);
    currency->addItems(list);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(amount);
    hbox->addWidget(currency);
    setLayout(hbox);
}


rgCurrencyComponent::rgCurrencyComponent(QWidget *parent, QStringList list, QString value) {
    amount = new QSpinBox(this);
    currency = new QComboBox(this);
    currency->addItems(list);
    QString v = value;

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(amount);
    hbox->addWidget(currency);
    setLayout(hbox);

    // Determine if string is valid and update fields if it is
    int found = value.indexOf(' ');
    if(found != -1) {
        int index = currency->findText(value.right(value.length() - (found+1)));
        if(index != -1) {
            amount->setValue(value.left(found).toInt());
            currency->setCurrentIndex(index);
        }
    }
}


// Currency string will be ### $$ (amount space currency symbol)
QString rgCurrencyComponent::value() {
    QString v;
    v = QString("%1").arg(amount->value());
    v.append(' ');
    v.append(currency->currentText());
    return v;
}

void rgCurrencyComponent::setValue(QString value) {
    int found = value.indexOf(' ');
    if(found != -1) {
        int index = currency->findText(value.right(value.length() - found));
        if(index != -1) {
            amount->setValue(value.left(found).toInt());
            currency->setCurrentIndex(index);
        }
    }
}
