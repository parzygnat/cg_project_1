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

    myFilter = new CustomFilter;
    myDither = new DitherDialog;
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
    connect(ui->actionExit, &QAction::triggered, this, &PhotoEditor::exit);
    connect(ui->actionLaboratory, &QAction::triggered, this, &PhotoEditor::convolution_saltpepper);


}



void PhotoEditor::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    setWindowTitle(fileName);
    if(!fileName.isEmpty()) {
        QImage image(fileName);
        current = initial = QPixmap::fromImage(image);
        ui->label->setFixedWidth(image.width());ui->label->setFixedHeight(image.height()); ui->label_2->setFixedHeight(image.height()); ui->label_2->setFixedWidth(image.width());
        ui->label->setPixmap(initial);
        ui->label_2->setPixmap(initial);
    }
}
void PhotoEditor::exit() { this->close();}

void PhotoEditor::save() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
                                                    QString(),
                                                    tr("Images (*.png)"));
    if (!fileName.isEmpty())
    {
      current.save(fileName);
    }
}

void PhotoEditor::function_filter(int (*operation)(int)) {
    QImage image = current.toImage().convertToFormat(QImage::Format_RGB888);
    uchar* ptr = image.bits();
    uchar* end = ptr + 3 * image.width() * image.height();
    for (; ptr < end; ptr++){
        *ptr = operation(*ptr);
    }
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current);
}

void PhotoEditor::inverse()
{
    function_filter([](int a) { return 255 - a; });
}

void PhotoEditor::brightness()
{
    function_filter([](int a) { return (a + 20)>255?255:(a + 20); });

}
void PhotoEditor::contrast()
{
    function_filter([](int a) { return (int)qBound(0.0, (1.1*(a - 128) + 128), 255.0); });

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
    ui->label->setPixmap(current);
}

void PhotoEditor::convolution(int sizeX, int sizeY, double* values, int anchorX, int anchorY, double offset, double divisor) {
    QImage image = current.toImage();
    QImage image2 = current.toImage();
    uchar* ptr = image.bits();
    uchar* ptr2 = image2.bits();
    uchar* end = ptr + 4 * image.width() * image.height();
    int idx = 0;
    int x = 0;
    int y = 0;
    double sum = 0;
    double sum_of_coeffs = 0;
    for(; ptr < end; ptr++){
        if(idx%4==3)
        {
            x++;
            idx++;
            if(x == image.width()){
                x = 0;
                ++y;
            }
            continue;
        }

        if((x - anchorX >= 0 && y - anchorY >= 0) && (x + (sizeX - anchorX - 1) < image.width()) && (y + (sizeY - anchorY - 1) < image.height())) {
            for(int j = 0; j < sizeX*sizeY; ++j) {
                int n_ind = idx - (anchorY - j/sizeX)*image.width()*4 - (anchorX - j%sizeX)*4;
                if(n_ind < 0 || n_ind > image.width()*image.height()*4)
                    continue;
                sum += values[j] * ptr2[n_ind];
                sum_of_coeffs += values[j];
            }
            if(sum_of_coeffs == 0) sum_of_coeffs = 1;
            if(divisor != 0) sum_of_coeffs = divisor;
            *ptr = qBound(0.0, offset + (double)sum/(double)sum_of_coeffs, 255.0);
        }

        ++idx;
        sum = 0;
        sum_of_coeffs = 0;
    }
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current);
}

struct Triple
{
    int color[3];
};

void PhotoEditor::convolution_saltpepper() {
    QImage image = current.toImage();
    QImage image2 = current.toImage();
    uchar* ptr = image.bits();
    uchar* ptr2 = image2.bits();
    uchar* end = ptr + 4 * image.width() * image.height();
    std::vector<double> intensities;
    std::map<double, Triple> colors;
    int idx = 0;
    int x = 0;
    int y = 0;
    for(; ptr < end; ptr += 4){
        if((x - 1 >= 0 && y - 1 >= 0) && (x + (3 - 1 - 1) < image.width()) && (y + (3 - 1 - 1) < image.height())) {
        for(int j = 0; j < 9; ++j) {
            int n_ind = idx - (1 - j/3)*image.width()*4 - (1 - j%3)*4;
            if(n_ind < 0 || n_ind > image.width()*image.height()*4) continue;
            intensities.push_back(0.3*(ptr2[n_ind]) + 0.6*(ptr2[n_ind + 1]) + 0.1*(ptr2[n_ind + 2]));
            Triple triple = {ptr2[n_ind], ptr2[n_ind + 1], ptr2[n_ind + 2]};
            colors.insert(std::pair<double, Triple>(0.3*(ptr2[n_ind]) + 0.6*(ptr2[n_ind + 1]) + 0.1*(ptr2[n_ind + 2]), triple));
        }
        std::sort(intensities.begin(), intensities.end());
        *ptr = colors[intensities[4]].color[0];
        *(ptr + 2) = colors[intensities[4]].color[2];
        *(ptr + 1) = colors[intensities[4]].color[1];
        intensities.clear();
        colors.clear();
        }
        idx += 4;
        x++;
        if(x == image.width()){
            x = 0;
            ++y;
        }

    }

    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current);
}

void PhotoEditor::edge_detection()
{
    double vals[9] = {1, 0, -1, 1, 0, -1, 1, 0, -1};
    PhotoEditor::convolution(3, 3, vals, 1, 1, 90);
}

