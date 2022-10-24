#include "landscapeview.h"
#include <QtGui>
#include <math.h>

//int NumVertices;
int tWidth, tHeight;
MyVec3 *VertexArray;
MyColor *ColorArray;
int wndWidth, wndHeight;

void exportLandscape(QString path, float scale) {
    QFile file(path);

    if (file.open(QIODevice::WriteOnly))
    {
        file.write("# Landscape created by SPNG (Simple Perline Noise Generator)\n");
        file.write("o Landscape\n");
        for (int i = 0; i < tWidth * tHeight; i++) {
            file.write("v ");
            file.write(QString::number(VertexArray[i].x * scale, 'f', 6).toUtf8() + " ");
            file.write(QString::number(VertexArray[i].z * scale, 'f', 6).toUtf8() + " ");
            file.write(QString::number(VertexArray[i].y * scale, 'f', 6).toUtf8() + "\n");
        }
        file.write("s off\n");
        for (int y = 0; y < tHeight - 1; y++) {
            for (int x = 0; x < tWidth - 1; x++) {
                file.write("f ");
                file.write(QString::number(tWidth * y + x + 1).toUtf8() + " ");
                file.write(QString::number(tWidth * y + (x + 1) + 1).toUtf8() + " ");
                file.write(QString::number(tWidth * (y + 1) + (x + 1) + 1).toUtf8() + " ");
                file.write(QString::number(tWidth * (y + 1) + x + 1).toUtf8() + "\n");
            }
        }
        file.close();
    }
}

void LandscapeView::createArrays(int size) {
    VertexArray = new MyVec3[size];
    ColorArray = new MyColor[size];
}

void LandscapeView::clearArrays() {
    if (VertexArray != nullptr)
        delete [] VertexArray;
    if (ColorArray != nullptr)
        delete [] ColorArray;
}

LandscapeView::LandscapeView(QWidget* parent) : QGLWidget(parent)
{
   xRot=-90; yRot=0; zRot=0; zTra=0; nSca=1; tWidth = 0; tHeight = 0;
}

void LandscapeView::initializeGL()
{
   qglClearColor(Qt::black);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
   //glEnable(GL_CULL_FACE);

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);
}

void LandscapeView::reinitGL() {

}

void LandscapeView::resizeGL(int nWidth, int nHeight)
{
   wndWidth = nWidth;
   wndHeight = nHeight;
}

void LandscapeView::paintGL()
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   GLfloat ratio=(GLfloat)wndHeight/(GLfloat)wndWidth;

   if (wndWidth>=wndHeight)
      glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -10.0, 10.0);
   else
      glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -10.0, 10.0);

   glViewport(0, 0, (GLint)wndWidth, (GLint)wndHeight);
   //-------
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glScalef(nSca, nSca, nSca);
   glTranslatef(0.0f, zTra, 0.0f);
   glRotatef(xRot, 1.0f, 0.0f, 0.0f);
   glRotatef(yRot, 0.0f, 1.0f, 0.0f);
   glRotatef(zRot, 0.0f, 0.0f, 1.0f);

   drawAxis();
   drawFigure();
}

void LandscapeView::mousePressEvent(QMouseEvent* pe)
{
   ptrMousePosition = pe->pos();
}

/*void LandscapeView::mouseReleaseEvent(QMouseEvent* pe)
{

} */

void LandscapeView::mouseMoveEvent(QMouseEvent* pe)
{
   xRot += 180*(GLfloat)(pe->y()-ptrMousePosition.y())/height();
   zRot += 180*(GLfloat)(pe->x()-ptrMousePosition.x())/width();

   ptrMousePosition = pe->pos();

   updateGL();
}

void LandscapeView::wheelEvent(QWheelEvent* pe)
{
   if ((pe->delta())>0) scale_plus(); else if ((pe->delta())<0) scale_minus();

   updateGL();
}

void LandscapeView::keyPressEvent(QKeyEvent* pe)
{
   switch (pe->key())
   {
      case Qt::Key_Plus:
         scale_plus();
      break;

      case Qt::Key_Equal:
         scale_plus();
      break;

      case Qt::Key_Minus:
         scale_minus();
      break;

      case Qt::Key_Up:
         rotate_up();
      break;

      case Qt::Key_Down:
         rotate_down();
      break;

      case Qt::Key_Left:
        rotate_left();
      break;

      case Qt::Key_Right:
         rotate_right();
      break;

      case Qt::Key_Z:
         translate_down();
      break;

      case Qt::Key_X:
         translate_up();
      break;

      case Qt::Key_R:
         reinitGL();
      break;

      case Qt::Key_Space:
         defaultScene();
      break;

      case Qt::Key_Escape:
         this->close();
      break;
   }

   updateGL();
}

void LandscapeView::scale_plus()
{
   nSca = nSca*1.1;
}

void LandscapeView::scale_minus()
{
   nSca = nSca/1.1;
}

void LandscapeView::rotate_up()
{
   xRot += 1.0;
}

void LandscapeView::rotate_down()
{
   xRot -= 1.0;
}

void LandscapeView::rotate_left()
{
   zRot += 1.0;
}

void LandscapeView::rotate_right()
{
   zRot -= 1.0;
}

void LandscapeView::translate_down()
{
   zTra -= 0.05;
}

void LandscapeView::translate_up()
{
   zTra += 0.05;
}

void LandscapeView::defaultScene()
{
   xRot=-90; yRot=0; zRot=0; zTra=0; nSca=1;
}

