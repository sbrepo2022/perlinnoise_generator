#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QFileDialog>
#include <ctime>
#include "noise.h"
#include "landscapeview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    noise::PerlinNoise generator;
    LandscapeView view3d;
    QImage HeightMap;

public slots:
    void createTexture();
    void saveTexture();
    void saveLandscape();
};

#endif // MAINWINDOW_H
