#ifndef RPGRULEBOOK_H
#define RPGRULEBOOK_H

#include "rpg.h"


#define RPG_HEADER 0x52504700   // RPG.
#define RBK_HEADER 0x52424B00   // RBK.
#define CUR_VERSION 0.1


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

#endif // RPGRULEBOOK_H
