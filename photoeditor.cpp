#include "photoeditor.h"
#include "ui_photoeditor.h"
#include <QFileDialog>
#include <QMessageBox>

PhotoEditor::PhotoEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PhotoEditor)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->horizontalFrame);
    connect(ui->actionLoad_Image, &QAction::triggered, this, &PhotoEditor::open);
    connect(ui->actionInversion, &QAction::triggered, this, &PhotoEditor::inverse);
    connect(ui->actionBrightness, &QAction::triggered, this, &PhotoEditor::brightness);
    connect(ui->actionGamma, &QAction::triggered, this, &PhotoEditor::gamma);
    connect(ui->actionContrast, &QAction::triggered, this, &PhotoEditor::contrast);


}

void PhotoEditor::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    setWindowTitle(fileName);
    QImage image(fileName);
    current = initial = QPixmap::fromImage(image);
    ui->label->setPixmap(initial.scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));
    ui->label_2->setPixmap(initial.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::inverse()
{
    QImage image = current.toImage();
    int size = image.height() * image.width();
    QRgb* data = new QRgb[size];
    memmove(data, image.bits(), image.height() * image.width() * sizeof(QRgb));
    QRgb* ptr = data;
    QRgb* end = ptr + image.width() * image.height();
    for (; ptr < end; ++ptr)
        *ptr = qRgb(255 - qRed(*ptr), 255 - qGreen(*ptr), 255 - qBlue(*ptr));
    memmove(image.bits(), data, image.height() * image.width() * sizeof(QRgb));
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::brightness()
{
    QImage image = current.toImage();
    int size = image.height() * image.width();
    QRgb* data = new QRgb[size];
    memmove(data, image.bits(), image.height() * image.width() * sizeof(QRgb));
    unsigned int* ptr = data;
    unsigned int* end = ptr + image.width() * image.height();
    for (; ptr < end; ++ptr)
        *ptr = qRgb((qRed(*ptr) + 20)>255?255:(qRed(*ptr) + 20), (qGreen(*ptr) + 20)>255?255:(qGreen(*ptr) + 20), (qBlue(*ptr) + 20)>255?255:(qBlue(*ptr) + 20));
    memmove(image.bits(), data, image.height() * image.width() * sizeof(QRgb));
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::gamma()
{
    QImage image = current.toImage();
    int size = image.height() * image.width();
    QRgb* data = new QRgb[size];
    memmove(data, image.bits(), image.height() * image.width() * sizeof(QRgb));
    QRgb* ptr = data;
    QRgb* end = ptr + image.width() * image.height();
    for (; ptr < end; ++ptr)
        *ptr = qRgb((qRed(*ptr)/255)*(qRed(*ptr)/255)*255, (qGreen(*ptr)/255)*(qGreen(*ptr)/255)*255, (qBlue(*ptr)/255)*(qBlue(*ptr)/255)*255);
    memmove(image.bits(), data, image.height() * image.width() * sizeof(QRgb));
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::contrast()
{
    QImage image = current.toImage();
    int size = image.height() * image.width();
    QRgb* data = new QRgb[size];
    memmove(data, image.bits(), image.height() * image.width() * sizeof(QRgb));
    QRgb* ptr = data;
    QRgb* end = ptr + image.width() * image.height();
    for (; ptr < end; ++ptr)
        *ptr = qRgb(255 - qRed(*ptr), 255 - qRed(*ptr), 255 - qRed(*ptr));
    memmove(image.bits(), data, image.height() * image.width() * sizeof(QRgb));
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::resizeEvent(QResizeEvent *event) {
    ui->label->setPixmap(current.scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));
    ui->label_2->setPixmap(initial.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

PhotoEditor::~PhotoEditor()
{
    delete ui;
}

