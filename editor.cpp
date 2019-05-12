#include "editor.h"

Editor::Editor(QString filename, QRectF window):
    _viewPort(0,0),
    _mousePressed(false),
    _menuRect(0,0,window.width(), 50),
    _mapRect(0,50,window.width(),window.height()-50),
    _map(new Map(filename)),
    _nWidgets(25)
{
    _widgets = (EditorWidget**)malloc(sizeof(EditorWidget*) * _nWidgets);
    for(int i = 0; i < _nWidgets; i++){
        _widgets[i] = new EditorWidget(PIXMAP_POLYGON, QRectF(i*45+5*(i+1), 2.5, 45, 45));
    }
}

void Editor::draw(QPainter *qp)
{
    qp->save();
    qp->translate(-_viewPort);
    qp->scale(_zoomfactor, _zoomfactor);
    _map->draw(qp, _viewPort, _mapRect, true);
    qp->restore();
    qp->fillRect(_menuRect, Qt::blue);
    for(int i = 0; i < _nWidgets ; i++){
        _widgets[i]->draw(qp);
    }

}

void Editor::keyInput(int key, bool down, QKeyEvent *e)
{
    switch(key){
    case Qt::Key_Left:
        if(down)
            _viewPort.rx()-=10;
        break;
    case Qt::Key_Right:
        if(down)
            _viewPort.rx()+=10;
        break;
    case Qt::Key_Up:
        if(down)
            _viewPort.ry()-=10;
        break;
    case Qt::Key_Down:
        if(down)
            _viewPort.ry()+=10;
        break;
    case Qt::Key_Shift:
        _shiftKeyPressed = down;
        break;
    case Qt::Key_Control:
        _controlPressed = down;
        break;
    case Qt::Key_A:
        // Move selected obstacles 10 pixels left
        if(down)
        {
            for(int i = 0; i < _map->getNObstacles(); i++){
                SelectablePolygonF * sp = _map->getObstacles()[i];
                if(sp->isSelected()){
                    sp->translate(QPointF(-10,0));
                }
            }
        }
        break;
    case Qt::Key_D:
        // Move selected obstacles 10 pixels right
        if(down){
            for(int i = 0; i < _map->getNObstacles(); i++){
                SelectablePolygonF * sp = _map->getObstacles()[i];
                if(sp->isSelected()){
                    sp->translate(QPointF(10,0));
                }
            }
        }
        break;
    case Qt::Key_W:
        // Move selected obstacles 10 pixels up
        if(down)
        {
            for(int i = 0; i < _map->getNObstacles(); i++){
                SelectablePolygonF * sp = _map->getObstacles()[i];
                if(sp->isSelected()){
                    sp->translate(QPointF(0,-10));
                }
            }

        }
        break;
    case Qt::Key_S:
        // Move selected obstacles 10 pixels down
        if(down)
        {
            for(int i = 0; i < _map->getNObstacles(); i++){
                SelectablePolygonF * sp = _map->getObstacles()[i];
                if(sp->isSelected()){
                    sp->translate(QPointF(0,10));
                }
            }
        }
        break;
    case Qt::Key_O:
        // Save map
        if(down){
            _map->saveMap("map.txt");
        }
        break;
    case Qt::Key_Plus:
        // Add node to selected obstacles
        if(down){
            if(_controlPressed){
                _map->addObstacle(QPointF(100,100));


            } else {
                for(int i = 0; i < _map->getNObstacles() - FRAME_ELEMENTS; i++){
                    SelectablePolygonF *sp = _map->getObstacles()[i];
                    if(sp->isSelected()){
                        sp->append((sp->first() + sp->last()) / 2);
                    }
                }
            }
        }
        break;
    case Qt::Key_Minus:
        // Remove node from selected obstacles
        if(down){
            for(int i = 0; i < _map->getNObstacles() - FRAME_ELEMENTS; i++){
                SelectablePolygonF *sp = _map->getObstacles()[i];
                if(sp->isSelected() && sp->length() > 3){
                    sp->pop_back();
                }
            }
        }
        break;
    case Qt::Key_Underscore:
        if(down){
            if(_controlPressed){
                for(int i = 0; i < _map->getNObstacles() - FRAME_ELEMENTS; i++){
                    SelectablePolygonF *sp = _map->getObstacles()[i];
                    if(sp->isSelected()){
                        _map->removeObstacle(i);
                        break;
                    }
                }

            }
        }
        break;
    case Qt::Key_Z:
        if(down){
            for(int i = 0; i < _map->getNObstacles() - FRAME_ELEMENTS; i++){
                SelectablePolygonF *sp = _map->getObstacles()[i];
                if(sp->isSelected()){
                    sp->setPixmap((PixmapIdentifier)(sp->getPixmap()-1));
                    break;
                }
            }
        }
        break;
    case Qt::Key_X:
        if(down){
            for(int i = 0; i < _map->getNObstacles() - FRAME_ELEMENTS; i++){
                SelectablePolygonF *sp = _map->getObstacles()[i];
                if(sp->isSelected()){
                    sp->setPixmap((PixmapIdentifier)(sp->getPixmap()+1));
                    break;
                }
            }
        }
        break;
    default:
        break;

    }
}

