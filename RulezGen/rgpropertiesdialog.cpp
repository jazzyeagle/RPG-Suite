#include "rgpropertiesdialog.h"

RGPropertiesDialog::RGPropertiesDialog(QWidget *parent, RPGRulebook *rbk, DataType t) : QDialog(parent) {
    rulebook = rbk;
    dtype = t;
    grid = new QGridLayout(this);

    // Set Window Title
    switch (dtype) {
        case GAM:
            setWindowTitle(tr("Game Properties"));
            break;
        case RBK:
            setWindowTitle(tr("Rulebook Properties"));
            break;
        case CHR:
            setWindowTitle(tr("Character Properties"));
            break;
        case CUR:
            setWindowTitle(tr("Currency Properties"));
            break;
        case ATT:
            setWindowTitle(tr("Attribute Properties"));
            break;
        case SKI:
            setWindowTitle(tr("Skill Properties"));
            break;
        case MDT:
            setWindowTitle(tr("Modifier Type Properties"));
        case MOD:
            setWindowTitle(tr("Modifier Properties"));
            break;
        case ITT:
            setWindowTitle(tr("Item Type Properties"));
            break;
        case ITM:
            setWindowTitle(tr("Item Properties"));
            break;
    }

    if(dtype==ITT) { // If this is an Item Type, we only want a name for the type and that's it.
        table = new QTableWidget(0, 1, this);
        table->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Item Type")));
        table->setColumnWidth(0, 280);    
    } else if(dtype==MDT) {
        table = new QTableWidget(0, 1, this);
        table->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Modifier Type")));
        table->setColumnWidth(0, 280);
    } else {        //  Everything else has a property, type, and editable option.
        table = new QTableWidget(0, 3, this);
        table->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Property")));
        table->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Type")));
        table->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Edit")));
        table->setColumnWidth(0, 150);
        table->setColumnWidth(1, 100);
        table->setColumnWidth(2, 30);
    }

    addButton = new QPushButton("Add Property", this);
    delButton = new QPushButton("Delete Property", this);
    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                   Qt::Horizontal, this);

    if(dtype==ITM) { // Items have an extra combo box that let the user control the item type they are viewing.
        typebox = new QComboBox(this);
        current = 0;

        properties["ITT"] = rulebook->getProperties(ITT);
        for(int i=0; i<properties["ITT"].count(); i++) {
            QString pname = properties["ITT"].at(i)->getName();
            typebox->addItem(pname);

            //Load Properties
            properties[pname] = rulebook->getProperties(ITM, pname);
        }

        grid->addWidget(typebox,   0, 0, 1, 5);
        grid->addWidget(table,     1, 0, 4, 5);
        grid->addWidget(addButton, 6, 1, 1, 1);
        grid->addWidget(delButton, 6, 3, 1, 1);
        grid->addWidget(buttons,   7, 0, 1, 5);
        for(int i=0; i<properties[typebox->currentText()].count(); i++)
            addLine(properties[typebox->currentText()].at(i));

        connect(typebox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onComboChange(QString)));
    } else if(dtype==MOD) {
        typebox = new QComboBox(this);
        current = 0;

        properties["MDT"] = rulebook->getProperties(MDT);
        for(int i=0; i<properties["MDT"].count(); i++) {
            QString pname = properties["MDT"].at(i)->getName();
            typebox->addItem(pname);

            //Load Properties
            properties[pname] = rulebook->getProperties(MOD, pname);
        }

        grid->addWidget(typebox,   0, 0, 1, 5);
        grid->addWidget(table,     1, 0, 4, 5);
        grid->addWidget(addButton, 6, 1, 1, 1);
        grid->addWidget(delButton, 6, 3, 1, 1);
        grid->addWidget(buttons,   7, 0, 1, 5);
        for(int i=0; i<properties[typebox->currentText()].count(); i++)
            addLine(properties[typebox->currentText()].at(i));

        connect(typebox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onComboChange(QString)));
    } else {
        grid->addWidget(table,     0, 0, 5, 5);
        grid->addWidget(addButton, 6, 1, 1, 1);
        grid->addWidget(delButton, 6, 3, 1, 1);

        grid->addWidget(buttons,   7, 0, 1, 5);

        // Load Properties
        properties[""] = rulebook->getProperties(dtype);
        for(int i=0; i<properties[""].count(); i++)
            addLine(properties[""].at(i));
    }

    connect(buttons, SIGNAL(accepted()), this, SLOT(onAccept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(onReject()));

    connect(addButton, SIGNAL(clicked()), this, SLOT(addLine()));
    connect(delButton, SIGNAL(clicked()), this, SLOT(delLine()));

    setLayout(grid);
    resize(320, 400);
    show();
}

