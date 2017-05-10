#include "stegano.h"
#include <QDebug>

using namespace cv;
Stegano::Stegano ()
{
  planes = new Mat[3];
}
Stegano::Stegano (const char *image_path)
{
  /* Load image */
  set_data (image_path);

  planes = new Mat[3];
}

Stegano::~Stegano ()
{
  delete[] planes;
}

/*void Stegano::set_message (QString new_message)
{
 message = new_message;
}*/

bool Stegano::set_data (const char *image_path)
{
  original_container = imread (image_path);
  if (!original_container.data) {
    qDebug () << "unknow path or file";
    return false;
  } else {
    result_container = original_container.clone ();
    show_image ("container", result_container);
    return true;
  }
}

void Stegano::show_image (const char *win_name, Mat image)
{
  namedWindow (win_name, WINDOW_AUTOSIZE);
  imshow (win_name, image);
}

void Stegano::split_container ()
{
  split (original_container, planes);
}

void Stegano::read_message_from_file (const char *file_name)
{
  QFile input_file (file_name);
  QString current_line;
  bool ret;
  ret = input_file.open (QIODevice::ReadOnly);
  if (ret != false) {
    QTextStream input_stream (&input_file);
    while (!input_stream.atEnd ()) {
      current_line = input_stream.readLine ();
      message += current_line;
    }
    input_file.close ();
  } else
    qDebug () << "error of opening text file\n";
}
