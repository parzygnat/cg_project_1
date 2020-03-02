#ifndef PHOTOEDITOR_H
#define PHOTOEDITOR_H

#include <QMainWindow>

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
    void contrast();

private:
    Ui::PhotoEditor *ui;
    QString currentFile;
    QPixmap initial;
    QPixmap current;
};
#endif // PHOTOEDITOR_H