void RGPropertiesDialog::onAccept() {
    QList<RPGProperty *> newproperties;

    if(dtype==MOD) {
        newproperties.empty();
        newproperties = update();
        if(newproperties.isEmpty()) { // User must go back and fix or cancel out of dialog box.
            QMessageBox qmb;
            qmb.setWindowTitle(tr("Error!"));
            qmb.setText(tr("All fields must be filled out before clicking \"Ok\"."));
            qmb.exec();
        } else {
            properties[typebox->currentText()] = newproperties;
            for(int i=0; i<properties["MDT"].size(); i++) {  // Cycle through and update each Item Type
                QString name = properties["MDT"].at(i)->getName();
                emit updateProperties(MOD, name, properties[name]);
                this->close();
            }
        }
    } else if(dtype==ITM) {
        newproperties.empty();
        newproperties = update();
        if(newproperties.isEmpty()) { // User must go back and fix or cancel out of dialog box.
            QMessageBox qmb;
            qmb.setWindowTitle(tr("Error!"));
            qmb.setText(tr("All fields must be filled out before clicking \"Ok\"."));
            qmb.exec();
        } else {
            properties[typebox->currentText()] = newproperties;
            for(int i=0; i<properties["ITT"].size(); i++) {  // Cycle through and update each Item Type
                QString name = properties["ITT"].at(i)->getName();
                emit updateProperties(ITM, name, properties[name]);
                this->close();
            }
        }
    } else {
        newproperties.empty();
        newproperties = update();
        if(newproperties.isEmpty()) { // User must go back and fix or cancel out of dialog box.
            QMessageBox qmb;
            qmb.setWindowTitle(tr("Error!"));
            qmb.setText(tr("All fields must be filled out before clicking \"Ok\"."));
            qmb.exec();
        } else {
            emit updateProperties(dtype, newproperties);
            this->close();
        }
    }
}

void RGPropertiesDialog::onReject() {
    this->close();
}

QList<RPGProperty *> RGPropertiesDialog::update() {
    bool ok = true; // If there are any problems, mark false.
    QList<RPGProperty *> newproperties;

    for(int i=0; i < table->rowCount(); i++) {
        newproperties.append(new RPGProperty());
        newproperties.at(i)->setDataType(dtype);

        QLineEdit *le1 = (QLineEdit *)table->cellWidget(i, 0);
        if(le1->text()=="New Property" || le1->text().isEmpty()) {
            ok = false;
        } else
            newproperties.at(i)->setName(le1->text());

        if(!(dtype == ITT || dtype == MDT)) {
            QComboBox *c = (QComboBox *)table->cellWidget(i, 1);
            if(c->currentText().isEmpty())
                ok = false; // A selection must be chosen
            else if (c->currentText()==tr("Text"))
                newproperties.at(i)->setValueType(text);
            else if (c->currentText()==tr("Number"))
                newproperties.at(i)->setValueType(number);
            else if (c->currentText()==tr("Integer"))
                newproperties.at(i)->setValueType(integer);
            else if (c->currentText()==tr("Dice"))
                newproperties.at(i)->setValueType(dice);
            else if (c->currentText()==tr("Currency"))
                newproperties.at(i)->setValueType(currency);
            else if (c->currentText()==tr("Attribute"))
                newproperties.at(i)->setValueType(attribute);
            else if (c->currentText()==tr("Skill"))
                newproperties.at(i)->setValueType(skill);
        }

        newproperties.at(i)->setRequired(true);
        newproperties.at(i)->setEditable(false);

        if(ok == false) {
            newproperties.empty();
            return newproperties;      // If one thing fails, return empty.
        }
    }

    return newproperties;  // Only happens if ok is true by the end.
}

