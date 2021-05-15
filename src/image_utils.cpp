//
// Created by yzy on 4/19/21.
//

#include "image_utils.h"
#include <cmath>

void padding_image(cv::Mat &image, int target_size){

    // resize image
    int height = image.size().height, width = image.size().width;

    double ratio = fmax(height, width) / target_size;
    if (height > width)
        cv::resize(image, image, cv::Size(static_cast<int>(width / ratio), 320));
    else
        cv::resize(image, image, cv::Size(320, static_cast<int>(height / ratio)));

    // add padding
    double dw = (target_size - image.size().width) / 2.0;
    double dh = (target_size - image.size().height) / 2.0;

    int top = static_cast<int>(round(dh-0.1));
    int bottom = static_cast<int>(round(dh+0.1));
    int left = static_cast<int>(round(dw-0.1));
    int right = static_cast<int>(round(dw+0.1));

    cv::copyMakeBorder(image, image, top, bottom, left, right,
                       cv::BORDER_CONSTANT, cv::Scalar(114, 114, 114));
}