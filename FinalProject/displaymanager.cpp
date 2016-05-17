#include "displaymanager.h"
#include <QDebug>

DisplayManager::DisplayManager(QObject *parent) : QObject(parent),
    chartPaddingH(10),
    chartPaddingV(150),
    chartWidth(400),
    chartHeight(100),
    renderWidth(420),
    renderHeight(500)
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

void DisplayManager::Render(QGLWidget* renderArea)
{
    list<Chart>::iterator iterator;
    QString label;

    // Loop through all charts and render
    for (iterator = charts.begin(); iterator != charts.end(); iterator++)
    {
        iterator->Render(renderArea);
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

    // Save the current width and height
    DisplayManager::renderWidth = width;
    DisplayManager::renderHeight = height;
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
        emit updateStatusLine("Files loaded.");
    }
    else
    {
        // Build "remaining files" string
        QString remainingFiles = "";
        int numRemainingFiles = 0;

        if (!updatedFileName[INTERIM])
        {
            remainingFiles += " interim ";
            numRemainingFiles++;
        }
        if (!updatedFileName[PLANNING])
        {
            remainingFiles += " planning ";
            numRemainingFiles++;
        }
        if (!updatedFileName[FINAL])
        {
            remainingFiles += " final ";
            numRemainingFiles++;
        }

        // Construct beginning of string
        remainingFiles = "You still need to add " + QString::number(numRemainingFiles) + " CSV data files: " + remainingFiles;

        // Update the status line with this new string
        emit updateStatusLine(remainingFiles);
    }
}

// Adds a requested chart
void DisplayManager::chartAdd(const string &category, bool isDemo)
{
    Chart newChart(chartWidth, chartHeight);

    // Specify data for chart
    newChart.setType((isDemo? DEMO : MARKET));
    newChart.setName(category);
    newChart.setList(*graphData);

    // Set size of chart
    newChart.setAbsX(chartPaddingH);
    newChart.setAbsY((charts.size() + 1) * chartPaddingV + chartHeight * charts.size());
    newChart.setDimensions();

    charts.push_back(newChart);

    emit requestRedraw();
}

void DisplayManager::setSortOrder(int sortOrder)
{
    list<Chart>::iterator iterator = charts.begin();

    if (sortOrder >= 0 && sortOrder <= 4)
    {
        SortOrder functionCall = sortMethods[sortOrder];

        // Loop through each of the charts setting the new sort order
        for ( ; iterator != charts.end(); iterator++)
        {
            ((*iterator).*functionCall)();
        }

        emit requestRedraw();
    }
}
