#ifndef MAP_H
#define MAP_H

#include <QPainter>
#include "globals.h"
#include "item.h"
#include "player.h"
#include "enemy.h"
#include "movingobject.h"
#include "wrappingobject.h"
#include "selectablepolygonf.h"

#define FRAME_ELEMENTS 3
#define N_ITEMS 25

class Map
{
public:
    Map(QString filename);
    void draw(QPainter *qp, QPointF viewPort, QRectF renderRect, bool editorMode = false);
    SelectablePolygonF **getObstacles();
    int getNObstacles();
    void addObstacle(QPointF);
    void removeObstacle(int index);

    void keyInput(int key, bool down, QKeyEvent *e);
    void updateEntities();
    QPointF getCenterPoint();

    void saveMap(QString filename);

    Item **getItems();
    int getNItems();

    int getWidth();
    int getHeight();
    ~Map();
private:
    void setupObstacles(QString filename);
    SelectablePolygonF **_obstacles;
    QPolygonF **_shrubbery;
    WrappingObject **_clouds;
    Item **_items;
    QRectF _mapRect;
    int _width;
    int _height;
    int _nObs;
    int _nItems;
    Player *_p;
    Enemy *_nmy;
};

#endif // MAP_H
