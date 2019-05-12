#include "wrappingobject.h"
#include <QDebug>

WrappingObject::WrappingObject(QPixmap *pixmap, QPointF pos, QPointF speed, QSizeF size, QRectF *boundries):
    MovingObject(pixmap, pos, speed, size),
    _boundries(boundries)
{

}

void WrappingObject::updatePosition()
{
    MovingObject::updatePosition();
    if(!_boundries->intersects(QRectF(getPos(), getSize()))){
        setPos(QPointF((getSpeed().x() < 0)?_boundries->width(): -getSize().width(),getPos().y()));
    }
}
