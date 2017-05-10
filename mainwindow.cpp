#include "mainwindow.h"
#include "stegano.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (QWidget *parent)
    : QMainWindow (parent), ui (new Ui::MainWindow)
{
  container.set_data ("lenna.bmp");
  ui->setupUi (this);
}

MainWindow::~MainWindow ()
{
  delete ui;
}

void MainWindow::on_messageArea_textChanged ()
{
  QString edit_data = ui->messageArea->toPlainText ();
  container.set_message (edit_data);
}
