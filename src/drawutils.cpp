#include "drawutils.h"

#include "images.h"
#include "simple-2d/image.h"

void text(const std::string_view text, int x, const int y, const double scale) {
  for (const char c : text) {
    const Image *img = nullptr;
    switch (c) {
    case '0':
      img = &text_0_img;
      break;
    case '1':
      img = &text_1_img;
      break;
    case '2':
      img = &text_2_img;
      break;
    case '3':
      img = &text_3_img;
      break;
    case '4':
      img = &text_4_img;
      break;
    case '5':
      img = &text_5_img;
      break;
    case '6':
      img = &text_6_img;
      break;
    case '7':
      img = &text_7_img;
      break;
    case '8':
      img = &text_8_img;
      break;
    case '9':
      img = &text_9_img;
      break;
    default:
      continue; // Skip unsupported characters
    }
    image(*img, x, y, img->width * scale, img->height * scale);
    x += img->width * scale; // Move x position for next character
  }
}