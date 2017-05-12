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
  Size container_dimensions;
  original_container = imread (image_path, IMREAD_COLOR);
  if (!original_container.data) {
    qDebug () << "unknow path or file";
    return false;
  } else {
    container_dimensions = original_container.size ();
    result_container.create (container_dimensions.height,
                             container_dimensions.width, CV_8UC3);
  }

  /* Separate image on three color planes: B, G and R respectively */
  split_container (original_container);

  /* Expand color data to vector */
  planes_to_vector (planes);

  show_image ("Original image", original_container);

  return true;
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
void Stegano::split_container (Mat input_image)
{
  split (input_image, planes);
}

/* Merge color planes in one image  */
void Stegano::merge_planes (Mat result_image)
{
  merge (planes, 3, result_image);
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
void Stegano::planes_to_vector (Mat *input_planes)
{
  Size plane_size = input_planes[0].size ();
  int width = plane_size.width;
  int height = plane_size.height;
  int counter = 0;
  int num_elements = height * width;

  /* Copy planes to the vector */
  container_data.resize (num_elements * 3);
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      container_data[counter] = input_planes[0].at<char> (Point (x, y));
      container_data[counter + num_elements] =
          input_planes[1].at<char> (Point (x, y));
      container_data[counter + num_elements * 2] =
          input_planes[2].at<char> (Point (x, y));
      ++counter;
    }
  }
}

/* Separate vector color data in a separate planes  */
void Stegano::vector_to_planes (Mat *output_planes)
{
  Size plane_size = output_planes[0].size ();
  int width = plane_size.width;
  int height = plane_size.height;
  int counter = 0;
  int num_elements = height * width;

  /* Copy vector data in B, G and R color planes respectively */
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
     output_planes[0].at<char> (y, x) = container_data[counter];
     output_planes[1].at<char> (y, x) = container_data[counter + num_elements];
     output_planes[2].at<char> (y, x) = container_data[counter + num_elements * 2];
      ++counter;
    }
  }
  /* Frees vector */
  container_data.remove(0,container_data.size());
}

/* Insert message in the container  */
bool Stegano::ins_message ()
{
  char current_bit;
  char msg_byte;
  int msg_length = message.length ();
  int container_size = container_data.size ();

  /* Check that message does not goes beyond the container bounds */
  if (container_size < msg_length) {
    qDebug () << "Too large message\n";
    return false;
  }

  /* Insert message bytes in the least significant bits of container data */
  for (int i = 0; i < msg_length; i++) {
    msg_byte = message.at (i);
    for (int j = 7; j >= 0; j--) {
      current_bit = msg_byte | LSB_MASK;
      container_data[j + 8 * i] = container_data[j + 8 * i] & current_bit;
      msg_byte >>= 1;
    }
  }

  return true;
}

/* Insert message in the container and show result  */
void Stegano::hide_message ()
{
  /* Insert message in the data vector */
  ins_message ();
  /* Split vector to the color planes */
  vector_to_planes (planes);
  /* Merge planes in result image */
  merge_planes (result_container);
  /* Show output image */
  show_image ("Result image", result_container);
}

/* Extract image from container and save it in the file  */
void Stegano::extract_message(){

}

void Stegano::print_bit_view (const char *prepending_text, char symbol)
{
  bitset<8> bview (symbol);
  cout << prepending_text << bview << endl;
}
