#include "map.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTime>


Map::Map(QString filename)
{
    qsrand(QTime::currentTime().msec());
    setupObstacles(filename);
    _p = new Player(QPointF(22,300));

    // Create the enemy
    _nmy = new Enemy(QPointF(300,300));

    // Create the clouds
    _clouds = (WrappingObject **)malloc(sizeof(WrappingObject *) * 10);
    for(int i = 0; i< 10; i++){
        double speed = -(qrand()%100/1000.f+.1);
        _clouds[i] = new WrappingObject(getPixmap(PIXMAP_CLOUD), QPointF(qrand()%5000, qrand()%300+70), QPointF(speed,0)*SCALE, QSizeF(126,64), &_mapRect);
    }


}

void Map::draw(QPainter *qp, QPointF viewPort, QRectF renderRect, bool editorMode)
{
//    qp->save();
    // TODO: Make this global or at least a member
    QLinearGradient bgGradient;
    //qp->setWindow(renderRect.toRect());
    bgGradient.setStart(0,0);
    bgGradient.setFinalStop(QPointF(0, _height));
    bgGradient.setColorAt(0, Qt::cyan);
    bgGradient.setColorAt(1, Qt::blue);
    qp->setBrush(QBrush(bgGradient));
    qp->drawRect(QRectF(0,0,_width,_height));

    // CLOUDS
    if(!editorMode){
        for( int i = 0; i < 10; i++ ){
            _clouds[i]->draw(qp);
        }
    }




//    qp->shear(0.1,-0.1);
    for(int i = 0; i < _nObs; i++){

        if(_obstacles[i]->isSelected()){
            qp->setPen(QPen(QBrush(Qt::magenta),10));
        } else {
            qp->setPen(Qt::NoPen);
        }
        qp->setBrush(QBrush(*getPixmap(_obstacles[i]->getPixmap())));
        qp->drawConvexPolygon(*_obstacles[i]);
        // Paint the corner points if in editor and skip the bushing.
        // Else: the other way around
        if(editorMode){
            for(int p = 0; p < _obstacles[i]->length(); p++){
                qp->fillRect(_obstacles[i]->at(p).x()-5, _obstacles[i]->at(p).y()-5, 10,10, Qt::green);
            }
        } else {
            qp->setBrush(Qt::green);
            qp->setBrush(QBrush(*getPixmap(PIXMAP_GRASS)));
            qp->drawConvexPolygon(*_shrubbery[i]);
        }
    }


    for(int i = 0; i < _nItems; i++){
        _items[i]->draw(qp);
        if(_items[i]->isSelected()){
            qp->fillRect(_items[i]->getHitbox(), Qt::cyan);
        }
    }
    _p->draw(qp);

    _nmy->draw(qp);
    //Print score text if not in editor.
    if(!editorMode){
        qp->setFont(QFont("Helvetica", 32, 5));
        qp->setPen(Qt::black);
        char s[11];
        sprintf(s, "Score: %d", _p->getScore());
        qp->drawText(viewPort + QPointF(430,50), s);
    }
    qp->drawPixmap(QPointF(300,440), *getPixmap(PIXMAP_BUSH));

}

SelectablePolygonF **Map::getObstacles()
{
    return _obstacles;
}

int Map::getNObstacles()
{
    return _nObs;
}

void Map::addObstacle(QPointF)
{
    SelectablePolygonF **temp = (SelectablePolygonF**)malloc(sizeof(SelectablePolygonF*) * _nObs);
    for(int i = 0; i < _nObs; i++){
        temp[i] = new SelectablePolygonF(_obstacles[i]);
        delete _obstacles[i];
    }
    _nObs++;
    _obstacles = (SelectablePolygonF**)malloc(sizeof(SelectablePolygonF*) * _nObs);

    _obstacles[0] = new SelectablePolygonF(QRectF(100,100,100,100));
    for(int i = 1; i < _nObs; i++){
        _obstacles[i] = new SelectablePolygonF(temp[i-1]);
        delete temp[i-1];
    }
    delete temp;
}

void Map::removeObstacle(int index)
{
    SelectablePolygonF **temp = (SelectablePolygonF**)malloc(sizeof(SelectablePolygonF*) * _nObs);
    for(int i = 0; i < _nObs; i++){
        temp[i] = new SelectablePolygonF(_obstacles[i]);
        delete _obstacles[i];
    }
    _nObs--;
    _obstacles = (SelectablePolygonF**)malloc(sizeof(SelectablePolygonF*) * _nObs);

    int j = 0;
    for(int i = 0; i < _nObs; i++){
        if(i == index) j++;
        _obstacles[i] = new SelectablePolygonF(temp[j]);
        delete temp[j];
        j++;
    }
    delete temp[index];
    delete temp;
}

