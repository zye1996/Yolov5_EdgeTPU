//
// Created by yzy on 4/18/21.
//

#ifndef YOLO_EDGETPU_YOLODECODER_H
#define YOLO_EDGETPU_YOLODECODER_H

#include <tensorflow/lite/interpreter.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "src/label_utils.h"

struct bbox{
    float x, y, w, h;
    int cls_label;
    float score;
};

template <typename T>
T clip(const T& n, const T& lower, const T& upper) {
    return std::max(lower, std::min(n, upper));
}

class YoloDecoder {
public:
    YoloDecoder(int image_sz, int max_detection=100):
    _image_sz(image_sz), _max_detection(max_detection){};
    std::vector<bbox> decode(const std::vector<float> &output_data, tflite::Interpreter* interpreter) const;

    void scale_bbox(std::vector<bbox> &bboxes, const cv::Size& image_raw_sz) const;
    static std::vector<bbox> nms(std::vector<bbox> bboxes, float conf_thres=0.5, float iou_thres=0.45);
    static void draw_result(const std::vector<bbox> &bboxes, cv::Mat& img, const std::unordered_map<int, std::string> &label,  const std::vector<cv::Scalar> &color);

private:
    int _image_sz;
    int _max_detection;

};


#endif //YOLO_EDGETPU_YOLODECODER_H
