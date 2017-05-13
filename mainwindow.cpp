#include "mainwindow.h"
#include "stegano.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (QWidget *parent)
    : QMainWindow (parent), ui (new Ui::MainWindow)
{
  container.set_container ("lenna.bmp");

  /* Read message from text file */
  container.read_message_from_file("message.txt");

  /* Insert message in the container */
  container.hide_message();

  /* Extract message from the container */
  container.extract_message();
  ui->setupUi (this);
}

MainWindow::~MainWindow ()
{
  delete ui;
}

