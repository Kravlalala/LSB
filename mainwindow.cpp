#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (QWidget *parent)
    : QMainWindow (parent), ui (new Ui::MainWindow)
{
  ui->setupUi (this);
}

void MainWindow::closeEvent (QCloseEvent *event)
{
  destroyAllWindows ();
  event->accept ();
}

MainWindow::~MainWindow ()
{
  delete ui;
}

void MainWindow::on_loadContainer_clicked ()
{

  QString image_path = QFileDialog::getOpenFileName (
      this, tr ("Open File"), "./", tr ("BMP image (*.bmp)"));
  container.set_original_container (image_path.toLatin1 ().data ());
}

void MainWindow::on_insertMessage_clicked ()
{
  QString file_path;
  QMessageBox msb;

  bool radio_state = ui->fileRadio->isChecked ();

  /* Check that container is not empty */
  if (!container.needs_container ()) {

    /* Check message location */
    if (radio_state == true) {
      file_path = QFileDialog::getOpenFileName (this, tr ("Open File"), "./",
                                                tr ("Text (*.txt)"));
      /* Read message from the text file */
      input_message = container.read_message_from_file (
          file_path.toLatin1 ().data (), "n@ch@L0", "k0nEz$");

      /* Insert message in the container */
      container.hide_message (input_message);
    }
  } else {
    msb.setText ("Firstly you should to load an image");
    msb.setWindowTitle ("No containers avalible");
    msb.exec ();
  }
}

void MainWindow::on_extractMessage_clicked ()
{
  QString file_path = QFileDialog::getOpenFileName (
      this, tr ("Open File"), "./", tr ("BMP image (*.bmp)"));

  /* Set result container */
  container.set_result_container (file_path.toLatin1 ().data ());

  /* Extract message from the container */
  container.extract_message (&result_message, "n@ch@L0", "k0nEz$");

  /* Save message into the file */
  container.save_message (result_message, "./new_gui_message.txt");
}
