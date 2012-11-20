#include "rgmainwindow.h"

RGMainWindow::RGMainWindow(QWidget *parent) : QMainWindow(parent) {
    rulebook = new RPGRulebook();
    connect(rulebook, SIGNAL(loadImage(QImage)), this, SLOT(loadImage(QImage)));
    setWindowTitle(tr("RulezGen"));
    // Create Menus
    QMenuBar *bar = this->menuBar();
    QMenu *file = new QMenu(tr("&File"), this);
        file->addAction(tr("&New Rulebook"), this, SLOT(onFileNew()), QKeySequence::New);
        file->addAction(tr("&Open Rulebook"), this, SLOT(onFileOpen()), QKeySequence::Open);
        file->addAction(tr("&Close Rulebook"), this, SLOT(onFileClose()), QKeySequence::Close);
        file->addSeparator();
        file->addAction(tr("&Save Rulebook"), this, SLOT(onFileSave()), QKeySequence::Save);
        file->addAction(tr("Save Rulebook &As"), this, SLOT(onFileSaveAs()), QKeySequence::SaveAs);
        file->addSeparator();
        file->addAction(tr("E&xit RulezGen"), this, SLOT(onFileExit()), QKeySequence::Quit);
    bar->addMenu(file);
    QMenu *properties = new QMenu(tr("P&roperties"), this);
        properties->addAction(tr("&Game/Rulebook"), this, SLOT(onPropertiesGame()));
        properties->addAction(tr("&Character"), this, SLOT(onPropertiesCharacter()));
        properties->addAction(tr("C&urrencies"), this, SLOT(onPropertiesCurrencies()));
        properties->addAction(tr("&Attributes"), this, SLOT(onPropertiesAttributes()));
        properties->addAction(tr("&Skills"), this, SLOT(onPropertiesSkills()));
        properties->addAction(tr("Modifier T&ypes"), this, SLOT(onPropertiesModifierTypes()));
        properties->addAction(tr("&Modifiers"), this, SLOT(onPropertiesModifiers()));
        properties->addAction(tr("Item &Types"), this, SLOT(onPropertiesItemTypes()));
        properties->addAction(tr("&Items"), this, SLOT(onPropertiesItems()));
    bar->addMenu(properties);
    QMenu *tables = new QMenu(tr("&Tables"), this);
        // Game/Rulebook info entered under properties.  No need for table here.
        // Character info entered only by user in CharGen.  No need for table here.
        tables->addAction(tr("C&urrencies"), this, SLOT(onTablesCurrencies()));
        tables->addAction(tr("&Attributes"), this, SLOT(onTablesAttributes()));
        tables->addAction(tr("&Skills"), this, SLOT(onTablesSkills()));
        tables->addAction(tr("&Modifiers"), this, SLOT(onTablesModifiers()));
        tables->addAction(tr("&Items"), this, SLOT(onTablesItems()));
        //tables->addAction(tr("Ac&tions"), this, SLOT(onTablesActions())); // To be implemented later
    bar->addMenu(tables);
    QMenu *page = new QMenu(tr("&Pages"), this);
        page->addAction(tr("&Add Page"), this, SLOT(onPagesAddPage()), Qt::Key_F2);
        page->addAction(tr("&Load Image"), this, SLOT(onPagesLoadImage()), Qt::Key_F3);
        page->addAction(tr("&Remove Page"), this, SLOT(onPagesRemovePage()), Qt::Key_F4);
    bar->addMenu(page);
    QMenu *help = new QMenu (tr("&Help"), this);
        help->addAction(tr("&About"), this, SLOT(onHelpAbout()));
    bar->addMenu(help);

    // Create Toolbar
    QToolBar *tool = new QToolBar(this);
        tool->addAction(QIcon(":/images/blank.png"),    tr("Deselect"),      this, SLOT(onToolDeselect()));
        tool->addAction(QIcon(":/images/textbox.png"),  tr("Draw TextBox"),  this, SLOT(onToolTextBox()));
        tool->addAction(QIcon(":/images/checkbox.png"), tr("Draw CheckBox"), this, SLOT(onToolCheckBox()));
    this->addToolBar(tool);

    // Create Tab Widget
    pages = new QTabWidget(this);
    setCentralWidget(pages);
    resize(600, 400);
    show();
}

RGMainWindow::~RGMainWindow() {
}


// Event Handlers //
void RGMainWindow::onFileNew() {
    RGNewDialog *dlg = new RGNewDialog(this);
    dlg->exec();

    if(dlg->result()==QDialog::Accepted) {
        if(rulebook != NULL) {
            delete rulebook;
            pages->clear();
        }
        rulebook = new RPGRulebook(dlg->rulebookHeader());
        setWindowIcon(dlg->getIcon());
    }
}


