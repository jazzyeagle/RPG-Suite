#include "rgtabpage.h"
#include <QMessageBox>

RGTabPage::RGTabPage(QWidget *parent, quint8 p) : QScrollArea(parent) {
    this->setWidget(new QLabel());
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pagenumber = p;
    panel = new QWidget(this);
    panel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    imagelabel = new QLabel();
    setWidget(panel);

    rubberband = new QRubberBand(QRubberBand::Rectangle, this);
    ctype = none;
    if(!components.empty()) components.clear();
}


RGTabPage::~RGTabPage() { RPGRulebook::Instance()->removeImage(pagenumber); }


void RGTabPage::setImage(QString filepath) {
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


void RGTabPage::setImage(QImage image) {
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


void RGTabPage::mousePressEvent(QMouseEvent *e) {
    if(e->button() == Qt::LeftButton) {
        origin = e->pos();
        rubberband->setGeometry(QRect(origin, QSize()));
        rubberband->show();
    } else {
        QScrollArea::mousePressEvent(e);
    }
}


void RGTabPage::mouseMoveEvent(QMouseEvent *e) {
    rubberband->setGeometry(QRect(origin, e->pos()).normalized());
}

void RGTabPage::mouseReleaseEvent(QMouseEvent *e) {
    rubberband->hide();
    destination = e->pos();


    QPoint adjustedOrigin(this->horizontalScrollBar()->value() + origin.x(),
                          this->verticalScrollBar()->value() + origin.y());

    QPoint adjustedDestination(this->horizontalScrollBar()->value() + e->x(),
                                this->verticalScrollBar()->value() + e->y());

    RPGComponent *comp;

    switch(ctype) {
        case none:
            break;
        case textbox:
            comp = new RGTextBox(panel, page(), adjustedOrigin, adjustedDestination);
            break;
        case checkbox:
            comp = new RGCheckBox(panel, page(), adjustedOrigin, adjustedDestination);
            break;
    }

    components.append(comp);
}

void RGTabPage::setComponentType(ComponentType ct) {
    ctype = ct;
}

quint8 RGTabPage::page() {
    return pagenumber;
}

void RGTabPage::setPage(quint8 p) {
    pagenumber = p;
}

void RGTabPage::addComponent(QString name, QString prop, QString var, quint16 x, quint16 y, quint16 w, quint16 h) {
    RPGComponent *comp;

    // If comp's property value is empty, then it is a checkbox, otherwise textbox.
    if(prop.isEmpty()) {
        qDebug("Checkbox");
        comp = new RGCheckBox(panel, page(), QPoint(x, y), QPoint(w, h));
    } else {
        qDebug("Textbox");
        comp = new RGTextBox(panel, page(), QPoint(x, y), QPoint(w, h));
        // Add Text to TextBox
        RGTextBox *tb = (RGTextBox *)comp;
        if(var.isEmpty())  // Is it a character trait?
            tb->setText(name + ":" + prop);
        else
            tb->setText(name + ":" + var + ":" + prop);

        tb->show();
    }

    comp->setDataName(name);
    comp->setDataProperty(prop);
    comp->setDataVariable(var);
    components.append(comp);
    RPGRulebook::Instance()->addComponent(comp);
}
