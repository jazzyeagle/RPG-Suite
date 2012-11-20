#include "rpgrulebook.h"
#include <QMessageBox>


// RPGRulebookHeader functions
RPGRulebookHeader::RPGRulebookHeader() {
    setCore(true);
    setGameName(QString());
    setVersion(0.0);
    setIcon(QIcon());
    setGameId(QString());
    setGameCopyright(QString());
    setGameDescription(QString());

    setRulebookName(QString());
    setRulebookFilename(QString());
    setRulebookCopyright(QString());
    setRulebookDescription(QString());
}

RPGRulebookHeader::~RPGRulebookHeader() { }

bool    RPGRulebookHeader::isCore() { return core; }
void    RPGRulebookHeader::setCore(bool c) { core = c; }

QString RPGRulebookHeader::getGameName() { return gamename; }
void    RPGRulebookHeader::setGameName(QString n) { gamename = n; }
qreal   RPGRulebookHeader::getVersion() { return gameversion; }
void    RPGRulebookHeader::setVersion(qreal v) { gameversion = v; }
QIcon   RPGRulebookHeader::getIcon() { return gameicon; }
void    RPGRulebookHeader::setIcon(QIcon i) { gameicon = i; }
QString RPGRulebookHeader::getGameId() { return gameid; }
void    RPGRulebookHeader::setGameId(QString id) { gameid = id; }
QString RPGRulebookHeader::getGameCopyright() { return gamecopyright; }
void    RPGRulebookHeader::setGameCopyright(QString c) { gamecopyright = c; }
QString RPGRulebookHeader::getGameDescription() { return gamedescription; }
void    RPGRulebookHeader::setGameDescription(QString d) { gamedescription = d; }

QString RPGRulebookHeader::getRulebookName() { return rulebookname; }
void    RPGRulebookHeader::setRulebookName(QString n) { rulebookname = n; }
QString RPGRulebookHeader::getRulebookFilename() { return rulebookfilename; }
void    RPGRulebookHeader::setRulebookFilename(QString f) { rulebookfilename = f; }
QString RPGRulebookHeader::getRulebookCopyright() { return rulebookcopyright; }
void    RPGRulebookHeader::setRulebookCopyright(QString c) { rulebookcopyright = c; }
QString RPGRulebookHeader::getRulebookDescription() { return rulebookdescription; }
void    RPGRulebookHeader::setRulebookDescription(QString d) { rulebookdescription = d; }

void RPGRulebookHeader::readv0p2Header(QDataStream *in) {
    *in >> core;
    *in >> gamename;
    *in >> gameversion;
    *in >> gameicon;
    *in >> gameid;
    *in >> gamecopyright;
    *in >> gamedescription;
    *in >> rulebookname;
    *in >> rulebookfilename;
    *in >> rulebookcopyright;
    *in >> rulebookdescription;
}

void RPGRulebookHeader::writev0p2Header(QDataStream *out) {
    *out << isCore();
    *out << getGameName();
    *out << getVersion();
    *out << getIcon();
    *out << getGameId();
    *out << getGameCopyright();
    *out << getGameDescription();
    *out << getRulebookName();
    *out << getRulebookFilename();
    *out << getRulebookCopyright();
    *out << getRulebookDescription();
}


// RPGRulebook functions

// Static declaration
RPGRulebook* RPGRulebook::currentInstance = NULL;

// This constructor used when user selects a new Rulebook or when app opens
RPGRulebook::RPGRulebook(RPGRulebookHeader *h) : QObject() {
    currentInstance = this;

    header = h;

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

    // Add modifier types
    QList<RPGProperty *>modifiertype_properties;
        modifiertype_properties.append(new RPGProperty(tr("New Property"), MDT, text, true, false));
    properties[MDT] = modifiertype_properties;

    // Add Item Type properties
    QList<RPGProperty *> itemtype_properties;
        itemtype_properties.append(new RPGProperty(tr("New Property"), ITT, text, true, false));
    properties[ITT] = itemtype_properties;
}

RPGRulebook::RPGRulebook() : QObject() {
    currentInstance = this;
}

RPGRulebook::~RPGRulebook() {
    currentInstance = NULL;
}

