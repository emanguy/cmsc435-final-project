/**
 * This class is used to remove charts from the render area via their relative index on the screen.
 * It renders as a black and white "X" and fills the background with red when hovered over
 *
 * @author Evan Rittenhouse <eritte2@umbc.edu>
 */

#ifndef CHARTCLOSEBUTTON_H
#define CHARTCLOSEBUTTON_H

#include <QObject>
#include <GL/glut.h>
#include "Chart.h"
#include <list>

class ChartCloseButton : public QObject
{
    Q_OBJECT
public:
    explicit ChartCloseButton(int absX, int absY, QObject *parent = 0);

    // Setters for position and represented index
    void setX(int x);
    void setY(int y);
    void setRelevantChart(list<Chart>::iterator chart);
    void setSelfIterator(list<ChartCloseButton*>::iterator self);

    // Render the "X" button
    void Render();

    // Should be referenced when the mouse is moved within the GLWidget
    void mouseMoved(int x, int y);

    // Should be referenced when the mouse is clicked anywhere within the GLWidget, fires clicked() if mouse is over
    void mouseClicked();

private:
    int absX, absY; // Measured at upper left corner, as per usual
    bool hovered; // Becomes true when the mouse hovers over the button
    list<Chart>::iterator chartIterator;
    list<ChartCloseButton*>::iterator selfIterator;


signals:
    // Will be fired when the button is clicked and will show what index this is for
    void clicked(list<Chart>::iterator chart, list<ChartCloseButton*>::iterator button);
};

#endif // CHARTCLOSEBUTTON_H
