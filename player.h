#ifndef PLAYER_H
#define PLAYER_H

#include <QElapsedTimer>
#include <QPointF>
#include <QPainter>
#include <QDebug>
#include <QtMultimedia/QSound>
#include <QDir>
#include <QKeyEvent>
#include <qmath.h>

#include "item.h"
#include "globals.h"
#include "selectablepolygonf.h"

//Speed constants
#define MAXSPEEDX 2.5f
#define MAXSPEEDY 15.0f

#define COLLSION_ITER 4
#define N_COLLISIONPOINTS 8


class Player
{
public:
    Player(QPointF pos);
    void draw(QPainter *qp);
    void updatePosition(SelectablePolygonF **colliders, int n_colliders);
    void checkItems(Item **items, int n_items);
    void keyInput(int key, bool down, QKeyEvent *e);

    QPointF getCenter();
    ~Player();
    int getScore();
private:
    void setupCollisionPoints();
    QPixmap *getCurrentPixmap();
    QPointF linearMovement(QPointF speed);
    void updateCurrentState();


    QPointF _pos;
    QPointF _acc;
    QPointF _dec;
    //QPointF _prevPos;
    QPointF _speed;
 //   QPointF _direction;
    float _maxSpeedX, _maxSpeedY;
    float _jumpStartSpeedY;
    qint8 _width;
    qint8 _height;


    QPointF _collisionPoints[N_STATES][N_COLLISIONPOINTS];
    QPointF _collisionOffset[N_STATES][2];
    SpriteDirection _lastDir;
    SpriteState _currentState;
    SpriteIdentifier spriteStateMap[N_STATES];
    bool _jumping;
    bool _jumpKeyPressed;
    bool _leftKeyPressed;
    bool _rightKeyPressed;
    bool _downKeyPressed;
    int _lastKeyPressed;


    bool _xBlocked;
    bool _yTopBlocked;
    bool _yBottomBlocked;


    int _score;
};

#endif // PLAYER_H
