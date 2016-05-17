#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include "nielsenvis.h"
#include "ui_nielsenvis.h"

NielsenVis::NielsenVis(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NielsenVis),
    hasDDIData(false)
{
    ui->setupUi(this);

    // Set the initial value of the status area
    ui->statusBar->showMessage("Please add the necessary CSV data files.");

    // Bind unique identifiers to the radio buttons
    ui->sortStyles->setId(ui->Name, ALPHA_SORT);
    ui->sortStyles->setId(ui->Int, INTER_SORT);
    ui->sortStyles->setId(ui->Plan, PLANN_SORT);
    ui->sortStyles->setId(ui->Final, FINAL_SORT);

    /////////////// END RADIO BUTTON STUFF ///////////////

    // Connect all radio buttons to DisplayManager sort orders
    connect(ui->sortStyles, SIGNAL(buttonClicked(int)), &renderer, SLOT(setSortOrder(int)));

    // Connect buttons to local file dialog functions
    connect(ui->Planning, SIGNAL(clicked()), this, SLOT(findPlanningData()));
    connect(ui->Interim, SIGNAL(clicked()), this, SLOT(findCurrentData()));
    connect(ui->FinalCsv, SIGNAL(clicked()), this, SLOT(findFinalData()));

    // Connect the gotDataFile signal to the display manager
    connect(this, SIGNAL(gotDataFile(string,eDataType)), &renderer, SLOT(updateFile(string,eDataType)));

    // Make the combo box populate when the DDIMatrix changes
    connect(&renderer, SIGNAL(ddiDataAdded(list<string>,list<string>)),
            this, SLOT(updateDDI(list<string>,list<string>)));
    connect(ui->PrimKey, SIGNAL(activated(int)), this, SLOT(populateCBox(int)));

    // Set off the event chain when the Add Graph button is clicked
    connect(ui->Add, SIGNAL(clicked()), this, SLOT(prepareChartAdd()));
    connect(this, SIGNAL(chartAddRequest(string,bool)), &renderer, SLOT(chartAdd(string, bool)));
    connect(&renderer, SIGNAL(requestRedraw()), ui->Render, SLOT(chartAdded()));

    // Make the display manager resize its contents when the window resizes
    connect(ui->Render, SIGNAL(resized(int,int)), &renderer, SLOT(renderAreaResized(int,int)));

    // Make the display manager report what files it has
    connect(&renderer, SIGNAL(updateStatusLine(QString)), ui->statusBar, SLOT(showMessage(QString)));

    ////////////// END EVENT BINDING //////////////////

    // Set the display manager on the render widget
    ui->Render->setDisplayManager(&renderer);
}

NielsenVis::~NielsenVis()
{
    delete ui;
}

// Show file picker for interim data
void NielsenVis::findCurrentData()
{
     QString selectedFile = QFileDialog::getOpenFileName(this,
                                                         "Find interim data",
                                                         "./",
                                                         "CSV file (*.csv)");

     if (selectedFile != "")
     {
        emit gotDataFile(selectedFile.toUtf8().constData(), INTERIM);
     }
}

// Show file picker for planning data
void NielsenVis::findPlanningData()
{
    QString selectedFile = QFileDialog::getOpenFileName(this,
                                                        "Find planning data",
                                                        "./",
                                                        "CSV file (*.csv)");

    if (selectedFile != "")
    {
        emit gotDataFile(selectedFile.toUtf8().constData(), PLANNING);
    }
}

// Show file picker for final data
void NielsenVis::findFinalData()
{
    QString selectedFile = QFileDialog::getOpenFileName(this,
                                                        "Find final data",
                                                        "./",
                                                        "CSV file (*.csv)");

    if (selectedFile != "")
    {
        emit gotDataFile(selectedFile.toUtf8().constData(), FINAL);
    }
}

// Update combobox to reflect new DDI data
void NielsenVis::updateDDI(list<string> demos, list<string> markets)
{
    demoList = demos;
    mktList = markets;

    // If this has been called it means that we actually have DDI data
    hasDDIData = true;

    // Populate dropdown box with data
    populateCBox(ui->PrimKey->currentIndex());
}

// Refresh demo/market combobox contents
void NielsenVis::populateCBox(int index)
{
    list<string>::iterator iterator;
    list<string>::iterator end;

    // Get iterators for beginning and end of list
    switch (index)
    {
    case 0:
        iterator = demoList.begin();
        end = demoList.end();
        break;
    case 1:
        iterator = mktList.begin();
        end = mktList.end();
        break;
    }

    // Clear the combo box and populate it
    ui->CatDDown->clear();

    for ( ; iterator != end; iterator++)
    {
        ui->CatDDown->addItem(iterator->c_str());
    }
}

// Prepare combobox data for adding a chart
void NielsenVis::prepareChartAdd()
{
    // If we actually have DDI data prepare to add a chart
    if (hasDDIData)
    {
        // Give the demo/market and mark whether the text is a demo or market
        emit chartAddRequest(ui->CatDDown->currentText().toUtf8().constData(), (ui->PrimKey->currentIndex() == 0));
    }
    else
    {
        ui->statusBar->showMessage("You must add data files first.", 3000);
    }
}
