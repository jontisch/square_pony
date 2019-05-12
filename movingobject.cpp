#include "movingobject.h"
#include "globals.h"
MovingObject::MovingObject(QPixmap *pixmap, QPointF pos, QPointF speed, QSizeF size):
    _pixmap(pixmap),
    _pos(pos),
    _speed(speed),
    _size(size){

}

void MovingObject::setPos(QPointF pos){
    _pos = pos;
}

QPointF MovingObject::getPos(){
    return _pos;
}
void MovingObject::move(QPointF deltapos){
    _pos+=deltapos;
}

void MovingObject::setSpeed(QPointF speed){
    _speed = speed;
}

QPointF MovingObject::getSpeed(){
    return _speed;
}

void MovingObject::setSize(QSizeF size){
    _size = size;
}

QSizeF MovingObject::getSize(){
    return _size;
}

void MovingObject::alterspeed(QPointF deltaspeed){
    _speed+=deltaspeed;
}

void MovingObject::draw(QPainter *qp){
    qp->drawPixmap(_pos, *_pixmap);
}

void MovingObject::updatePosition(){
    _pos += this->linearMovement(_speed);
}

QPointF MovingObject::linearMovement(QPointF pixelsPerSecond){
   float secsElapsed = (getElapsed() - getLastUpdate()) / 1000.f;
#if DEBUG
    if(pixelsPerSecond.x())
        qDebug() << secsElapsed << _speed << pixelsPerSecond * secsElapsed;
#endif
    return pixelsPerSecond * secsElapsed;
}
