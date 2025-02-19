#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include "lineitem.h"

class GraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    GraphicsView(QWidget *parent = nullptr);
    ~GraphicsView();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsScene *scene;
    LineItem *currentLine;
    QPointF startPoint;
    bool isDrawing;
    bool isPanning; // Режим перемещения сцены
    QPoint panStartPos; // Начальная позиция курсора при перемещении
};

#endif // GRAPHICSVIEW_H
