#include "renderarea.h"

RenderArea::RenderArea( QWidget *parent):
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    width(100),
    height(100),
    chartsAdded(0),
    renderer(NULL)
{
}

void RenderArea::setDisplayManager(DisplayManager *renderer)
{
    RenderArea::renderer = renderer;
}

void RenderArea::initializeGL()
{
    // Set the clear color and allow colored rendering
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
}

void RenderArea::paintGL()
{
    // Clear display buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render display manager contents
    if (renderer != NULL)
    {
        renderer->Render(this);
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

void RenderArea::resizeEvent(QResizeEvent *)
{
    // Notify listeners that the render area has been resized
    emit resized(geometry().width(), geometry().height());
    resizeGL(geometry().width(), geometry().height());
}

void RenderArea::mouseMoveEvent(QMouseEvent *e)
{
    emit mouseMoved(e->x(), e->y());
}
void RenderArea::mouseReleaseEvent(QMouseEvent *)
{
    emit mouseClicked();
}

void RenderArea::chartAdded()
{
    // Make a request to rerender the screen content
    updateGL();
}
