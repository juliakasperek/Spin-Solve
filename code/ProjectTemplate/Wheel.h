#ifndef WHEEL_H
#define WHEEL_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsPixmapItem>

// Custom QGraphicsPixmapItem with rotation property for animation
class WheelItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)

public:
    explicit WheelItem(const QPixmap &pixmap)
        : QGraphicsPixmapItem(pixmap) {}
};

// Wheel widget containing the spinning wheel and arrow
class Wheel : public QWidget {
    Q_OBJECT

public:
    explicit Wheel(QWidget *parent = nullptr);
    void spinWheel();

signals:
    void landedSegment(int index);

private slots:
    void handleAnimationEnd();

private:
    // Graphics components
    QGraphicsScene *scene = nullptr;
    QGraphicsView *view = nullptr;
    QGraphicsPixmapItem *arrowItem = nullptr;

    // Animation
    QPropertyAnimation *animation = nullptr;
    WheelItem *wheelItem = nullptr;

    // Spin properties
    int endAngle;
    int numSegments = 8;
    double segmentAngle = 360.0 / numSegments;
    bool isSpinning = false;
    double currentRotation = 0;

    // Setup functions
    void setUpWheelItem();
    void setUpArrow();
};

#endif // WHEEL_H
