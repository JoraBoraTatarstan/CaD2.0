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

    // Включаем полосы прокрутки
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Отключаем перемещение сцены по умолчанию
    setDragMode(QGraphicsView::NoDrag);

    // Устанавливаем размер сцены
    scene->setSceneRect(0, 0, width(), height());
}

GraphicsView::~GraphicsView() {}

void GraphicsView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) { // Нажатие на колесико мыши
        isPanning = true; // Включаем режим перемещения
        panStartPos = event->pos(); // Запоминаем начальную позицию курсора
        setCursor(Qt::ClosedHandCursor); // Меняем курсор на "руку"
    } else if (event->button() == Qt::LeftButton) {
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
    if (isPanning) { // Если включен режим перемещения
        // Вычисляем смещение курсора
        QPoint delta = event->pos() - panStartPos;
        panStartPos = event->pos(); // Обновляем начальную позицию

        // Перемещаем сцену
        if (horizontalScrollBar() && verticalScrollBar()) { // Проверяем, что полосы прокрутки существуют
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        }
    } else if (isDrawing && currentLine) {
        // Преобразуем координаты мыши в координаты сцены
        QPointF endPoint = mapToScene(event->pos());
        // Обновляем конечную точку линии
        currentLine->setEndPoint(endPoint);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) { // Отпускание колесика мыши
        isPanning = false; // Выключаем режим перемещения
        setCursor(Qt::ArrowCursor); // Возвращаем стандартный курсор
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
