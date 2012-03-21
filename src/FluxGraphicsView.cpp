#include "FluxGraphicsView.h"
#include <QResizeEvent>
#include <QPoint>
#include <QRect>

GraphicsView::GraphicsView() :
    QGraphicsView()
{

}

void GraphicsView::resizeEvent(QResizeEvent *event) {
    if (scene())
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
    QGraphicsView::resizeEvent(event);
}

