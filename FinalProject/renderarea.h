#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QGLWidget>
#include <GL/glut.h>

class RenderArea : public QGLWidget
{
Q_OBJECT
public:
    explicit RenderArea( QWidget *parent = 0);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    int width, height;
    int chartsAdded;
signals:

public slots:
    void chartAdded();
};

#endif // RENDERAREA_H
