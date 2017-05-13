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
  max_message_length = container.set_original_container (image_path.toLatin1 ().data ());
}

void MainWindow::on_insertMessage_clicked ()
{
  QString file_path;
  QString message_buf;
  QMessageBox msb;

  bool radio_state = ui->fileRadio->isChecked ();

  /* Check that container is not empty */
  if (!container.needs_container ()) {

    /* Check message location */
    if (radio_state == true) {
      file_path = QFileDialog::getOpenFileName (this, tr ("Open File"), "./",
                                                tr ("Text (*.txt)"));
      if (!file_path.isEmpty ()) {
        /* Read message from the text file */
        input_message = container.read_message_from_file (
            file_path.toLatin1 ().data (), "n@ch@L0", "k0nEz$");

        /* Insert message in the container */
        container.hide_message (input_message);
      }
    } else {
      /* Get message from text editor */
      input_message = ui->textEdit->toPlainText ().toLatin1 ().data ();

      /* Insert message in the container */
      if (!input_message.isEmpty ()) {
        container.wrap_message (&input_message, "n@ch@L0", "k0nEz$");
        container.hide_message (input_message);
      } else {
        msb.setText ("Print something in the text editor, before inserting");
        msb.setWindowTitle ("Editor field is empty");
        msb.exec ();
      }
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
  if (!file_path.isEmpty ()) {

    /* Set result container */
    container.set_result_container (file_path.toLatin1 ().data ());

    /* Extract message from the container */
    container.extract_message (&result_message, "n@ch@L0", "k0nEz$");

    QString fileName =
        QFileDialog::getSaveFileName (this, tr ("Extracted message location"),
                                      "./", tr ("Text files (*.txt)"));
    /* Save message into the file */
    container.save_message (result_message, fileName.toLatin1 ().data ());
  }
}
