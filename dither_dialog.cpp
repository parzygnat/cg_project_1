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
    return ui->spinBox->value();
}

int DitherDialog::getK()
{
    return ui->spinBox_2->value();
}

DitherDialog::~DitherDialog()
{
    delete ui;
}
