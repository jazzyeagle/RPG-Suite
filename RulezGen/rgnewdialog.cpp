#include "rgnewdialog.h"

RGNewDialog::RGNewDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("New Rulebook"));
    QGridLayout *grid = new QGridLayout(this);

    QLabel *dialoglabel1 = new QLabel(tr("Select Game..."));
    grid->addWidget(dialoglabel1, 0, 0, 1, 2);
    combo = new QComboBox(this);
        combo->addItem(tr("New Game"));
        headers = RPGRulebook::getHeaders();
        for(int i=0; i < headers.count(); i++)
            if(headers.at(i)->isCore())
                combo->addItem(headers.at(i)->getIcon(), headers.at(i)->getGameName());
        connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboSelectionChange(int)));
    grid->addWidget(combo, 0, 2, 1, 4);

    tabs = new QTabWidget(this);
    tabs->setTabPosition(QTabWidget::West);
        gametab = new QWidget(tabs);
        QGridLayout *gamegrid = new QGridLayout(gametab);
        QLabel *gamelabel1 = new QLabel(tr("Name:"));
        gamegrid->addWidget(gamelabel1, 0, 0, 1, 2);
        gamename = new QLineEdit(gametab);
        gamegrid->addWidget(gamename, 0, 2, 1, 4);
        QLabel *gamelabel2 = new QLabel(tr("Version:"));
        gamegrid->addWidget(gamelabel2, 1, 0, 1, 2);
        gameversion = new QDoubleSpinBox(gametab);
        gamegrid->addWidget(gameversion, 1, 2, 1, 1);
        QLabel *gamelabel3 = new QLabel(tr("Icon:"));
        gamegrid->addWidget(gamelabel3, 1, 4, 1, 1);
        gameicon = new QToolButton(gametab);
        connect(gameicon, SIGNAL(clicked()), this, SLOT(onIconButtonClick()));
        gamegrid->addWidget(gameicon, 1, 5, 1, 1);
        QLabel *gamelabel4 = new QLabel(tr("ID:"));
        gamegrid->addWidget(gamelabel4, 2, 0, 1, 2);
        gameid = new QLineEdit(gametab);
        gamegrid->addWidget(gameid, 2, 2, 1, 4);
    tabs->addTab(gametab, tr("Game"));
        rbktab = new QWidget(tabs);
        QGridLayout *rbkgrid = new QGridLayout(rbktab);
        QLabel *rbklabel1 = new QLabel(tr("Core Rulebook:"), rbktab);
        rbkgrid->addWidget(rbklabel1, 0, 0, 1, 3);
        coreyes = new QRadioButton(tr("Yes"), rbktab);
        coreyes->setChecked(true);
        connect(coreyes, SIGNAL(toggled(bool)), this, SLOT(onRadioButtonChange()));
        rbkgrid->addWidget(coreyes, 0, 3, 1, 1);
        coreno = new QRadioButton(tr("No"), rbktab);
        rbkgrid->addWidget(coreno, 0, 4, 1, 1);
        QLabel *rbklabel2 = new QLabel(tr("Name:"), rbktab);
        rbkgrid->addWidget(rbklabel2, 1, 0, 1, 2);
        rbkname = new QLineEdit(rbktab);
        rbkgrid->addWidget(rbkname, 1, 2, 1, 4);
        QLabel *rbklabel3 = new QLabel(tr("Filename:"), rbktab);
        rbkgrid->addWidget(rbklabel3, 2, 0, 1, 2);
        rbkfile = new QLineEdit(rbktab);
        rbkgrid->addWidget(rbkfile, 2, 2, 1, 4);
        QLabel *rbklabel4 = new QLabel(tr("Copyright:"), rbktab);
        rbkgrid->addWidget(rbklabel4, 3, 0, 1, 2);
        rbkcopyright = new QLineEdit(rbktab);
        rbkgrid->addWidget(rbkcopyright, 3, 2, 1, 4);
        QLabel *rbklabel5 = new QLabel(tr("Description:"), rbktab);
        rbkgrid->addWidget(rbklabel5, 4, 0, 1, 2);
        rbkdescription = new QLineEdit(rbktab);
        rbkgrid->addWidget(rbkdescription, 4, 2, 1, 4);
        rbktab->setLayout(rbkgrid);
    tabs->addTab(rbktab, tr("Rulebook"));
    tabs->setCurrentIndex(1);

    grid->addWidget(tabs, 1, 0, 9, 7);

    descriptionlabel = new QTextEdit(this);
    descriptionlabel->setWordWrapMode(QTextOption::WordWrap);
    grid->addWidget(descriptionlabel, 0, 7, 9, 3);

    copyrightlabel = new QLineEdit(this);
    grid->addWidget(copyrightlabel, 9, 7, 1, 3);

    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    grid->addWidget(buttons, 10, 0, 1, 10);
}


