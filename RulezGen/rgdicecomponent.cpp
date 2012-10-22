#include "rgdicecomponent.h"

rgDiceComponent::rgDiceComponent(QWidget *parent) : QWidget(parent) {
    // Create a list of the possible dice types
    QStringList dtypes;
    dtypes << "D2" << "D3" << "D4" << "D6" << "D8" << "D10" << "D12" << "D20" << "D100";

    QHBoxLayout *hbox = new QHBoxLayout;
    // initialize the components
    numdice = new QSpinBox(this);
    numdice->setValue(0);
    hbox->addWidget(numdice);

    dicetype = new QComboBox(this);
    dicetype->addItems(dtypes);
    dicetype->setCurrentIndex(3);   // Default to D6
    hbox->addWidget(dicetype);

    setLayout(hbox);
    show();
}


rgDiceComponent::rgDiceComponent(QWidget *parent, QString data) : QWidget(parent) {
    // Create a list of the possible dice types
    QStringList dtypes;
    dtypes << "D2" << "D3" << "D4" << "D6" << "D8" << "D10" << "D12" << "D20" << "D100";

    int i = data.indexOf('D');
    QHBoxLayout *hbox = new QHBoxLayout;
    // initialize the components
    numdice = new QSpinBox(this);
    numdice->setValue(data.left(i).toInt());
    hbox->addWidget(numdice);

    dicetype = new QComboBox(this);
    dicetype->addItems(dtypes);
    int index = dicetype->findText(data.right(data.length() - i));  //  Locate the index of the dice...
    dicetype->setCurrentIndex(index);                               //  ... then set it to that index.
    hbox->addWidget(dicetype);

    setLayout(hbox);
    show();
}


// Returns the value in ##D# format.
QString rgDiceComponent::value() {
    QString v;
    v = QString("%1").arg(numdice->value());
    v.append(dicetype->currentText());

    return v;
}


// I'm not anticipating using this, but adding it just in case.
void rgDiceComponent::setValue(QString v) {
    int i = v.indexOf('D');
    numdice->setValue(v.left(i-1).toInt());
    int index = dicetype->findText(v.right(v.length() - i));   //  Locate the index of the dice...
    dicetype->setCurrentIndex(index);                          //  ... then set it to that index.
}
