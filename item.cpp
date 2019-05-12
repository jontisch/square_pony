#include "item.h"
#include <QDebug>


Item::Item(QPointF pos, int val):_pos(pos),_width(48),_height(47),_active(true), _scoreValue(val), _selected(false)
{
    _pixmap = new QPixmap(":/graphics/coin.gif");
    _hitbox = QRectF(_pos + QPointF(3,3), QSizeF(_width-6, _height-6));
}

void Item::draw(QPainter *qp)
{
    if(_active){
        qp->drawPixmap(_pos, *_pixmap);
#if DEBUG
        qp->setBrush(Qt::NoBrush);
        qp->setPen(Qt::black);
        qp->drawRect(_hitbox);
#endif
    }
}

QRectF Item::getHitbox()
{
    return _hitbox;
}

void Item::inactivate(){
    _active = false;
}

bool Item::isActive()
{
    return _active;
}

int Item::getScoreVal(){
    return _scoreValue;
}

void Item::setScoreVal(int val){
    _scoreValue = val;
}

bool Item::isSelected()
{
    return _selected;
}

void Item::setSelected(bool s)
{
    _selected = s;
}

QPointF Item::getPos()
{
    return _pos;
}

void Item::move(QPointF delta)
{
    _pos += delta;
    _hitbox.translate(delta);
}

Item::~Item()
{
    delete _pixmap;
}
