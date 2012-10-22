#include "rgtabledialog.h"

RGTableDialog::RGTableDialog(QWidget *parent, RPGRulebook *rbk, DataType t) : QDialog(parent) {
    dtype = t;
    rulebook = rbk;

    switch(dtype) {
        case GAM:
            setWindowTitle(tr("Game Table"));
            break;
        case RBK:
            setWindowTitle(tr("Rulebook Table"));
            break;
        case CHR:
            setWindowTitle(tr("Character Table"));
            break;
        case CUR:
            setWindowTitle(tr("Currency Table"));
            break;
        case ATT:
            setWindowTitle(tr("Attribute Table"));
            break;
        case SKI:
            setWindowTitle(tr("Skill Table"));
            break;
        case MOD:
            setWindowTitle(tr("Modifier Table"));
            break;
        case ITT:
            setWindowTitle(tr("Item Type Table"));
            break;
        case ITM:
            setWindowTitle(tr("Item Table"));
            break;
    }

    addButton = new QPushButton("Add", this);
    delButton = new QPushButton("Delete", this);
    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                   Qt::Horizontal, this);

    connect(buttons, SIGNAL(accepted()), this, SLOT(onAccept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(onReject()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addLine()));
    connect(delButton, SIGNAL(clicked()), this, SLOT(delLine()));

    grid = new QGridLayout(this);

    if(dtype==ITM) {
        properties["ITT"] = rbk->getProperties(ITT);
        typebox = new QComboBox(this);
        current = 0;

        for(int i=0; i<properties["ITT"].count(); i++) {
            QString name = properties["ITT"].at(i)->getName();
            properties[name] = rbk->getProperties(name);
            variables[name] = rbk->getVariables(name);
            typebox->addItem(name);
        }
        typebox->setCurrentIndex(0);

        table = new QTableWidget(0, properties[typebox->currentText()].count()+1, this);
        table->setHorizontalHeaderItem(0, new QTableWidgetItem("Item Name"));
        table->setColumnWidth(0, 150);

        for(int i=0; i < properties[typebox->currentText()].count(); i++) {
            table->setHorizontalHeaderItem(i+1, new QTableWidgetItem(properties[typebox->currentText()].at(i)->getName()));
            table->setColumnWidth(i+1, 150);
        }

        if(variables[typebox->currentText()].size() > 0) {
            for(int j=0; j < variables[typebox->currentText()].size(); j++) {
                addLine(variables[typebox->currentText()].at(j));
            }
        }

        grid->addWidget(typebox,   0, 0, 1, 5);
        grid->addWidget(table,     1, 0, 5, 5);
        grid->addWidget(addButton, 6, 1, 1, 1);
        grid->addWidget(delButton, 6, 3, 1, 1);
        grid->addWidget(buttons,   7, 0, 1, 5);
        connect(typebox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onComboChange(QString)));
    } else {
        properties[""] = rbk->getProperties(dtype);
        table = new QTableWidget(0, properties[""].count()+1, this);
        table->setHorizontalHeaderItem(0, new QTableWidgetItem("Variable Name"));
        table->setColumnWidth(0, 150);
        for(int i=0; i<properties[""].count(); i++) {
            table->setHorizontalHeaderItem(i+1, new QTableWidgetItem(properties[""].at(i)->getName()));
            table->setColumnWidth(i+1, 150);
        }

        variables[""] = rbk->getVariables(dtype);
        if (variables[""].size() > 0) {
            for(int j=0; j<variables[""].size(); j++) {
                addLine(variables[""].at(j));
            }
        }

        grid->addWidget(table,     0, 0, 5, 5);
        grid->addWidget(addButton, 6, 1, 1, 1);
        grid->addWidget(delButton, 6, 3, 1, 1);
        grid->addWidget(buttons,   7, 0, 1, 5);
    }

    setLayout(grid);
    resize(320, 400);
    show();
}


void RGTableDialog::onAccept() {
    bool ok = true; // If there are any problems, mark false, and don't allow
                    // dialog box to close.
    QList<RPGVariable *> newvariables;

    if(dtype==ITM) {
        QString itype = typebox->currentText();
        newvariables = update(itype);
        if (newvariables.isEmpty())
            ok = false;
        else variables[itype] = newvariables;
    } else {
        newvariables = update("");
        if(newvariables.isEmpty())
            ok = false;
        else variables[""] = newvariables;
    }


    if(ok==false) {
        QMessageBox qmb;
        qmb.setWindowTitle("Error!!");
        qmb.setText("All fields are required.  Please review all fields and complete any that are blank.");
        qmb.exec();
    } else {
        if(dtype==ITM) {
            for(int i=0; i < typebox->count(); i++)
                emit updateVariables(typebox->itemText(i), variables[typebox->itemText(i)]);
            close();
        } else {
            emit updateVariables(dtype, variables[""]);
            close();
        }
    }
}


