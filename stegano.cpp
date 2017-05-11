#include "stegano.h"
#include <QDebug>

using namespace cv;
/* Empty constructor  */
Stegano::Stegano ()
{
  int size;
  planes = new Mat[3];
  start_stamp = "n@ch@L0";
  end_stamp = "k0nEz$";

}
/* Constructot with container setting  */
Stegano::Stegano (const char *image_path)
{
  /* Load image */
  set_data (image_path);

  planes = new Mat[3];
}

/* Destructor  */
Stegano::~Stegano ()
{
  delete[] planes;
}

/*void Stegano::set_message (QString new_message)
{
 message = new_message;
}*/

/*
 * Set container data.
 * @image_path - path to the image.
*/
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

/*
 * Show image in a separate window.
 * @win_name - window title.
 * @image - showing image.
*/
void Stegano::show_image (const char *win_name, Mat image)
{
  namedWindow (win_name, WINDOW_AUTOSIZE);
  imshow (win_name, image);
}

/* Split container in separate planes  */
void Stegano::split_container ()
{
  split (original_container, planes);
}

/* Merge color planes in one image  */
void Stegano::merge_planes ()
{
  merge (planes, 3, result_container);
}

/*
 * Read message from input file in ASII.
 * @file_name - path to the input file.
*/
bool Stegano::read_message_from_file (const char *file_name)
{
  QFile input_file (file_name);
  bool ret;
  ret = input_file.open (QIODevice::ReadOnly);
  if (ret != false) {
    message = input_file.readAll ();
    input_file.close ();
  } else{
    qDebug () << "error of opening text file\n";
    return false;
  }

  /* Wrap message with start and end stamps */
  message.append(end_stamp);
  message.prepend(start_stamp);

  return true;
}

