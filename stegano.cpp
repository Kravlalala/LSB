#include "stegano.h"

using namespace cv;
/* Empty constructor  */
Stegano::Stegano ()
{
  planes = new Mat[3];
}
/* Constructot with container setting  */
Stegano::Stegano (const char *image_path)
{
  /* Load image */
  set_original_container (image_path);

  planes = new Mat[3];
}

/* Destructor  */
Stegano::~Stegano ()
{
  delete[] planes;
}

/*
 * Set container data.
 * @image_path - path to the image.
*/
int Stegano::set_original_container (const char *image_path)
{
  Size container_dimensions;

  /* Read image */
  if (original_container.empty () == false)
    original_container.release ();
  original_container = imread (image_path, IMREAD_COLOR);

  if (!original_container.data) {
    qDebug () << "unknow path or file";
    return 0;
  } else {
    container_dimensions = original_container.size ();
    result_container.create (container_dimensions.height,
                             container_dimensions.width, CV_8UC3);
  }

  /* Separate image on three color planes: B, G and R respectively */
  split_container (original_container);

  /* Expand pixel data from planes to data vector */
  planes_to_vector (planes);

  show_image ("Original image", original_container);

  return container_dimensions.width * container_dimensions.height * 3 / 8;
}

bool Stegano::set_result_container (const char *image_path)
{
  /* Read image */
  if (result_container.empty () == false)
    result_container.release ();
  result_container = imread (image_path, IMREAD_COLOR);

  if (!result_container.data) {
    qDebug () << "Error of opening container-result";
    return false;
  }

  show_image ("Image with message", result_container);
  return true;
}

/* Check that container for message has been created  */
bool Stegano::needs_container ()
{
  if (result_container.empty ())
    return true;
  return false;
}

/*
 * Read message from input file in ASII.
 * @file_path - path to the input file.
*/
QByteArray Stegano::read_message_from_file (const char *file_path)
{
  QByteArray msg_buffer;
  QFile input_file (file_path);
  bool ret;

  /* Read text data to the buffer*/
  ret = input_file.open (QIODevice::ReadOnly);
  if (ret != false) {
    msg_buffer = input_file.readAll ();
    input_file.close ();
  } else {
    qDebug () << "error of opening text file\n";
    return NULL;
  }

  return msg_buffer;
}

void Stegano::wrap_message (QByteArray *message, const char *start_stamp,
                            const char *end_stamp)
{
  message->prepend (start_stamp);
  message->append (end_stamp);
}

/*
 * Save message into the file.
 * @message - array of message bytes.
 * @file_path - path to the file.
*/
bool Stegano::save_message (QByteArray message, const char *file_path)
{
  QFile output_file (file_path);
  bool ret;

  ret = output_file.open (QIODevice::WriteOnly | QIODevice::Text |
                          QIODevice::Truncate);
  if (ret != false) {
    output_file.write (message);
    output_file.close ();
    return true;
  } else {
    qDebug () << "Error of opening output file\n";
    return false;
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
void Stegano::split_container (Mat input_image)
{
  /* Check that buffer planes are empty */
  if ((planes[0].empty () == true || planes[1].empty () == true ||
       planes[2].empty () == true)) {
    planes[0].release ();
    planes[1].release ();
    planes[2].release ();
  }
  split (input_image, planes);
}

/* Merge color planes in one image  */
void Stegano::merge_planes (Mat result_image)
{
  merge (planes, 3, result_image);
  planes[0].release ();
  planes[1].release ();
  planes[2].release ();
}

/* Create vector, containing data from all color planes  */
void Stegano::planes_to_vector (Mat *input_planes)
{
  Size plane_size = input_planes[0].size ();
  int width = plane_size.width;
  int height = plane_size.height;
  int counter = 0;
  int num_elements = height * width;

  /* Check that data vector is empty */
  if (container_data.size () != 0)
    container_data.remove (0, container_data.size ());

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
      output_planes[2].at<char> (y, x) =
          container_data[counter + num_elements * 2];
      ++counter;
    }
  }
  /* Free vector data */
  container_data.remove (0, container_data.size ());
}

