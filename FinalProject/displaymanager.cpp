#include "displaymanager.h"

// Construct a DisplayManager object
DisplayManager::DisplayManager(QObject *parent) : QObject(parent),
    chartWidth(400),
    chartHeight(100),
    chartPaddingV(150), // Bars are positioned weird so I had to seriously bump up the padding between them
    chartPaddingH(20),
    renderWidth(420),
    renderHeight(500)
{
        graphData = NULL;
        updatedFileName[INTERIM] = false;
        updatedFileName[PLANNING] = false;
        updatedFileName[FINAL] = false;

        sortMethods[0] = &Chart::sortAlpha;
        sortMethods[1] = &Chart::sortInter;
        sortMethods[2] = &Chart::sortPlan;
        sortMethods[3] = &Chart::sortFinal;
}

// Deletes dynamically allocated DDIMatrix when this is destroyed
DisplayManager::~DisplayManager()
{
        if (graphData != NULL)
        {
                delete graphData;
                graphData = NULL;
        }

        // TODO remove listeners before delete
        for (list<ChartCloseButton*>::iterator it = closeButtons.begin(); it != closeButtons.end(); it++)
        {
            disconnect(*it, SIGNAL(clicked(list<Chart>::iterator, list<ChartCloseButton*>::iterator)),
                       this, SLOT(removeChart(list<Chart>::iterator,list<ChartCloseButton*>::iterator)));
            delete (*it);
        }

        closeButtons.clear();
}

// Called during a chart removal to remove chart buttons from the list
void DisplayManager::garbageCollect()
{
    for (list<list<ChartCloseButton*>::iterator>::iterator it = garbage.begin(); it != garbage.end(); it++)
    {
        closeButtons.erase(*it);
    }

    garbage.clear();
}

// Called by the GL widget to render charts
void DisplayManager::Render(QGLWidget* renderArea)
{
    list<Chart>::iterator iterator;
    list<ChartCloseButton*>::iterator btnIterator;

    // Loop through all charts and render
    for (iterator = charts.begin(); iterator != charts.end(); iterator++)
    {
        iterator->Render(renderArea);
    }

    // Loop through all buttons and render
    for (btnIterator = closeButtons.begin(); btnIterator != closeButtons.end(); btnIterator++)
    {
        (*btnIterator)->Render();
    }

    // Add a legend at the bottom
    glColor3f(0, 1, 0);
    renderArea->renderText(0, renderHeight - 50, "Interim");

    glColor3f(1, 0 , 0);
    renderArea->renderText(0, renderHeight - 30, "Planning");

    glColor3f(0, 0.5, 1);
    renderArea->renderText(0, renderHeight - 10, "Final");

}

// Set the width and height of all charts manually
void DisplayManager::setChartDimensions(float width, float height)
{
    chartWidth = width;
    chartHeight = height;
}

// Set vertical and horizontal padding between charts
void DisplayManager::setChartPadding(float vertical, float horizontal)
{
    chartPaddingV = vertical;
    chartPaddingH = horizontal;
}

// Adjusts rendered content based on a widget resize
void DisplayManager::renderAreaResized(int width, int height)
{
    // Set up an iterator and compute the starting height of charts as well as width
    list<Chart>::iterator it = charts.begin();
    list<ChartCloseButton*>::iterator btnIt = closeButtons.begin();
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

        (*btnIt)->setX(chartPaddingH + chartWidth + 5);
        (*btnIt)->setY(currentHeight);

        currentHeight += chartHeight + chartPaddingV;
        btnIt++; // Advance button iterator in time with with chart iterator
    }

    // Save the current width and height
    DisplayManager::renderWidth = width;
    DisplayManager::renderHeight = height;
}

// Slot for adding data files
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
    list<Chart>::iterator chartIt;

    // Specify data for chart
    newChart.setType((isDemo? DEMO : MARKET));
    newChart.setName(category);
    newChart.setList(*graphData);

    // Set size of chart
    newChart.setAbsX(chartPaddingH);
    newChart.setAbsY((charts.size() + 1) * chartPaddingV + chartHeight * charts.size());
    newChart.setDimensions();

    chartIt = charts.insert(charts.end(), newChart);

    // Add button to go along with chart
    ChartCloseButton* chartBtn = new ChartCloseButton(chartPaddingH + chartWidth + 5, newChart.getChartY());
    list<ChartCloseButton*>::iterator btnIt;

    // Add relevant iterators to button (removing elements does not invalidate iterators), attach event listener, add to list
    chartBtn->setRelevantChart(chartIt);
    connect(chartBtn, SIGNAL(clicked(list<Chart>::iterator, list<ChartCloseButton*>::iterator)),
            this, SLOT(removeChart(list<Chart>::iterator, list<ChartCloseButton*>::iterator)));
    btnIt = closeButtons.insert(closeButtons.end(), chartBtn);
    chartBtn->setSelfIterator(btnIt);

    emit requestRedraw();
}

// Set the sort order of all visible charts
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

// Remove a chart on request
void DisplayManager::removeChart(list<Chart>::iterator chart, list<ChartCloseButton*>::iterator button)
{
    ChartCloseButton* tempStorage = *button;

    // Remove chart
    charts.erase(chart);

    // Disconnect event listener, delete dynamically allocated button, remove button from list
    disconnect(tempStorage, SIGNAL(clicked(list<Chart>::iterator,list<ChartCloseButton*>::iterator)),
               this, SLOT(removeChart(list<Chart>::iterator,list<ChartCloseButton*>::iterator)));
    delete tempStorage;
    garbage.push_back(button);
}

void DisplayManager::handleMouseMove(int x, int y)
{
    // Notify all ChartCloseButtons of mouse movement
    for (list<ChartCloseButton*>::iterator it = closeButtons.begin(); it != closeButtons.end(); it++)
    {
        (*it)->mouseMoved(x, y);
    }

    emit requestRedraw();
}

void DisplayManager::handleClick()
{
    // Notify all ChartCloseButtons of mouse click
    for (list<ChartCloseButton*>::iterator it = closeButtons.begin(); it != closeButtons.end(); it++)
    {
        (*it)->mouseClicked();
    }

    // Garbage collect leftover iterators
    garbageCollect();

    // Hacky method to reposition elements
    renderAreaResized(renderWidth, renderHeight);
    emit requestRedraw();}
