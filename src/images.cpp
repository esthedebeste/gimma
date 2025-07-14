#include "images.h"
#include "resources.h"
#include <iostream>

Image load_resource_image(const unsigned char *data, unsigned size) {
  const std::vector image_data(data, data + size);
  auto img = decode_image(image_data);
  if (!img) {
    std::cerr << "Failed to load image from resource data" << std::endl;
    exit(1);
  }
  return img.value();
}

Image hit_peg_img = load_resource_image(RESOURCES_hit_peg_png,
                                        RESOURCES_hit_peg_png_size);
Image peg_img = load_resource_image(RESOURCES_peg_png,
                                    RESOURCES_peg_png_size);
Image required_hit_peg_img = load_resource_image(RESOURCES_required_hit_peg_png,
                                                 RESOURCES_required_hit_peg_png_size);
Image required_peg_img = load_resource_image(RESOURCES_required_peg_png,
                                             RESOURCES_required_peg_png_size);
Image rectangle_img = load_resource_image(RESOURCES_rect_png,
                                          RESOURCES_rect_png_size);
Image small_rectangle_img = load_resource_image(RESOURCES_small_rect_png,
                                                RESOURCES_small_rect_png_size);
Image gun_img = load_resource_image(RESOURCES_gun_png,
                                    RESOURCES_gun_png_size);
Image ball_img = load_resource_image(RESOURCES_ball_png,
                                     RESOURCES_ball_png_size);

Image text_0_img = load_resource_image(RESOURCES_0_png,
                                       RESOURCES_0_png_size);
Image text_1_img = load_resource_image(RESOURCES_1_png,
                                       RESOURCES_1_png_size);
Image text_2_img = load_resource_image(RESOURCES_2_png,
                                       RESOURCES_2_png_size);
Image text_3_img = load_resource_image(RESOURCES_3_png,
                                       RESOURCES_3_png_size);
Image text_4_img = load_resource_image(RESOURCES_4_png,
                                       RESOURCES_4_png_size);
Image text_5_img = load_resource_image(RESOURCES_5_png,
                                       RESOURCES_5_png_size);
Image text_6_img = load_resource_image(RESOURCES_6_png,
                                       RESOURCES_6_png_size);
Image text_7_img = load_resource_image(RESOURCES_7_png,
                                       RESOURCES_7_png_size);
Image text_8_img = load_resource_image(RESOURCES_8_png,
                                       RESOURCES_8_png_size);
Image text_9_img = load_resource_image(RESOURCES_9_png,
                                       RESOURCES_9_png_size);