void Editor::zoom(bool in)
{
    if(in){
        _zoomfactor+=.1;
    } else {
        _zoomfactor-=.1;
    }
}

void Editor::mousePressEvent(QMouseEvent *e, bool pressed)
{
    if(_menuRect.contains(e->pos())){
        for(int i = 0; i < _nWidgets; i++){
            if(_widgets[i]->getRect()->contains(e->pos())){
                qDebug() << "Widget "<< i << "pushed";
            }
        }
    } else {
        if(pressed){
            _mouseLastClicked = (e->pos() + _viewPort )/_zoomfactor;
        }

        QPointF clickPos = (e->pos() + _viewPort) / _zoomfactor;
        if(pressed){


            for(int i = 0; i < _map->getNObstacles() - FRAME_ELEMENTS; i++){
                SelectablePolygonF *sp = _map->getObstacles()[i];

                // Point dragging test
                _selectedPoint = 0;
                for(int p = 0; p < sp->length(); p++){
                    if (QRectF(sp->at(p).x()-10, sp->at(p).y()-10, 20, 20).contains(clickPos)){
                        _selectedPoint = &(sp->data()[p]);


                        return;
                    }
                }
                // End of it */


                if(!_shiftKeyPressed) sp->setSelected(false);
                if(sp->containsPoint(clickPos,Qt::OddEvenFill)){
                    _selectedPolygon = sp;
                    if(!sp->isSelected()){
                        sp->setSelected(true);
                    }
                }
            }

            for(int i = 0; i < _map->getNItems(); i++){
                Item *item = _map->getItems()[i];
                if(!_shiftKeyPressed) item->setSelected(false);
                if(item->getHitbox().contains(clickPos)){
                    if(!item->isSelected()){
                        item->setSelected(true);
                    }
                }
            }
        }
    }
}

void Editor::mouseMoveEvent(QMouseEvent *e)
{
   QPointF movement = (e->pos() + _viewPort) / _zoomfactor - _mouseLastClicked;
   for(int i = 0; i < _map->getNObstacles() - FRAME_ELEMENTS; i++){
       SelectablePolygonF * sp = _map->getObstacles()[i];
       if(_selectedPoint){
           *_selectedPoint += movement;
           break;
       }


       if(sp->isSelected()){
           sp->translate(movement);
       }
   }
   for(int i = 0; i < _map->getNItems(); i++){
       Item *it = _map->getItems()[i];
       if(it->isSelected()){
           it->move(movement);
       }

   }
   _mouseLastClicked = (e->pos() + _viewPort) / _zoomfactor;
}

Editor::~Editor()
{
    delete _map;
}
