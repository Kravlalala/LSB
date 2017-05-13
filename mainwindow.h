#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stegano.h"
#include <QCloseEvent>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow (QWidget *parent = 0);
  void closeEvent (QCloseEvent *event);
  ~MainWindow ();

private slots:
  void on_loadContainer_clicked();

  void on_insertMessage_clicked();

  void on_extractMessage_clicked();

private:
  Stegano container;
  QByteArray input_message;
  QByteArray result_message;
  int max_message_length;
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
