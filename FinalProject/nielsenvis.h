#ifndef NIELSENVIS_H
#define NIELSENVIS_H

#include <QMainWindow>

namespace Ui {
class NielsenVis;
}

class NielsenVis : public QMainWindow
{
    Q_OBJECT

public:
    explicit NielsenVis(QWidget *parent = 0);
    ~NielsenVis();

private:
    Ui::NielsenVis *ui;
};

#endif // NIELSENVIS_H
