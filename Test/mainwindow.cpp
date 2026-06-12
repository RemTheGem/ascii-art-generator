#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QAbstractButton>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QtGui/qimage.h>
#include <QGuiApplication>
#include <QClipboard>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboPreset->setCurrentIndex(1);
    setWindowTitle("ASCII Art Generator");
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
    currentImage = image;
    ui->label->setText("Selected: " + (file));

}


void MainWindow::on_pushButton_2_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->textAscii->toPlainText());
}




void MainWindow::on_buttonCreate_clicked()
{
    if (currentImage.isNull()) return;
    QImage image = currentImage;
    int outWidth = 0;
    QString preset = ui->comboPreset->currentText();
    if (preset == "High Detail") {
        outWidth = 180;
    }
    else if (preset == "Balanced") {
        outWidth = 100;
    }
    else if (preset == "Terminal") {
        outWidth = 60;
    }
    int outHeight = image.height() * outWidth / image.width() / 2;
    image = image.scaled(outWidth, outHeight);
    QString chars = "@#S%?*+;:,. ";
    QString result;

    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            int gray = qGray(image.pixel(x, y));
            gray = std::clamp(gray * 1.3, 0.0, 255.0);
            if (ui->checkBoxInvert->isChecked()){
                gray = 255 - gray;
            }
            int index = (gray * (chars.size() - 1)) / 255;
            result += chars[index];
        }
        result += "\n";
    }

    ui->textAscii->setPlainText(result);
}


void MainWindow::on_pushButtonSaveTxt_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save ASCII as TXT",
        "",
        "Text Files (*.txt)"
        );

    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << ui->textAscii->toPlainText();
    if (!fileName.endsWith(".txt")) {
        fileName += ".txt";
    }
    file.close();
}

