#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include "huffmantree.h"
#include <string>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set button size

    // You can also connect the button's signal to a custom slot
    // connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClick);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    // Open file dialog to select a file
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", "", "All Files (*)");

    // Check if a file was selected
    if (!filePath.isEmpty()) {
        // Show the selected file path
        ui->filepath->setText(filePath);
    } else {
        // If no file was selected
        QMessageBox::information(this, "No File", "No file was selected.");
    }
}

void MainWindow::on_zipButton_clicked()
{
    QString Qpath;
    string path;
    bool zip = false;

    if (ui->filepath->text().isEmpty() && ui->Text_insertion->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "Error", "No text chosen! ");
    } else if (!ui->filepath->text().isEmpty() && !ui->Text_insertion->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "Error", "Please choose one method to zip the text");
    } else if (!ui->filepath->text().isEmpty()) {
        if (ui->filepath->text().endsWith(".txt", Qt::CaseInsensitive)) {
            Qpath = ui->filepath->text();
            qDebug() << "Absolute File Path: " << Qpath;
            zip = true;
        } else {
            QMessageBox::critical(this, "Error", "Not a txt file!");
        }
    } else if (!ui->Text_insertion->toPlainText().isEmpty()) {
        // Get the text from QTextEdit
        QString text = ui->Text_insertion->toPlainText();
        QString absolutePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/output.txt";

        // Open the file in write mode
        QFile file(absolutePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            // Write the text to the file
            out << text;
            file.close();

            Qpath = absolutePath; // Set Qpath to the absolute path of the newly written file
            qDebug() << "Saved File Path: " << Qpath;
            zip = true;
        } else {
            // Handle the case when the file can't be opened
            QMessageBox::critical(this, "Error", "Could not open the file for writing");
        }
    }

    if (zip) {
        path = Qpath.toStdString();
        if (!path.empty()) {
            huffmanTree.Zip(path);
        } else {
            QMessageBox::critical(this, "Error", "File path is empty. Cannot proceed with zipping.");
        }
    }
}

void MainWindow::on_UnzipButton_clicked()
{
    QString Qpath;
    string path;
    bool zip = false;

    if (ui->filepath->text().isEmpty() && ui->Text_insertion->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "Error", "No text chosen! ");
    } else if (!ui->filepath->text().isEmpty() && !ui->Text_insertion->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "Error", "Please choose one method to unzip the text");
    } else if (!ui->filepath->text().isEmpty()) {
        if (ui->filepath->text().endsWith(".huff", Qt::CaseInsensitive)) {
            Qpath = ui->filepath->text();
            qDebug() << "Absolute File Path: " << Qpath;
            zip = true;
        } else {
            QMessageBox::critical(this, "Error", "Not a huff file!");
        }
    } else if (!ui->Text_insertion->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "Error", "You cannot input a .huff file manually!");
    }

    if (zip) {
        path = Qpath.toStdString();
        if (!path.empty()) {
            huffmanTree.UnZip(path);
        } else {
            QMessageBox::critical(this, "Error", "File path is empty. Cannot proceed with unzipping.");
        }
    }
}
