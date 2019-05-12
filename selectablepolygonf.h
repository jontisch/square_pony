#ifndef SELECTABLEPOLYGONF_H
#define SELECTABLEPOLYGONF_H
#include <QPolygonF>
#include <iostream>
#include "globals.h"

class SelectablePolygonF : public QPolygonF
{

public:
    SelectablePolygonF();
    SelectablePolygonF(QPolygonF* sp);
    SelectablePolygonF(QVector<QPointF> &v);
    SelectablePolygonF(QRectF r);

    void setSelected(bool s);
    bool isSelected();
     PixmapIdentifier getPixmap();
    void setPixmap(PixmapIdentifier i);
    QString toString();
private:
    bool _selected;
    PixmapIdentifier _pixmap;
};


#endif // SELECTABLEPOLYGONF_H
