#include "globals.h"
#include <QDebug>
#include <QPixmap>
#include <QtMultimedia/QSoundEffect>

QMediaPlayer *mediaPlayer;
QSoundEffect *soundEffect;
Sprite **sprites;
QPixmap **pixmaps;
QElapsedTimer *elapsedTimer;
qint64 lastUpdate = 0;


void playTune(QString filename){
    if(!soundEffect) soundEffect = new QSoundEffect;
    soundEffect->setVolume(.25f);
    soundEffect->setSource(QUrl(filename));
    soundEffect->play();
    //mediaPlayer = new QMediaPlayer;
    //mediaPlayer->setVolume(10);
    //mediaPlayer->setMedia(QUrl(filename));
    //mediaPlayer->play();
}

void deleteMediaPlayer(){
//    if(mediaPlayer)
//        if(mediaPlayer->mediaStatus() == QMediaPlayer::EndOfMedia)
//            delete mediaPlayer;
}


qint64 getLastUpdate(){
    return lastUpdate;
}

void setLastUpdate(){
    lastUpdate = getElapsed();
}

void initGlobals(){
    initSprites();
    initPixmaps();
    elapsedTimer = new QElapsedTimer();
    elapsedTimer->start();
}

void initSprites()
{
    sprites = (Sprite **)malloc(sizeof(Sprite *)* N_SPRITES);

    sprites[SPRITE_SQUARE_PONY_STANDING] = (Sprite *)malloc(sizeof(Sprite));
    sprites[SPRITE_SQUARE_PONY_STANDING]->height = 64;
    sprites[SPRITE_SQUARE_PONY_STANDING]->width = 76;
    sprites[SPRITE_SQUARE_PONY_STANDING]->nFrames = 1;
    sprites[SPRITE_SQUARE_PONY_STANDING]->pixmaps = (QPixmap**)malloc(sizeof(QPixmap*) * sprites[SPRITE_SQUARE_PONY_STANDING]->nFrames);
    sprites[SPRITE_SQUARE_PONY_STANDING]->pixmaps[0] = new QPixmap(":/graphics/square_pony.png");
    sprites[SPRITE_SQUARE_PONY_STANDING]->frameDuration = 0;

    sprites[SPRITE_SQUARE_PONY_CROUCHING] = (Sprite *)malloc(sizeof(Sprite));
    sprites[SPRITE_SQUARE_PONY_CROUCHING]->height = 64;
    sprites[SPRITE_SQUARE_PONY_CROUCHING]->width = 76;
    sprites[SPRITE_SQUARE_PONY_CROUCHING]->nFrames = 1;
    sprites[SPRITE_SQUARE_PONY_CROUCHING]->pixmaps = (QPixmap**)malloc(sizeof(QPixmap*) * sprites[SPRITE_SQUARE_PONY_STANDING]->nFrames);
    sprites[SPRITE_SQUARE_PONY_CROUCHING]->pixmaps[0] = new QPixmap(":/graphics/square_pony_bow.png");
    sprites[SPRITE_SQUARE_PONY_CROUCHING]->frameDuration = 0;

    sprites[SPRITE_SQUARE_PONY_WALKING] = (Sprite *)malloc(sizeof(Sprite));
    sprites[SPRITE_SQUARE_PONY_WALKING]->height = 64;
    sprites[SPRITE_SQUARE_PONY_WALKING]->width = 76;
    sprites[SPRITE_SQUARE_PONY_WALKING]->nFrames = 4;
    sprites[SPRITE_SQUARE_PONY_WALKING]->pixmaps = (QPixmap**)malloc(sizeof(QPixmap*) * sprites[SPRITE_SQUARE_PONY_STANDING]->nFrames);
    sprites[SPRITE_SQUARE_PONY_WALKING]->frameDuration = 125;
    QPixmap walking(":/graphics/square_pony_walking.png");
    for(int i = 0; i < sprites[SPRITE_SQUARE_PONY_WALKING]->nFrames; i++){
        sprites[SPRITE_SQUARE_PONY_WALKING]->pixmaps[i] = new QPixmap(walking.copy(QRect(i*76, 0, 76, 64)));
    }
}

Sprite *getSprite(SpriteIdentifier index){
    return sprites[index];
}

void initPixmaps()
{
    pixmaps = (QPixmap **)malloc(sizeof(QPixmap *) * N_PIXMAPS);

    pixmaps[PIXMAP_CLOUD] = new QPixmap(":/graphics/cloud.png");
    pixmaps[PIXMAP_BUSH] = new QPixmap(":/graphics/bush.png");
    pixmaps[PIXMAP_COIN] = new QPixmap(":/graphics/coin.gif");
    pixmaps[PIXMAP_TILE] = new QPixmap(":/graphics/tile.png");
    pixmaps[PIXMAP_GRASS] = new QPixmap(":/graphics/grass.png");
    pixmaps[PIXMAP_POLYGON] = new QPixmap(":/graphics/polygon.png");
}

QPixmap *getPixmap(PixmapIdentifier index)
{
    return pixmaps[index];
}

qint64 getElapsed()
{
    return elapsedTimer->elapsed();
}

void resetElapsed()
{
    elapsedTimer->restart();
}
