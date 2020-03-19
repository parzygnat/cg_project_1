#include "dither_dialog.h"
#include "ui_dither_dialog.h"

DitherDialog::DitherDialog(QWidget *parent)
    : QDialog(parent)
    ,ui(new Ui::DitherDialog)
{
    ui->setupUi(this);
}

int DitherDialog::getN()
{
    return ui->radioButton_2->isChecked()?2:ui->radioButton_3->isChecked()?3:ui->radioButton_4->isChecked()?4:6;
}

int DitherDialog::getK()
{
    return ui->spinBox->value();
}

DitherDialog::~DitherDialog()
{
    delete ui;
}
