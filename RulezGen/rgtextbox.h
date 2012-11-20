#ifndef RGTEXTBOX_H
#define RGTEXTBOX_H

#include <QLineEdit>
#include <QMenu>
#include <QMouseEvent>
#include "rpgrulebook.h"

class RGTextBox : public QLineEdit, public RPGComponent {
    Q_OBJECT
public:
    explicit RGTextBox(QWidget *parent, quint8 p, QPoint origin, QPoint destination);
    DataType      getDataType();
    void          setDataType(DataType dt);
    QString       getDataName();
    void          setDataName(QString dn);
    QString       getDataProperty();
    void          setDataProperty(QString dp);
    QString       getDataVariable();
    void          setDataVariable(QString dv);
    QString       getDataValue();
    void          setDataValue(QString dv);
    ComponentType getComponentType();
    void          setComponentType(ComponentType ct);
    quint8 page();
    void     setPage(quint8 p);

public slots:
    void     mousePressEvent(QMouseEvent *e);
    void     onMenuSelect(QAction *a);
};

#endif // RGTEXTBOX_H
