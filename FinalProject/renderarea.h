#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QGLWidget>
#include <GL/glut.h>
#include "displaymanager.h"

/**
 * Class for rendering charts from the Display Manager.
 */
class RenderArea : public QGLWidget
{
Q_OBJECT
public:
    explicit RenderArea( QWidget *parent = 0);

    void setDisplayManager(DisplayManager *renderer);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void resizeEvent(QResizeEvent *);

private:
    int width, height;
    int chartsAdded;

    DisplayManager *renderer;

signals:
    void resized(int width, int height);
public slots:
    void chartAdded();
};

#endif // RENDERAREA_H
