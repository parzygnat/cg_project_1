#include "photoeditor.h"
#include "ui_photoeditor.h"
#include <QFileDialog>
#include <QMessageBox>
#include <cmath>
#include <QtGlobal>
#include <QInputDialog>

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
    connect(ui->actionSave_Image, &QAction::triggered, this, &PhotoEditor::save);
    //Convolution filters
    connect(ui->actionBox_Blur, &QAction::triggered, this, &PhotoEditor::box_blur);
    connect(ui->actionGaussian_Blur, &QAction::triggered, this, &PhotoEditor::gaussian_blur);
    connect(ui->actionEdge_Detection, &QAction::triggered, this, &PhotoEditor::edge_detection);
    connect(ui->actionEmboss, &QAction::triggered, this, &PhotoEditor::emboss);
    connect(ui->actionSharpen, &QAction::triggered, this, &PhotoEditor::sharpen);


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

void PhotoEditor::save() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
                                                    QString(),
                                                    tr("Images (*.png)"));
    if (!fileName.isEmpty())
    {
      current.save(fileName);
    }
}

void PhotoEditor::inverse()
{
    QImage image = current.toImage().convertToFormat(QImage::Format_RGB888);
    uchar* ptr = image.bits();
    uchar* end = ptr + 3 * image.width() * image.height();
    for (; ptr < end; ptr++){
        *ptr = 255 - *ptr;
    }
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::brightness()
{
    QImage image = current.toImage().convertToFormat(QImage::Format_RGB888);
    uchar* ptr = image.bits();
    uchar* end = ptr + 3 * image.width() * image.height();
    for (; ptr < end; ptr++){
        *ptr = (*ptr + 20)>255?255:(*ptr + 20);
    }
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::gamma()
{
    bool ok;
    double gamma_val;
    QString text = QInputDialog::getText(this, tr("Gamma Value"),
                                         tr("Gamma Amount:"), QLineEdit::Normal,
                                         "0.5", &ok);
    if (ok && !text.isEmpty())
        gamma_val = text.toDouble();

    QImage image = current.toImage().convertToFormat(QImage::Format_RGB888);
    uchar* ptr = image.bits();
    uchar* end = ptr + 3 * image.width() * image.height();
    for (; ptr < end; ptr++){
        *ptr = 255 * std::pow((*ptr)/255.0, 1.0/gamma_val);
    }
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::contrast()
{
    QImage image = current.toImage().convertToFormat(QImage::Format_RGB888);
    uchar* ptr = image.bits();
    uchar* end = ptr + 3 * image.width() * image.height();
    for (; ptr < end; ptr++){
        *ptr = qBound(0.0, (1.1*(*ptr - 128) + 128), 255.0);
    }
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::box_blur()
{
    double vals[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    PhotoEditor::convolution(3, 3, vals, 4);
}

void PhotoEditor::convolution(int sizeX, int sizeY, double* values, int anchor) {
    QImage image = current.toImage().convertToFormat(QImage::Format_RGB888);
    QImage image2 = current.toImage().convertToFormat(QImage::Format_RGB888);
    uchar* ptr = image.bits();
    uchar* ptr2 = image2.bits();
    uchar* end = ptr + 3 * image.width() * image.height();
    int idx = 0;
    int x = 1;
    int y = 1;
    double sum = 0;
    double sum_of_coeffs = 0;
    for (; ptr < end; ptr++){
        for(int j = 0; j < sizeX*sizeY; ++j) {
            sum += values[j] * ptr2[idx - ((anchor/sizeX) - j/sizeX)*image.width()*3 - ((anchor%sizeX) - j%sizeX)*3];
            sum_of_coeffs += values[j];
        }
        *ptr = sum/sum_of_coeffs;
        ++x;
        if(x >= image.width()){
            x = 0;
            ++y;
        }
        ++idx;
        sum = 0;
        sum_of_coeffs = 0;
    }
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::edge_detection()
{
    QImage image = current.toImage().convertToFormat(QImage::Format_RGB888);
    uchar* ptr = image.bits();
    uchar* end = ptr + 3 * image.width() * image.height();
    for (; ptr < end; ptr++){
        *ptr = 255 - *ptr;
    }
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::emboss()
{
    QImage image = current.toImage().convertToFormat(QImage::Format_RGB888);
    uchar* ptr = image.bits();
    uchar* end = ptr + 3 * image.width() * image.height();
    for (; ptr < end; ptr++){
        *ptr = 255 - *ptr;
    }
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::sharpen()
{
    QImage image = current.toImage().convertToFormat(QImage::Format_RGB888);
    uchar* ptr = image.bits();
    uchar* end = ptr + 3 * image.width() * image.height();
    for (; ptr < end; ptr++){
        *ptr = 255 - *ptr;
    }
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current.scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::gaussian_blur()
{
    QImage image = current.toImage().convertToFormat(QImage::Format_RGB888);
    uchar* ptr = image.bits();
    uchar* end = ptr + 3 * image.width() * image.height();
    for (; ptr < end; ptr++){
        *ptr = 255 - *ptr;
    }
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

