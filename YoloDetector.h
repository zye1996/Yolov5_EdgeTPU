//
// Created by yzy on 4/9/21.
//

#ifndef YOLO_EDGETPU_YOLODETECTOR_H
#define YOLO_EDGETPU_YOLODETECTOR_H

#include <string>
#include <opencv2/opencv.hpp>

using namespace std;

class YoloDetector {
public:
    YoloDetector(string model_path="yolov5s-int8_edgetpu.tflite");

    void run_inference(const cv::Mat&);

private:
    void nms();

};


#endif //YOLO_EDGETPU_YOLODETECTOR_H
