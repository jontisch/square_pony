#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H
#include <QPointF>
#include <stdint.h>

#define N_COLLISIONPOINTS 8

class Boxcollider
{
public:
    Boxcollider();

private:
    QPointF _collisionPoints[N_STATES][N_COLLISIONPOINTS];
    QPointF _collisionOffset[N_STATES][2];


    uint16_t _nStates;

    bool _xBlocked;
    bool _yTopBlocked;
    bool _yBottomBlocked;
};

#endif // BOXCOLLIDER_H
