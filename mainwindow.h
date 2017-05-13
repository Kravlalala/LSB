#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stegano.h"
#include <QCloseEvent>
#include <QMainWindow>
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

private:
  Stegano container;
  QByteArray input_message;
  QByteArray result_message;
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