RGNewDialog::RGNewDialog(QWidget *parent, RPGRulebookHeader *h) : QDialog(parent) {
    setWindowTitle(tr("New Rulebook"));
    QGridLayout *grid = new QGridLayout(this);

    QLabel *dialoglabel1 = new QLabel(tr("Select Game..."));
    grid->addWidget(dialoglabel1, 0, 0, 1, 2);
    combo = new QComboBox(this);
        combo->addItem(RPGRulebook::Instance()->getHeader()->getIcon(),
                       RPGRulebook::Instance()->getHeader()->getGameName());
        combo->setEnabled(false);  // Once game is selected, they can't change it.
    grid->addWidget(combo, 0, 2, 1, 4);

    tabs = new QTabWidget(this);
    tabs->setTabPosition(QTabWidget::West);
        gametab = new QWidget(tabs);
        QGridLayout *gamegrid = new QGridLayout(gametab);
        QLabel *gamelabel1 = new QLabel(tr("Name:"));
        gamegrid->addWidget(gamelabel1, 0, 0, 1, 2);
        gamename = new QLineEdit(gametab);
        gamename->setText(h->getGameName());
        gamename->setEnabled(false);
        gamegrid->addWidget(gamename, 0, 2, 1, 4);
        QLabel *gamelabel2 = new QLabel(tr("Version:"));
        gamegrid->addWidget(gamelabel2, 1, 0, 1, 2);
        gameversion = new QDoubleSpinBox(gametab);
        gameversion->setValue(h->getVersion());
        gamegrid->addWidget(gameversion, 1, 2, 1, 1);
        QLabel *gamelabel3 = new QLabel(tr("Icon:"));
        gamegrid->addWidget(gamelabel3, 1, 4, 1, 1);
        gameicon = new QToolButton(gametab);
        gameicon->setIcon(h->getIcon());
        gameicon->setEnabled(false);
        gamegrid->addWidget(gameicon, 1, 5, 1, 1);
        QLabel *gamelabel4 = new QLabel(tr("ID:"));
        gamegrid->addWidget(gamelabel4, 2, 0, 1, 2);
        gameid = new QLineEdit(gametab);
        gameid->setText(h->getGameId());
        gamegrid->addWidget(gameid, 2, 2, 1, 4);
    tabs->addTab(gametab, tr("Game"));
        rbktab = new QWidget(tabs);
        QGridLayout *rbkgrid = new QGridLayout(rbktab);
        QLabel *rbklabel1 = new QLabel(tr("Core Rulebook:"), rbktab);
        rbkgrid->addWidget(rbklabel1, 0, 0, 1, 3);
        coreyes = new QRadioButton(tr("Yes"), rbktab);
        coreno = new QRadioButton(tr("No"), rbktab);
        connect(coreyes, SIGNAL(toggled(bool)), this, SLOT(onRadioButtonChange()));
        if(h->isCore())
            coreyes->setChecked(true);
        else
            coreno->setChecked(true);
        rbkgrid->addWidget(coreyes, 0, 3, 1, 1);
        rbkgrid->addWidget(coreno, 0, 4, 1, 1);
        QLabel *rbklabel2 = new QLabel(tr("Name:"), rbktab);
        rbkgrid->addWidget(rbklabel2, 1, 0, 1, 2);
        rbkname = new QLineEdit(rbktab);
        rbkname->setText(h->getRulebookName());
        rbkgrid->addWidget(rbkname, 1, 2, 1, 4);
        QLabel *rbklabel3 = new QLabel(tr("Filename:"), rbktab);
        rbkgrid->addWidget(rbklabel3, 2, 0, 1, 2);
        rbkfile = new QLineEdit(rbktab);
        rbkfile->setText(h->getRulebookFilename());
        rbkgrid->addWidget(rbkfile, 2, 2, 1, 4);
        QLabel *rbklabel4 = new QLabel(tr("Copyright:"), rbktab);
        rbkgrid->addWidget(rbklabel4, 3, 0, 1, 2);
        rbkcopyright = new QLineEdit(rbktab);
        rbkcopyright->setText(h->getRulebookCopyright());
        rbkgrid->addWidget(rbkcopyright, 3, 2, 1, 4);
        QLabel *rbklabel5 = new QLabel(tr("Description:"), rbktab);
        rbkgrid->addWidget(rbklabel5, 4, 0, 1, 2);
        rbkdescription = new QLineEdit(rbktab);
        rbkdescription->setText(h->getRulebookDescription());
        rbkgrid->addWidget(rbkdescription, 4, 2, 1, 4);
        rbktab->setLayout(rbkgrid);
    tabs->addTab(rbktab, tr("Rulebook"));
    tabs->setCurrentIndex(1);

    grid->addWidget(tabs, 1, 0, 9, 7);

    descriptionlabel = new QTextEdit(this);
    descriptionlabel->setWordWrapMode(QTextOption::WordWrap);
    descriptionlabel->setText(h->getGameDescription());
    descriptionlabel->setEnabled(false);
    grid->addWidget(descriptionlabel, 0, 7, 9, 3);

    copyrightlabel = new QLineEdit(this);
    copyrightlabel->setText(h->getGameCopyright());
    copyrightlabel->setEnabled(false);
    grid->addWidget(copyrightlabel, 9, 7, 1, 3);

    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    grid->addWidget(buttons, 10, 0, 1, 10);
}


