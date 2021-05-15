//
// Created by yzy on 4/19/21.
//

#ifndef YOLO_EDGETPU_IMAGE_UTILS_H
#define YOLO_EDGETPU_IMAGE_UTILS_H

#include <opencv2/opencv.hpp>

void padding_image(cv::Mat &image, int target_size);

#endif //YOLO_EDGETPU_IMAGE_UTILS_H

