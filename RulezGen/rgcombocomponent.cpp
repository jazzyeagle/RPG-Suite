#include "rgcombocomponent.h"

rgComboComponent::rgComboComponent(QWidget *parent, QStringList list) : QWidget(parent) {
    QHBoxLayout *hbox = new QHBoxLayout();
    box = new QComboBox(this);
    box->addItems(list);
    hbox->addWidget(box);
    setLayout(hbox);
}


rgComboComponent::rgComboComponent(QWidget *parent, QStringList list, QString value) : QWidget(parent) {
    QHBoxLayout *hbox = new QHBoxLayout();
    box = new QComboBox(this);
    box->addItems(list);
    hbox->addWidget(box);
    setLayout(hbox);

    int found = box->findText(value);
    if(found != -1)
        box->setCurrentIndex(found);
}


QString rgComboComponent::value() {
    return box->currentText();
}


// setValue will only update if the string sent to it is one of the attributes.  Otherwise, it does nothing.
void rgComboComponent::setValue(QString value) {
    int found = box->findText(value);
    if(found != -1)
        box->setCurrentIndex(found);
}