void RGPropertiesDialog::addLine() {
    int newrow = table->rowCount();
    table->setRowCount(newrow+1);

    QLineEdit *propertytext = new QLineEdit(tr("New Property"), table);
    table->setCellWidget(newrow, 0, propertytext);

    if(!(dtype == ITT || dtype == MDT)) {
    QComboBox *typebox = new QComboBox(table);
        typebox->addItem(QString());
        typebox->addItem(tr("Text"));
        typebox->addItem(tr("Number"));
        typebox->addItem(tr("Integer"));
        typebox->addItem(tr("Dice"));
        typebox->addItem(tr("Currency"));
        typebox->addItem(tr("Attribute"));
        typebox->addItem(tr("Skill"));
        table->setCellWidget(newrow, 1, typebox);

        QCheckBox *editbox = new QCheckBox(table);
        editbox->setEnabled(false);
        table->setCellWidget(newrow, 2, editbox);
    }
}


// Used when loading Rulebooks from a file.  The variable information will be sent during the loading
// process to add the appropriate lines and values based on the information in the file.
void RGPropertiesDialog::addLine(RPGProperty *var) {
    int newrow = table->rowCount();
    table->setRowCount(newrow+1);

    QLineEdit *propertytext = new QLineEdit(var->getName(), table);
    table->setCellWidget(newrow, 0, propertytext);

    if(!(dtype == ITT || dtype==MDT)) {
        QComboBox *typebox = new QComboBox(table);
        typebox->addItem(QString());
        typebox->addItem(tr("Text"));
        typebox->addItem(tr("Number"));
        typebox->addItem(tr("Integer"));
        typebox->addItem(tr("Dice"));
        typebox->addItem(tr("Currency"));
        typebox->addItem(tr("Attribute"));
        typebox->addItem(tr("Skill"));

        switch(var->getValueType()) {
            case text:
                typebox->setCurrentIndex(typebox->findText(tr("Text")));
                break;
            case number:
                typebox->setCurrentIndex(typebox->findText(tr("Number")));
                break;
            case integer:
                typebox->setCurrentIndex(typebox->findText(tr("Integer")));
                break;
            case dice:
                typebox->setCurrentIndex(typebox->findText(tr("Dice")));
                break;
            case currency:
                typebox->setCurrentIndex(typebox->findText(tr("Currency")));
                break;
            case attribute:
                typebox->setCurrentIndex(typebox->findText(tr("Attribute")));
                break;
            case skill:
                typebox->setCurrentIndex(typebox->findText(tr("Skill")));
                break;
            default:
                break;
        }
        table->setCellWidget(newrow, 1, typebox);

        QCheckBox *editbox = new QCheckBox(table);
        editbox->setEnabled(false);
        table->setCellWidget(newrow, 2, editbox);
    }
}

// Must check for if the line is a required field and not allow a delete if it is a required field.
void RGPropertiesDialog::delLine() {
    if(table->currentRow() < 3) {
        QMessageBox qmb;
        qmb.setWindowTitle("Error!");
        qmb.setText("This row is required and cannot be deleted.");
        qmb.exec();
    } else
        table->removeRow(table->currentRow());
}


void RGPropertiesDialog::onComboChange(QString newval) {
    QList<RPGProperty *> p;
    QString oldname = typebox->itemText(current);
    p = update();

    if(p.isEmpty()) {
        QMessageBox qmb;
        qmb.setWindowTitle(tr("Error!"));
        qmb.setText(tr("All fields must be filled out before selecting another Item Type."));
        qmb.exec();

        typebox->setCurrentIndex(current);
    } else {
        properties[oldname] = p;
        delete table;

        table = new QTableWidget(0, 3, this);
        table->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Property")));
        table->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Type")));
        table->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Edit")));
        table->setColumnWidth(0, 150);
        table->setColumnWidth(1, 100);
        table->setColumnWidth(2, 30);
        grid->addWidget(table, 1, 0, 5, 5);

        for(int i=0; i<properties[newval].count(); i++)
            addLine(properties[newval].at(i));
        current = typebox->currentIndex();
    }
}
