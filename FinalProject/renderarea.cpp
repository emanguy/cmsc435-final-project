#include "renderarea.h"

RenderArea::RenderArea( QWidget *parent):
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    width(100),
    height(100),
    chartsAdded(0)
{

}

void RenderArea::initializeGL()
{
    // Set the clear color and allow colored rendering
    glClearColor(0,0,0,1);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
}

void RenderArea::paintGL()
{
    // Clear display buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Set color to red
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(3.0);

    // Draw "charts"
    for (int chart = 0; chart < chartsAdded; chart++)
    {
        renderText( width / 8, height * (chart + 1) / 16, "A chart has been added" );
    }

    glFlush();
}

void RenderArea::resizeGL(int w, int h)
{
    // Set up 2D
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    width = w;
    height = h;
}

void RenderArea::chartAdded()
{
    // "Add" a new chart
    chartsAdded++;
    updateGL();
}
