#include "enemy.h"


Enemy::Enemy(QPointF pos):
    _pos(pos),
    _acc(0.1f * SCALE, 0.50f * SCALE), // 0.2,0.13
    _dec(0.4f * SCALE, 0.0),//0.4, 0.0
    _speed(0,0),//0.0
    _maxSpeedX(MAXSPEEDX * SCALE),//2.0
    _maxSpeedY(MAXSPEEDY * SCALE),//10
    _jumpStartSpeedY(15.0f * SCALE),//100
    _width(76),
    _height(64),
    _lastDir(LEFT),
    _currentState(NORMAL),
    _jumping(false),
    _jumpKeyPressed(false),
    _leftKeyPressed(false),
    _rightKeyPressed(false),
    _downKeyPressed(false),
    _lastKeyPressed(0),
    _xBlocked(false),
    _yTopBlocked(false),
    _yBottomBlocked(false),
    _score(0)
{
    spriteStateMap[NORMAL] = SPRITE_SQUARE_PONY_STANDING;
    spriteStateMap[CROUCH] = SPRITE_SQUARE_PONY_CROUCHING;
    spriteStateMap[JUMP] = SPRITE_SQUARE_PONY_STANDING;
    spriteStateMap[WALK] = SPRITE_SQUARE_PONY_WALKING;
    setupCollisionPoints();

}

void Enemy::draw(QPainter *qp){

    QPixmap tmp = QPixmap(*getCurrentPixmap());
    if(_lastDir == LEFT){
        tmp = tmp.transformed(QTransform().scale(-1,1));
    }
    qp->drawPixmap( _pos, tmp );
    #if DEBUG
    qp->setPen(QPen(QBrush(Qt::green), 4, Qt::SolidLine, Qt::RoundCap));

    for(int i=0; i< 8; i++){

        qp->drawPoint(_collisionPoints[_currentState][i] + _pos + _collisionOffset[_currentState][_lastDir]);
    }
    #endif
}

