#include "cgmainwindow.h"

CGMainWindow::CGMainWindow(QWidget *parent) : QMainWindow(parent) {
    QMenuBar *bar = this->menuBar();
    QMenu *file = new QMenu(tr("&File"), this);
        file->addAction(tr("&New Character"), this, SLOT(onFileNew()), QKeySequence::New);
        file->addAction(tr("&Open Character"), this, SLOT(onFileOpen()), QKeySequence::Open);
        file->addAction(tr("&Close Character"), this, SLOT(onFileClose()), QKeySequence::Close);
        file->addSeparator();
        file->addAction(tr("&Save Character"), this, SLOT(onFileSave()), QKeySequence::Save);
        file->addAction(tr("Save Character &As"), this, SLOT(onFileSaveAs()), QKeySequence::SaveAs);
        file->addSeparator();
        file->addAction(tr("E&xit RulezGen"), this, SLOT(onFileExit()), QKeySequence::Quit);
    bar->addMenu(file);

    QMenu *help = new QMenu (tr("&Help"), this);
        help->addAction(tr("&About"), this, SLOT(onHelpAbout()));
    bar->addMenu(help);

    resize(600, 400);
    show();
}

CGMainWindow::~CGMainWindow() { }

void CGMainWindow::onFileNew() { }

void CGMainWindow::onFileOpen() { }

void CGMainWindow::onFileSave() { }

void CGMainWindow::onFileSaveAs() { }

void CGMainWindow::onFileClose() { }

void CGMainWindow::onFileExit() { close(); }
