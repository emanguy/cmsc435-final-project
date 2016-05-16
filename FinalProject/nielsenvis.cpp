#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include "nielsenvis.h"
#include "ui_nielsenvis.h"

NielsenVis::NielsenVis(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NielsenVis)
{
    ui->setupUi(this);

    // Connect buttons to local file dialog functions
    connect(ui->Planning, SIGNAL(clicked(bool)), this, SLOT(findPlanningData(bool)));
    connect(ui->Interim, SIGNAL(clicked(bool)), this, SLOT(findCurrentData(bool)));
    connect(ui->FinalCsv, SIGNAL(clicked(bool)), this, SLOT(findFinalData(bool)));

    // Connect the gotDataFile signal to the display manager
    connect(this, SIGNAL(gotDataFile(string,eDataType)), &renderer, SLOT(updateFile(string,eDataType)));

    // Make the combo box populate when the DDIMatrix changes
    connect(&renderer, SIGNAL(ddiDataAdded(list<string>,list<string>)),
            this, SLOT(updateDDI(list<string>,list<string>)));
    connect(ui->PrimKey, SIGNAL(activated(int)), this, SLOT(populateCBox(int)));
}

NielsenVis::~NielsenVis()
{
    delete ui;
}

// Show file picker for interim data
void NielsenVis::findCurrentData(bool checked)
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
void NielsenVis::findPlanningData(bool checked)
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
void NielsenVis::findFinalData(bool checked)
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
