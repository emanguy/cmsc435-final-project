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

    SortOrder sortMethods[4] = { &Chart::sortAlpha, &Chart::sortInter, &Chart::sortPlan, &Chart::sortFinal };

public:
    explicit DisplayManager(QObject *parent = 0);
    void Render(QGLWidget* renderArea);
    void setChartDimensions(float width, float height);
    void setChartPadding(float vertical, float horizontal);
    ~DisplayManager();

signals:
    void ddiDataAdded(list<string> demoNames, list<string> mktNames);
    void requestRedraw();

public slots:
    // Updates elements to conform to the size of the render area
    void renderAreaResized(int width, int height);

    // Slot for changing source file names.
    void updateFile(const string& fileName, eDataType whichFile);

    // Adds a chart based on request
    void chartAdd(const string& category, bool isDemo);

    // Set sort order for all charts
    void setSortOrder(int sortOrder);
};

#endif // DISPLAYMANAGER_H
