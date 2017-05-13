#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (QWidget *parent)
    : QMainWindow (parent), ui (new Ui::MainWindow)
{
  container.set_container ("lenna.bmp");

  /* Read message from text file */
  input_message =
      container.read_message_from_file ("message.txt", "n@ch@L0", "k0nEz$");

  /* Insert message in the container */
  container.hide_message (input_message);

  /* Extract message from the container */
  container.extract_message (&result_message, "n@ch@L0", "k0nEz$");

  /* Save message into the file */
  container.save_message (result_message, "./extracted_message.txt");
  ui->setupUi (this);
}

void MainWindow::closeEvent (QCloseEvent *event)
{
  destroyAllWindows();
  event->accept ();
}

MainWindow::~MainWindow ()
{
  delete ui;
}