int RPGRulebook::OpenRulebook(QString fn) {
    QFile file(fn);

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
        if(version == 0.1) {
            in.setVersion(QDataStream::Qt_4_8);
            OpenRulebookv0p1(&in);
        }

        if(version == 0.2) {
            in.setVersion(QDataStream::Qt_4_8);
            OpenRulebookv0p2(&in);
        }

        file.close();
        filename = fn;
        return 0;
}



int RPGRulebook::OpenRulebookv0p1(QDataStream *in) {
    bool core;
    int i, j, k, count;
    quint16 compcount, p, x, y, w, h;
    quint8 datatype; DataType dtype;
    QString propname; quint8 dt, vt; bool r, e;   // Properties Temp variables
    QString varname, name, value;                 // Variables Temp variables
    QImage img;


        *in >> core;

        if(core) {
            *in >> count;    // # of images
            for(i = 0; i < count; i++) {
                *in >> images[i];
                emit loadImage(images[i]);
            }
        }
        // add other data
        while (!in->atEnd()) {
            *in >> datatype; dtype = (DataType)datatype;

            switch(dtype) {
                case GAM:
                case RBK:
                case CUR:
                    *in >> count;
                    for(i=0; i < count; i++) {
                        *in >> propname >> dt >> vt >> r >> e;
                        properties[dtype].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                    }
                    *in >> count;
                    for(i = 0; i < count; i++) {
                        RPGVariable *v = new RPGVariable();
                        *in >> varname; // Read in variable name
                        v->setName(varname);
                        for(j=0; j < properties[dtype].count(); j++) {
                            *in >> value;
                            v->setValue(properties[dtype].at(j)->getName(), value);
                        }
                        variables[dtype].append(v);
                    }
                    break;
                case ATT:
                case SKI:
                case MOD:
                    *in >> count;
                    for(i=0; i < count; i++) {
                        *in >> propname >> dt >> vt >> r >> e;
                        properties[dtype].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                    }
                    *in >> count;
                    for(i = 0; i < count; i++) {
                        RPGVariable *v = new RPGVariable();
                        *in >> varname; // Read in variable name
                        v->setName(varname);
                        for(j=0; j < properties[dtype].count(); j++) {
                            *in >> value;
                            v->setValue(properties[dtype].at(j)->getName(), value);
                        }
                        variables[dtype].append(v);
                        *in >> compcount;
                        for(j=0; j < compcount; j++) {
                            *in >> name >> propname >> varname >> p >> x >> y >> w >> h;
                            // Create Component
                            emit loadComponent(p, name, propname, varname, x, y, w, h);
                        }
                    }
                    break;
                case CHR:
                    *in >> count;
                    for(i=0; i < count; i++) {
                        *in >> propname >> dt >> vt >> r >> e;
                        properties[dtype].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                        *in >> compcount;
                        for(j=0; j < compcount; j++) {
                            *in >> name >> propname >> p >> x >> y >> w >> h;
                            // Create Component
                            emit loadComponent(p, name, propname, varname, x, y, w, h);
                        }
                    }
                    break;
                case ITT:
                    *in >> count;
                    for(i=0; i < count; i++) {
                        *in >> propname >> dt >> vt >> r >> e;
                        properties[dtype].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                    }
                    break;                
                case ITM:
                    for(i=0; i < properties[ITT].count(); i++) {            // ITT is in the file before ITM, so it's
                        QString ittname = properties[ITT].at(i)->getName(); // safe to assume these are already loaded.
                        *in >> count;
                        for(j=0; j < count; j++) {
                            *in >> propname >> dt >> vt >> r >> e;
                            itemproperties[ittname].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                        }
                        *in >> count;
                        for(j=0; j < count; j++) {
                            RPGVariable *v = new RPGVariable();
                            *in >> varname;  // Read in variable name
                            v->setName(varname);
                            for(k=0; k < itemproperties[ittname].count(); k++) {
                                *in >> value;
                                v->setValue(itemproperties[ittname].at(k)->getName(), value);
                            }
                            itemvariables[ittname].append(v);
                            *in >> compcount;
                            for(k=0; k < compcount; k++) {
                                *in >> name >> propname >> varname >> p >> x >> y >> w >> h;
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

        return 0;
}


// Version 0.2:  Rearranged GAM & RBK info in front of the image info,
//               added Icon support for new dialog box & Window.
int RPGRulebook::OpenRulebookv0p2(QDataStream *in) {
    int i, j, k, count;
    quint16 compcount, p, x, y, w, h;
    quint8 datatype; DataType dtype;
    QString propname; quint8 dt, vt; bool r, e;   // Properties Temp variables
    QString varname, name, value;                 // Variables Temp variables
    QImage img;

    header = new RPGRulebookHeader();
    header->readv0p2Header(in);

    if(header->isCore()) {
        *in >> count;    // # of images
        for(i = 0; i < count; i++) {
            *in >> img;
            emit loadImage(img);
        }
    }
        // load other data
        while (!in->atEnd()) {
            *in >> datatype; dtype = (DataType)datatype;

            switch(dtype) {
                case CUR:
                    *in >> count;
                    for(i=0; i < count; i++) {
                        *in >> propname >> dt >> vt >> r >> e;
                        properties[dtype].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                    }
                    *in >> count;
                    for(i = 0; i < count; i++) {
                        RPGVariable *v = new RPGVariable();
                        *in >> varname; // Read in variable name
                        v->setName(varname);
                        for(j=0; j < properties[dtype].count(); j++) {
                            *in >> value;
                            v->setValue(properties[dtype].at(j)->getName(), value);
                        }
                        variables[dtype].append(v);
                    }
                    break;
                case ATT:
                case SKI:
                    *in >> count;
                    for(i=0; i < count; i++) {
                        *in >> propname >> dt >> vt >> r >> e;
                        properties[dtype].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                    }
                    *in >> count;
                    for(i = 0; i < count; i++) {
                        RPGVariable *v = new RPGVariable();
                        *in >> varname; // Read in variable name
                        v->setName(varname);
                        for(j=0; j < properties[dtype].count(); j++) {
                            *in >> value;
                            v->setValue(properties[dtype].at(j)->getName(), value);
                        }
                        variables[dtype].append(v);
                        *in >> compcount;
                        for(j=0; j < compcount; j++) {
                            *in >> name >> propname >> varname >> p >> x >> y >> w >> h;
                            // Create Component
                            emit loadComponent(p, name, propname, varname, x, y, w, h);
                        }
                    }
                    break;
                case CHR:
                    *in >> count;
                    for(i=0; i < count; i++) {
                        *in >> propname >> dt >> vt >> r >> e;
                        properties[dtype].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                        *in >> compcount;
                        for(j=0; j < compcount; j++) {
                            *in >> name >> propname >> p >> x >> y >> w >> h;
                            // Create Component
                            emit loadComponent(p, name, propname, varname, x, y, w, h);
                        }
                    }
                    break;
                case MDT:
                case ITT:
                    *in >> count;
                    for(i=0; i < count; i++) {
                        *in >> propname >> dt >> vt >> r >> e;
                        properties[dtype].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                    }
                    break;
                case MOD:
                    for(i=0; i < properties[MDT].count(); i++) {            // ITT is in the file before ITM, so it's
                        QString mdtname = properties[MDT].at(i)->getName(); // safe to assume these are already loaded.
                        *in >> count;
                        for(j=0; j < count; j++) {
                            *in >> propname >> dt >> vt >> r >> e;
                            modproperties[mdtname].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                        }
                        *in >> count;
                        for(j=0; j < count; j++) {
                            RPGVariable *v = new RPGVariable();
                            *in >> varname;  // Read in variable name
                            v->setName(varname);
                            for(k=0; k < modproperties[mdtname].count(); k++) {
                                *in >> value;
                                v->setValue(modproperties[mdtname].at(k)->getName(), value);
                            }
                            modvariables[mdtname].append(v);
                            *in >> compcount;
                            for(k=0; k < compcount; k++) {
                                *in >> name >> propname >> varname >> p >> x >> y >> w >> h;
                                // Create Component
                                emit loadComponent(p, name, propname, varname, x, y, w, h);
                            }
                        }
                    }
                    break;
                case ITM:
                    for(i=0; i < properties[ITT].count(); i++) {            // ITT is in the file before ITM, so it's
                        QString ittname = properties[ITT].at(i)->getName(); // safe to assume these are already loaded.
                        *in >> count;
                        for(j=0; j < count; j++) {
                            *in >> propname >> dt >> vt >> r >> e;
                            itemproperties[ittname].append(new RPGProperty(propname, (DataType)dt, (ValueType)vt, r, e));
                        }
                        *in >> count;
                        for(j=0; j < count; j++) {
                            RPGVariable *v = new RPGVariable();
                            *in >> varname;  // Read in variable name
                            v->setName(varname);
                            for(k=0; k < itemproperties[ittname].count(); k++) {
                                *in >> value;
                                v->setValue(itemproperties[ittname].at(k)->getName(), value);
                            }
                            itemvariables[ittname].append(v);
                            *in >> compcount;
                            for(k=0; k < compcount; k++) {
                                *in >> name >> propname >> varname >> p >> x >> y >> w >> h;
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

        return 0;
}


int RPGRulebook::SaveRulebook(QString fn) {
    QFile file(fn);
    QByteArray img;
    int i, j, k;
    QList<RPGVarProperty *> values;

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

        header->writev0p2Header(&out);

        if(header->isCore()) {
            out << images.count();
            QMapIterator<int, QImage> map(images);
            while(map.hasNext()) {
                map.next();
                out << map.value();
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

        // Add Modifier Type Data
        // Add ItemType Data
        out << (quint8)MDT;
        out << properties[MDT].count();
        for(i=0; i < properties[MDT].count(); i++) {
            out << properties[MDT].at(i)->getName();
            out << (quint8)properties[MDT].at(i)->getDataType();
            out << (quint8)properties[MDT].at(i)->getValueType();
            out << properties[MDT].at(i)->isRequired();
            out << properties[MDT].at(i)->isEditable();
        }
        // No Variables or Components for MDT

        // Add Modifier Data for Each Modifier Type
        out << (quint8)MOD;
        for(i=0; i < properties[MDT].count(); i++) {
            QString mdtname = properties[MDT].at(i)->getName();

            out << modproperties[mdtname].count();
            for(j=0; j < modproperties[mdtname].count(); j++) {
                out << modproperties[mdtname].at(j)->getName();
                out << (quint8)modproperties[mdtname].at(j)->getDataType();
                out << (quint8)modproperties[mdtname].at(j)->getValueType();
                out << modproperties[mdtname].at(j)->isRequired();
                out << modproperties[mdtname].at(j)->isEditable();
            }
            out << modvariables[mdtname].count();
            for(j=0; j < modvariables[mdtname].count(); j++) {
                out << modvariables[mdtname].at(j)->getName();
                values = modvariables[mdtname].at(j)->getValues();
                for(k=0; k < values.count(); k++) {
                    out << values.at(k)->getValue();
                }
                out << (quint16)modvariables[mdtname].at(j)->getComponentCount();
                for(k=0; k < modvariables[mdtname].at(j)->getComponentCount(); k++) {
                    out << modvariables[mdtname].at(j)->getComponent(k)->getDataName();
                    out << modvariables[mdtname].at(j)->getComponent(k)->getDataProperty();
                    out << modvariables[mdtname].at(j)->getComponent(k)->getDataVariable();
                    out << (quint16)modvariables[mdtname].at(j)->getComponent(k)->page();

                    out << (quint16)modvariables[mdtname].at(i)->getComponent(j)->saveorigin.x();
                    out << (quint16)modvariables[mdtname].at(i)->getComponent(j)->saveorigin.y();
                    out << (quint16)modvariables[mdtname].at(i)->getComponent(j)->savedestination.x();
                    out << (quint16)modvariables[mdtname].at(i)->getComponent(j)->savedestination.y();                }
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

// This static function provides basic game info for "new" functions in RulezGen & CharGen.
// If a file does not match a rulebook format, that file will simply be skipped.  No warnings.
QList<RPGRulebookHeader *> RPGRulebook::getHeaders() {
    QList<RPGRulebookHeader *> headers;
    QDir dir = QDir::current();
    QFile currfile;
    QDataStream in;
    quint32 headercheck;
    qreal version;

    if(dir.cd("Rulebooks")) {
        QFileInfoList files = dir.entryInfoList(QStringList("*.rbk"));
        for(int i=0; i<files.count(); i++) {
            currfile.setFileName(files.at(i).absoluteFilePath());
            if(currfile.open(QFile::ReadOnly)) {
                in.setDevice(&currfile);

                in >> headercheck;
                if(headercheck==(quint32)RPG_HEADER) {
                    in >> headercheck;
                    if(headercheck==(quint32)RBK_HEADER) {
                        in >> version;
                        //if(version==0.1) else
                        if(version==0.2) {
                            in.setVersion(QDataStream::Qt_4_8);
                            RPGRulebookHeader *h = new RPGRulebookHeader();
                            h->readv0p2Header(&in);
                            headers.append(h);
                        }
                    }
                }
                currfile.close();
            }
        }
    }
    return headers;
}



QString RPGRulebook::getFilename() { return filename; }

void RPGRulebook::updateProperties(DataType d, QList<RPGProperty *> list) {
    properties[d].clear();
    properties[d].append(list);
}

void RPGRulebook::updateProperties(DataType t, QString d, QList<RPGProperty *> list) {
    if(t==MOD) {
        modproperties[d].clear();
        modproperties[d].append(list);
    } else if(t==ITM) {
        itemproperties[d].clear();
        itemproperties[d].append(list);
    }
}

void RPGRulebook::updateVariables(DataType d, QList<RPGVariable *> list) {
    variables[d].clear();
    variables[d].append(list);
}

void RPGRulebook::updateVariables(DataType t, QString d, QList<RPGVariable *> list) {
    if(t==MOD) {
        modvariables[d].clear();
        modvariables[d].append(list);
    } else if (t==ITM) {
        itemvariables[d].clear();
        itemvariables[d].append(list);
    }
}

QList<RPGProperty *> RPGRulebook::getProperties(DataType t) {
    return properties[t];
}

QList<RPGProperty *> RPGRulebook::getProperties(DataType d, QString t) {
    if(d==MOD)
        return modproperties[t];
    else if(d==ITM)
        return itemproperties[t];
    else                // We should never hit this.  Just adding to appease the compiler.
        return QList<RPGProperty *>();
}

QList<RPGVariable *> RPGRulebook::getVariables(DataType t) {
    return variables[t];
}

QList<RPGVariable *> RPGRulebook::getVariables(DataType d, QString t) {
    if(d==MOD)
        return modvariables[t];
    else if(d==ITM)
        return itemvariables[t];
    else            // We should never hit this.  Just adding to appease the compiler.
        return QList<RPGVariable *>();
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
/*        } else if (c->getDataName()=="Modifiers") {
            qDebug("RPGRulebook::addComponent - Modifier");
            while(counter < variables[ATT].count() && !found)
                if(variables[MOD].at(counter)->getName() == c->getDataVariable())
                    found = true;
                else counter++;

            if(found)
                variables[MOD].at(counter)->addComponent(c); */
        } else {    // Modifier or Item - Cycle through Modifiers first.  If not found,
                    //      cycle through items.
            while(counter < modvariables[c->getDataName()].count() && !found) {
                if(modvariables[c->getDataName()].at(counter)->getName() == c->getDataVariable())
                    found = true;
                else counter++;
            }
            if(found) {      // If it is a modifier...
                qDebug("RPGRulebook::addComponent - Modifier");
                modvariables[c->getDataName()].at(counter)->addComponent(c);
            } else {
                counter = 0;  // reset counter to cycle through items
                while(counter < itemvariables[c->getDataName()].count() && !found)
                    if(itemvariables[c->getDataName()].at(counter)->getName() == c->getDataVariable())
                        found = true;
                    else counter++;

                if(found) {
                    qDebug("RPGRulebook::addComponent - Item");
                    itemvariables[c->getDataName()].at(counter)->addComponent(c);
                } else qDebug("RPGRulebook::addComponent - Not Found");
            }
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

RPGRulebookHeader* RPGRulebook::getHeader() { return header;}

void RPGRulebook::setHeader(RPGRulebookHeader *h) {
    delete header;
    header = h;
}

QIcon RPGRulebook::getIcon() {
    return header->getIcon();
}
