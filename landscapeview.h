#ifndef LANDSCAPEVIEW_H
#define LANDSCAPEVIEW_H

#include <QGLWidget>
#include <QImage>
#include <QFile>
#include <vector>
#include <fstream>

void exportLandscape(QString path, float scale);

class MyVec3 {
public:
  float x, y, z;
  MyVec3() {}
  MyVec3(float x, float y, float z) {this->x = x; this->y = y; this->z = z;}
};

class MyVec2 {
public:
  float x, y;
};

class Index {
public:
  float p, t, n;
};

class MyColor {
public:
    int r, g, b;
    MyColor() {}
    MyColor(int r, int g, int b) {this->r = r; this->g = g; this->b = b;}
};

class LandscapeView : public QGLWidget {
   private:
      void createArrays(int size);
      void clearArrays();

      GLfloat xRot;
      GLfloat yRot;
      GLfloat zRot;
      GLfloat zTra;
      GLfloat nSca;

      QPoint ptrMousePosition;

      void scale_plus();
      void scale_minus();
      void rotate_up();
      void rotate_down();
      void rotate_left();
      void rotate_right();
      void translate_down();
      void translate_up();
      void defaultScene();

      void drawAxis();
      void drawFigure();

      void getColorArray(MyColor color1, MyColor color2, float streight);

   protected:
      void initializeGL();
      void reinitGL();
      void resizeGL(int nWidth, int nHeight);
      void paintGL();
      void mousePressEvent(QMouseEvent* pe);
      void mouseMoveEvent(QMouseEvent* pe);
      //void mouseReleaseEvent(QMouseEvent* pe);
      void wheelEvent(QWheelEvent* pe);
      void keyPressEvent(QKeyEvent* pe);

   public:
      LandscapeView(QWidget* parent = 0);
      void createLandscape(const QImage &heightmap, float scale = 1.0f, float streight = 1.0f);
};

#endif
