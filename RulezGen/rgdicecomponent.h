#ifndef RGDICECOMPONENT_H
#define RGDICECOMPONENT_H

#include <QtGui>

class rgDiceComponent : public QWidget
{
    Q_OBJECT
public:
    rgDiceComponent(QWidget *parent = 0);
    rgDiceComponent(QWidget *parent, QString data);
    QString value();
    void setValue(QString v);
signals:
    
public slots:
    
private:
    QSpinBox *numdice;
    QComboBox *dicetype;    // Yes, I know singular is technically die.  I don't care.  Deal with it.  :)
};

#endif // RGDICECOMPONENT_H
