#include "rgcheckbox.h"

RGCheckBox::RGCheckBox(QWidget *parent, quint8 p, QPoint origin, QPoint destination) : QCheckBox(parent), RPGComponent() {
    setDataName(QString());
    setDataProperty(QString());
    setDataVariable(QString());

    setCheckable(false);
    move(origin);
    pagenumber = p;
    resize(QSize(destination.x() - origin.x(), destination.y() - origin.y()));
    saveorigin = origin;
    savedestination = destination;
    show();
}

DataType RGCheckBox::getDataType() { return dtype; }

void     RGCheckBox::setDataType(DataType dt) { dtype = dt; }

QString  RGCheckBox::getDataName() { return compname; }

void     RGCheckBox::setDataName(QString dn) { compname = dn; }

QString  RGCheckBox::getDataProperty() { return compproperty; }

void     RGCheckBox::setDataProperty(QString dp) { compproperty = dp; }

QString  RGCheckBox::getDataVariable() { return compvariable; }

void     RGCheckBox::setDataVariable(QString dv) { compvariable = dv; }

QString  RGCheckBox::getDataValue() {
    if(this->isChecked())
        return "1";
    else return "0";
}

void     RGCheckBox::setDataValue(QString dv) {
    if(dv=="1")
        this->setCheckState(Qt::Checked);
    else if (dv== "0")
        this->setCheckState(Qt::Unchecked);
    else {
        QMessageBox qmb;
        qmb.setWindowTitle(tr("Warning!"));
        qmb.setText(tr("Invalid Check State").arg(dv));
        qmb.exec();
    }
}

ComponentType RGCheckBox::getComponentType() { return ctype; }

void          RGCheckBox::setComponentType(ComponentType ct) { ctype = ct; }

void     RGCheckBox::mousePressEvent(QMouseEvent *e) {
    int h, i;
    QList<RPGVariable *> variables;
    QList<RPGProperty *> properties, types;
    QAction *a;

    if(e->button()==Qt::RightButton) {
        QMenu *context = new QMenu();

        // Load Attributes
        QMenu *attributes = new QMenu(tr("Attributes"), context);
        variables = RPGRulebook::Instance()->getVariables(ATT);
        properties = RPGRulebook::Instance()->getProperties(ATT);
        for(i=0; i < variables.count(); i++) {
            a = new QAction(variables.at(i)->getName(), attributes);
            a->setCheckable(true);
            if(compname==attributes->title() && compvariable==a->text())
                a->setChecked(true);
            attributes->addAction(a);
        }
        context->addMenu(attributes);

        // Load Skills
        QMenu *skills = new QMenu(tr("Skills"), context);
        variables = RPGRulebook::Instance()->getVariables(SKI);
        properties = RPGRulebook::Instance()->getProperties(SKI);
        for(i=0; i < variables.count(); i++) {
            a = new QAction(variables.at(i)->getName(), skills);
            a->setCheckable(true);
            if(compname==skills->title() && compvariable==a->text())
                a->setChecked(true);
            skills->addAction(a);
        }
        context->addMenu(skills);

        // Load Modifiers
        QMenu *modifiers = new QMenu(tr("Modifiers"), context);
        variables = RPGRulebook::Instance()->getVariables(MOD);
        properties = RPGRulebook::Instance()->getProperties(MOD);
        for(i=0; i < variables.count(); i++) {
            a = new QAction(variables.at(i)->getName(), modifiers);
            a->setCheckable(true);
            if(compname==modifiers->title() && compvariable==a->text())
                a->setChecked(true);
            modifiers->addAction(a);
        }
        context->addMenu(modifiers);
        context->addSeparator();

        // Load Items
        types = RPGRulebook::Instance()->getProperties(ITT);
        for(h = 0; h < types.count(); h++) {
            QMenu *item = new QMenu(types.at(h)->getName(), context);
            variables = RPGRulebook::Instance()->getVariables(ITM, types.at(h)->getName());
            properties = RPGRulebook::Instance()->getProperties(ITM, types.at(h)->getName());

            for(i=0; i < variables.count(); i++) {
                a = new QAction(variables.at(i)->getName(), item);
                a->setCheckable(true);
                if(compname==item->title() && compvariable==a->text())
                    a->setChecked(true);
                item->addAction(a);
            }
            context->addMenu(item);
        }
    connect(context, SIGNAL(triggered(QAction*)), this, SLOT(onClick(QAction*)));
    context->popup(e->globalPos());
    } // else do nothing
}


void RGCheckBox::onClick(QAction *a) {
    // Remove from old variable/property...
    if(getDataName().isEmpty()) {
        RPGRulebook::Instance()->removeComponent(this);
        setDataName(QString());
        setDataProperty(QString());
        setDataVariable(QString());
    }

    // Add to new
    QMenu *m = (QMenu *)a->parent();
    setDataName(m->title());
    setDataVariable(a->text());
    RPGRulebook::Instance()->addComponent(this);
}

quint8 RGCheckBox::page() { return pagenumber; }

void RGCheckBox::setPage(quint8 p) { pagenumber = p; }
