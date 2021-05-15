//
// Created by yzy on 4/18/21.
//

#include "YoloDecoder.h"
#include "anchor.h"
#include <cmath>
#include <iostream>
#include <algorithm>

std::vector <float> sigmoid (const std::vector <float>& m1) {

    /*  Returns the value of the sigmoid function f(x) = 1/(1 + e^-x).
        Input: m1, a vector.
        Output: 1/(1 + e^-x) for every element of the input matrix m1.
    */

    const unsigned long VECTOR_SIZE = m1.size();
    std::vector <float> output (VECTOR_SIZE);

    for( unsigned i = 0; i != VECTOR_SIZE; ++i ) {
        output[ i ] = 1.0 / (1 + exp(-m1[ i ]));
    }

    return output;
}

inline double bbox_iou(const bbox &box_1, const bbox &box_2){

    double box1_x1 = box_1.x - 0.5 * box_1.w;
    double box1_x2 = box_1.x + 0.5 * box_1.w;
    double box1_y1 = box_1.y - 0.5 * box_1.h;
    double box1_y2 = box_1.y + 0.5 * box_1.h;

    double box2_x1 = box_2.x - 0.5 * box_2.w;
    double box2_x2 = box_2.x + 0.5 * box_2.w;
    double box2_y1 = box_2.y - 0.5 * box_2.h;
    double box2_y2 = box_2.y + 0.5 * box_2.h;

    double x_left = fmax(box1_x1, box2_x1);
    double y_top = fmax(box1_y1, box2_y1);
    double x_right = fmin(box1_x2, box2_x2);
    double y_bottom = fmin(box1_y2, box2_y2);
    double intersection = fmax(0, x_right - x_left) * fmax(0, y_bottom - y_top);
    return intersection / (box_1.w * box_1.h + box_2.w * box_2.h - intersection);

}

std::vector<bbox> YoloDecoder::decode(const std::vector<float> &output_data, tflite::Interpreter* interpreter) const {

    // Get output dims
    const auto& output_indices = interpreter->outputs();
    const int num_outputs = output_indices.size();

    std::vector<bbox> ret;

    // Decode result
    int beg_idx = 0;
    auto output_data_sigmoid = sigmoid(output_data);
    // for (auto p: output_data_sigmoid)
    //    std::cout << p << std::endl;
    for (int idx = 0; idx < num_outputs; idx++){
        const TfLiteIntArray* dims = interpreter->tensor(output_indices[idx])->dims;
        // w: num anchor; h: num_pixel in feat_map; c: num class+bbox
        int w = dims->data[1], h = dims->data[2], c = dims->data[3];
        int feat_map_width = static_cast<int>(sqrt(h));
        int stride = _image_sz / feat_map_width;
        for (int i=0; i<w; i++){
            for (int j=0; j<h; j++){
                bbox box;
                box.x = (output_data_sigmoid[beg_idx+c*h*i+c*j+0] * 2 - 0.5 + (j%feat_map_width)) * stride;
                box.y = (output_data_sigmoid[beg_idx+c*h*i+c*j+1] * 2 - 0.5 + (j/feat_map_width)) * stride;
                box.w = pow(output_data_sigmoid[beg_idx+c*h*i+c*j+2] * 2, 2) * anchor[idx][0+2*i];
                box.h = pow(output_data_sigmoid[beg_idx+c*h*i+c*j+3] * 2, 2) * anchor[idx][1+2*i];
                box.score = output_data_sigmoid[beg_idx+c*h*i+c*j+4];
                box.cls_label = std::max_element(output_data_sigmoid.begin()+beg_idx+c*h*i+c*j+5,
                                                 output_data_sigmoid.begin()+beg_idx+c*h*i+c*j+c) - (output_data_sigmoid.begin()+beg_idx+c*h*i+c*j+5);
                // std::cout << box.score << std::endl;
                ret.insert(ret.end(), std::move(box));
            }
        }
        // std::cout << "break" <<std::endl;
        beg_idx += w*h*c;
    }

    // sort the bboxes
    std::sort(ret.begin(), ret.end(), [](const bbox &box1, const bbox& box2)->bool{return box1.score > box2.score;});

    return ret;
}

std::vector<bbox> YoloDecoder::nms(std::vector<bbox> bboxes, float conf_thres, float iou_thres){

    // delete those under conf_thres
    for(auto it=bboxes.begin(); it != bboxes.end(); it++){
        if(it->score < conf_thres){
            bboxes.erase(it, bboxes.end());
            break;
        }
    }

    // nms
    std::vector<bbox> ret;
    while(!bboxes.empty()){
        ret.push_back(bboxes[0]);
        auto it = bboxes.erase(bboxes.begin());
        for(; it!= bboxes.end(); it++){
            if(bbox_iou(ret.back(), *it) > iou_thres) {
                bboxes.erase(it);
                it--;
            }
        }
    }
    return ret;


}

void YoloDecoder::scale_bbox(std::vector<bbox> &bboxes, const cv::Size& image_raw_sz) const{

    double gain = fmin(_image_sz / (double)image_raw_sz.width, _image_sz / (double)image_raw_sz.height);
    double pad_x = (_image_sz - image_raw_sz.width * gain) / 2;
    double pad_y = (_image_sz - image_raw_sz.height * gain) / 2;


    for(auto &bbox: bboxes){
        bbox.w /= gain;
        bbox.h /= gain;
        bbox.x = (bbox.x - pad_x) / gain;
        bbox.y = (bbox.y - pad_y) / gain;
    }

}

void YoloDecoder::draw_result(const std::vector<bbox> &bboxes, cv::Mat& img, const std::unordered_map<int, std::string> &label, const std::vector<cv::Scalar> &color){
    for (auto &bbox: bboxes){
        cv::rectangle(img,
                      cv::Point(clip((int)(bbox.x-bbox.w*0.5), 0, img.size().width), clip((int)(bbox.y-bbox.h*0.5), 0, img.size().height)),
                      cv::Point(clip((int)(bbox.x+bbox.w*0.5), 0, img.size().width), clip((int)(bbox.y+bbox.h*0.5), 0, img.size().height)),
                      color[bbox.cls_label], 5);
        auto it = label.find(bbox.cls_label);
        std::ostringstream stringStream;
        stringStream << it->second << " " << std::setprecision(2) << bbox.score;
        if(it != label.end()){
            int tl = 2;
            int tf = tl - 1;
            int baseline=0;
            cv::Size t_size = cv::getTextSize(stringStream.str(), 0, (float)tl / 3, tf, &baseline);
            cv::rectangle(img,
                          cv::Point(clip((int)(bbox.x-bbox.w*0.5), 0, img.size().width), clip((int)(bbox.y-bbox.h*0.5), 0, img.size().height)),
                          cv::Point(clip((int)(bbox.x-bbox.w*0.5+t_size.width), 0, img.size().width), clip((int)(bbox.y-bbox.h*0.5+t_size.height+3), 0, img.size().height)),
                          color[bbox.cls_label],
                          cv::FILLED);
            cv::putText(img,
                        stringStream.str(),
                        cv::Point(bbox.x-bbox.w*0.5, bbox.y-bbox.h*0.5+t_size.height),
                        cv::FONT_HERSHEY_DUPLEX,
                        (float)tl / 3,
                        CV_RGB(225, 255, 255), //font color
                        2,
                        cv::LINE_AA);
        }
    }
}

