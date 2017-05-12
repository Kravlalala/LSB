#ifndef STEGANO_H
#define STEGANO_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <QDebug>
#include <QString>
#include <QFile>
#include <QVector>
#include <bitset>
#include <iostream>

#define LSB_MASK    254
using namespace cv;
using namespace std;
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
  bool ins_message ();
  void hide_message ();
  void print_bit_view (const char *, char);
};

#endif // STEGANO_H
