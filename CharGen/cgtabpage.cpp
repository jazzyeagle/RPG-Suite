#include "cgtabpage.h"
#include <QMessageBox>

CGTabPage::CGTabPage(QWidget *parent, quint8 p) : QScrollArea(parent) {
    this->setWidget(new QLabel());
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pagenumber = p;
    panel = new QWidget(this);
    panel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    imagelabel = new QLabel();
    setWidget(panel);

    ctype = none;
    if(!components.empty()) components.clear();
}


CGTabPage::~CGTabPage() { RPGRulebook::Instance()->removeImage(pagenumber); }


void CGTabPage::setImage(QString filepath) {
    // Delete old Label
    RPGRulebook::Instance()->removeImage(pagenumber);
    imagelabel->hide();
    delete imagelabel;


    // Create new label
    imagelabel = new QLabel(panel);
    QImage image(filepath);
    RPGRulebook::Instance()->addImage(pagenumber, image);
    imagelabel->setPixmap(QPixmap::fromImage(image));
    imagelabel->resize(image.size());
    panel->resize(image.size());
    imagelabel->show();
}


void CGTabPage::setImage(QImage image) {
    // Delete old Label
    RPGRulebook::Instance()->removeImage(pagenumber);
    imagelabel->hide();
    delete imagelabel;


    // Create new label
    imagelabel = new QLabel(panel);
    RPGRulebook::Instance()->addImage(pagenumber, image);
    imagelabel->setPixmap(QPixmap::fromImage(image));
    imagelabel->resize(image.size());
    panel->resize(image.size());
    imagelabel->show();
}


void CGTabPage::setComponentType(ComponentType ct) {
    ctype = ct;
}


quint8 CGTabPage::page() {
    return pagenumber;
}


void CGTabPage::setPage(quint8 p) {
    pagenumber = p;
}


void CGTabPage::addComponent(QString name, QString prop, QString var, quint16 x, quint16 y, quint16 w, quint16 h) {
    //RPGComponent *comp;
    QLineEdit *tb;
    QCheckBox *cb;

    // If comp's property value is empty, then it is a checkbox, otherwise textbox.
    if(prop.isEmpty()) {
        //comp = new RGCheckBox(panel, page(), QPoint(x, y), QPoint(w, h));
        cb = new QCheckBox(this);
        cb->move(x, y);
        cb->resize(w, h);
    } else {
        //comp = new RGTextBox(panel, page(), QPoint(x, y), QPoint(w, h));
        tb = new QLineEdit(this);
        tb->move(x, y);
        tb->resize(w, h);

        // Add Text to TextBox
        //RGTextBox *tb = (RGTextBox *)comp;
        if(var.isEmpty())  // Is it a character trait?
            tb->setText(name + ":" + prop);
        else
            tb->setText(name + ":" + var + ":" + prop);

        tb->show();
    }

/*    comp->setDataName(name);
    comp->setDataProperty(prop);
    comp->setDataVariable(var);
    components.append(comp);
    RPGRulebook::Instance()->addComponent(comp); */
}
