/**
 * This class is used for displaying, managing, and rendering different charts in the associated RenderArea.
 *
 * @author Evan Rittenhouse
 */

#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <QObject>
#include <QGLWidget>
#include <string>
#include <list>
#include <GL/glut.h>
#include "DDIMatrix.h"
#include "Triangle.h"
#include "Chart.h"
#include "chartclosebutton.h"

using namespace std;

// Defines used for sort order
#define ALPHA_SORT 0
#define INTER_SORT 1
#define PLANN_SORT 2
#define FINAL_SORT 3

class DisplayManager : public QObject
{
    Q_OBJECT
public:
    typedef void (Chart::*SortOrder)();
private:
    float chartWidth, chartHeight, chartPaddingV, chartPaddingH;
    int renderWidth;
    int renderHeight;
    bool updatedFileName[3];
    string fileName[3];

    DDIMatrix *graphData;
    list<Chart> charts;
    list<ChartCloseButton*> closeButtons;
    list<list<ChartCloseButton*>::iterator> garbage; // For removing unnecessary iterators

    SortOrder sortMethods[4];

    void garbageCollect();

public:
    explicit DisplayManager(QObject *parent = 0);
    void Render(QGLWidget* renderArea);
    void setChartDimensions(float width, float height);
    void setChartPadding(float vertical, float horizontal);
    ~DisplayManager();

signals:
    void ddiDataAdded(list<string> demoNames, list<string> mktNames);
    void requestRedraw();
    void updateStatusLine(const QString& statusLine);

public slots:
    // Updates elements to conform to the size of the render area
    void renderAreaResized(int width, int height);

    // Slot for changing source file names.
    void updateFile(const string& fileName, eDataType whichFile);

    // Adds a chart based on request
    void chartAdd(const string& category, bool isDemo);

    // Set sort order for all charts
    void setSortOrder(int sortOrder);

    // Remove a chart when requested
    void removeChart(list<Chart>::iterator chart, list<ChartCloseButton*>::iterator button);

    // Mouse handling
    void handleMouseMove(int x, int y);
    void handleClick();
};

#endif // DISPLAYMANAGER_H
