#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void function(QString);
    void on_Copress_button_clicked();

    void on_zipButton_clicked();

    void on_browseButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
