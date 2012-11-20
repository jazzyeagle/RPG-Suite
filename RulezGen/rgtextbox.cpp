#include "rgtextbox.h"

RGTextBox::RGTextBox(QWidget *parent, quint8 p, QPoint origin, QPoint destination) : QLineEdit(parent), RPGComponent() {
    setDataName(QString());
    setDataProperty(QString());
    setDataVariable(QString());
    setReadOnly(true);
    setFrame(false);
    move(origin);
    pagenumber = p;
    resize(QSize(destination.x() - origin.x(), destination.y() - origin.y()));
    saveorigin = origin;
    savedestination = destination;
    show();
}

DataType RGTextBox::getDataType() { return dtype; }

void     RGTextBox::setDataType(DataType dt) { dtype = dt; }

QString  RGTextBox::getDataName() { return compname; }

void     RGTextBox::setDataName(QString dn) { compname = dn; }

QString  RGTextBox::getDataProperty() { return compproperty; }

void     RGTextBox::setDataProperty(QString dp) { compproperty = dp; }

QString  RGTextBox::getDataVariable() { return compvariable; }

void     RGTextBox::setDataVariable(QString dv) { compvariable = dv; }

QString  RGTextBox::getDataValue() { return this->text(); }

void     RGTextBox::setDataValue(QString dv) { this->setText(dv); }

ComponentType RGTextBox::getComponentType() { return ctype; }

void          RGTextBox::setComponentType(ComponentType ct) { ctype = ct; }

void     RGTextBox::mousePressEvent(QMouseEvent *e) {
    int h, i, j;
    QList<RPGVariable *> variables;
    QList<RPGProperty *> properties, types;
    QMenu *varMenu;

    if(e->button()==Qt::RightButton) {
        QMenu *context = new QMenu();

        // Load Character
        QMenu *character = new QMenu(tr("Character Traits"), context);
        properties = RPGRulebook::Instance()->getProperties(CHR);
        for(i=0; i < properties.count(); i++) {
            character->addAction(properties.at(i)->getName());
        }
        context->addMenu(character);
        context->addSeparator();

        // Load Attributes
        QMenu *attributes = new QMenu(tr("Attributes"), context);
        variables = RPGRulebook::Instance()->getVariables(ATT);
        properties = RPGRulebook::Instance()->getProperties(ATT);
        for(i=0; i < variables.count(); i++) {
            varMenu = new QMenu(variables.at(i)->getName(), attributes);
            for(j=0; j < properties.count(); j++)
                varMenu->addAction(properties.at(j)->getName());
            attributes->addMenu(varMenu);
        }
        context->addMenu(attributes);

        // Load Skills
        QMenu *skills = new QMenu(tr("Skills"), context);
        variables = RPGRulebook::Instance()->getVariables(SKI);
        properties = RPGRulebook::Instance()->getProperties(SKI);
        for(i=0; i < variables.count(); i++) {
            varMenu = new QMenu(variables.at(i)->getName(), skills);
            for(j=0; j < properties.count(); j++)
                varMenu->addAction(properties.at(j)->getName());
            skills->addMenu(varMenu);
        }
        context->addMenu(skills);
        context->addSeparator();

        // Load Modifiers
        types = RPGRulebook::Instance()->getProperties(MDT);
        for(h = 0; h < types.count(); h++) {
            QMenu *item = new QMenu(types.at(h)->getName(), context);
            variables = RPGRulebook::Instance()->getVariables(MOD, types.at(h)->getName());
            properties = RPGRulebook::Instance()->getProperties(MOD, types.at(h)->getName());

            for(i=0; i < variables.count(); i++) {
                varMenu = new QMenu(variables.at(i)->getName(), item);
                for(j=0; j < properties.count(); j++)
                    varMenu->addAction(properties.at(j)->getName());
                item->addMenu(varMenu);
            }
            context->addMenu(item);
        }
        context->addSeparator();

        // Load Items
        types = RPGRulebook::Instance()->getProperties(ITT);
        for(h = 0; h < types.count(); h++) {
            QMenu *item = new QMenu(types.at(h)->getName(), context);
            variables = RPGRulebook::Instance()->getVariables(ITM, types.at(h)->getName());
            properties = RPGRulebook::Instance()->getProperties(ITM, types.at(h)->getName());

            for(i=0; i < variables.count(); i++) {
                varMenu = new QMenu(variables.at(i)->getName(), item);
                for(j=0; j < properties.count(); j++)
                    varMenu->addAction(properties.at(j)->getName());
                item->addMenu(varMenu);
            }
            context->addMenu(item);
        }

        connect(context, SIGNAL(triggered(QAction*)), this, SLOT(onMenuSelect(QAction*)));
        context->exec(e->globalPos());
    }
    // else do nothing
}

void     RGTextBox::onMenuSelect(QAction *a) {
    // Clear old values
    if(!getDataName().isEmpty()) {
        RPGRulebook::Instance()->removeComponent(this);
        setDataName(QString());
        setDataVariable(QString());
        setDataProperty(QString());
    }

    // Add to new & update textbox text
    QMenu *submenu = (QMenu *)a->parent();
    QMenu *menu = (QMenu *)submenu->parent();
        if(submenu->title()=="Character Traits") {
        setText(submenu->title() + ":" + a->text());
        setDataName(submenu->title());
        setDataProperty(a->text());
    } else {
        setText(menu->title() + ":" + submenu->title() + ":" + a->text());
        setDataName(menu->title());
        setDataVariable(submenu->title());
        setDataProperty(a->text());
    }
    //qDebug("onMenuSelect: Adding Component");
    RPGRulebook::Instance()->addComponent(this);
}

quint8 RGTextBox::page() { return pagenumber; }

void RGTextBox::setPage(quint8 p) { pagenumber = p; }