void Map::keyInput(int key, bool down, QKeyEvent *e)
{
    _p->keyInput(key, down, e);
    if(key == Qt::Key_F5 && !down){
        resetElapsed();
    }
    if(key == Qt::Key_F8 && down){
        saveMap("map.txt");
    }

}

void Map::updateEntities()
{
    _p->updatePosition(_obstacles, _nObs);
    _nmy->setSpeed(100);
    _nmy->updatePosition(_obstacles, _nObs);

    _p->checkItems(_items, _nItems);

    for(int i = 0; i < 10; i++){
        _clouds[i]->updatePosition();

    }
}

QPointF Map::getCenterPoint()
{
    return _p->getCenter();
}

void Map::saveMap(QString filename)
{
    qDebug() << "Saving map";
    QFile mapFile(filename);
    if(!mapFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "File" << filename << "not open";
    }

    QTextStream out(&mapFile);
    // Hash to mark section "Dimensions and quantities"
    out << '#' << endl;
    out << _nObs - FRAME_ELEMENTS << ';' << _nItems << ';';
    out << _width << ';' << _height << endl;
    // $ to mark section "Items"
    out << "// Items" << endl;
    out << '$' << endl;
    for(int i = 0; i < _nItems; i++){
        out << _items[i]->getPos().x() << ',' << _items[i]->getPos().y() << endl;
    }

    // Section Obstacles
    out << "// Obstacles" << endl;
    for(int i = 0; i < _nObs - FRAME_ELEMENTS; i++){
        out << _obstacles[i]->toString() << endl;
    }

    mapFile.close();
}

Item **Map::getItems()
{
    return _items;
}

int Map::getNItems()
{
    return _nItems;
}

int Map::getWidth()
{
    return _width;
}

int Map::getHeight()
{
    return _height;
}

Map::~Map()
{
    for(int i = 0; i < _nObs; i++){
        delete _obstacles[i];
    }
    delete _obstacles;
    delete _p;
    delete _nmy;
}

void Map::setupObstacles(QString filename)
{
    QFile mapFile(filename);
    if(!mapFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "File" << filename << "not open";
    }

    QTextStream in(&mapFile);
    if(in.atEnd())
    {
        qDebug() << "No map name found";
        mapFile.close();
        return;
    }
    // flag set if hash is found
    bool nObsFound = false;
    bool itemListFound = false;
    // number found in map file
    _nObs = 0;
    // The index in polygon array
    int obsCnt = 0;
    int itemCnt = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        if(line.length() < 1) break;
        if(line.at(0) == '/') continue;
        if(line.at(0) == '#'){
            nObsFound = true;
            continue;
        }
        if(nObsFound){
            QStringList data = line.split(';');
            _nObs = data[0].toInt() + FRAME_ELEMENTS;
            _nItems = data[1].toInt();
            _width = data[2].toInt();
            _height = data[3].toInt();
            _mapRect = QRectF(0,0,_width,_height);
            qDebug() << _nObs << "Obstacles";
            qDebug() << _nItems << "Items";
            nObsFound = false;
            _obstacles = (SelectablePolygonF **)malloc(sizeof(SelectablePolygonF *) * _nObs);
            _shrubbery = (QPolygonF **)malloc(sizeof(QPolygonF *) * _nObs);
            _items = (Item **)malloc(sizeof(Item *) * _nItems);
            continue;
        }
        if(line.at(0) == '$'){
            itemListFound = true;
            continue;
        }
        if(itemListFound){

            QStringList sl = line.split(',');

            _items[itemCnt] = new Item(QPointF(sl[0].toDouble(),sl[1].toDouble()), 10);
            itemCnt++;
            if(itemCnt >= _nItems) itemListFound = false;
            continue;
        }
        QStringList sl = line.split(';');
        _obstacles[obsCnt] = new SelectablePolygonF();
        QVector<QPointF> poly(sl.length());
        for(int i = 0; i < sl.length(); i++){
            QStringList point = sl[i].split(',');
            double x = point[0].toDouble();
            double y = point[1].toDouble();
            poly[i] = QPointF(x,y);
        }
        _obstacles[obsCnt] = new SelectablePolygonF(poly);
        obsCnt++;
    }
    _obstacles[obsCnt] = new SelectablePolygonF(QRectF(0,0,32,_height));
    _obstacles[obsCnt+1] = new SelectablePolygonF(QRectF(_width-32,0,32,_height));
    _obstacles[obsCnt+2] = new SelectablePolygonF(QRectF(0,_height-32,_width,32));
    for(int i = 0; i < _nObs; i++){
        QPolygonF bush(_obstacles[i]->translated(0,-30).subtracted(*_obstacles[i]));
        bush.translate(0,20);
        _shrubbery[i] = new QPolygonF(bush);
    }
}