void PhotoEditor::emboss()
{
    double vals[9] = {-1, -1, 0, -1, 1, 1, 0, 1, 1};
    PhotoEditor::convolution(3, 3, vals, 1, 1);
}

void PhotoEditor::box_blur()
{
    double vals[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    PhotoEditor::convolution(3, 3, vals, 1, 1);
}

void PhotoEditor::sharpen()
{
    double vals[9] = {0, -1, 0, -1, 9, -1, 0, -1, 0};
    PhotoEditor::convolution(3, 3, vals, 1, 1);
}

void PhotoEditor::gaussian_blur()
{
    double vals[9] = {0, 1, 0, 1, 4, 1, 0, 1, 0};
    PhotoEditor::convolution(3, 3, vals, 1, 1);
}


void PhotoEditor::resizeEvent(QResizeEvent *event) {
    ui->label->setPixmap(current);
    ui->label_2->setPixmap(initial);
}

PhotoEditor::~PhotoEditor()
{
    delete ui;
}


void PhotoEditor::on_actionReset_triggered()
{
    current = initial;
    ui->label->setPixmap(current.scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));
}

void PhotoEditor::on_actionCustom_Filter_triggered()
{
    myFilter->exec();
    std::vector<double> values = myFilter->getVector();
    double arr[values.size()];
    std::copy(values.begin(), values.end(), arr);
    int coo_x = myFilter->getX();
    int coo_y = myFilter->getY();
    int anch_x = myFilter->getAnchorX();
    int anch_y = myFilter->getAnchorY();
    double divisor = myFilter->getCustom()?myFilter->getDivisor():0;
    convolution(coo_x, coo_y, arr, anch_x, anch_y, 0, divisor);

}

void PhotoEditor::on_actionOrdered_Dithering_triggered()
{
    myDither->exec();
    int n = myDither->getN(), k = myDither->getK();

    // algorithm

    QImage image = current.toImage();
    uchar* ptr = image.bits();
    uchar* init_ptr = image.bits();
    uchar* end = ptr + 4 * image.width() * image.height();
    int idx = 0;
    int x = 0;
    int y = 0;
    int mod = image.width()%n;
    std::vector<double> matrix;
    std::vector<double> levels;
    double add = 1.0/(k - 1);
    for(int i = 0; i < k; ++i) {
        levels.push_back(i*add);
    }

    switch (n) {
       case 2:
        matrix = {1, 3, 4, 2};
        break;
       case 3:
        matrix = {3, 7, 4, 6, 1, 9, 2, 8, 5};
        break;
       case 4:
        matrix = {1, 9, 3, 11, 13, 5, 15, 7, 4, 12, 2, 10, 16, 8, 14, 6};
        break;
       case 6:
        matrix = {9, 25, 13, 11, 27, 15, 21, 1, 33, 23, 3, 35, 5, 29, 17, 7, 31, 19, 12, 28, 16, 10, 26, 14, 24, 4, 36, 22, 2, 34, 8, 32, 20, 6, 30, 18};
    }
    for(size_t i = 0; i < matrix.size(); ++i) matrix[i] = matrix[i]/(matrix.size()+1);
    for(; ptr < end; ptr++){
        if(y + n > image.height()) break;
        //making sure that we operate starting from the valid left upper corners
        if(idx%4==3)
        {
            x += n;
            idx += n * 4 - 3;
            ptr += n * 4 - 4;
            if(x >= image.width() - mod){
                x = 0;
                y += n;
                init_ptr += n*image.width()*4;
                ptr = init_ptr - 1;
                idx = 0;
            }
            continue;
        }

        //code for NxN dithering
        printf("%d \n", idx);
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j)
            {
                double init_color = (*(ptr + j*4 + i*image.width()*4))/255.0;
                int color = (int)(k - 1)*init_color;
                double re = (k - 1)*init_color - color;
                if ( re >= matrix[i*n + j] ) ++color;
                *(ptr + j*4 + i*image.width()*4) = levels[color]*255.0;
            }
        }
        ++idx;
    }
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current);

}

void PhotoEditor::on_actionB_W_triggered()
{
    QImage image = current.toImage();
    uchar* ptr = image.bits();
    uchar* end = ptr + 4 * image.width() * image.height();
    for(; ptr < end; ptr += 4){
        *ptr = *(ptr + 1) = *(ptr + 2) = 0.3*(*ptr) + 0.59*(*(ptr + 1)) + 0.11*(*(ptr + 2));
    }
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current);
}

void PhotoEditor::on_actionUniform_Quantization_triggered()
{
    bool ok;
    int idx = 0;
    QImage image = current.toImage();
    uchar* ptr = image.bits();
    uchar* end = ptr + 4 * image.width() * image.height();
    double k_val;
    QString text = QInputDialog::getText(this, tr("Number of intervals"),
                                         tr("Number of Intervals:"), QLineEdit::Normal,
                                         "3", &ok);
    if (ok && !text.isEmpty())
        k_val = text.toDouble();
    else k_val = 3;
    double edge = 256.0/k_val;
    for(; ptr < end; ptr++){
        if(idx%4==3){
            ++idx;
            continue;
        }
        *ptr = ((int)((*ptr)/edge) + 0.5)*edge;
        ++idx;
    }
    current = QPixmap::fromImage(image);
    ui->label->setPixmap(current);
}