void RGTableDialog::onReject() {
    close();
}


QList<RPGVariable *> RGTableDialog::update(QString t) {
    bool ok = true; // If there are any problems, mark false.
    QList<RPGVariable *> newvariables;
    QLineEdit *line;
    QSpinBox *spin;
    QDoubleSpinBox *dspin;
    rgDiceComponent *dc;
    rgComboComponent *ac;
    rgCurrencyComponent *cc;
    ValueType value;
    QString prop, v;

    for(int i=0; i < table->rowCount(); i++) {
        newvariables.append(new RPGVariable());

        // Handle Variable Name separately
        line = (QLineEdit *)table->cellWidget(i, 0);
        if(line->text().isEmpty())
            ok = false;
        else newvariables.at(i)->setName(line->text());

        for(int j=1; j < table->columnCount(); j++) {
            value = properties[t].at(j-1)->getValueType();
            prop = properties[t].at(j-1)->getName();

            switch(value) {
                case text:
                    line = (QLineEdit *)table->cellWidget(i, j);
                    if(line->text().isEmpty())
                        ok = false;
                    else newvariables.at(i)->setValue(prop, line->text());
                    break;
                case integer:
                    spin = (QSpinBox *)table->cellWidget(i, j);
                    v = QString("%1").arg(spin->value());
                    newvariables.at(i)->setValue(prop, v);
                    break;
                case number:
                    dspin = (QDoubleSpinBox *)table->cellWidget(i, j);
                    v = QString("%1").arg(dspin->value());
                    newvariables.at(i)->setValue(prop, v);
                    break;
                case dice:
                    dc = (rgDiceComponent *)table->cellWidget(i, j);
                    newvariables.at(i)->setValue(prop, dc->value());
                    break;
                case currency:
                    cc = (rgCurrencyComponent *)table->cellWidget(i, j);
                    newvariables.at(i)->setValue(prop, cc->value());
                    break;
                case attribute:
                    ac = (rgComboComponent *)table->cellWidget(i, j);
                    newvariables.at(i)->setValue(prop, ac->value());
                    break;
                case skill:
                    ac = (rgComboComponent *)table->cellWidget(i, j);
                    newvariables.at(i)->setValue(prop, ac->value());
                    break;
                case boolean:
                    ac = (rgComboComponent *)table->cellWidget(i, j);
                    newvariables.at(i)->setValue(prop, ac->value());
                    break;
                default:
                    break;
            }
        }

        if(ok == false) {
            newvariables.empty();
            return newvariables;      // If one thing fails, return empty.
        }
    }

    return newvariables;  // Only happens if ok is true by the end.
}


void RGTableDialog::addLine() {
    QSpinBox *sb;
    QDoubleSpinBox *db;
    rgDiceComponent *d;
    rgComboComponent *ac;
    rgCurrencyComponent *cc;
    QString propname;
    QList<RPGVariable *>vars;
    QStringList varstrings;

    int newrow = table->rowCount();
    table->setRowCount(newrow + 1);

    // Load name
    table->setCellWidget(newrow, 0, new QLineEdit(this));

    if(dtype==ITM)
        propname = typebox->currentText();
    else propname = "";

    for(int i=1; i<table->columnCount(); i++) { // Skip name
        switch(properties[propname].at(i-1)->getValueType()) {
            case text:
                table->setCellWidget(newrow, i, new QLineEdit(this));
                break;
            case integer:
                sb = new QSpinBox(this);
                sb->setValue(0);
                table->setCellWidget(newrow, i, sb);
                break;
            case number:
                db = new QDoubleSpinBox(this);
                db->setValue(0);
                table->setCellWidget(newrow, i, db);
                break;
            case dice:          // Below are not yet implemented
                d = new rgDiceComponent(this);
                // Constructor defines defaults.  No set needed.
                table->setCellWidget(newrow, i, d);
                break;
            case currency:
                vars = rulebook->getVariables(CUR);
                varstrings.clear();
                for(int j=0; j<vars.count(); j++)
                    varstrings.append(vars.at(j)->getValue("Symbol"));
                cc = new rgCurrencyComponent(this, varstrings);
                table->setCellWidget(newrow, i, cc);
                break;
            case attribute:
                // List of Attributes needs to be pulled.  Pull those and create the QStringList to send to the component.
                vars = rulebook->getVariables(ATT);
                varstrings.clear();
                for(int j=0; j<vars.count(); j++)
                    varstrings.append(vars.at(j)->getName());
                ac = new rgComboComponent(this, varstrings);
                table->setCellWidget(newrow, i, ac);
                break;
            case skill:
                // List of Skills needs to be pulled.  Pull those and create the QStringList to send to the component.
                vars = rulebook->getVariables(SKI);
                varstrings.clear();
                for(int j=0; j<vars.count(); j++)
                    varstrings.append(vars.at(j)->getName());
                ac = new rgComboComponent(this, varstrings);
                table->setCellWidget(newrow, i, ac);
                break;
            case boolean:
                varstrings.clear();
                varstrings.append("True");
                varstrings.append("False");
                ac = new rgComboComponent(this, varstrings);
                table->setCellWidget(newrow, i, ac);
                break;
            default:
                break;
        }
    }
}


