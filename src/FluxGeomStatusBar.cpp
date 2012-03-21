#include "FluxGeomStatusBar.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

GeomStatusBar::GeomStatusBar(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();

    QLabel *geomManipMode = new QLabel(this);
    QLineEdit *te = new QLineEdit("Hey there");
    te->setMaximumWidth(50);
    geomManipMode->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    geomManipMode->setText("Translate");
    geomManipMode->setAlignment(Qt::AlignBottom | Qt::AlignRight);

    layout->addWidget(geomManipMode);
    layout->addWidget(te);

    layout->setAlignment(geomManipMode,Qt::AlignLeft);
    this->setLayout(layout);
}


GeomStatusBar::~GeomStatusBar() {
    qDebug("Need to fix constructor");
}
