#include "mainwindow.h"
#include "stegano.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (QWidget *parent)
    : QMainWindow (parent), ui (new Ui::MainWindow)
{
  container.set_data ("lenna.bmp");
  container.split_container();
  container.merge_planes();
  container.planes_to_vector();
  container.vector_to_planes();

  /* Read message from text file */
  container.read_message_from_file("message.txt");
  ui->setupUi (this);
}

MainWindow::~MainWindow ()
{
  delete ui;
}

