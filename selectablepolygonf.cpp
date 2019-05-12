#include "selectablepolygonf.h"
#include <qdebug.h>

SelectablePolygonF::SelectablePolygonF():_selected(false), _pixmap(PIXMAP_TILE) {}
SelectablePolygonF::SelectablePolygonF(QPolygonF *sp): QPolygonF(*sp), _selected(false), _pixmap(PIXMAP_TILE) {}
SelectablePolygonF::SelectablePolygonF(QRectF r): QPolygonF(r), _selected(false), _pixmap(PIXMAP_TILE) {}
SelectablePolygonF::SelectablePolygonF(QVector<QPointF> &v): QPolygonF(v), _selected(false), _pixmap(PIXMAP_TILE) {}


QString SelectablePolygonF::toString()
{
    QString str;
    for(int i = 0; i < size(); i++){
        if(i) str.append(';');
        str.append(QString::number(at(i).x()));
        str.append(',');
        str.append(QString::number(at(i).y()));
    }
    return str;
}






PixmapIdentifier SelectablePolygonF::getPixmap()
{
    return _pixmap;
}

void SelectablePolygonF::setPixmap(PixmapIdentifier i)
{
    if(i == -1) i = (PixmapIdentifier)(N_PIXMAPS -1);
    _pixmap = (PixmapIdentifier)(i % N_PIXMAPS);
}


void SelectablePolygonF::setSelected(bool s)
{
    _selected = s;
}

bool SelectablePolygonF::isSelected(){
    return _selected;
}


