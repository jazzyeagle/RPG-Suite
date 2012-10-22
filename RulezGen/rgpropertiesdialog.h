#ifndef RGPROPERTIESDIALOG_H
#define RGPROPERTIESDIALOG_H

#include <QtGui>
#include "rpg.h"

class RGPropertiesDialog : public QDialog {
    Q_OBJECT
public:
    RGPropertiesDialog(QWidget *parent, RPGRulebook *rbk, DataType t);
    
signals:
    void updateProperties(DataType d, QList<RPGProperty *> properties);
    void updateProperties(QString d, QList<RPGProperty *> properties);

public slots:
    void onAccept();
    void onReject();

    void addLine();
    void addLine(RPGProperty *var);
    void delLine();
    void onComboChange(QString newval);
    void onCellActivated();

private:
    QList<RPGProperty *> update();

    QGridLayout                          *grid;
    QTableWidget                         *table;
    QDialogButtonBox                     *buttons;
    QPushButton                          *addButton, *delButton;
    QSignalMapper                        *sm;
    RPGRulebook                          *rulebook;
    QMap<QString, QList<RPGProperty *> >  properties;
    DataType                              dtype;
    QComboBox                            *typebox;  // Used for items only
    int                                   current;  // Current index of typebox - Tracked separately in case selection needs to be reset.
};

#endif // RGPROPERTIESDIALOG_H