bool RGNewDialog::isCore() {
    return core;
}


void RGNewDialog::onRadioButtonChange() {
    if(coreyes->isChecked())
        tabs->setTabEnabled(0, true);
    else {
        if(combo->currentText()==tr("New Game")) {
            QMessageBox qmb;
            qmb.setText(tr("When creating a new game, you MUST create a core rulebook."));
            qmb.exec();
            coreyes->setChecked(true);
            coreno->setChecked(false);
        } else tabs->setTabEnabled(0, false);
    }
}


// Instead of sending all of the various variables to the RPGRulebook to update the header
// it already has, this will compile all of the variables into a new header to send to
// the RPGRulebook, which will simply replace the header.
RPGRulebookHeader* RGNewDialog::rulebookHeader() {
    RPGRulebookHeader *h = new RPGRulebookHeader();

    if(coreyes->isChecked())
        h->setCore(true);
    else
        h->setCore(false);

    h->setGameName(gamename->text());
    h->setVersion((qreal)gameversion->value());
    h->setGameId(gameid->text());
    h->setGameCopyright(copyrightlabel->text());
    h->setGameDescription(descriptionlabel->toPlainText());
    h->setIcon(gameicon->icon());

    h->setRulebookName(rbkname->text());
    h->setRulebookFilename(rbkfile->text());
    h->setRulebookCopyright(rbkcopyright->text());
    h->setRulebookDescription(rbkdescription->text());

    return h;
}

void RGNewDialog::onIconButtonClick() {
    QString ifn = QFileDialog::getOpenFileName(this, tr("Select Icon"), QString(), tr("Icon Files (*.ico)"));
    gameicon->setIcon(QIcon(ifn));
}

void RGNewDialog::onComboSelectionChange(int selection) {
    selection = selection - 1;  // New Game would otherwise be 0 and throw off headers index

    // If new game...
    if(selection==-1) {
        gamename->setText(QString());
        setEnabled(true);
        gameversion->setValue(0);
        gameid->setText(QString());
        gameicon->setIcon(QIcon());
        gameicon->setEnabled(true);
        descriptionlabel->setText(QString());
        descriptionlabel->setEnabled(true);
        copyrightlabel->setText(QString());
        copyrightlabel->setEnabled(true);
    } else {
        gamename->setText(headers.at(selection)->getGameName());
        gamename->setEnabled(false);
        gameversion->setValue(0);   // There can be multiple versions per game.
        gameid->setText(QString()); // This can change for each version
        gameicon->setIcon(headers.at(selection)->getIcon());
        gameicon->setEnabled(false);
        descriptionlabel->setText(headers.at(selection)->getGameDescription());
        descriptionlabel->setEnabled(false);
        copyrightlabel->setText(headers.at(selection)->getGameCopyright());
        copyrightlabel->setEnabled(false);
    }
}

QIcon RGNewDialog::getIcon() {
    return gameicon->icon();
}