/* Insert message in the container and show result  */
void Stegano::hide_message (QByteArray message, const char *start_stamp,
                            const char *end_stamp)
{
  /* Wrap message with start and end stamps */
  wrap_message (&message, start_stamp, end_stamp);
  /* Insert message in the data vector */
  set_lsb_data (message);
  /* Change unused lsb's value with uniform distribution */
  blur_insertion_bound (message);
  /* Split vector on the color planes */
  vector_to_planes (planes);
  /* Merge planes in result image */
  merge_planes (result_container);
  /* Show output image */
  show_image ("This image contains your message", result_container);
}

/* Insert message in the container  */
bool Stegano::set_lsb_data (QByteArray message)
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
    // print_bit_view ("msg:", msg_byte);
    for (int j = 7; j >= 0; j--) {
      current_bit = msg_byte & 1;
      // print_bit_view ("cur:", current_bit);
      // print_bit_view("dat:",container_data[j + 8 * i]);
      if (current_bit == 1)
        container_data[j + 8 * i] |= 1;
      else
        container_data[j + 8 * i] &= (~1);
      // print_bit_view ("new:", container_data[j + 8 * i]);
      msg_byte >>= 1;
      // print_bit_view("msg:",msg_byte);
    }
  }

  return true;
}

void Stegano::blur_insertion_bound (QByteArray message)
{
  int msg_len = message.length ();
  int container_size = container_data.size ();
  char new_value;

  /* Generate random numbers from pix intensity values */
  srand (time (NULL));

  if (msg_len < container_size) {
    for (int i = msg_len * 8; i < container_size; i++) {
      new_value = (qrand () % 256) & 1;
      // print_bit_view("new:", new_value);
      // print_bit_view("dat:", container_data[i]);
      if (new_value == 1)
        container_data[i] |= 1;
      else
        container_data[i] &= ~1;
      // print_bit_view("dat:",container_data[i]);
    }
  }
}

/* Extract message from container and save it in the file  */
void Stegano::extract_message (QByteArray *extracted_msg,
                               const char *start_stamp, const char *end_stamp)
{
  /* Split container on the color planes */
  split_container (result_container);
  /* Expand plains to the vector */
  planes_to_vector (planes);
  /* Extract all lsb's from container */
  get_lsb_data (extracted_msg);
  /* Extract message from lsb data */
  unwrap_message (extracted_msg, start_stamp, end_stamp);
}

/* Get all lsb's from container  */
void Stegano::get_lsb_data (QByteArray *extracted_msg)
{
  char current_lsb;
  char msg_byte;
  int container_size = container_data.size ();
  /* Get message bytes from least significant bits of the container data */
  for (int i = 0; i < container_size / 8; i++) {
    msg_byte = 0;
    for (int j = 0; j < 8; j++) {
      msg_byte <<= 1;
      // print_bit_view ("msg:", msg_byte);
      // print_bit_view ("data:", container_data[j + 8 * i]);
      current_lsb = container_data[j + 8 * i] & 1;
      // print_bit_view ("lsb:", current_lsb);
      if (current_lsb == 1)
        msg_byte |= 1;
      else
        msg_byte &= ~(1);
      // print_bit_view ("new:", msg_byte);
    }
    extracted_msg->append (msg_byte);
  }
}

/* Found message, wrapped in stamps and get it out  */
void Stegano::unwrap_message (QByteArray *lsb_data, const char *start_stamp,
                              const char *end_stamp)
{
  int start_index;
  int end_index;
  int start_stamp_len = QByteArray (start_stamp).length ();
  start_index = lsb_data->indexOf (start_stamp);
  lsb_data->remove (0, start_index + start_stamp_len);
  end_index = lsb_data->indexOf (end_stamp);
  lsb_data->remove (end_index, lsb_data->length () - end_index);
}

/* Hide message via pseudo random interval method  */
void Stegano::random_interval_hide (QByteArray message)
{
  /* Set bits with intervals, depending on number of 1 in previous byte
   * position  */
}

void Stegano::set_interval_data (QByteArray message)
{
}

/* Print char value in binary format  */
void Stegano::print_bit_view (const char *prepending_text, char symbol)
{
  bitset<8> bview (symbol);
  cout << prepending_text << bview << endl;
}
