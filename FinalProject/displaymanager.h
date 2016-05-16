#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <QObject>
#include <string>
#include <list>
#include <GL/glut.h>
#include "DDIMatrix.h"
#include "Triangle.h"
#include "Chart.h"

using namespace std;

class DisplayManager : public QObject
{
    Q_OBJECT
private:
    float chartWidth, chartHeight, chartPaddingV, chartPaddingH;

    bool updatedFileName[3];
    string fileName[3];

    DDIMatrix *graphData;
    list<Chart> charts;

public:
    explicit DisplayManager(QObject *parent = 0);
    void Render();
    void setChartDimensions(float width, float height);
    void setChartPadding(float vertical, float horizontal);
    ~DisplayManager();

signals:
    void ddiDataAdded(list<string> demoNames, list<string> mktNames);

public slots:
    // Updates elements to conform to the size of the render area
    void renderAreaResized(int width, int height);

    // Slots for changing source file names.
    void updateFile(const string& fileName, eDataType whichFile);
};

#endif // DISPLAYMANAGER_H
