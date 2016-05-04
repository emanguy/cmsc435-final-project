#include "nielsenvis.h"
#include "ui_nielsenvis.h"

NielsenVis::NielsenVis(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NielsenVis)
{
    ui->setupUi(this);
}

NielsenVis::~NielsenVis()
{
    delete ui;
}
