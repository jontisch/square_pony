#ifndef ITEM_H
#define ITEM_H

#include <QPointF>
#include <QPainter>
#include <QPixmap>
#include "globals.h"

class Item
{
public:
    Item(QPointF pos, int val);
    void draw(QPainter *qp);
    QRectF getHitbox();
    void inactivate();
    bool isActive();
    int getScoreVal();
    void setScoreVal(int val);
    bool isSelected();
    void setSelected(bool s);
    QPointF getPos();
    void move(QPointF delta);
    ~Item();

private:
    QPixmap *_pixmap;
    QPointF _pos;
    QRectF _hitbox;
    int _width;
    int _height;
    bool _active;
    int _scoreValue;
    bool _selected;

};

#endif // ITEM_H
