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
    for(int a = 0; a < 9; ++a) {
        for(int b = 0; b < 9; ++b)
        {
            inputs.push_back(new QDoubleSpinBox(this)); ui->gridLayout->addWidget(inputs[9*a + b],a, b);
        }
}
}


CustomFilter::~CustomFilter()
{
    delete ui;
}



void CustomFilter::on_horizontalSlider_5_sliderMoved(int position)
{
    int prev_num_y = ui->label_6->text().toInt();
    int prev_num_x = ui->label_5->text().toInt();
    int num = 3 + 2*position;
    CustomFilter::ui->label_5->setNum(num);
    CustomFilter::SliderMove(num, prev_num_y, prev_num_x, prev_num_y);

}

void CustomFilter::on_horizontalSlider_6_sliderMoved(int position)
{
    int prev_num_x = ui->label_5->text().toInt();
    int prev_num_y = ui->label_6->text().toInt();
    int num = 3 + 2*position;
    CustomFilter::ui->label_6->setNum(num);
    CustomFilter::SliderMove(prev_num_x, num, prev_num_x, prev_num_y);

}

void CustomFilter::SliderMove(int positionX, int positionY, int prev_pos_x, int prev_pos_y) {
    if(!inputs.empty())
        for(int i = prev_pos_x * prev_pos_y - 1; i >= 0; --i) {
            ui->gridLayout->removeWidget(inputs[i]);
            inputs[i]->setParent(nullptr);
        }

    for(int a = 0; a < positionY; ++a) {
        for(int b = 0; b < positionX; ++b)
        {
            ui->gridLayout->addWidget(inputs[a*positionX + b], b, a);
        }
}
}

std::vector<double> CustomFilter::getVector() {
    std::vector<double> values;
    for(int i = 0; i < this->getX()*this->getY(); ++i) {
        values.push_back(inputs[i]->text().toDouble());
    }
    return values;
}
int CustomFilter::getX() { return ui->label_5->text().toInt();}
int CustomFilter::getY() {return ui->label_6->text().toInt();}
int CustomFilter::getAnchorX(){return ui->spinBox->text().toInt();}
int CustomFilter::getAnchorY(){return ui->spinBox_2->text().toInt();}
