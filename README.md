# Yolov5_EdgeTPU

This is a C++ inference implementation of Yolov5 with EdgeTPU. Runs at 12 FPS on desktop with Coral EdgeTPU enabled.

## Demo
![](demo/demo.gif)

## Installation

1. First you will need absl library to be installed, refer to [quickstart](https://abseil.io/docs/cpp/quickstart-cmake.html)
to install absl library
   
2. From the project directory, run the following command:
    - create build directory to keep source directory clean: ```mkdir build && cd build```

    - generate makefile: ```cmake ..```

    - compile: ```make -j4```
   
3. Run the demo with command ```out/k8/Yolo_EdgeTPU``` with default model selection.



## Acknowledgement
Thanks to excellent yolov5 work from [Ultralytics](https://github.com/ultralytics/yolov5)
and model conversion code from [zldrobit](https://github.com/zldrobit/yolov5/tree/tf-android)
