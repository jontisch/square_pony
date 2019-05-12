#ifndef GLOBALS_H
#define GLOBALS_H

#include <QtMultimedia/QMediaPlayer>
#include <QElapsedTimer>

#define SCALE 100.f
#define DEBUG false

enum PixmapIdentifier{
    PIXMAP_CLOUD,
    PIXMAP_BUSH,
    PIXMAP_COIN,
    PIXMAP_TILE,
    PIXMAP_GRASS,
    PIXMAP_POLYGON,
    N_PIXMAPS
};

enum SpriteIdentifier{
    SPRITE_SQUARE_PONY_STANDING,
    SPRITE_SQUARE_PONY_CROUCHING,
    SPRITE_SQUARE_PONY_WALKING
};

enum SpriteDirection{
    LEFT = 0,
    RIGHT = 1,
};

enum SpriteState{
    NORMAL = 0,
    CROUCH,
    JUMP,
    WALK,
    N_STATES
};
struct Sprite{
    QPixmap **pixmaps;
    int nFrames;
    int width;
    int height;
    int frameDuration;
};
#define N_SPRITES 3

void initSprites();
Sprite *getSprite(SpriteIdentifier index);

void initPixmaps();
QPixmap *getPixmap(PixmapIdentifier index);

void playTune(QString filename);
void deleteMediaPlayer();

qint64 getLastUpdate();
void setLastUpdate();

void initGlobals();

qint64 getElapsed();
void resetElapsed();

#endif // GLOBALS_H

