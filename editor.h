#ifndef EDITOR_H
#define EDITOR_H

#include "map.h"
#include "selectablepolygonf.h"
#include "editorwidget.h"

class Editor
{
public:
    Editor(QString filename, QRectF window);
    void draw(QPainter *qp);
    void keyInput(int key, bool down, QKeyEvent *e);
    void zoom(bool in);
    void mousePressEvent(QMouseEvent *e, bool pressed);
    void mouseMoveEvent(QMouseEvent *e);
    ~Editor();
private:
    Map *_map;
    QPointF _viewPort;
    QPointF *_selectedPoint;
    SelectablePolygonF *_selectedPolygon;

    QRectF _menuRect;
    QRectF _mapRect;

    EditorWidget ** _widgets;
    int _nWidgets;

    bool _mousePressed;
    QPointF _mouseLastClicked;
    bool _shiftKeyPressed;
    bool _controlPressed;
    double _zoomfactor = 1.f;
};

#endif // EDITOR_H
