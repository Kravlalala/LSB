#ifndef STEGANO_H
#define STEGANO_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QString>

using namespace cv;
class Stegano
{
private:
  Mat original_container;
  Mat result_container;
  Mat *planes;
  QString message;

public:
  Stegano ();
  Stegano (const char*);
  ~Stegano ();
  void set_message(QString);
  bool set_data(const char*);
  void show_image (const char *, Mat);
  void split_container ();
};

#endif // STEGANO_H
