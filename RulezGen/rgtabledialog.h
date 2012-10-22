#ifndef RGTABLEDIALOG_H
#define RGTABLEDIALOG_H

#include <QtGui>
#include "rpg.h"
#include "rgdicecomponent.h"
#include "rgcombocomponent.h"
#include "rgcurrencycomponent.h"

class RGTableDialog : public QDialog
{
    Q_OBJECT
public:
    RGTableDialog(QWidget *parent, RPGRulebook *rbk, DataType t);

signals:
    void updateVariables(DataType, QList<RPGVariable*>);
    void updateVariables(QString, QList<RPGVariable*>);

public slots:
    void onAccept();
    void onReject();
    void addLine();
    void addLine(RPGVariable *var);
    void delLine();
    void onComboChange(QString newval);

private:
    QList<RPGVariable *> update(QString t);
    QGridLayout                         *grid;
    QMap<QString, QList<RPGProperty *> > properties;
    QMap<QString, QList<RPGVariable *> > variables;
    QTableWidget                        *table;
    QComboBox                           *typebox;
    QDialogButtonBox                    *buttons;
    QPushButton                         *addButton, *delButton;
    QSignalMapper                       *sm;
    int                                  current;
    DataType                             dtype;
    RPGRulebook                         *rulebook;
};


#endif // RGTABLEDIALOG_H
