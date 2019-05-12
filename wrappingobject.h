#ifndef WRAPPINGOBJECT_H
#define WRAPPINGOBJECT_H

#include "movingobject.h"

class WrappingObject : public MovingObject
{
public:
    WrappingObject(QPixmap *pixmap, QPointF pos, QPointF speed, QSizeF size, QRectF *boundries);
    void updatePosition();
private:
    QRectF *_boundries;
};

#endif // WRAPPINGOBJECT_H
