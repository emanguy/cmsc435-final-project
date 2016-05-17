#include "chartclosebutton.h"

// Construct the chart close button
ChartCloseButton::ChartCloseButton(int absX, int absY, QObject *parent) : QObject(parent),
    absX(absX),
    absY(absY),
    hovered(false)
{

}

// Setters
void ChartCloseButton::setX(int x)
{
    absX = x;
}

void ChartCloseButton::setY(int y)
{
    absY = y;
}

void ChartCloseButton::setRelevantChart(list<Chart>::iterator chart)
{
    chartIterator = chart;
}

void ChartCloseButton::setSelfIterator(list<ChartCloseButton*>::iterator self)
{
    selfIterator = self;
}

// Renders this button
void ChartCloseButton::Render()
{
    // Set line color based on hover and draw the frame and "X"
    if (hovered)
    {
        glColor3f(0, 0, 0);
    }
    else
    {
        glColor3f(1, 1, 1);
    }

    glLineWidth(1.5);

    // Draw box for "X"
    glBegin(GL_LINE_STRIP);
        glVertex2f(absX, absY);
        glVertex2f(absX + 10, absY);
        glVertex2f(absX + 10, absY + 10);
        glVertex2f(absX, absY + 10);
        glVertex2f(absX, absY);
    glEnd();

    // Draw the "X" itself
    glBegin(GL_LINES);
        glVertex2f(absX, absY);
        glVertex2f(absX + 10, absY + 10);
        glVertex2f(absX + 10, absY);
        glVertex2f(absX, absY + 10);
    glEnd();

    glLineWidth(1);

    // Set the background color and begin drawing the button
    if (hovered)
    {
        glColor3f(1, 0, 0);
    }
    else
    {
        glColor3f(0,0,0);
    }

    // Draw background
    glBegin(GL_QUADS);
        glVertex2f(absX, absY);
        glVertex2f(absX + 10, absY);
        glVertex2f(absX + 10, absY + 10);
        glVertex2f(absX, absY + 10);
    glEnd();
}

// Called when mouse is moved in the GLWidget
void ChartCloseButton::mouseMoved(int x, int y)
{
    hovered = (x >= absX && x <= absX + 10 && y >= absY && y <= absY + 10);
}

// Called when the mouse is clicked within the glWidget
void ChartCloseButton::mouseClicked()
{
    if (hovered)
    {
        emit clicked(chartIterator, selfIterator);
    }
}