void Enemy::updatePosition(SelectablePolygonF **colliders, int n_colliders)
{
    //---------------------//
    // Collision detection //
    //---------------------//

    // Set the contact variables to true before heading into the loop
    _xBlocked = _yTopBlocked = _yBottomBlocked = true;

    // Looping through the colliders three times to check wheter adjust the speed.
    for(int i = 0; i < COLLSION_ITER && (_xBlocked || _yTopBlocked || _yBottomBlocked); i++){

        QPointF nextMove = linearMovement(_speed);

        // Bounding box optimization
        // Checking every colliders if their bounding boxes are in close proximity to Enemys next move.
        // If that's the case, add them to a vector and discard the rest.
        QVector<SelectablePolygonF *> boundObjects;

        QRectF EnemyBounds(QPointF(qMin(_pos.x(), _pos.x() + nextMove.x()),
                            qMin(_pos.y(), _pos.y() + nextMove.y())),
                            QPointF(qMax(_pos.x() + _width, _pos.x() + _width + nextMove.x()),
                            qMax(_pos.y() + _height, _pos.y() + _height + nextMove.y())));

        for(int c = 0; c < n_colliders; c++){
            QRectF bounds(QPointF(qMin(EnemyBounds.left(), colliders[c]->boundingRect().left()),
                          qMin(EnemyBounds.top(), colliders[c]->boundingRect().top())),
                          QPointF(qMax(EnemyBounds.right(), colliders[c]->boundingRect().right()),
                          qMax(EnemyBounds.bottom(), colliders[c]->boundingRect().bottom())));

            if(bounds.width() < colliders[c]->boundingRect().width() + EnemyBounds.width() &&
               bounds.height() < colliders[c]->boundingRect().height() + EnemyBounds.height()){

                boundObjects.append(colliders[c]);

            }



        }
        // End of bounding box check

        _xBlocked = _yTopBlocked = _yBottomBlocked = false;

        QPointF projectedMove, originalMove;
        originalMove = nextMove;

        // Variables for bullet through paper prevention (Speculative contacts)

        float vectorLength;
        int segments;

        // End of it

        for(int c = 0; c < boundObjects.size() && !_xBlocked && !_yTopBlocked && !_yBottomBlocked; c++){

            // Bullet through paper prevention (Speculative contacts)

            for(int dir = 0; dir < 4; dir++){
                if(dir == 0 && nextMove.y() > 0) continue;
                if(dir == 1 && nextMove.y() < 0) continue;
                if(dir == 2 && nextMove.x() > 0) continue;
                if(dir == 3 && nextMove.x() < 0) continue;

                projectedMove.rx() = 0;
                projectedMove.ry() = 0;

                vectorLength = sqrt(pow(nextMove.x(),2) + pow(nextMove.y(),2));
                segments = 0;

                while(!boundObjects.at(c)->containsPoint(_collisionPoints[_currentState][dir*2]+_collisionOffset[_currentState][_lastDir] + _pos + projectedMove, Qt::OddEvenFill) &&
                      !boundObjects.at(c)->containsPoint(_collisionPoints[_currentState][dir*2+1]+_collisionOffset[_currentState][_lastDir] +_pos + projectedMove, Qt::OddEvenFill) &&
                      segments < vectorLength)
                {
                    projectedMove += nextMove/vectorLength;
                    segments++;
                }

                if(segments < vectorLength){
                    if(segments > 0){
                        projectedMove -= nextMove / vectorLength;
                    }
                    if(dir >= 2 && dir <= 3) nextMove.setX(projectedMove.x());
                    if(dir >= 0 && dir <= 1) nextMove.setY(projectedMove.y());
                }

            }

            // end




            for(int dir = 0; dir < 4; dir++){
                if(dir == 0 && nextMove.y() > 0) continue;
                if(dir == 1 && nextMove.y() < 0) continue;
                if(dir == 2 && nextMove.x() > 0) continue;
                if(dir == 3 && nextMove.x() < 0) continue;

                projectedMove.rx() = (dir >= 2? nextMove.x() : 0);
                projectedMove.ry() = (dir <  2? nextMove.y() : 0);

                while(boundObjects.at(c)->containsPoint(_collisionPoints[_currentState][dir*2]+_collisionOffset[_currentState][_lastDir] + _pos + projectedMove, Qt::OddEvenFill) ||
                      boundObjects.at(c)->containsPoint(_collisionPoints[_currentState][dir*2+1]+_collisionOffset[_currentState][_lastDir] +_pos+projectedMove, Qt::OddEvenFill)){
                    if(dir==0) projectedMove.ry()+=.1f;
                    if(dir==1) projectedMove.ry()-=.1f;
                    if(dir==2) projectedMove.rx()+=.1f;
                    if(dir==3) projectedMove.rx()-=.1f;
                }

                if(dir >= 2 && dir <= 3) nextMove.rx() = projectedMove.x();
                if(dir >= 0 && dir <= 1) nextMove.ry() = projectedMove.y();
            }

            if(nextMove.y() > originalMove.y() && originalMove.y() < 0){
                _yTopBlocked = true;
            }
            if(nextMove.y() < originalMove.y() && originalMove.y() > 0){
                _yBottomBlocked = true;
            }
            if(abs(nextMove.x() - originalMove.x()) > 0.01f){
                _xBlocked = true;
            }

            if(_xBlocked && _yTopBlocked && _speed.y() < 0){
                _speed.ry() = nextMove.ry() = 0;
            }
        }

        if(_yTopBlocked || _yBottomBlocked){
            _pos.ry() += nextMove.y();
            _speed.ry() = 0;

            if (_yBottomBlocked){
                _jumping = false;
            }
        }

        if(_xBlocked){
            _pos.rx() += nextMove.x();
            _speed.rx() = 0;
        }
    }

    //Move Enemy
    _pos += this->linearMovement(_speed);

    bool moveRequest = false;

    //Handle input
    if(_leftKeyPressed && !_rightKeyPressed){
        _speed.rx() -= _acc.x();
        moveRequest = true;
    }
    if(_rightKeyPressed && !_leftKeyPressed){
        _speed.rx() += _acc.x();
        moveRequest = true;
    }
    if(_jumpKeyPressed && !_jumping && _speed.y() < 30.f){
        _jumping = true;
        _jumpKeyPressed = false;
        _speed.ry() = (abs(_speed.x()) > MAXSPEEDX * SCALE + _acc.x())? -_jumpStartSpeedY: -_jumpStartSpeedY*.75;
        //playTune("qrc:/sounds/jump.wav");

    }
    // TODO: Kanske fixa dubbelsynk på hoppknappen

    if(_speed.x() > _maxSpeedX)
        _speed.rx() = _maxSpeedX;

    if(_speed.x() < -_maxSpeedX)
        _speed.rx() = -_maxSpeedX;

    if(_speed.y() < -_maxSpeedY)
        _speed.ry() = -_maxSpeedY;

    //Apply the force of gravity
    _speed.ry() += _acc.y();

    //Decelerate the Enemy's x movement if left or right wasn't pressed
    if(!moveRequest){
        if (_speed.x() < 0)
            _speed.rx() += _dec.x();
        if (_speed.x() > 0)
            _speed.rx() -= _dec.x();

        if(_speed.x() > 0 && _speed.x() < _dec.x())
            _speed.setX(0);
        if(_speed.x() < 0 && _speed.x() > -_dec.x())
            _speed.setX(0);

    }

    updateCurrentState();

}

