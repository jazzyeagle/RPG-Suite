#ifndef RGCURRENCYBOX_H
#define RGCURRENCYBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include "rpg.h"

class RGCurrencyBox : public QWidget {
    Q_OBJECT
public:
    RGCurrencyBox(QWidget *w, RPGRulebook *rulebook);
    RGCurrencyBox(QWidget *w, RPGRulebook *rulebook, QString value);
    QString getValue();
    QString getAmount();
    QString getCurrencyType();
        // Setting occurs during constructor, so no set functions needed
signals:
    
public slots:
    void onEdit();  //  Check to make sure only numbers are being used.

private:
    QGridLayout *layout;
    QLineEdit *amount;
    QComboBox *combo;
};

#endif // RGCURRENCYBOX_H