void RGMainWindow::onFileOpen() {
    QString openname = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Rulebook Files (*.rbk)"));
    if(openname!="")
        if(rulebook != NULL) {
            delete rulebook;
            pages->clear();
        }
        rulebook = new RPGRulebook();
        connect(rulebook, SIGNAL(loadImage(QImage)), this, SLOT(loadImage(QImage)));
        connect(rulebook, SIGNAL(loadComponent(int, QString, QString, QString, quint16, quint16, quint16, quint16)),
                    this,  SLOT(loadComponent(int, QString, QString, QString, quint16, quint16, quint16, quint16)));
        rulebook->OpenRulebook(openname);
        setWindowIcon(rulebook->getIcon());
}

void RGMainWindow::onFileClose() { }

void RGMainWindow::onFileSave() {
    if(RPGRulebook::Instance()->getFilename().isEmpty())
        onFileSaveAs();
    else RPGRulebook::Instance()->SaveRulebook(RPGRulebook::Instance()->getFilename());
}

void RGMainWindow::onFileSaveAs() {
    QString savename = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Rulebook Files (*.rbk)"));
    if(savename!="") {
        RPGRulebook::Instance()->SaveRulebook(savename);
    }
}
void RGMainWindow::onFileExit() { close(); }

void RGMainWindow::onPagesAddPage() {
    RGTabPage *page = new RGTabPage(this, pages->count()+1);
    connect(this, SIGNAL(setComponentType(ComponentType)), page, SLOT(setComponentType(ComponentType)));
    pages->addTab(page, QString(tr("Page #%1")).arg(page->page()));
}

void RGMainWindow::onPagesLoadImage() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Select Page Image"), QDir::currentPath(), tr("Image Files (*.png *.jpg *.gif);;All Files (*.*)"));
    if(filename != NULL) {
        RGTabPage *temp = (RGTabPage*)pages->currentWidget();
        temp->setImage(filename);
    }
}

void RGMainWindow::onPagesRemovePage() {
    QMessageBox qmb;
    qmb.setWindowTitle(tr("Warning!"));
    qmb.setText(tr("If you delete this page, all components on this page will be permanently lost.  Do you wish to continue?"));
    qmb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    qmb.setDefaultButton(QMessageBox::No);
    if(qmb.exec()==QMessageBox::Yes) {
        pages->removeTab(pages->currentIndex());

        for(int i=0; i<pages->count(); i++) {
            pages->setTabText(i, QString(tr("Page #%1")).arg(i+1));
        }
    }
}

void RGMainWindow::onPropertiesGame() {
    RGNewDialog *dlg = new RGNewDialog(this, RPGRulebook::Instance()->getHeader());
    dlg->exec();

    if(dlg->result()==QDialog::Accepted) {
        rulebook->setHeader(dlg->rulebookHeader());
        setWindowIcon(dlg->getIcon());
    }
}

void RGMainWindow::onPropertiesCurrencies() {
    RGPropertiesDialog *dialog = new RGPropertiesDialog(this, rulebook, CUR);
    connect(dialog, SIGNAL(updateProperties(DataType,QList<RPGProperty*>)), rulebook, SLOT(updateProperties(DataType,QList<RPGProperty*>)));
    dialog->exec();
}

void RGMainWindow::onPropertiesCharacter() {
    RGPropertiesDialog *dialog = new RGPropertiesDialog(this, rulebook, CHR);
    connect(dialog, SIGNAL(updateProperties(DataType,QList<RPGProperty*>)), rulebook, SLOT(updateProperties(DataType,QList<RPGProperty*>)));
    dialog->exec();
}

void RGMainWindow::onPropertiesAttributes() {
    RGPropertiesDialog *dialog = new RGPropertiesDialog(this, rulebook, ATT);
    connect(dialog, SIGNAL(updateProperties(DataType,QList<RPGProperty*>)), rulebook, SLOT(updateProperties(DataType,QList<RPGProperty*>)));
    dialog->exec();
}

void RGMainWindow::onPropertiesSkills() {
    RGPropertiesDialog *dialog = new RGPropertiesDialog(this, rulebook, SKI);
    connect(dialog, SIGNAL(updateProperties(DataType,QList<RPGProperty*>)), rulebook, SLOT(updateProperties(DataType,QList<RPGProperty*>)));
    dialog->exec();
}