void Enemy::checkItems(Item **items, int n_items){
    QVector<Item *> boundItems;

    QRectF EnemyBounds(QPointF(qMin(_pos.x(), _pos.x() + _speed.x()),
                        qMin(_pos.y(), _pos.y() + _speed.y())),
                        QPointF(qMax(_pos.x() + _width, _pos.x() + _width + _speed.x()),
                        qMax(_pos.y() + _height, _pos.y() + _height + _speed.y())));

    for(int c = 0; c < n_items; c++){
        QRectF bounds(QPointF(qMin(EnemyBounds.left(), items[c]->getHitbox().left()),
                      qMin(EnemyBounds.top(), items[c]->getHitbox().top())),
                      QPointF(qMax(EnemyBounds.right(), items[c]->getHitbox().right()),
                      qMax(EnemyBounds.bottom(), items[c]->getHitbox().bottom())));

        if(bounds.width() < items[c]->getHitbox().width() + EnemyBounds.width() &&
           bounds.height() < items[c]->getHitbox().height() + EnemyBounds.height()){

            boundItems.append(items[c]);

        }
    }

    for(int i = 0; i < boundItems.size(); i++){
        Item *item = boundItems.at(i);
        if(!item->isActive()) continue;
        for(int p = 0; p < N_COLLISIONPOINTS; p++){
            if(item->getHitbox().contains(_collisionPoints[_currentState][p] + _pos) && item->isActive()){
                item->inactivate();
                _score+= item->getScoreVal();
                //playTune("qrc:/sounds/coin.wav");

                return;
            }
        }
    }
}

void Enemy::keyInput(int key, bool down, QKeyEvent *e)
{
    switch(key) {
    case Qt::Key_Space:
        if(down && !e->isAutoRepeat()){
            _jumpKeyPressed = true;
        } else {
            _jumpKeyPressed = false;
        }
        break;
    case Qt::Key_F5:
        if  (!down){
            _pos = QPointF(300,100);
            _speed = QPointF(0,0);
        }
    break;
    case Qt::Key_Shift:
        if(down){
            _maxSpeedX = MAXSPEEDX * 2 * SCALE;
        } else {
            _maxSpeedX = MAXSPEEDX * SCALE;
        }
        break;
    case Qt::Key_Control:
        if(down){
            _maxSpeedX = MAXSPEEDX / 2 * SCALE;
        } else {
            _maxSpeedX = MAXSPEEDX * SCALE;
        }
        break;
    case Qt::Key_Left:
        if(down) _lastDir = LEFT;
        _leftKeyPressed = down;
        break;
    case Qt::Key_Right:
        if(down) _lastDir = RIGHT;
        _rightKeyPressed = down;
        break;
    case Qt::Key_Down:
        _downKeyPressed = down;
        break;
    default:
        break;
    }
    if(down) _lastKeyPressed = key;
}

