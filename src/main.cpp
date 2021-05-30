#include <iostream>
#include <chrono>
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>
#include <edgetpu.h>
#include <random>
#include <opencv2/opencv.hpp>

#include "model_utils.h"
#include "label_utils.h"
#include "image_utils.h"
#include "YoloDecoder.h"


int main(int argc, char** argv) {

    // parse arguments
    cv::String keys =
            "{model_path  | model/yolov5s6-int8_edgetpu.tflite | edgetpu model path}"
            "{label_path  | model/coco-labels.csv | label path}"
            "{source      | 0    | camera index}"
            "{help        |      | show help message}";

    cv::CommandLineParser parser(argc, argv, keys);
    if (parser.has("help")) {
        parser.printMessage();
        return 0;
    }

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    // Read model.
    auto model_path = parser.get<cv::String>("model_path");
    auto model =
            tflite::FlatBufferModel::BuildFromFile(model_path.c_str());

    if (model == nullptr) {
        std::cerr << "Model not found!" << std::endl;
        return 1;
    }

    // Build interpreter
    std::shared_ptr<edgetpu::EdgeTpuContext> edgetpuContext
        = edgetpu::EdgeTpuManager::GetSingleton()->OpenDevice();
    if (!edgetpuContext) {
        std::cerr << "No coral found!" << std::endl;
        return 1;
    }
    std::unique_ptr<tflite::Interpreter> interpreter
        = coral::BuildEdgeTpuInterpreter(*model, edgetpuContext.get());
    const auto& required_shape = coral::GetInputShape(*interpreter, 0);

    // Build decoder
    YoloDecoder yoloDecoder(required_shape[0]);
    auto label_path = parser.get<cv::String>("label_path");
    auto label = coral::ReadLabelFile(label_path);
    std::vector<cv::Scalar> color;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,255);
    for (int i=0; i<label.size(); i++){
        color.emplace_back(distribution(generator), distribution(generator), distribution(generator), 0);
    }

    // Invoke the interpreter once to load the model onto the coral
    interpreter->Invoke();

    // Open the cap
    auto cap = cv::VideoCapture(parser.get<int>("source"));
    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    cv::Mat img, raw_image;

    for(;;){
        auto time_beg = std::chrono::steady_clock::now();
        cap.read(raw_image);
        cv::flip(raw_image, raw_image, 3);
        img = raw_image.clone();

        // Read an image
        // cv::Mat img = cv::imread("/home/yzy/CLionProjects/Yolo_EdgeTPU/img.png", cv::IMREAD_COLOR);
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        padding_image(img, 320);

        int height = img.size().height,
            width = img.size().width,
            channels = img.channels();

        // Check input size
        const auto& required_shape = coral::GetInputShape(*interpreter, 0);
        if (height != required_shape[0] || width != required_shape[1] ||
            channels != required_shape[2]) {
            std::cerr << "Input size mismatches: "
                      << "width: " << width << " vs " << required_shape[0]
                      << ", height: " << height << " vs " << required_shape[1]
                      << ", channels: " << channels << " vs " << required_shape[2]
                      << std::endl;
            std::abort();
        }

        // Image to vector
        std::vector<uint8_t> input_data;
        if (img.isContinuous()) {
            // array.assign(mat.datastart, mat.dataend); // <- has problems for sub-matrix like mat = big_mat.row(i)
            input_data.assign(img.data, img.data + img.total()*img.channels());
        } else {
            for (int i = 0; i < img.rows; ++i) {
                input_data.insert(input_data.end(), img.ptr<uchar>(i), img.ptr<uint8_t>(i)+img.cols*img.channels());
            }
        }

        const auto& result = coral::RunInference(input_data, interpreter.get());

        // Decode
        auto decoded_data = yoloDecoder.decode(result, interpreter.get());
        auto bboxes = YoloDecoder::nms(decoded_data);
        yoloDecoder.scale_bbox(bboxes, raw_image.size());


        // Draw boxes
        // cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
        YoloDecoder::draw_result(bboxes, raw_image, label, color);
        cv::imshow("yolo", raw_image);

        // calc time
        auto time_end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_beg).count() << "[ms]" << std::endl;

        if (cv::waitKey(1) >= 0)
            break;
    }

    return 0;
}
