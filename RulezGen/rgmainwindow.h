#ifndef RGMAINWINDOW_H
#define RGMAINWINDOW_H

#include <QtGui>
#include "rgtabpage.h"
#include "rgpropertiesdialog.h"
#include "rgtabledialog.h"

class RGMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit RGMainWindow(QWidget *parent = 0);
    ~RGMainWindow();

signals:
    void setComponentType(ComponentType ct);

public slots:
    void onFileNew();
    void onFileOpen();
    void onFileClose();
    void onFileSave();
    void onFileSaveAs();
    void onFileExit();

    void onPagesAddPage();
    void onPagesLoadImage();
    void onPagesRemovePage();

    void onPropertiesGame();
    void onPropertiesRulebook();
    void onPropertiesCurrencies();
    void onPropertiesCharacter();
    void onPropertiesAttributes();
    void onPropertiesSkills();
    void onPropertiesModifiers();
    void onPropertiesItemTypes();
    void onPropertiesItems();
    // void onPropertiesActions(); // To be implemented later

    void onTablesGame();
    void onTablesRulebook();
    void onTablesCurrencies();
    //void onTablesCharacter();
    void onTablesAttributes();
    void onTablesSkills();
    void onTablesModifiers();
    void onTablesItems();
    //void onTablesActions();  // To be implemented later

    void onHelpAbout();

    void onToolDeselect();
    void onToolTextBox();
    void onToolCheckBox();

    void loadImage(QImage img);
    void loadComponent(int p, QString name, QString prop, QString var, quint16 x, quint16 y, quint16 w, quint16 h);
    
private:
    QTabWidget *pages;
    RPGRulebook *rulebook;
};

#endif // RGMAINWINDOW_H
