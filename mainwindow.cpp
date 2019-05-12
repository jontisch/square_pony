#include "mainwindow.h"
#include <QtMath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _currentState(MAINSTATE_MENU)
{

    this->setFixedSize(1280, 690);
    initGlobals();
//    _map = 0;

    _drawTimer = new QTimer(this);

    _drawTimer->setInterval(10);

    connect(_drawTimer, SIGNAL(timeout()), this, SLOT(drawTimerTimeout()));

    _drawTimer->start();

}

void MainWindow::paintEvent(QPaintEvent *e){
    QPainter qp(this);
    qp.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    switch(_currentState){
    case MAINSTATE_LOADING:
    {
        qp.setBrush(Qt::blue);
        qp.drawRect(0,0,this->width(), this->height());

        break;
    }
    case MAINSTATE_MENU:
    {
        drawMenu(&qp);
        break;
    }
    case MAINSTATE_GAME:
    {
        setLastUpdate();

        QPointF viewPort = (_map->getCenterPoint() - QPointF(qp.window().width()/2,qp.window().height()/2));
        if(viewPort.x() < 0){
            viewPort.setX(0);
        }
        if(viewPort.y() < 0){
            viewPort.setY(0);
        }
        if(viewPort.x()+qp.window().width() > _map->getWidth()){
            viewPort.setX(_map->getWidth()-qp.window().width());
        }
        if(viewPort.y()+qp.window().height() > _map->getHeight()){
            viewPort.setY(_map->getHeight()-qp.window().height());
        }
        qp.translate(-viewPort);
        qp.setOpacity(1.0);
        _map->draw(&qp, viewPort, qp.window());

        qp.setPen(Qt::black);
        unsigned long time = getElapsed();
        char t[10];
        sprintf(t, "%02lu:%02lu:%01lu",time/60000, (time/1000)%60, (time/100)%10);
        qp.drawText(viewPort + QPointF(25,50), t);
        break;
    }
    case MAINSTATE_EDITOR:
    {
        _editor->draw(&qp);
        break;
    }
    default:
        break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e){
    switch(_currentState){
    case MAINSTATE_LOADING:
        if(e->key() == Qt::Key_Return){
            _currentState = MAINSTATE_MENU;
            resetElapsed();
        }
        break;
    case MAINSTATE_MENU:
        if(e->key() == Qt::Key_Return){
            //_map = new Map(":/maps/test.txt");
            _map = new Map("map.txt");
            _currentState = MAINSTATE_GAME;
            resetElapsed();
        }
        if(e->key() == Qt::Key_E){
            _editor = new Editor("map.txt", this->rect());
            _currentState = MAINSTATE_EDITOR;
        }

        break;
    case MAINSTATE_GAME:
    {

        if(e->key() == Qt::Key_Escape){
            _currentState = MAINSTATE_MENU;
            delete _map;
            break;
        }
        _map->keyInput(e->key(), true, e);

        break;
    }
    case MAINSTATE_EDITOR:
    {
        if(e->key() == Qt::Key_Escape){
            _currentState = MAINSTATE_MENU;
            delete _editor;
            break;
        }
        _editor->keyInput(e->key(), true, e);
        break;
    }

    default:
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e){
    switch(_currentState){
    case MAINSTATE_LOADING:
        if(e->key() == Qt::Key_Return){
            _currentState = MAINSTATE_MENU;
            resetElapsed();
        }
        break;
    case MAINSTATE_MENU:
        break;
    case MAINSTATE_GAME:
    {
        _map->keyInput(e->key(), false, e);
        break;
    }
    case MAINSTATE_EDITOR:
    {
        _editor->keyInput(e->key(), false, e);
        break;
    }

    default:
        break;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    switch (_currentState) {
    case MAINSTATE_EDITOR:
        _editor->mousePressEvent(event, true);
        break;
    default:
        break;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    switch (_currentState) {
    case MAINSTATE_EDITOR:
        _editor->mousePressEvent(event, false);
        break;
    default:
        break;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    switch (_currentState) {
    case MAINSTATE_EDITOR:
        _editor->mouseMoveEvent(event);
        break;
    default:
        break;
    }
}


void MainWindow::wheelEvent(QWheelEvent *event)
{
    switch (_currentState) {
    case MAINSTATE_EDITOR:
        _editor->zoom(event->delta()>0);
        break;
    default:
        break;
    }
}


void MainWindow::drawTimerTimeout(){
    this->updateEntities();

    this->repaint();
}

void MainWindow::drawMenu(QPainter *qp)
{
    qp->save();
    qp->setBrush(Qt::red);
    qp->drawRect(0,0,qp->window().width(), qp->window().height());
    qp->setBrush(Qt::white);
    qp->setPen(Qt::white);
    qp->setFont(QFont("Fixedsys", 60, 10));
    qp->drawText(30,20,300,100,0,"SQUARE PONY");
    qp->drawText(30,60,300,100,0,"Return > Game");
    qp->drawText(30,100,300,100,0,"E      > Editor");
    qp->fillRect(215,135,210,210, Qt::white);
    qp->setBrush(Qt::red);
    qp->setPen(Qt::red);
    qp->drawPie(170,90,300,300,0,-(getElapsed()*2%5760));
    qp->restore();
    return;
}

bool MainWindow::setupResources()
{
    return true;
}

void MainWindow::releaseResources()
{

}

void MainWindow::updateEntities(){
    deleteMediaPlayer();
    switch(_currentState){
    case MAINSTATE_LOADING:
    {
        //if(getElapsed() > 2880){
        //    _currentState = MAINSTATE_MENU;
            //playTune("qrc:/sounds/start.wav");
        //    resetElapsed();
        //}
        break;
    }
    case MAINSTATE_MENU:
    {
        //if(getElapsed() > 2880){
        //    _currentState = MAINSTATE_GAME;
            //playTune("qrc:/sounds/start.wav");
        //    resetElapsed();
        //}
        break;
    }
    case MAINSTATE_GAME:
    {
        _map->updateEntities();
        break;
    }
    case MAINSTATE_EDITOR:
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete _map;
    delete _editor;
    delete _drawTimer;
}
