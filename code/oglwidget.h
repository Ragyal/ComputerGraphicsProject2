///////////////////////////////////////////////////////////
/// Annika Diekmann, Sven Fröhling, Ove von Stackelberg ///
///////////////////////////////////////////////////////////

#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

#include <mesh.h>
#include <surface.h>
#include <curve.h>


class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
Q_OBJECT

public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();

public slots:
    void stepAnimation();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

protected:
    QTimer* animtimer; // Timer needed to step animation every x msec
    int animstep;      // Current animation step (used to rotate triangle
private:
	Mesh* mesh;
    Surface* surface;
	Curve* curve;
};

#endif // OGLWIDGET_H
