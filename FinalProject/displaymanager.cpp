#include "displaymanager.h"

DisplayManager::DisplayManager(QObject *parent) : QObject(parent),
    chartPaddingV(10),
    chartPaddingH(10),
    chartWidth(400),
    chartHeight(100)
{
        graphData = NULL;
        updatedFileName[INTERIM] = false;
        updatedFileName[PLANNING] = false;
        updatedFileName[FINAL] = false;
}

DisplayManager::~DisplayManager()
{
        if (graphData != NULL)
        {
                delete graphData;
                graphData = NULL;
        }
}

void DisplayManager::setChartDimensions(float width, float height)
{
    chartWidth = width;
    chartHeight = height;
}

void DisplayManager::setChartPadding(float vertical, float horizontal)
{
    chartPaddingV = vertical;
    chartPaddingH = horizontal;
}

void DisplayManager::renderAreaResized(int width, int height)
{
    // Set up an iterator and compute the starting height of charts as well as width
    list<Chart>::iterator it = charts.begin();
    float currentHeight = chartPaddingV;
    chartWidth = width - 2 * chartPaddingH;

    // Set dimensions on every chart, then recalculate for next height
    for ( ; it != charts.end(); it++)
    {
        it->setWidth(chartWidth);
        it->setHeight(chartHeight);
        it->setAbsX(chartPaddingH);
        it->setAbsY(currentHeight);
        it->setDimensions();

        currentHeight += chartHeight + chartPaddingV;
    }
}

void DisplayManager::updateFile(const string &fileName, eDataType whichFile)
{
    // Update file name for file type and mark it as having been updated
    DisplayManager::fileName[whichFile] = fileName;
    updatedFileName[whichFile] = true;

    // If all file names have been updated, clear charts and load in new data files
    if (updatedFileName[INTERIM] && updatedFileName[PLANNING] && updatedFileName[FINAL])
    {
        updatedFileName[INTERIM] = false;
        updatedFileName[PLANNING] = false;
        updatedFileName[FINAL] = false;
        charts.clear();

        // Delete old graph data if it exists
        if (graphData != NULL)
        {
            delete graphData;
            graphData = NULL;
        }

        // Create a new graph data object and fire a data added event
        graphData = new DDIMatrix(DisplayManager::fileName[INTERIM],
                                  DisplayManager::fileName[PLANNING],
                                  DisplayManager::fileName[FINAL]);

        list<string> demos = graphData->getDemos();
        list<string> markets = graphData->getMarkets();

        emit ddiDataAdded(demos, markets);
    }
}

// TODO write functions for getting all demo/market names