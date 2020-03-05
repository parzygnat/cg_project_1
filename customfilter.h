#ifndef CUSTOMFILTER_H
#define CUSTOMFILTER_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class CustomFilter; }
QT_END_NAMESPACE

class CustomFilter : public QDialog
{
    Q_OBJECT

public:
    CustomFilter(QWidget *parent = nullptr, uchar* _filter = nullptr, int* _sizeX = nullptr, int* _sizeY = nullptr);
    ~CustomFilter();

private slots:
    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::CustomFilter *ui;
    uchar* filter;
    int* sizeX;
    int* sizeY;
};
#endif
