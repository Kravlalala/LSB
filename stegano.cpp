#include "stegano.h"

using namespace cv;
/* Empty constructor  */
Stegano::Stegano ()
{
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
  original_container = imread (image_path, IMREAD_COLOR);
  if (!original_container.data) {
    qDebug () << "unknow path or file";
    return false;
  } else {
    result_container = original_container.clone ();
    //show_image ("container", result_container);
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
  } else {
    qDebug () << "error of opening text file\n";
    return false;
  }

  /* Wrap message with start and end stamps */
  message.append (end_stamp);
  message.prepend (start_stamp);

  return true;
}

/* Create vector, containing data from all color planes  */
void Stegano::planes_to_vector ()
{
  Size plane_size = planes[0].size ();
  int width = plane_size.width;
  int height = plane_size.height;
  int counter = 0;
  int num_elements = height * width;

  /* Copy planes to the vector */
  container_data.resize (num_elements * 3);
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      container_data[counter] = planes[0].at<char> (Point (x, y));
      container_data[counter + num_elements] =
          planes[1].at<char> (Point (x, y));
      container_data[counter + num_elements * 2] =
          planes[2].at<char> (Point (x, y));
      ++counter;
    }
  }
  show_image("B", planes[0]);
  show_image("G", planes[1]);
  show_image("R", planes[2]);
  qDebug () << "list was filled\n";
}

/* Separate vector color data in a separate planes  */
void Stegano::vector_to_planes ()
{
  Size plane_size = planes[0].size ();
  int width = plane_size.width;
  int height = plane_size.height;
  int counter = 0;
  int num_elements = height * width;

  /* Copy vector data in B, G and R color planes respectively */
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      planes[0].at<char> (y, x) = container_data[counter];
      planes[1].at<char> (y, x) = container_data[counter + num_elements];
      planes[2].at<char> (y, x) = container_data[counter + num_elements * 2];
      ++counter;
    }
  }
  show_image("reestablished B",planes[0]);
  show_image("reestablished G",planes[1]);
  show_image("reestablished R",planes[2]);

}
