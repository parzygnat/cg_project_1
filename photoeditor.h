#ifndef PHOTOEDITOR_H
#define PHOTOEDITOR_H

#include <QMainWindow>
#include "customfilter.h"
#include "dither_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PhotoEditor; }
QT_END_NAMESPACE

class PhotoEditor : public QMainWindow
{
    Q_OBJECT

public:
    PhotoEditor(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event);
    ~PhotoEditor();
private slots:
    void open();
    void box_blur();
    void gaussian_blur();
    void emboss();
    void edge_detection();
    void sharpen();
    void save();
    void inverse();
    void brightness();
    void gamma();
    void exit();
    void contrast();
    void convolution(int sizeX, int sizeY, double* values, int anchorX, int anchorY, double offset = 0, double divisor = 0);
    void convolution_saltpepper();
    void function_filter(int (*operation)(int));
    void on_actionReset_triggered();
    void on_actionCustom_Filter_triggered();

    void on_actionOrdered_Dithering_triggered();

    void on_actionB_W_triggered();

private:
    Ui::PhotoEditor *ui;
    QString currentFile;
    QPixmap initial;
    QPixmap current;
    CustomFilter *myFilter;
    DitherDialog *myDither;
};
#endif // PHOTOEDITOR_H
