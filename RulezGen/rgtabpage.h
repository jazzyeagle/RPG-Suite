#ifndef RGTABPAGE_H
#define RGTABPAGE_H

#include <QtGui>
#include <QRubberBand>
#include <QList>
#include "rpg.h"
#include "rgtextbox.h"
#include "rgcheckbox.h"


class RGTabPage : public QScrollArea
{
    Q_OBJECT
public:
    explicit RGTabPage(QWidget *parent, quint8 p);
            ~RGTabPage();
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
    QRubberBand          *rubberband;
    void                  mousePressEvent(QMouseEvent *e);
    void                  mouseMoveEvent(QMouseEvent *e);
    void                  mouseReleaseEvent(QMouseEvent *e);
    ComponentType         ctype;
    QList<RPGComponent *> components;
};

#endif // RGTABPAGE_H
