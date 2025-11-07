#include "Wheel.h"
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QtMath>
#include <QPropertyAnimation>

Wheel::Wheel(QWidget *parent)
    : QWidget(parent)
{
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    // Set up wheel and arrow graphics
    setUpWheelItem();
    setUpArrow();

    // Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(view);
    setLayout(layout);

    // Animation setup
    animation = new QPropertyAnimation(wheelItem, "rotation", this);
    animation->setDuration(5000);  // duration in milliseconds
    animation->setEasingCurve(QEasingCurve::OutCubic);
    connect(animation, &QPropertyAnimation::finished, this, &Wheel::handleAnimationEnd);
}

void Wheel::setUpWheelItem()
{
    QPixmap wheelPixmap(":/images/images/pink.png");
    wheelItem = new WheelItem(wheelPixmap);

    // Set rotation origin to center of the wheel
    wheelItem->setTransformOriginPoint(wheelPixmap.width() / 2.0, wheelPixmap.height() / 2.0);

    // Center the wheel in the scene
    wheelItem->setPos(-wheelPixmap.width() / 2.0, -wheelPixmap.height() / 2.0);

    scene->addItem(wheelItem);

    // Set scene rect and view properties
    scene->setSceneRect(-wheelPixmap.width() / 2.0, -wheelPixmap.height() / 2.0,
                        wheelPixmap.width(), wheelPixmap.height());
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    view->setAlignment(Qt::AlignCenter);
    view->scale(10, 10);  // optional zoom
}

void Wheel::setUpArrow()
{
    QPixmap arrowPixmap(":/images/images/arrow.png");
    arrowItem = new QGraphicsPixmapItem(arrowPixmap);

    // Position the arrow relative to the scene
    arrowItem->setPos(-scene->width() / 2 + 120, -scene->height() / 2 - 20);
    arrowItem->setScale(0.5);

    scene->addItem(arrowItem);
}

void Wheel::spinWheel()
{
    if (isSpinning) return; // Prevent double spins
    isSpinning = true;

    int fullRotations = 6;
    int randomAngle = QRandomGenerator::global()->bounded(360);
    endAngle = currentRotation + 360 * fullRotations + randomAngle;

    animation->setStartValue(currentRotation);
    animation->setEndValue(endAngle);
    animation->setDuration(4000);  // smoother, consistent spin speed
    animation->setEasingCurve(QEasingCurve::OutCubic);

    animation->start();
}

void Wheel::handleAnimationEnd()
{
    double finalAngle = fmod(endAngle, 360.0);
    currentRotation = finalAngle;  // remember last angle
    isSpinning = false;            // allow spinning again

    double arrowOffset = 45.0 / 2.0;
    double adjustedAngle = fmod(finalAngle + arrowOffset, 360.0);

    int index = static_cast<int>(adjustedAngle / segmentAngle);
    index = numSegments - 1 - index;

    emit landedSegment(index);
}
