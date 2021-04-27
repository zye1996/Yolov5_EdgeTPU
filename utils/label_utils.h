//
// Created by yzy on 4/19/21.
//

#ifndef YOLO_EDGETPU_LABEL_UTILS_H
#define YOLO_EDGETPU_LABEL_UTILS_H

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

namespace coral {
// Reads labels from text file and store it in an unordered_map.
//
// This function supports the following format:
//   Each line contains id and description separated by a space.
//   Example: '0 cat'.
    std::unordered_map<int, std::string> ReadLabelFile(
            const std::string& file_path);

}  // namespace coral

#endif //YOLO_EDGETPU_LABEL_UTILS_H
