#include "rgcurrencybox.h"

RGCurrencyBox::RGCurrencyBox(QWidget *parent, RPGRulebook *rulebook) : QWidget(parent) {
    layout = new QGridLayout(this);

    amount = new QLineEdit(this);
    connect(amount, SIGNAL(textChanged(QString)), this, SLOT(onEdit()));
    combo = new QComboBox(this);

    QList<RPGProperty *>currencies = rulebook->getProperties(CUR);
    for(int i=0; i < currencies.count(); i++) {
        combo->addItem(currencies.at(i)->getName());
    }
    //this->setContentsMargins(QMargins(0, 0, 0, 0));
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    layout->addWidget(amount, 0, 0, 1, 2);
    layout->addWidget(combo, 0, 2, 1, 1);

    setLayout(layout);

}

RGCurrencyBox::RGCurrencyBox(QWidget *parent, RPGRulebook *rulebook, QString value) : QWidget(parent) {
    layout = new QGridLayout(this);

    // Separate amount from currency type
    int last = value.lastIndexOf(QRegExp("\\d"));
    QString amt = value.left(last);
    QString type = value.right(last+1);

    amount = new QLineEdit(this);
    amount->setText(amt);

    combo = new QComboBox(this);

    QList<RPGProperty *>currencies = rulebook->getProperties(CUR);
    for(int i=0; i < currencies.count(); i++) {
        combo->addItem(currencies.at(i)->getName());
    }

    combo->setCurrentIndex(combo->findText(type));

    layout->addWidget(amount, 0, 0, 2, 1);
    layout->addWidget(combo, 0, 2, 1, 1);

    setLayout(layout);
}

QString RGCurrencyBox::getAmount() { return amount->text(); }
QString RGCurrencyBox::getCurrencyType() { return combo->currentText(); }
QString RGCurrencyBox::getValue() { return QString("%1%2").arg(amount->text()).arg(combo->currentText()); }

void RGCurrencyBox::onEdit() {
    amount->text().remove(QRegExp("\\D+"));
}