void RGTableDialog::addLine(RPGVariable *var) {
    QSpinBox *sb;
    QDoubleSpinBox *db;
    rgDiceComponent *d;
    rgComboComponent *ac;
    rgCurrencyComponent *cc;
    QString propname;
    QList<RPGVariable *>vars;
    QStringList varstrings;

    int newrow = table->rowCount();
    table->setRowCount(newrow + 1);

    // Load name
    table->setCellWidget(newrow, 0, new QLineEdit(var->getName(), this));

    if(dtype==ITM)
        propname = typebox->currentText();
    else propname = "";

    for(int i=1; i<table->columnCount(); i++) { // Skip name
        switch(properties[propname].at(i-1)->getValueType()) {
            case text:
                table->setCellWidget(newrow, i, new QLineEdit(var->getValue(table->horizontalHeaderItem(i)->text()), this));
                break;
            case integer:
                sb = new QSpinBox(this);
                sb->setValue(var->getValue((table->horizontalHeaderItem(i)->text())).toInt());
                table->setCellWidget(newrow, i, sb);
                break;
            case number:
                db = new QDoubleSpinBox(this);
                db->setValue(var->getValue(table->horizontalHeaderItem(i)->text()).toDouble());
                table->setCellWidget(newrow, i, db);
                break;
            case dice:          // Below are not yet implemented
                d = new rgDiceComponent(this, var->getValue(table->horizontalHeaderItem(i)->text()));
                // Constructor defines defaults.  No set needed.
                table->setCellWidget(newrow, i, d);
                break;
                case currency:
                vars = rulebook->getVariables(CUR);
                varstrings.empty();
                for(int j=0; j<vars.count(); j++)
                    varstrings.append(vars.at(j)->getValue("Symbol"));
                cc = new rgCurrencyComponent(this, varstrings, var->getValue(table->horizontalHeaderItem(i)->text()));
                table->setCellWidget(newrow, i, cc);
                break;
            case attribute:
                // List of Attributes needs to be pulled.  Pull those and create the QStringList to send to the component.
                vars = rulebook->getVariables(ATT);
                varstrings.empty();
                for(int j=0; j<vars.count(); j++)
                    varstrings.append(vars.at(j)->getName());
                ac = new rgComboComponent(this, varstrings, var->getValue(table->horizontalHeaderItem(i)->text()));
                table->setCellWidget(newrow, i, ac);
                break;
            case skill:
                // List of Skills needs to be pulled.  Pull those and create the QStringList to send to the component.
                vars = rulebook->getVariables(SKI);
                varstrings.empty();
                for(int j=0; j<vars.count(); j++)
                    varstrings.append(vars.at(j)->getName());
                ac = new rgComboComponent(this, varstrings, var->getValue(table->horizontalHeaderItem(i)->text()));
                table->setCellWidget(newrow, i, ac);
                break;
            case boolean:
                varstrings.empty();
                varstrings.append("True");
                varstrings.append("False");
                ac = new rgComboComponent(this, varstrings, var->getValue(table->horizontalHeaderItem(i)->text()));
                table->setCellWidget(newrow, i, ac);
                break;
            default:
                break;
        }
    }
}


void RGTableDialog::delLine() {
    table->removeRow(table->currentRow());
}


void RGTableDialog::onComboChange(QString newval) {
    Q_UNUSED(newval)
    QList<RPGVariable *> p;
    QString oldname = typebox->itemText(current);
    p = update(oldname);

    if(p.isEmpty()) {
        QMessageBox qmb;
        qmb.setWindowTitle(tr("Error!"));
        qmb.setText(tr("All fields must be filled out before selecting another Item Type."));
        qmb.exec();

        typebox->setCurrentIndex(current);
    } else {
        variables[oldname] = p;
        delete table;

        table = new QTableWidget(0, properties[typebox->currentText()].count()+1, this);
        table->setHorizontalHeaderItem(0, new QTableWidgetItem("Variable Name"));
        table->setColumnWidth(0, 150);

        for(int i=0; i < properties[typebox->currentText()].count(); i++) {
            table->setHorizontalHeaderItem(i+1, new QTableWidgetItem(properties[typebox->currentText()].at(i)->getName()));
            table->setColumnWidth(i+1, 150);
        }

        if(variables[typebox->currentText()].size() > 0) {
            for(int j=0; j < variables[typebox->currentText()].size(); j++) {
                addLine(variables[typebox->currentText()].at(j));
            }
        }

        grid->addWidget(table, 1, 0, 5, 5);
        current = typebox->currentIndex();
    }
}
