#include "graphicsview.h"
#include <QGraphicsLineItem>
#include <QDebug>
#include <QWheelEvent>
#include <QScrollBar>
GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent), currentLine(nullptr), isDrawing(false), isPanning(false) {
    scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);

    // Отключаем стандартное перемещение сцены
    setDragMode(QGraphicsView::NoDrag);

    // Устанавливаем размер сцены
    scene->setSceneRect(0, 0, 5000, 5000); // Увеличим размер сцены, чтобы было куда двигать

    // Позволяет перемещать сцену плавно
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

GraphicsView::~GraphicsView() {}

void GraphicsView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        isPanning = true;
        panStartPos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    } else if (event->button() == Qt::LeftButton) {
        startPoint = mapToScene(event->pos());
        currentLine = new LineItem(startPoint, startPoint, nullptr);
        scene->addItem(currentLine);
        isDrawing = true;
    }
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if (isPanning) {
        // Вычисляем смещение мыши
        QPointF delta = mapToScene(event->pos()) - mapToScene(panStartPos);
        panStartPos = event->pos(); // Обновляем начальную позицию

        // Получаем текущий трансформ и сдвигаем его
        QTransform transform = this->transform();
        transform.translate(delta.x(), delta.y());
        setTransform(transform); // Применяем новый трансформ
    } else if (isDrawing && currentLine) {
        QPointF endPoint = mapToScene(event->pos());
        currentLine->setEndPoint(endPoint);
    }
    QGraphicsView::mouseMoveEvent(event);
}


void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
    } else if (event->button() == Qt::LeftButton && isDrawing) {
        isDrawing = false;
        currentLine = nullptr;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::wheelEvent(QWheelEvent *event) {
    // Точка, относительно которой будет происходить масштабирование
    QPointF scenePos = mapToScene(event->position().toPoint());

    // Масштабирование
    qreal scaleFactor = (event->angleDelta().y() > 0) ? 1.1 : 0.9; // Увеличение или уменьшение на 10%
    scale(scaleFactor, scaleFactor);

    // Корректируем положение сцены, чтобы точка под курсором оставалась на месте
    QPointF delta = mapToScene(event->position().toPoint()) - scenePos;
    translate(delta.x(), delta.y());
}
