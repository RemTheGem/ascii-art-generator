#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QAbstractButton>
#include <QPushButton>
#include <QFileDialog>
#include <QtGui/qimage.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.jpeg)");
    if (file.isEmpty()) return;
    QImage image(file);
    if (image.isNull()) return;
    image = image.scaled(100, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    image = image.scaled(120, 60);
    image = image.scaled(image.width(), image.height() * 0.5);
    QString chars = "@#S%?*+;:,. ";
    QString result;

    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            int gray = qGray(image.pixel(x, y));
            gray = std::clamp(gray * 1.3, 0.0, 255.0);
            int index = (gray * (chars.size() - 1)) / 255;
            result += chars[index];
        }
        result += "\n";
    }

    ui->textAscii->setPlainText(result);
}


void MainWindow::on_pushButton_2_clicked()
{

}

