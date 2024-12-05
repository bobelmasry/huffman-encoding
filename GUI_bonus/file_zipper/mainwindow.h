#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "huffmantree.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_browseButton_clicked();
    void on_zipButton_clicked();


    void on_UnzipButton_clicked();

private:
    Ui::MainWindow *ui;
    HuffmanTree huffmanTree;
};
#endif // MAINWINDOW_H
