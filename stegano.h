#ifndef STEGANO_H
#define STEGANO_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>
#include <bitset>
#include <iostream>

using namespace cv;
using namespace std;
class Stegano
{
private:
  Mat original_container;
  Mat result_container;
  Mat *planes;
  QByteArray message;
  QByteArray extracted_message;
  QByteArray start_stamp;
  QByteArray end_stamp;
  QVector<char> container_data;

public:
  Stegano ();
  Stegano (const char *);
  ~Stegano ();
  bool set_container (const char *);
  void show_image (const char *, Mat);
  void split_container (Mat);
  void merge_planes (Mat);
  bool read_message_from_file (const char *);
  void planes_to_vector (Mat *);
  void vector_to_planes (Mat *);
  bool set_lsb_data ();
  void hide_message ();
  void extract_message ();
  QByteArray get_lsb_data ();
  void unwrap_message (QByteArray);
  void print_bit_view (const char *, char);
};

#endif // STEGANO_H
