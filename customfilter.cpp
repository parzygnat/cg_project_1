#include "customfilter.h"
#include "ui_customfilter.h"
#include <QFileDialog>
#include <QMessageBox>
#include <cmath>
#include <QtGlobal>
#include <QInputDialog>

CustomFilter::CustomFilter(QWidget *parent, uchar* _filter, int* _sizeX, int* _sizeY)
    : QDialog(parent)
    , ui(new Ui::CustomFilter), filter(_filter), sizeX(_sizeX), sizeY(_sizeY)
{
    ui->setupUi(this);
}


CustomFilter::~CustomFilter()
{
    delete ui;
}



void CustomFilter::on_horizontalSlider_sliderMoved(int position)
{
    CustomFilter::ui->label->setNum(3 + 2*position);
}
