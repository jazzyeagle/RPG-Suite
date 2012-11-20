#ifndef RGCHECKBOX_H
#define RGCHECKBOX_H

#include <QCheckBox>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include "rpgrulebook.h"

class RGCheckBox : public QCheckBox, public RPGComponent
{
    Q_OBJECT
public:
    explicit RGCheckBox(QWidget *parent, quint8 p, QPoint origin, QPoint destination);
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

    quint8   page();
    void     setPage(quint8 p);
    
public slots:
    void     mousePressEvent(QMouseEvent *e);
    void     onClick(QAction *a);    
};

#endif // RGCHECKBOX_H
