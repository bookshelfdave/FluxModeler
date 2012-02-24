#ifndef FLUXGRAPHICSVIEW_H
#define FLUXGRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView();
protected:
    void resizeEvent(QResizeEvent *event);


};

#endif // FLUXGRAPHICSVIEW_H
