#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stegano.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Stegano container;
    QByteArray input_message;
    QByteArray result_message;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
