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



// RPGRulebook functions


// Static declaration
RPGRulebook* RPGRulebook::currentInstance = NULL;

// This constructor used when user selects a new Rulebook or when app opens
RPGRulebook::RPGRulebook() : QObject() {
    currentInstance = this;
    core = true;

    // Add Game properties
    QList<RPGProperty *> game_properties;
        game_properties.append(new RPGProperty(tr("ID"), GAM, text, true, false));
        game_properties.append(new RPGProperty(tr("Copyright"), GAM, text, true, false));
        game_properties.append(new RPGProperty(tr("Description"), GAM, text, true, false));
        game_properties.append(new RPGProperty(tr("Version #"), GAM, number, true, false));
    properties[GAM] = game_properties;

    // Add rulebook properties
    QList<RPGProperty *> rulebook_properties;
        rulebook_properties.append(new RPGProperty(tr("Filename"), RBK, text, true, false));
        rulebook_properties.append(new RPGProperty(tr("Copyright"), RBK, text, true, false));
        rulebook_properties.append(new RPGProperty(tr("Description"), RBK, text, true, false));
    properties[RBK] = rulebook_properties;

    // Add Currency properties
    QList<RPGProperty *> currency_properties;
        currency_properties.append(new RPGProperty(tr("Symbol"), CUR, text, true, false));
    properties[CUR] = currency_properties;

    //Add Character properties
    QList<RPGProperty *> character_properties;
        character_properties.append(new RPGProperty(tr("Age"), CHR, text, true, false));
        character_properties.append(new RPGProperty(tr("Hair Color"), CHR, text, true, false));
        character_properties.append(new RPGProperty(tr("Eye Color"), CHR, text, true, false));
    properties[CHR] = character_properties;

    // Add Attribute properties
    QList<RPGProperty *> attribute_properties;
        attribute_properties.append(new RPGProperty(tr("Abbrev"), ATT, text, true, false));
        attribute_properties.append(new RPGProperty(tr("Minimum Value"), ATT, integer, true, false));
        attribute_properties.append(new RPGProperty(tr("Maximum Value"), ATT, integer, true, false));
        attribute_properties.append(new RPGProperty(tr("Rank"), ATT, integer, true, true));
        attribute_properties.append(new RPGProperty(tr("Modifiers"), ATT, integer, true, false));
        attribute_properties.append(new RPGProperty(tr("Total"), ATT, integer, true, false));
        attribute_properties.append(new RPGProperty(tr("Cost"), ATT, currency, true, false));
    properties[ATT] = attribute_properties;

    // Add Skill properties
    QList<RPGProperty *> skill_properties;
        skill_properties.append(new RPGProperty(tr("Attribute"), SKI, attribute, true, false));
        skill_properties.append(new RPGProperty(tr("Minimum Value"), SKI, integer, true, false));
        skill_properties.append(new RPGProperty(tr("Maximum Value"), SKI, integer, true, false));
        skill_properties.append(new RPGProperty(tr("Rank"), SKI, integer, true, true));
        skill_properties.append(new RPGProperty(tr("Modifiers"), SKI, integer, true, false));
        skill_properties.append(new RPGProperty(tr("Total"), ATT, integer, true, false));
        skill_properties.append(new RPGProperty(tr("Cost"), ATT, currency, true, false));
    properties[SKI] = skill_properties;

    // Add Modifier properties
    QList<RPGProperty *> modifier_properties;
        modifier_properties.append(new RPGProperty(tr("Selected"), MOD, boolean, true, false));
        modifier_properties.append(new RPGProperty(tr("Attribute"), MOD, attribute, true, false));
        modifier_properties.append(new RPGProperty(tr("Att Mod"), MOD, integer, true, false));
        modifier_properties.append(new RPGProperty(tr("Skill"), MOD, skill, true, false));
        modifier_properties.append(new RPGProperty(tr("Skill MOD"), MOD, integer, true, false));
        modifier_properties.append(new RPGProperty(tr("Cost"), MOD, currency, true, false));
    properties[MOD] = modifier_properties;

    // Add Item Type properties
    QList<RPGProperty *> itemtype_properties;
        itemtype_properties.append(new RPGProperty(tr("New Property"), ITT, text, true, false));
    properties[ITT] = itemtype_properties;
}

