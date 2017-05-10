#ifndef STEGANO_H
#define STEGANO_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QString>
#include <QFile>

using namespace cv;
class Stegano
{
private:
  Mat original_container;
  Mat result_container;
  Mat *planes;
  QByteArray message;
  //QString message;

public:
  Stegano ();
  Stegano (const char*);
  ~Stegano ();
  //void set_message(QString);
  bool set_data(const char *);
  void show_image (const char *, Mat);
  void split_container ();
  void read_message_from_file (const char *);
};

#endif // STEGANO_H
