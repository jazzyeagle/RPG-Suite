#ifndef RPG_H
#define RPG_H

#include <QBuffer>
#include <QFile>
#include <QList>
#include <QMap>
#include <QObject>
#include <QString>
#include <QWidget>

#define RPG_HEADER 0x52504700   // RPG.
#define RBK_HEADER 0x52424B00   // RBK.
#define CUR_VERSION 0.1

// ValueType:  enum that indicates the type of expected value for a particular
//             variable within a Data Type
enum ValueType {
    text = 0,
    integer,    // Identifies whole numbers
    number,     // Identifies decimals allowed
    dice,       // Identifies dice calculation, e.g. 2D10, D20, 3D6 (+1?  TBD)
    currency,   // Identifies a currency type, which will allow member to add an integer and select the currency type under "value"
    attribute,  // Used for skills, mods and items that affect an attribute
    skill,      // Used for mods and items that affect a skill
    itemtype,   // Used for items to identify what type they belong to.
    boolean
};


// DataType:  Identifies the type of Data (Attribute, Skill, etc.)  The Data Type indicates
//            what value types will be available to to the user depending on their selections.
//            some will not be able to be created by hand on a form.  They are used for =
enum DataType {
    GAM = 0,    // Game (Basic attributes to every game, name, copyright, etc.)
    RBK,        // Rulebook properties (rulebook name, copyright, etc.)
    CUR,        // Currency
    CHR,        // Character Info (background, eye color, skin tone, etc.)
    ATT,        // Attribute
    SKI,        // Skill
    MOD,        // Modifiers (e.g. Edges/Flaws, Feats, etc.)
    ITT,        // Item Types
    ITM        // Item
//    ACT,        // Actions  // To be implemented later
};


enum ComponentType {
    none = 0,
    textbox,
    checkbox
};


class RPGComponent {
public:
    RPGComponent();
    RPGComponent(QWidget *w);

    virtual DataType      getDataType() =0;
    virtual void          setDataType(DataType dt) =0;
    virtual QString       getDataName() =0;
    virtual void          setDataName(QString dn) =0;
    virtual QString       getDataProperty() =0;
    virtual void          setDataProperty(QString dp) =0;
    virtual QString       getDataVariable() =0;
    virtual void          setDataVariable(QString dv) =0;
    virtual QString       getDataValue() =0;
    virtual void          setDataValue(QString dv) =0;
    virtual ComponentType getComponentType() =0;
    virtual void          setComponentType(ComponentType ct) =0;
    virtual quint8        page() =0;
    virtual void          setPage(quint8 p) =0;
    QPoint                saveorigin;
    QPoint                savedestination;

protected:
    quint8        pagenumber;
    ComponentType ctype;
    DataType      dtype;
    QString       compname;
    QString       compproperty;
    QString       compvariable;
    QString       compvalue;
};


// This class will be used as part of the RuleBook, Character Record, etc., which will
// maintain signals and slots.  No need to extend QObject.
class RPGProperty {
public:
    RPGProperty();
    RPGProperty(QString name, DataType d, ValueType v);
    RPGProperty(QString name, DataType d, ValueType v, bool req, bool edit);

    QString getName();
    void setName(QString n);

    DataType getDataType();
    void setDataType(DataType d);

    ValueType getValueType();
    void setValueType(ValueType v);

    QString getItemType();
    void    setItemType(QString t);

    RPGComponent* getComponent(int i);
    quint8        getComponentCount();
    void          addComponent(RPGComponent *c);
    void          removeComponent(RPGComponent *c);

    bool          isRequired();
    void          setRequired(bool req);
    bool          isEditable();
    void          setEditable(bool edit);

private:
    QString       name;          // name of the variable.
    DataType      datatype;
    ValueType     valuetype;     // value always stored as a string, but will be converted based on
    QString       itemtype;      // Used only for items
    bool          required;      // For properties the apps will expect, users cannot delete them.
    bool          editable;      // Used for game properties and supplements.
    QList<RPGComponent*> component;
};


class RPGVarProperty {
public:
    RPGVarProperty();
    RPGVarProperty(QString n, QString v);

    QString getName();
    void    setName(QString n);
    QString getValue();
    void    setValue(QString v);
private:
    QString name;
    QString value;
};

class RPGVariable {
public:
    RPGVariable();
    QString getName();
    void    setName(QString n);
    QList<RPGVarProperty *> getValues();
    QString getValue(QString n);
    void    setValue(QString n, QString v);
    RPGComponent* getComponent(int i);
    quint8        getComponentCount();
    void          addComponent(RPGComponent *c);
    void          removeComponent(RPGComponent *c);

private:
    QString name;
    QList<RPGVarProperty *> properties;
    QList<RPGComponent *> component;
};


// extending QObject so we can use signals and slots
class RPGRulebook : public QObject {
    Q_OBJECT
public:
    RPGRulebook();
    RPGRulebook(QString fn);
    ~RPGRulebook();
    int OpenRulebook(QString fn);
    int SaveRulebook(QString fn);
    QString              getFilename();
    QList<RPGProperty *> getProperties(DataType t);
    QList<RPGProperty *> getProperties(QString t);
    QList<RPGVariable *> getVariables(DataType t);
    QList<RPGVariable *> getVariables(QString t);
    static RPGRulebook  *Instance();
    void                 addComponent(RPGComponent *c);
    void                 removeComponent(RPGComponent *c);
    void                 addImage(int page, QImage image);
    void                 removeImage(int page);


signals:
    void loadImage(QImage img);
    void loadComponent(int p, QString name, QString prop, QString var, quint16 x, quint16 y, quint16 w, quint16 h);

public slots:
    void updateProperties(DataType d, QList<RPGProperty *> list);
    void updateProperties(QString d, QList<RPGProperty *> list);  // QString = Item Type
    void updateVariables(DataType d, QList<RPGVariable *> list);
    void updateVariables(QString d, QList<RPGVariable *> list);   // QString = Item Type

protected:
    QMap<DataType, QList<RPGProperty *> > properties;
    QMap<QString, QList<RPGProperty *> > itemproperties;
    QMap<DataType, QList<RPGVariable *> > variables;
    QMap<QString, QList<RPGVariable *> > itemvariables;
    QMap<int, QImage> images;   // int = page #

    static RPGRulebook *currentInstance;
    QString filename;
    bool core;

};

#endif // RPG_H
