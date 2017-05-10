#include "stegano.h"
#include <QDebug>

using namespace cv;
Stegano::Stegano ()
{
}
Stegano::Stegano (const char *image_path)
{
  set_data (image_path);
}

Stegano::~Stegano ()
{
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
    return true;
  }
}