void RGMainWindow::onPropertiesModifierTypes() {
    RGPropertiesDialog *dialog = new RGPropertiesDialog(this, rulebook, MDT);
    connect(dialog, SIGNAL(updateProperties(DataType,QList<RPGProperty*>)), rulebook, SLOT(updateProperties(DataType,QList<RPGProperty*>)));
    dialog->exec();
}

void RGMainWindow::onPropertiesModifiers() {
    RGPropertiesDialog *dialog = new RGPropertiesDialog(this, rulebook, MOD);
    connect(dialog, SIGNAL(updateProperties(DataType,QString,QList<RPGProperty*>)), rulebook, SLOT(updateProperties(DataType,QString,QList<RPGProperty*>)));
    dialog->exec();
}

void RGMainWindow::onPropertiesItemTypes() {
    RGPropertiesDialog *dialog = new RGPropertiesDialog(this, rulebook, ITT);
    connect(dialog, SIGNAL(updateProperties(DataType,QList<RPGProperty*>)), rulebook, SLOT(updateProperties(DataType,QList<RPGProperty*>)));
    dialog->exec();
}

void RGMainWindow::onPropertiesItems() {
    RGPropertiesDialog *dialog = new RGPropertiesDialog(this, rulebook, ITM);
    connect(dialog, SIGNAL(updateProperties(DataType,QString,QList<RPGProperty*>)), rulebook, SLOT(updateProperties(DataType,QString,QList<RPGProperty*>)));
    dialog->exec();
}

// void RGMainWindow::onPropertiesActions() { } // To be implemented later

void RGMainWindow::onTablesCurrencies() {
    RGTableDialog *dialog = new RGTableDialog(this, rulebook, CUR);
    connect(dialog, SIGNAL(updateVariables(DataType, QList<RPGVariable*>)), rulebook, SLOT(updateVariables(DataType,QList<RPGVariable*>)));
    dialog->exec();
}

void RGMainWindow::onTablesAttributes() {
    RGTableDialog *dialog = new RGTableDialog(this, rulebook, ATT);
    connect(dialog, SIGNAL(updateVariables(DataType, QList<RPGVariable*>)), rulebook, SLOT(updateVariables(DataType,QList<RPGVariable*>)));
    dialog->exec();
}
void RGMainWindow::onTablesSkills() {
    RGTableDialog *dialog = new RGTableDialog(this, rulebook, SKI);
    connect(dialog, SIGNAL(updateVariables(DataType, QList<RPGVariable*>)), rulebook, SLOT(updateVariables(DataType,QList<RPGVariable*>)));
    dialog->exec();
}

void RGMainWindow::onTablesModifiers() {
    RGTableDialog *dialog = new RGTableDialog(this, rulebook, MOD);
    connect(dialog, SIGNAL(updateVariables(DataType, QString, QList<RPGVariable*>)), rulebook, SLOT(updateVariables(DataType,QString,QList<RPGVariable*>)));
    dialog->exec();
}

void RGMainWindow::onTablesItems() {
    RGTableDialog *dialog = new RGTableDialog(this, rulebook, ITM);
    connect(dialog, SIGNAL(updateVariables(DataType, QString, QList<RPGVariable*>)), rulebook, SLOT(updateVariables(DataType, QString,QList<RPGVariable*>)));
    dialog->exec();
}

//void RGMainWindow::onTablesActions() { }  // To be implemented later

void RGMainWindow::onHelpAbout() {
    QMessageBox qmb;
    qmb.setWindowTitle("About RulezGen");
    qmb.setText("RulezGen, v0.1 pre-Alpha\nRPG Suite, v0.1 pre-Alpha\nThis suite is released in 2012 under the GPL license.");
    qmb.exec();
}

void RGMainWindow::onToolDeselect() { emit setComponentType(none); }
void RGMainWindow::onToolTextBox()  { emit setComponentType(textbox); }
void RGMainWindow::onToolCheckBox() { emit setComponentType(checkbox); }

void RGMainWindow::loadImage(QImage img) {
    RGTabPage *page = new RGTabPage(this, pages->count()+1);
    connect(this, SIGNAL(setComponentType(ComponentType)), page, SLOT(setComponentType(ComponentType)));
    pages->addTab(page, QString(tr("Page #%1")).arg(page->page()));
    page->setImage(img);
}

void RGMainWindow::loadComponent(int p, QString name, QString prop, QString var, quint16 x, quint16 y, quint16 w, quint16 h) {
    RGTabPage *page = (RGTabPage *)pages->widget(p-1);
    page->addComponent(name, prop, var, x, y, w, h);
}
