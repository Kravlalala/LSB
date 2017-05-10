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
    delete [] planes;
}

void Stegano::set_message (QString new_message)
{
  message = new_message;
}

bool Stegano::set_data (const char *image_path)
{
  original_container = imread (image_path);
  if (!original_container.data) {
    qDebug () << "unknow path or file";
    return false;
  } else {
    result_container = original_container.clone ();
    show_image("container",result_container);
    return true;
  }
}

void Stegano::show_image (const char *win_name, Mat image)
{
  namedWindow (win_name, WINDOW_AUTOSIZE);
  imshow (win_name, image);
}

void Stegano::split_container(){
    split(original_container,planes);
    show_image("blue", planes[0]);
    show_image("green",planes[1]);
    show_image("red",planes[2]);
}

