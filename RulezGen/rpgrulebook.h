#ifndef RPGRULEBOOK_H
#define RPGRULEBOOK_H

#include "rpg.h"
#include <QIcon>
#include <QDir>

#define RPG_HEADER 0x52504700   // RPG.
#define RBK_HEADER 0x52424B00   // RBK.
#define CUR_VERSION 0.2


// This class is only used for game selection in the new function of the Rulebook generator
// and the new function of the Character Generator
class RPGRulebookHeader {
public:
    RPGRulebookHeader();
    ~RPGRulebookHeader();
    bool    isCore();
    void    setCore(bool c);

    QString getGameName();
    void    setGameName(QString n);
    qreal   getVersion();
    void    setVersion(qreal v);
    QIcon   getIcon();
    void    setIcon(QIcon i);
    QString getGameId();
    void    setGameId(QString id);
    QString getGameCopyright();
    void    setGameCopyright(QString c);
    QString getGameDescription();
    void    setGameDescription(QString d);

    QString getRulebookName();
    void    setRulebookName(QString n);
    QString getRulebookFilename();
    void    setRulebookFilename(QString f);
    QString getRulebookCopyright();
    void    setRulebookCopyright(QString c);
    QString getRulebookDescription();
    void    setRulebookDescription(QString d);

    void    readv0p2Header(QDataStream *in);
    void    writev0p2Header(QDataStream *out);


private:
    bool    core;
    QString gamename, gameid, gamecopyright, gamedescription;
    qreal   gameversion;
    QString rulebookname, rulebookfilename, rulebookcopyright, rulebookdescription;
    QIcon   gameicon;
};


// extending QObject so we can use signals and slots
class RPGRulebook : public QObject {
    Q_OBJECT
public:
    RPGRulebook(RPGRulebookHeader *h);
    RPGRulebook();
    ~RPGRulebook();
    int OpenRulebook(QString fn);
    int SaveRulebook(QString fn);
    static QList<RPGRulebookHeader *> getHeaders();
    RPGRulebookHeader   *getHeader();
    void                 setHeader(RPGRulebookHeader *h);
    QString              getFilename();
    QList<RPGProperty *> getProperties(DataType t);
    QList<RPGProperty *> getProperties(DataType d, QString t);
    QList<RPGVariable *> getVariables(DataType t);
    QList<RPGVariable *> getVariables(DataType d, QString t);
    static RPGRulebook  *Instance();
    QIcon                getIcon();
    void                 addComponent(RPGComponent *c);
    void                 removeComponent(RPGComponent *c);
    void                 addImage(int page, QImage image);
    void                 removeImage(int page);


signals:
    void loadImage(QImage img);
    void loadComponent(int p, QString name, QString prop, QString var, quint16 x, quint16 y, quint16 w, quint16 h);

public slots:
    void updateProperties(DataType d, QList<RPGProperty *> list);
    void updateProperties(DataType t, QString d, QList<RPGProperty *> list);  // QString = Item Type
    void updateVariables(DataType d, QList<RPGVariable *> list);
    void updateVariables(DataType t, QString d, QList<RPGVariable *> list);   // QString = Item Type

protected:
    QMap<DataType, QList<RPGProperty *> > properties;
    QMap<QString, QList<RPGProperty *> > itemproperties;
    QMap<QString, QList<RPGProperty *> > modproperties;
    QMap<DataType, QList<RPGVariable *> > variables;
    QMap<QString, QList<RPGVariable *> > itemvariables;
    QMap<QString, QList<RPGVariable *> > modvariables;
    QMap<int, QImage> images;   // int = page #
    int OpenRulebookv0p1(QDataStream *in);
    int OpenRulebookv0p2(QDataStream *in);

    RPGRulebookHeader *header;
    static RPGRulebook *currentInstance;
    QString filename;
//    bool core;
};

#endif // RPGRULEBOOK_H
