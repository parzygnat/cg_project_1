#ifndef DITHER_DIALOG_H
#define DITHER_DIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <vector>
QT_BEGIN_NAMESPACE
namespace Ui { class DitherDialog; }
QT_END_NAMESPACE

class DitherDialog : public QDialog
{
    Q_OBJECT

public:
    DitherDialog(QWidget *parent = nullptr);
    int getN();
    int getK();
    ~DitherDialog();

private slots:


private:
    Ui::DitherDialog *ui;
};

#endif // DITHER_DIALOG_H
