#ifndef CGTABPAGE_H
#define CGTABPAGE_H

#include <QtGui>
#include <QList>
#include "rpgrulebook.h"
//#include "rgtextbox.h"
//#include "rgcheckbox.h"


class CGTabPage : public QScrollArea
{
    Q_OBJECT
public:
    explicit CGTabPage(QWidget *parent, quint8 p);
            ~CGTabPage();
    void     setImage(QString filename);
    void     setImage(QImage image);
    quint8   page();
    void     setPage(quint8 p);
    void     addComponent(QString name, QString prop, QString var, quint16 x, quint16 y, quint16 w, quint16 h);

public slots:
    void     setComponentType(ComponentType ct);

private:
    QWidget              *panel;
    QLabel               *imagelabel;
    quint8                pagenumber;
    QPoint                origin;
    QPoint                destination;
    ComponentType         ctype;
    QList<RPGComponent *> components;
};

#endif // CGTABPAGE_H
