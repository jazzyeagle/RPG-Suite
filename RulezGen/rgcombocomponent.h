#ifndef RGCOMBOCOMPONENT_H
#define RGCOMBOCOMPONENT_H

#include <QComboBox>
#include <QHBoxLayout>

class rgComboComponent : public QWidget {
public:
    rgComboComponent(QWidget *parent, QStringList list);
    rgComboComponent(QWidget *parent, QStringList list, QString value);
    QString value();
    void setValue(QString value);

private:
    QComboBox *box;
};

#endif // RGCOMBOCOMPONENT_H