void Enemy::setSpeed(double x)
{
    _lastDir = (x < 0)?LEFT:RIGHT;

    if(_xBlocked){
        x = -x;
    }
    _speed += QPointF(x,0);

}

QPointF Enemy::getCenter()
{
    return _pos;
}

Enemy::~Enemy()
{

}

int Enemy::getScore(){
    return _score;
}

void Enemy::setupCollisionPoints()
{
    //Collision points for normal sprite
    QPointF points0[8] = {QPointF(22,7), QPointF(65,7),       // Top collision points
                         QPointF(22,63), QPointF(65,63),     // Bottom collision points
                         QPointF(12,12), QPointF(12, 48),     // Left collision points
                         QPointF(75,12), QPointF(75,48)};   // Right collision points
    //Collision points for crouching sprite
    QPointF points1[8] = {QPointF(10,20), QPointF(70,20),       // Top collision points
                         QPointF(10,63), QPointF(53,63),     // Bottom collision points
                         QPointF(0,25), QPointF(0, 48),     // Left collision points
                         QPointF(75,25), QPointF(75,48)};   // Right collision points

    for(int i = 0; i < 8; i++){
        _collisionPoints[NORMAL][i] = points0[i];
        _collisionPoints[CROUCH][i] = points1[i];
        _collisionPoints[JUMP][i] = points0[i];
        _collisionPoints[WALK][i] = points0[i];
    }
    _collisionOffset[NORMAL][LEFT] = QPointF(0,0);
    _collisionOffset[NORMAL][RIGHT] = QPointF(-12,0);

    _collisionOffset[CROUCH][LEFT] = QPointF(0,0);
    _collisionOffset[CROUCH][RIGHT] = QPointF(0,0);

    _collisionOffset[JUMP][LEFT] = QPointF(0,0);
    _collisionOffset[JUMP][RIGHT] = QPointF(-12,0);

    _collisionOffset[WALK][LEFT] = QPointF(0,0);
    _collisionOffset[WALK][RIGHT] = QPointF(-12,0);

}

QPixmap *Enemy::getCurrentPixmap()
{
    Sprite *s = getSprite(spriteStateMap[_currentState]);
    if(!s->frameDuration) return s->pixmaps[0];
    int frame = (int)(getElapsed() / s->frameDuration) % s->nFrames;
    return s->pixmaps[frame];
}



QPointF Enemy::linearMovement(QPointF pixelsPerSecond){
    float secsElapsed = (getElapsed() - getLastUpdate()) / 1000.f;
#if DEBUG
    if(pixelsPerSecond.x())
        qDebug() << (getElapsed() - getLastUpdate()) / 1000.f << _speed << pixelsPerSecond * secsElapsed;
#endif
    return pixelsPerSecond * secsElapsed;
}

void Enemy::updateCurrentState()
{
    if((_lastKeyPressed == Qt::Key_Right && _rightKeyPressed) || (_rightKeyPressed && !_leftKeyPressed)){
        _lastDir = RIGHT;
    }
    if((_lastKeyPressed == Qt::Key_Left && _leftKeyPressed) || (_leftKeyPressed && !_rightKeyPressed)){
        _lastDir = LEFT;
    }
    if(abs(_speed.x()) > 0){
        _currentState = WALK;
        return;
    } else {
        if(_downKeyPressed){
            _currentState = CROUCH;
            return;
        }
        _currentState = NORMAL;
        return;
    }
}
