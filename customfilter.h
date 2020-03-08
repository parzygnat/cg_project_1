#ifndef CUSTOMFILTER_H
#define CUSTOMFILTER_H

#include "filter.h"
#include <QDialog>
#include <QDoubleSpinBox>
#include <vector>
QT_BEGIN_NAMESPACE
namespace Ui { class CustomFilter; }
QT_END_NAMESPACE

class CustomFilter : public QDialog
{
    Q_OBJECT

public:
    CustomFilter(QWidget *parent = nullptr, uchar* _filter = nullptr, int* _sizeX = nullptr, int* _sizeY = nullptr, bool custom = false);
    uchar* filter;
    int* sizeX;
    int* sizeY;
    std::vector<double> getVector();
    int getX();
    int getY();
    int getAnchorX();
    int getAnchorY();
    bool getCustom();
    double getDivisor();
    ~CustomFilter();

private slots:

    void on_horizontalSlider_5_sliderMoved(int position);

    void on_horizontalSlider_6_sliderMoved(int position);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_horizontalSlider_5_valueChanged(int value);

    void on_horizontalSlider_6_valueChanged(int value);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

private:
    void SliderMove(int positionX, int positionY, int prev_pos_x, int prev_pos_y);
    std::vector<QDoubleSpinBox*> inputs;
    Ui::CustomFilter *ui;
    int _index;
    std::vector<Filter> filters;
    bool custom;

};
#endif
