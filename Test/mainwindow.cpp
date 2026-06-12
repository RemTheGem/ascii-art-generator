#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QAbstractButton>
#include <QPushButton>
#include <QFileDialog>
#include <QtGui/qimage.h>
#include <QGuiApplication>
#include <QClipboard>
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
    QString file = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.jpeg *.webp)");
    if (file.isEmpty()){
        qDebug() << "No image selected";
        ui->label->setText("No file selected");
        return;
    }
    QImage image(file);

    if (image.isNull()) {
        qDebug() << "Failed to load image";
        return;
    }
    ui->label->setText("Selected: " + (file));
    int outWidth = 120;
    int outHeight = image.height() * outWidth / image.width() / 2;
    image = image.scaled(outWidth, outHeight);
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
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->textAscii->toPlainText());
}



