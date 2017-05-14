#ifndef STEGANO_H
#define STEGANO_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>
#include <bitset>
#include <ctime>
#include <iostream>

using namespace cv;
using namespace std;
class Stegano
{
private:
  /* Elements */
  Mat original_container;
  Mat result_container;
  Mat *planes;
  QVector<char> container_data;

  /* Methods */
  void split_container (Mat);
  void merge_planes (Mat);
  void planes_to_vector (Mat *);
  void vector_to_planes (Mat *);
  bool set_lsb_data (QByteArray);
  void get_lsb_data (QByteArray *);
  void blur_insertion_bound (QByteArray);
  void set_interval_data (QByteArray, int, int);
  void get_interval_data (QByteArray*, int, int);
  void unwrap_message (QByteArray *, const char *, const char *);
  int count_ones_number (int);

public:
  Stegano ();
  Stegano (const char *);
  ~Stegano ();
  int set_original_container (const char *);
  bool set_result_container (const char *);
  bool needs_container ();
  QByteArray read_message_from_file (const char *);
  void wrap_message (QByteArray *, const char *, const char *);
  bool save_message (QByteArray, const char *);
  void show_image (const char *, Mat);
  void hide_message (QByteArray, const char *, const char *);
  void extract_message (QByteArray *, const char *, const char *);
  void random_interval_hide (QByteArray, int, const char *, int);
  void random_interval_extract (QByteArray *, int, const char *, int);
  void print_bit_view (const char *, char);
};

#endif // STEGANO_H
