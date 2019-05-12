#include "editorwidget.h"

EditorWidget::EditorWidget(PixmapIdentifier pixmap, QRectF rect): _rect(rect)
{
    _pixmap = getPixmap(pixmap);
}

void EditorWidget::draw(QPainter *qp)
{
    qp->drawPixmap(_rect.toRect(),_pixmap->scaledToHeight(_rect.height()));
    //qp->fillRect(_rect, QBrush((_pixmap->scaledToHeight(_rect.height()))));
}

QRectF *EditorWidget::getRect()
{
    return &_rect;
}
