#ifndef CUSTOMFILTER_H
#define CUSTOMFILTER_H

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
    CustomFilter(QWidget *parent = nullptr, uchar* _filter = nullptr, int* _sizeX = nullptr, int* _sizeY = nullptr);
    uchar* filter;
    int* sizeX;
    int* sizeY;
    ~CustomFilter();

private slots:
    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_horizontalSlider_5_sliderMoved(int position);

    void on_horizontalSlider_6_sliderMoved(int position);

private:
    void SliderMove(int positionX, int positionY, int prev_pos_x, int prev_pos_y);
    std::vector<QDoubleSpinBox*> inputs;
    Ui::CustomFilter *ui;

};
#endif
