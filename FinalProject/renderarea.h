/**
 * This class works with DisplayManager to render charts within the GUI.
 *
 * @author Evan Rittenhouse
 */

#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QGLWidget>
#include <QMouseEvent>
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
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private:
    int width, height;
    int chartsAdded;

    DisplayManager *renderer;

signals:
    void resized(int width, int height);
    void mouseMoved(int x, int y);
    void mouseClicked();

public slots:
    void chartAdded();
};

#endif // RENDERAREA_H
