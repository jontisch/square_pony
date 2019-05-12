#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H
#include <QRectF>
#include <QPixmap>
#include <QPainter>
#include "globals.h"

class EditorWidget
{
public:
    EditorWidget(PixmapIdentifier pixmap, QRectF rect);
    void draw(QPainter *qp);
    QRectF *getRect();
private:
    QRectF _rect;
    QPixmap *_pixmap;
};

#endif // EDITORWIDGET_H
