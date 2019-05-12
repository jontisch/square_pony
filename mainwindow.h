#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include "globals.h"
#include "item.h"
#include "player.h"
#include "map.h"
#include "editor.h"

enum MainStates{
    MAINSTATE_LOADING,
    MAINSTATE_MENU,
    MAINSTATE_GAME,
    MAINSTATE_EDITOR
};

#define NUM_COLLIDERS 10
#define COLLIDER_SPEED 0//-0.6


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private slots:
    void drawTimerTimeout();

private:
    void drawMenu(QPainter *qp);

    bool setupResources();
    void releaseResources();
    void updateEntities();

    Map *_map;

    Editor *_editor;

    QTimer *_drawTimer;
    SelectablePolygonF **_obstacles;

    MainStates _currentState;
};

#endif // MAINWINDOW_H
