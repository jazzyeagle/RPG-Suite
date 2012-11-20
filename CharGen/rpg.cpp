#include "rpg.h"
#include <QMessageBox>


// RPGComponent Functions
RPGComponent::RPGComponent() { }


// RPGProperty Functions
RPGProperty::RPGProperty() { }

RPGProperty::RPGProperty(QString n, DataType d, ValueType v) {
    name = n;
    datatype = d;
    valuetype = v;
}
RPGProperty::RPGProperty(QString n, DataType d, ValueType v, bool req, bool edit)  {
    name = n;
    datatype = d;
    valuetype = v;
    required = req;
    editable = edit;
}

QString       RPGProperty::getName()  { return name; }
void          RPGProperty::setName(QString n) { name = n; }

DataType      RPGProperty::getDataType() { return datatype; }
void          RPGProperty::setDataType(DataType d) { datatype = d; }

ValueType     RPGProperty::getValueType() { return valuetype; }
void          RPGProperty::setValueType(ValueType v) { valuetype = v; }

QString       RPGProperty::getItemType() { return itemtype; }
void          RPGProperty::setItemType(QString t) { itemtype = t; }

RPGComponent* RPGProperty::getComponent(int i) { return component.at(i); }
quint8        RPGProperty::getComponentCount() { return component.count(); }
void          RPGProperty::addComponent(RPGComponent *c) { component.append(c); }
void          RPGProperty::removeComponent(RPGComponent *c) {
    int i = 0; bool found = false;
    while(i < component.count() && !found) {
        if(component.at(i) == c)
            found = true;
    }

    if(found) qDebug("RPGVariable Found."); else qDebug("RPGVariable not found.");
    if(found)
        component.removeAt(i);
}

bool          RPGProperty::isRequired() { return required; }
void          RPGProperty::setRequired(bool req) { required = req; }

bool          RPGProperty::isEditable() { return editable; }
void          RPGProperty::setEditable(bool edit) { editable = edit; }


// RPGVarProperty functions
RPGVarProperty::RPGVarProperty() { name.clear(); value.clear(); }
RPGVarProperty::RPGVarProperty(QString n, QString v) { name = n; value = v; }

QString RPGVarProperty::getName() { return name; }
void    RPGVarProperty::setName(QString n) { name = n; }
QString RPGVarProperty::getValue() { return value; }
void    RPGVarProperty::setValue(QString v) { value = v; }


// RPGVariable functions
RPGVariable::RPGVariable() { }

QString RPGVariable::getName() {
    return name;
}

void RPGVariable::setName(QString n) {
    name = n;
}

QList<RPGVarProperty *> RPGVariable::getValues() { return properties; }

QString RPGVariable::getValue(QString n) {
    bool found = false;
    int i = 0;

    while(!found && i < properties.count()) {
        if(properties.at(i)->getName()==n)
            found = true;
        else
            i++;
    }

    if(found)
        return properties.at(i)->getValue();
    else
        return QString();
}

void RPGVariable::setValue(QString n, QString v) {
    bool found = false;
    int i = 0;

    while(!found && i < properties.count()) {
        if(properties.at(i)->getName()==n)
            found = true;
        else
            i++;
    }

    // If the property already exists, change the value.  Otherwise, create a new property
    if(found)
        properties.at(i)->setValue(v);
    else
        properties.append(new RPGVarProperty(n, v));
}

RPGComponent* RPGVariable::getComponent(int i) { return component.at(i); }
quint8        RPGVariable::getComponentCount() { return component.count(); }
void          RPGVariable::addComponent(RPGComponent *c) { component.append(c); }
void          RPGVariable::removeComponent(RPGComponent *c) {
    int i = 0; bool found = false;
    while(i < component.count() && !found) {
        if(component.at(i) == c)
            found = true;
    }

    if(found) qDebug("RPGVariable Found."); else qDebug("RPGVariable not found.");
    if(found)
        component.removeAt(i);
}
