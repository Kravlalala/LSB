#ifndef STEGANO_H
#define STEGANO_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <QDebug>
#include <QString>
#include <QFile>
#include <QVector>

using namespace cv;
class Stegano
{
private:
  Mat original_container;
  Mat result_container;
  Mat *planes;
  QByteArray message;
  QByteArray start_stamp;
  QByteArray end_stamp;
  QVector<char> container_data;

  //QString message;

public:
  Stegano ();
  Stegano (const char*);
  ~Stegano ();
  //void set_message(QString);
  bool set_data(const char *);
  void show_image (const char *, Mat);
  void split_container ();
  void merge_planes ();
  bool read_message_from_file (const char *);
  void planes_to_vector ();
  void vector_to_planes ();
};

#endif // STEGANO_H