RPGRulebook::RPGRulebook(QString fn) : QObject() {
    Q_UNUSED(fn)
    currentInstance = this;
    //OpenRulebook(fn);
}

RPGRulebook::~RPGRulebook() {
    currentInstance = NULL;
}

int RPGRulebook::OpenRulebook(QString fn) {
    int i, j, k, count;
    quint16 compcount, p, x, y, w, h;
    quint8 datatype; DataType dtype;
    QString propname; quint8 dt, vt; bool r, e;   // Properties Temp variables
    QString varname, name, value;                 // Variables Temp variables
    QImage img;
    QFile file(fn);
    QMessageBox qmb;

    if(!file.open(QIODevice::ReadOnly))
        return 1;
    QDataStream in(&file);

        quint32 headercheck;
        in >> headercheck;
        if(headercheck!=(quint32)RPG_HEADER)
            // error handling
            return 1;

        in >> headercheck;
        if(headercheck!=(quint32)RBK_HEADER)
            // error handling
            return 1;

        qreal version;
        in >> version;
        if(version == 0.1)
            in.setVersion(QDataStream::Qt_4_8);

        in >> core;

        if(core) {
            in >> count;    // # of images
            for(i = 0; i < count; i++) {
                in >> images[i];
                //images[i] = img;
                emit loadImage(images[i]);
            }
        }
        // add other data
        while (!in.atEnd()) {
            in >> datatype; dtype = (DataType)datatype;

            switch(dtype) {
                case GAM:
                case RBK:
                case CUR:
                    in >> count;
                    for(i=0; i < count; i++) {
                        in >> propname >> dt >> vt >> r >> e;
                        properties[dtype].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                    }
                    in >> count;
                    for(i = 0; i < count; i++) {
                        RPGVariable *v = new RPGVariable();
                        in >> varname; // Read in variable name
                        v->setName(varname);
                        for(j=0; j < properties[dtype].count(); j++) {
                            in >> value;
                            v->setValue(properties[dtype].at(j)->getName(), value);
                        }
                        variables[dtype].append(v);
                    }
                    break;
                case ATT:
                case SKI:
                case MOD:
                    in >> count;
                    for(i=0; i < count; i++) {
                        in >> propname >> dt >> vt >> r >> e;
                        properties[dtype].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                    }
                    in >> count;
                    for(i = 0; i < count; i++) {
                        RPGVariable *v = new RPGVariable();
                        in >> varname; // Read in variable name
                        v->setName(varname);
                        for(j=0; j < properties[dtype].count(); j++) {
                            in >> value;
                            v->setValue(properties[dtype].at(j)->getName(), value);
                        }
                        variables[dtype].append(v);
                        in >> compcount;
                        for(j=0; j < compcount; j++) {
                            in >> name >> propname >> varname >> p >> x >> y >> w >> h;
                            // Create Component
                            emit loadComponent(p, name, propname, varname, x, y, w, h);
                        }
                    }
                    break;
                case CHR:
                    in >> count;
                    for(i=0; i < count; i++) {
                        in >> propname >> dt >> vt >> r >> e;
                        properties[dtype].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                        in >> compcount;
                        for(j=0; j < compcount; j++) {
                            in >> name >> propname >> p >> x >> y >> w >> h;
                            // Create Component
                            emit loadComponent(p, name, propname, varname, x, y, w, h);
                        }
                    }
                    break;
                case ITT:
                    in >> count;
                    for(i=0; i < count; i++) {
                        in >> propname >> dt >> vt >> r >> e;
                        properties[dtype].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                    }
                    break;
                case ITM:
                    for(i=0; i < properties[ITT].count(); i++) {            // ITT is in the file before ITM, so it's
                        QString ittname = properties[ITT].at(i)->getName(); // safe to assume these are already loaded.
                        in >> count;
                        for(j=0; j < count; j++) {
                            in >> propname >> dt >> vt >> r >> e;
                            itemproperties[ittname].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                        }
                        in >> count;
                        for(j=0; j < count; j++) {
                            RPGVariable *v = new RPGVariable();
                            in >> varname;  // Read in variable name
                            v->setName(varname);
                            for(k=0; k < itemproperties[ittname].count(); k++) {
                                in >> value;
                                v->setValue(itemproperties[ittname].at(k)->getName(), value);
                            }
                            itemvariables[ittname].append(v);
                            in >> compcount;
                            for(k=0; k < compcount; k++) {
                                in >> name >> propname >> varname >> p >> x >> y >> w >> h;
                                // Create Component
                                emit loadComponent(p, name, propname, varname, x, y, w, h);
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        file.close();
        filename = fn;
        return 0;
}

int RPGRulebook::SaveRulebook(QString fn) {
    QFile file(fn);
    QByteArray img;
    int i, j, k;
    QList<RPGVarProperty *> values;
    //QWidget *widg;

    if(!file.open(QIODevice::WriteOnly)) {
        // error handling
        return 1;
    } else {
        QDataStream out(&file);

        // Write Rulebook File Header Info
        out << (quint32)RPG_HEADER;
        out << (quint32)RBK_HEADER;
        out << (qreal)CUR_VERSION;  // Version 0.1
        out.setVersion(QDataStream::Qt_4_8);

        out << core;

        // If this is a Core Rulebook, store game info, icon and images,
        // otherwise skip these items.
        if(core) {
            out << images.count();
            QMapIterator<int, QImage> map(images);
            while(map.hasNext()) {
                map.next();
                out << map.value();
            }

            out << (quint8)GAM;
            out << properties[GAM].count();
            for(i=0; i < properties[GAM].count(); i++) {
                // Run through properties
                out << properties[GAM].at(i)->getName();
                out << (quint8)properties[GAM].at(i)->getDataType();
                out << (quint8)properties[GAM].at(i)->getValueType();
                out << properties[GAM].at(i)->isRequired();
                out << properties[GAM].at(i)->isEditable();
            }

            out << variables[GAM].count();
            for(i=0; i < variables[GAM].count(); i++) {
                out << variables[GAM].at(i)->getName();
                values = variables[GAM].at(i)->getValues();
                for(j=0; j<values.count(); j++) {
                    out << values.at(j)->getValue();
                }
            }
        }

        // Add Rulebook data
        out << (quint8)RBK;
        out << properties[RBK].count();
        for(i=0; i < properties[RBK].count(); i++) {
            out << properties[RBK].at(i)->getName();
            out << (quint8)properties[RBK].at(i)->getDataType();
            out << (quint8)properties[RBK].at(i)->getValueType();
            out << properties[RBK].at(i)->isRequired();
            out << properties[RBK].at(i)->isEditable();
        }
        out << variables[RBK].count();
        for(i=0; i < variables[RBK].count(); i++) {
            out << variables[RBK].at(i)->getName();
            values = variables[RBK].at(i)->getValues();
            for(j=0; j < values.count(); j++) {
                out << values.at(j)->getValue();
            }
        }

        // Add Currency Data
        out << (quint8)CUR;
        out << properties[CUR].count();
        for(i=0; i < properties[CUR].count(); i++) {
            out << properties[CUR].at(i)->getName();
            out << (quint8)properties[CUR].at(i)->getDataType();
            out << (quint8)properties[CUR].at(i)->getValueType();
            out << properties[CUR].at(i)->isRequired();
            out << properties[CUR].at(i)->isEditable();
        }
        out << variables[CUR].count();
        for(i=0; i < variables[CUR].count(); i++) {
            out << variables[CUR].at(i)->getName();
            values = variables[CUR].at(i)->getValues();
            for(j=0; j < values.count(); j++) {
                out << values.at(j)->getValue();
            }
        }

        // Add Character Data & their components
        out << (quint8)CHR;
        out << properties[CHR].count();
        for(i=0; i < properties[CHR].count(); i++) {
            out << properties[CHR].at(i)->getName();
            out << (quint8)properties[CHR].at(i)->getDataType();
            out << (quint8)properties[CHR].at(i)->getValueType();
            out << properties[CHR].at(i)->isRequired();
            out << properties[CHR].at(i)->isEditable();
            out << (quint16)properties[CHR].at(i)->getComponentCount();
            for(j=0; j < properties[CHR].at(i)->getComponentCount(); j++) {
                out << properties[CHR].at(i)->getComponent(j)->getDataName();
                out << properties[CHR].at(i)->getComponent(j)->getDataProperty();
                out << (quint16)properties[CHR].at(i)->getComponent(j)->page();

                out << (quint16)properties[CHR].at(i)->getComponent(j)->saveorigin.x();
                out << (quint16)properties[CHR].at(i)->getComponent(j)->saveorigin.y();
                out << (quint16)properties[CHR].at(i)->getComponent(j)->savedestination.x();
                out << (quint16)properties[CHR].at(i)->getComponent(j)->savedestination.y();
            }

        // No values to save in RulezGen mode
        }

        // Add Attribute Data & their components
        out << (quint8)ATT;
        out << properties[ATT].count();
        for(i=0; i < properties[ATT].count(); i++) {
            out << properties[ATT].at(i)->getName();
            out << (quint8)properties[ATT].at(i)->getDataType();
            out << (quint8)properties[ATT].at(i)->getValueType();
            out << properties[ATT].at(i)->isRequired();
            out << properties[ATT].at(i)->isEditable();
        }
        out << variables[ATT].count();
        for(i=0; i < variables[ATT].count(); i++) {
            out << variables[ATT].at(i)->getName();
            values = variables[ATT].at(i)->getValues();
            for(j=0; j < values.count(); j++) {
                out << values.at(j)->getValue();
            }
            out << (quint16)variables[ATT].at(i)->getComponentCount();
            for(j=0; j < variables[ATT].at(i)->getComponentCount(); j++) {
                out << variables[ATT].at(i)->getComponent(j)->getDataName();
                out << variables[ATT].at(i)->getComponent(j)->getDataProperty();
                out << variables[ATT].at(i)->getComponent(j)->getDataVariable();
                out << (quint16)variables[ATT].at(i)->getComponent(j)->page();

                out << (quint16)variables[ATT].at(i)->getComponent(j)->saveorigin.x();
                out << (quint16)variables[ATT].at(i)->getComponent(j)->saveorigin.y();
                out << (quint16)variables[ATT].at(i)->getComponent(j)->savedestination.x();
                out << (quint16)variables[ATT].at(i)->getComponent(j)->savedestination.y();
            }
        }

        // Add Skill Data and their components
        out << (quint8)SKI;
        out << properties[SKI].count();
        for(i=0; i < properties[SKI].count(); i++) {
            out << properties[SKI].at(i)->getName();
            out << (quint8)properties[SKI].at(i)->getDataType();
            out << (quint8)properties[SKI].at(i)->getValueType();
            out << properties[SKI].at(i)->isRequired();
            out << properties[SKI].at(i)->isEditable();
        }
        out << variables[SKI].count();
        for(i=0; i < variables[SKI].count(); i++) {
            out << variables[SKI].at(i)->getName();
            values = variables[SKI].at(i)->getValues();
            for(j=0; j < values.count(); j++) {
                out << values.at(j)->getValue();
            }
            out << (quint16)variables[SKI].at(i)->getComponentCount();
            for(j=0; j < variables[SKI].at(i)->getComponentCount(); j++) {
                out << variables[SKI].at(i)->getComponent(j)->getDataName();
                out << variables[SKI].at(i)->getComponent(j)->getDataProperty();
                out << variables[SKI].at(i)->getComponent(j)->getDataVariable();
                out << (quint16)variables[SKI].at(i)->getComponent(j)->page();

                out << (quint16)variables[SKI].at(i)->getComponent(j)->saveorigin.x();
                out << (quint16)variables[SKI].at(i)->getComponent(j)->saveorigin.y();
                out << (quint16)variables[SKI].at(i)->getComponent(j)->savedestination.x();
                out << (quint16)variables[SKI].at(i)->getComponent(j)->savedestination.y();
            }
        }

        // Add Modifier Data and their components
        out << (quint8)MOD;
        out << properties[MOD].count();
        for(i=0; i < properties[MOD].count(); i++) {
            out << properties[MOD].at(i)->getName();
            out << (quint8)properties[MOD].at(i)->getDataType();
            out << (quint8)properties[MOD].at(i)->getValueType();
            out << properties[MOD].at(i)->isRequired();
            out << properties[MOD].at(i)->isEditable();
        }
        out << variables[MOD].count();
        for(i=0; i < variables[MOD].count(); i++) {
            out << variables[MOD].at(i)->getName();
            values = variables[MOD].at(i)->getValues();
            for(j=0; j < values.count(); j++) {
                out << values.at(j)->getValue();
            }
            out << (quint16)variables[MOD].at(i)->getComponentCount();
            for(j=0; j < variables[MOD].at(i)->getComponentCount(); j++) {
                out << variables[MOD].at(i)->getComponent(j)->getDataName();
                out << variables[MOD].at(i)->getComponent(j)->getDataProperty();
                out << variables[MOD].at(i)->getComponent(j)->getDataVariable();
                out << (quint16)variables[MOD].at(i)->getComponent(j)->page();

                out << (quint16)variables[MOD].at(i)->getComponent(j)->saveorigin.x();
                out << (quint16)variables[MOD].at(i)->getComponent(j)->saveorigin.y();
                out << (quint16)variables[MOD].at(i)->getComponent(j)->savedestination.x();
                out << (quint16)variables[MOD].at(i)->getComponent(j)->savedestination.y();
            }
        }

        // Add ItemType Data
        out << (quint8)ITT;
        out << properties[ITT].count();
        for(i=0; i < properties[ITT].count(); i++) {
            out << properties[ITT].at(i)->getName();
            out << (quint8)properties[ITT].at(i)->getDataType();
            out << (quint8)properties[ITT].at(i)->getValueType();
            out << properties[ITT].at(i)->isRequired();
            out << properties[ITT].at(i)->isEditable();
        }
        // No Variables or Components for ITT

        // Add Item Data for Each ItemType
        out << (quint8)ITM;
        for(i=0; i < properties[ITT].count(); i++) {
            QString ittname = properties[ITT].at(i)->getName();

            out << itemproperties[ittname].count();
            for(j=0; j < itemproperties[ittname].count(); j++) {
                out << itemproperties[ittname].at(j)->getName();
                out << (quint8)itemproperties[ittname].at(j)->getDataType();
                out << (quint8)itemproperties[ittname].at(j)->getValueType();
                out << itemproperties[ittname].at(j)->isRequired();
                out << itemproperties[ittname].at(j)->isEditable();
            }
            out << itemvariables[ittname].count();
            for(j=0; j < itemvariables[ittname].count(); j++) {
                out << itemvariables[ittname].at(j)->getName();
                values = itemvariables[ittname].at(j)->getValues();
                for(k=0; k < values.count(); k++) {
                    out << values.at(k)->getValue();
                }
                out << (quint16)itemvariables[ittname].at(j)->getComponentCount();
                for(k=0; k < itemvariables[ittname].at(j)->getComponentCount(); k++) {
                    out << itemvariables[ittname].at(j)->getComponent(k)->getDataName();
                    out << itemvariables[ittname].at(j)->getComponent(k)->getDataProperty();
                    out << itemvariables[ittname].at(j)->getComponent(k)->getDataVariable();
                    out << (quint16)itemvariables[ittname].at(j)->getComponent(k)->page();

                    out << (quint16)itemvariables[ittname].at(i)->getComponent(j)->saveorigin.x();
                    out << (quint16)itemvariables[ittname].at(i)->getComponent(j)->saveorigin.y();
                    out << (quint16)itemvariables[ittname].at(i)->getComponent(j)->savedestination.x();
                    out << (quint16)itemvariables[ittname].at(i)->getComponent(j)->savedestination.y();                }
            }
        }

        file.close();

        filename = fn;
        return 0;
    }
}

QString RPGRulebook::getFilename() { return filename; }

void RPGRulebook::updateProperties(DataType d, QList<RPGProperty *> list) {
    properties[d].clear();
    properties[d].append(list);
}

void RPGRulebook::updateProperties(QString d, QList<RPGProperty *> list) {
    itemproperties[d].clear();
    itemproperties[d].append(list);
}

void RPGRulebook::updateVariables(DataType d, QList<RPGVariable *> list) {
    variables[d].clear();
    variables[d].append(list);
}

void RPGRulebook::updateVariables(QString d, QList<RPGVariable *> list) {
    itemvariables[d].clear();
    itemvariables[d].append(list);
}

QList<RPGProperty *> RPGRulebook::getProperties(DataType t) {
    return properties[t];
}

QList<RPGProperty *> RPGRulebook::getProperties(QString t) {
    return itemproperties[t];
}

QList<RPGVariable *> RPGRulebook::getVariables(DataType t) {
    return variables[t];
}

QList<RPGVariable *> RPGRulebook::getVariables(QString t) {
    return itemvariables[t];
}

RPGRulebook* RPGRulebook::Instance() {
    return currentInstance;
}

void RPGRulebook::addComponent(RPGComponent *c) {
    int counter = 0;
    bool found = false;

    if(c->getDataName()=="Character Traits") {  // Character Trait - Add to Property
        qDebug("RPGRulebook::addComponent - Character Trait");
        while(counter < properties[CHR].count() && !found)
            if(properties[CHR].at(counter)->getName() == c->getDataProperty())
                found = true;
            else counter++;

        if(found)
            properties[CHR].at(counter)->addComponent(c);
    } else { // Add all others to the Variables
        if(c->getDataName()=="Attributes") {
            qDebug("RPGRulebook::addComponent - Attribute");
            while(counter < variables[ATT].count() && !found)
                if(variables[ATT].at(counter)->getName() == c->getDataVariable())
                    found = true;
                else counter++;
            if(found) qDebug("Attribute found."); else qDebug("Attribute not found.");
            if(found)
                variables[ATT].at(counter)->addComponent(c);
        } else if (c->getDataName()=="Skills") {
            qDebug("RPGRulebook::addComponent - Skill");
            while(counter < variables[SKI].count() && !found)
                if(variables[SKI].at(counter)->getName() == c->getDataVariable())
                    found = true;
                else counter++;

            if(found)
                variables[SKI].at(counter)->addComponent(c);
        } else if (c->getDataName()=="Modifiers") {
            qDebug("RPGRulebook::addComponent - Modifier");
            while(counter < variables[ATT].count() && !found)
                if(variables[MOD].at(counter)->getName() == c->getDataVariable())
                    found = true;
                else counter++;

            if(found)
                variables[MOD].at(counter)->addComponent(c);
        } else {    // Item Type
            qDebug("RPGRulebook::addComponent - Item");
            while(counter < itemvariables[c->getDataName()].count() && !found)
                if(itemvariables[c->getDataName()].at(counter)->getName() == c->getDataVariable())
                    found = true;
                else counter++;

            if(found)
                itemvariables[c->getDataName()].at(counter)->addComponent(c);
        }
    }
}

void RPGRulebook::removeComponent(RPGComponent *c) {
    int counter = 0;
    bool found = false;

    if(c->getDataName()=="Character Traits") {  // Character Trait - Add to Property
        while(counter < properties[CHR].count() && !found)
            if(properties[CHR].at(counter)->getName() == c->getDataProperty())
                found = true;
            else counter++;

        if(found)
            properties[CHR].at(counter)->removeComponent(c);
    } else { // Add all others to the Variables
        if(c->getDataName()=="Attributes") {
            while(counter < variables[ATT].count() && !found)
                if(variables[ATT].at(counter)->getName() == c->getDataVariable())
                    found = true;
                else counter++;

            if(found)
                variables[ATT].at(counter)->removeComponent(c);
        } else if (c->getDataName()=="Skills") {
            while(counter < variables[SKI].count() && !found)
                if(variables[SKI].at(counter)->getName() == c->getDataVariable())
                    found = true;
                else counter++;

            if(found)
                variables[SKI].at(counter)->removeComponent(c);
        } else if (c->getDataName()=="Modifiers") {
            while(counter < variables[ATT].count() && !found)
                if(variables[MOD].at(counter)->getName() == c->getDataVariable())
                    found = true;
                else counter++;

            if(found)
                variables[MOD].at(counter)->removeComponent(c);
        } else {    // Item Type
            while(counter < itemvariables[c->getDataName()].count() && !found)
                if(itemvariables[c->getDataName()].at(counter)->getName() == c->getDataVariable())
                    found = true;
                else counter++;

            if(found)
                itemvariables[c->getDataName()].at(counter)->removeComponent(c);
        }
    }
}

void RPGRulebook::addImage(int page, QImage image) {
    images[page] = image;
}

void RPGRulebook::removeImage(int page) {
    if(images.contains(page))
        images.remove(page);
}
