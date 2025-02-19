#include "graphicsview.h"
#include <QGraphicsLineItem>
#include <QDebug>

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent), currentLine(nullptr), isDrawing(false) {
    scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);

    // Отключаем прокрутку
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Отключаем перемещение сцены
    setDragMode(QGraphicsView::NoDrag);

    // Устанавливаем размер сцены
    scene->setSceneRect(0, 0, width(), height());
}

GraphicsView::~GraphicsView() {}

void GraphicsView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // Преобразуем координаты мыши в координаты сцены
        startPoint = mapToScene(event->pos());
        qDebug() << "Mouse press at scene coordinates:" << startPoint;

        // Создаем линию с началом и концом в startPoint
        currentLine = new LineItem(startPoint, startPoint, nullptr);
        scene->addItem(currentLine);
        isDrawing = true;
    }
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if (isDrawing && currentLine) {
        // Преобразуем координаты мыши в координаты сцены
        QPointF endPoint = mapToScene(event->pos());
        // Обновляем конечную точку линии
        currentLine->setEndPoint(endPoint);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isDrawing) {
        isDrawing = false;
        currentLine = nullptr;
    }
    QGraphicsView::mouseReleaseEvent(event);
}
