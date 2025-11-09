#include "Wheel.h"

#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QtMath>
#include <QPropertyAnimation>

/*
 * The Wheel class handles the visual spinning wheel component,
 * including it's animation and the logic for determining results.
 */

Wheel::Wheel(QWidget *parent)
    : QWidget(parent)
{
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    // Set up wheel and arrow graphics
    setUpWheelItem();
    setUpArrow();

    // Layout set up for displaying the wheel
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(view);
    setLayout(layout);

    // Create the spin animation
    animation = new QPropertyAnimation(wheelItem, "rotation", this);
    animation->setDuration(5000);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    connect(animation, &QPropertyAnimation::finished, this, &Wheel::handleAnimationEnd);
}

// Loads the wheel image and positions it in the center of the screen
void Wheel::setUpWheelItem()
{
    QPixmap wheelPixmap(":/images/pink.png");
    wheelItem = new WheelItem(wheelPixmap);

    // Set rotation origin to center of the wheel
    wheelItem->setTransformOriginPoint(wheelPixmap.width() / 2.0, wheelPixmap.height() / 2.0);

    // Center the wheel in the scene
    wheelItem->setPos(-wheelPixmap.width() / 2.0, -wheelPixmap.height() / 2.0);

    scene->addItem(wheelItem);

    // Set the scene for proper display
    scene->setSceneRect(-wheelPixmap.width() / 2.0, -wheelPixmap.height() / 2.0,
                        wheelPixmap.width(), wheelPixmap.height());
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    view->setAlignment(Qt::AlignCenter);
    view->scale(10, 10);
}

// Loads the arrow image that points to the resulting segment after the spin.
void Wheel::setUpArrow()
{
    QPixmap arrowPixmap(":/images/arrow.png");
    arrowItem = new QGraphicsPixmapItem(arrowPixmap);

    // Place the arrow above the wheel
    arrowItem->setPos(-scene->width() / 2 + 130, -scene->height() / 2 - 20);
    arrowItem->setScale(0.5);

    scene->addItem(arrowItem);
}

// Starts the spinning animation for the wheel.
void Wheel::spinWheel()
{
    if (isSpinning) return; // Prevents double spins
    isSpinning = true;

    // Randomize final wheel position after its rotation
    int fullRotations = 6;
    int randomAngle = QRandomGenerator::global()->bounded(360);
    endAngle = currentRotation + 360 * fullRotations + randomAngle;

    // Set the right animation parameters
    animation->setStartValue(currentRotation);
    animation->setEndValue(endAngle);
    animation->setDuration(4000);
    animation->setEasingCurve(QEasingCurve::OutCubic);

    animation->start();
}

/* Called when the spin animation ends.
 * It calculates which segment the arrow points to.
 */
void Wheel::handleAnimationEnd()
{
    // Calculate the final resting segemnt of the wheel after it spins
    double finalAngle = fmod(endAngle, 360.0);
    currentRotation = finalAngle;  // Remember the last angle
    isSpinning = false;

    // Calculate where the wheel stopped and adjust the angle
    double arrowOffset = 45.0 / 2.0;
    double adjustedAngle = fmod(finalAngle + arrowOffset, 360.0);

    // Identify which segment corresponds to the wheel's final position
    int index = static_cast<int>(adjustedAngle / segmentAngle);
    index = numSegments - 1 - index;

    // Emit the result based on where the wheel stops (the arrow points to that segment)
    emit landedSegment(index);
}
