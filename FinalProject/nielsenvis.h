#ifndef NIELSENVIS_H
#define NIELSENVIS_H

#include <QMainWindow>
#include <string>
#include "displaymanager.h"

namespace Ui {
class NielsenVis;
}

using namespace std;

class NielsenVis : public QMainWindow
{
    Q_OBJECT

public:
    explicit NielsenVis(QWidget *parent = 0);
    ~NielsenVis();

private:
    Ui::NielsenVis *ui;
    DisplayManager renderer;

    list<string> demoList;
    list<string> mktList;

    bool hasDDIData;

signals:
    void gotDataFile(const string& fileName, eDataType whichFile);
    void chartAddRequest(const string& demoOrMktName, bool isDemo);

public slots:
    void findCurrentData();
    void findPlanningData();
    void findFinalData();

    // Updates DDI data from display manager when data is refreshed
    void updateDDI(list<string> demos, list<string> markets);

    // Repopulates the combo box with a different list of options
    void populateCBox(int index);

    // Prepares combobox data for a chartAddRequest emission
    void prepareChartAdd();
};

#endif // NIELSENVIS_H
