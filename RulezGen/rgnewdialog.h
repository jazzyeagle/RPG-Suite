#ifndef RGNEWDIALOG_H
#define RGNEWDIALOG_H

#include <QtGui>
#include "rpgrulebook.h"

class RGNewDialog : public QDialog
{
    Q_OBJECT
public:
    RGNewDialog(QWidget *parent = 0);
    RGNewDialog(QWidget *parent, RPGRulebookHeader *h);
    bool isCore();
    RPGRulebookHeader* rulebookHeader();
    QIcon getIcon();
    
signals:
    
public slots:
    void onRadioButtonChange();
    void onIconButtonClick();
    void onComboSelectionChange(int selection);

private:
    bool core;
    QTabWidget *tabs;
    QWidget    *gametab;
    QWidget    *rbktab;
    QDialogButtonBox *buttons;
    QList<RPGRulebookHeader *> headers;

    QComboBox *combo;
    QTextEdit *descriptionlabel;
    QLineEdit *copyrightlabel;

    QLineEdit      *gamename, *gameid, *gamecopyright, *gamedescription;
    QDoubleSpinBox *gameversion;
    QToolButton    *gameicon;
    QAction        *currenticon;

    QRadioButton *coreyes, *coreno;
    QLineEdit    *rbkname, *rbkfile, *rbkcopyright, *rbkdescription;
};

#endif // RGNEWDIALOG_H