void LandscapeView::drawAxis()
{
   glLineWidth(3.0f);

   glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
   glBegin(GL_LINES);
      glVertex3f( 100.0f,  0.0f,  0.0f);
      glVertex3f(-100.0f,  0.0f,  0.0f);
   glEnd();

   QColor halfGreen(0, 128, 0, 255);
   qglColor(halfGreen);
   glBegin(GL_LINES);
      glVertex3f( 0.0f,  100.0f,  0.0f);
      glVertex3f( 0.0f, -100.0f,  0.0f);

      glColor4f(0.00f, 0.00f, 1.00f, 1.0f);
      glVertex3f( 0.0f,  0.0f,  100.0f);
      glVertex3f( 0.0f,  0.0f, -100.0f);
   glEnd();
}

void LandscapeView::getColorArray(MyColor color1, MyColor color2, float streight) {
    float factor;
    for (int y = 0; y < tHeight; y++) {
        for (int x = 0; x < tWidth; x++) {
            factor = VertexArray[tWidth * y + x].z / streight + 0.5f;
            ColorArray[tWidth * y + x] = MyColor(color1.r * (1.0f - factor) + color2.r * factor, color1.g * (1.0f - factor) + color2.g * factor, color1.b * (1.0f - factor) + color2.b * factor);
        }
    }
}

/*void LandscapeView::createLandscape(const QImage &texture, float scale, float streight) {
    clearArrays();
    NumVertices = (texture.width() - 1) * (texture.height() - 1) * 4;
    createArrays(NumVertices);

    float currScale = 1.0f;
    if (texture.width() >= texture.height())
        currScale = scale * 1.0f / texture.width();
    else
        currScale = scale * 1.0f / texture.height();

    int currX = 0, currY = 0;
    for (int i = 0; i < NumVertices; i += 4) {
        VertexArray[i] = MyVec3(((float)currX - (float)texture.width() / 2) * currScale * (-1.0f), ((float)currY - (float)texture.height() / 2) * currScale, ((float)texture.pixelColor(currX, currY).blue() / 255.0f - 0.5f) * streight);
        VertexArray[i + 1] = MyVec3(((float)currX + 1.0f - (float)texture.width() / 2) * currScale * (-1.0f), ((float)currY - (float)texture.height() / 2) * currScale, ((float)texture.pixelColor(currX + 1, currY).blue() / 255.0f - 0.5f) * streight);
        VertexArray[i + 2] = MyVec3(((float)currX + 1.0f - (float)texture.width() / 2) * currScale * (-1.0f), ((float)currY + 1.0f - (float)texture.height() / 2) * currScale, ((float)texture.pixelColor(currX + 1, currY + 1).blue() / 255.0f - 0.5f) * streight);
        VertexArray[i + 3] = MyVec3(((float)currX - (float)texture.width() / 2) * currScale * (-1.0f), ((float)currY + 1.0f - (float)texture.height() / 2) * currScale, ((float)texture.pixelColor(currX, currY + 1).blue() / 255.0f - 0.5f) * streight);
        currX++;
        if (currX >= texture.width() - 1) {
            currX = 0;
            currY++;
        }
    }
    getColorArray(MyColor(0, 0, 0), MyColor(255, 255, 255), streight);
}*/

void LandscapeView::createLandscape(const QImage &texture, float scale, float streight) {
    clearArrays();
    tWidth = texture.width();
    tHeight = texture.height();
    createArrays(tWidth * tHeight);

    float currScale = 1.0f;
    if (tWidth >= tHeight)
        currScale = scale * 1.0f / tWidth;
    else
        currScale = scale * 1.0f / tHeight;

    for (int y = 0; y < tHeight; y++) {
        for (int x = 0; x < tWidth; x++) {
            VertexArray[tWidth * y + x] = MyVec3(((float)x - (float)tWidth / 2) * currScale * (-1.0f), ((float)y - (float)tHeight / 2) * currScale, ((float)texture.pixelColor(x, y).blue() / 255.0f - 0.5f) * streight);
        }
    }
    getColorArray(MyColor(0, 0, 0), MyColor(255, 255, 255), streight);
}

/*void LandscapeView::drawFigure()
{
  glBegin(GL_QUADS);

  for(int i = 0; i < NumVertices; i++) {
    glColor3f((float)ColorArray[i].r / 255.0f, (float)ColorArray[i].g / 255.0f, (float)ColorArray[i].b / 255.0f);
    glVertex3f(VertexArray[i].x, VertexArray[i].y, VertexArray[i].z);
  }

  glEnd();
}*/

void LandscapeView::drawFigure()
{
  glBegin(GL_QUADS);

  int index = 0;
  for (int y = 0; y < tHeight - 1; y++) {
      for(int x = 0; x < tWidth - 1; x++) {
          index = tWidth * y + x;
          glColor3f((float)ColorArray[index].r / 255.0f, (float)ColorArray[index].g / 255.0f, (float)ColorArray[index].b / 255.0f);
          glVertex3f(VertexArray[index].x, VertexArray[index].y, VertexArray[index].z);
          index = tWidth * y + x + 1;
          glColor3f((float)ColorArray[index].r / 255.0f, (float)ColorArray[index].g / 255.0f, (float)ColorArray[index].b / 255.0f);
          glVertex3f(VertexArray[index].x, VertexArray[index].y, VertexArray[index].z);
          index = tWidth * (y + 1) + x + 1;
          glColor3f((float)ColorArray[index].r / 255.0f, (float)ColorArray[index].g / 255.0f, (float)ColorArray[index].b / 255.0f);
          glVertex3f(VertexArray[index].x, VertexArray[index].y, VertexArray[index].z);
          index = tWidth * (y + 1) + x;
          glColor3f((float)ColorArray[index].r / 255.0f, (float)ColorArray[index].g / 255.0f, (float)ColorArray[index].b / 255.0f);
          glVertex3f(VertexArray[index].x, VertexArray[index].y, VertexArray[index].z);
      }
  }

  glEnd();
}

