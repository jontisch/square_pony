#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H

#include <QPointF>
#include <QPixmap>
#include <QPainter>
class MovingObject
{
public:
    MovingObject(QPixmap *pixmap, QPointF pos, QPointF speed, QSizeF size);
    void setPos(QPointF pos);
    QPointF getPos();
    void move(QPointF deltapos);
    void setSpeed(QPointF speed);
    QPointF getSpeed();
    void alterspeed(QPointF deltaspeed);
    void draw(QPainter *qp);
    void updatePosition();
    void setSize(QSizeF ssize);
    QSizeF getSize();
private:
    QPointF linearMovement(QPointF pixelsPerSecond);
    QPixmap *_pixmap;
    QPointF _pos;
    QPointF _speed;
    QSizeF _size;
};

#endif // MOVINGOBJECT_H
