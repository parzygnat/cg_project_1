#include "customfilter.h"
#include "ui_customfilter.h"
#include <QFileDialog>
#include <QMessageBox>
#include <cmath>
#include <QtGlobal>
#include <QInputDialog>

CustomFilter::CustomFilter(QWidget *parent, uchar* _filter, int* _sizeX, int* _sizeY, bool custom)
    : QDialog(parent)
    , filter(_filter), sizeX(_sizeX), sizeY(_sizeY), ui(new Ui::CustomFilter)
{
    _index = 0;
    ui->setupUi(this);
    for(int a = 0; a < 9; ++a) {
        for(int b = 0; b < 9; ++b)
        {
            inputs.push_back(new QDoubleSpinBox(this)); inputs[9*a + b]->setRange(-1000, 1000);ui->gridLayout->addWidget(inputs[9*a + b],a, b);
        }
    }
    std::vector<double> temp = {1, 1, 1, 1, 1, 1, 1, 1, 1}; //box blur
    filters.push_back(Filter(3, 3, 1, 1, temp, "Box Blur"));
    temp = {0, 1, 0, 1, 4, 1, 0, 1, 0}; //gaussian blur
    filters.push_back(Filter(3, 3, 1, 1, temp, "Gaussian Blur"));
    temp = {0, -1, 0, -1, 9, -1, 0, -1, 0}; //sharpen
    filters.push_back(Filter(3, 3, 1, 1, temp, "Sharpen"));
    temp = {-1, -1, -1, -1, 8, -1, -1, -1, -1}; //edge detection
    filters.push_back(Filter(3, 3, 1, 1, temp, "Edge Detection"));
    temp = {-1, -1, 0, -1, 1, 1, 0, 1, 1}; //emboss
    filters.push_back(Filter(3, 3, 1, 1, temp, "Emboss"));
    foreach (auto& filter, filters) {
       ui->comboBox->addItem(QString::fromStdString(filter.name));
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
    int num = 1 + 2*position;
    CustomFilter::ui->label_5->setNum(num);
    CustomFilter::SliderMove(num, prev_num_y, prev_num_x, prev_num_y);

}

void CustomFilter::on_horizontalSlider_6_sliderMoved(int position)
{
    int prev_num_x = ui->label_5->text().toInt();
    int prev_num_y = ui->label_6->text().toInt();
    int num = 1 + 2*position;
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


void CustomFilter::on_radioButton_clicked()
{
    custom = true;
}

void CustomFilter::on_radioButton_2_clicked()
{
    custom = false;
}

bool CustomFilter::getCustom() {return custom;}
double CustomFilter::getDivisor() {return ui->divisor->text().toDouble();}


void CustomFilter::on_comboBox_currentIndexChanged(int index)
{
    Filter filter = filters[index];
    ui->horizontalSlider_5->setSliderPosition((filter.getX() - 1)/2);
    ui->horizontalSlider_6->setSliderPosition((filter.getY() - 1)/2);
    ui->spinBox->setValue(filter.getAX());
    ui->spinBox_2->setValue(filter.getAY());
    for(int i = 0; i < this->getX() * this->getY(); ++i) {
        this->inputs[i]->setValue(filter.getValues()[i]);
    }
}

void CustomFilter::on_horizontalSlider_5_valueChanged(int value)
{
    int prev_num_y = ui->label_6->text().toInt();
    int prev_num_x = ui->label_5->text().toInt();
    int num = 1 + 2*value;
    CustomFilter::ui->label_5->setNum(num);
    CustomFilter::SliderMove(num, prev_num_y, prev_num_x, prev_num_y);
}

void CustomFilter::on_horizontalSlider_6_valueChanged(int value)
{
    int prev_num_x = ui->label_5->text().toInt();
    int prev_num_y = ui->label_6->text().toInt();
    int num = 1 + 2*value;
    CustomFilter::ui->label_6->setNum(num);
    CustomFilter::SliderMove(prev_num_x, num, prev_num_x, prev_num_y);

}

void CustomFilter::on_pushButton_clicked() //apply
{
    this->close();
}

void CustomFilter::on_pushButton_3_clicked() //save
{
    QString name = ui->plainTextEdit->toPlainText();
    if(name.isEmpty()) {
        name = QString::fromStdString("Custom(" + std::to_string(_index) + ")");
        ++_index;
    }
    filters.push_back(Filter(this->getX(), this->getY(), this->getAnchorX(), this->getAnchorY(), this->getVector(), name.toStdString()));
    ui->comboBox->addItem(name);
}

void CustomFilter::on_pushButton_4_clicked() //save and apply
{
    CustomFilter::on_pushButton_3_clicked();
    this->close();
}

void CustomFilter::on_pushButton_2_clicked() //delete
{
    int index = ui->comboBox->currentIndex();
    if(ui->comboBox->count() != 1) {
        filters.erase(filters.begin() + index);
        ui->comboBox->removeItem(index);
    }
}

void CustomFilter::on_pushButton_5_clicked()
{
    foreach(auto& field, this->inputs) {
        field->setValue(0);
    }
}

void CustomFilter::on_pushButton_6_clicked()
{
    int index = ui->comboBox->currentIndex();
    filters[index] = Filter(this->getX(), this->getY(), this->getAnchorX(), this->getAnchorY(), this->getVector(), filters[index].name);
}
