
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QDebug>




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

void MainWindow::function(QString lol){}
void MainWindow::on_zipButton_clicked()

{
    QString path;
    if(ui->filepath->text().isEmpty() && ui->Text_insertion->toPlainText().isEmpty()){
        QMessageBox::critical(this, "Error", "No text chosen! ");
    }
    else if(!ui->filepath->text().isEmpty() && !ui->Text_insertion->toPlainText().isEmpty()){
        QMessageBox::critical(this, "Error", "Please choose one method to zip the text");
    }
    else if (!ui->filepath->text().isEmpty()){
        path = ui->filepath->text();
        qDebug() << "Relative Path: " << path;
    }
    else if (!ui->Text_insertion->toPlainText().isEmpty()){
        // Get the text from QTextEdit
        QString text = ui->Text_insertion->toPlainText();
        QString relativePath;

        // Open the file in write mode
        QFile file("output.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            // Write the text to the file
            out << text;
            file.close();
            // Get the base directory (for example, the directory where your program is running)
            QString baseDir = QDir::currentPath(); // Get the current working directory

            // Get the relative path of the saved file
            QDir baseDirectory(baseDir);

            // Compute the relative path from the base directory
            path = baseDirectory.relativeFilePath("output.txt");

            // Print the relative path (or store it for later use)
            qDebug() << "Relative Path: " << path;

        } else {
            // Handle the case when the file can't be opened
            QMessageBox::critical(this, "Error", "Could not open the file for writing");
        }
    }
    function(path);
}



