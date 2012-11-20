#ifndef CGMAINWINDOW_H
#define CGMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QMenu>
#include <QMenuBar>

class CGMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit CGMainWindow(QWidget *parent = 0);
    ~CGMainWindow();
    
public slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onFileClose();
    void onFileExit();
private:
    QTabWidget *pages;
};

#endif // CGMAINWINDOW_H
