#include "class_timer.hpp"
#include "class_detector.h"

#include <memory>
#include <thread>

int main() {
    Config config_v3;
    config_v3.net_type = YOLOV3;
    config_v3.file_model_cfg = "../configs/yolov3.cfg";
    config_v3.file_model_weights = "../configs/yolov3.weights";
    config_v3.calibration_image_list_file_txt = "../configs/calibration_images.txt";
    config_v3.inference_precision = FP32;
    config_v3.detect_thresh = 0.5;

    Config config_v4;
    config_v4.net_type = YOLOV4;
    config_v4.file_model_cfg = "../configs/yolov4.cfg";
    config_v4.file_model_weights = "../configs/yolov4.weights";
    config_v4.calibration_image_list_file_txt = "../configs/calibration_images.txt";
    config_v4.inference_precision = FP32;
    config_v4.detect_thresh = 0.5;

    Config config_v5;
    config_v5.net_type = YOLOV5;
    config_v5.detect_thresh = 0.5;
    config_v5.file_model_cfg = "../configs/yolov5-6.0/yolov5l.cfg";
    config_v5.file_model_weights = "../configs/yolov5-6.0/yolov5l.weights";
    config_v5.calibration_image_list_file_txt = "../configs/calibration_images.txt";
    config_v5.inference_precision = FP32;

    std::unique_ptr<Detector> detector(new Detector());
    detector->init(config_v5);
    cv::Mat image0 = cv::imread("../configs/dog.jpg", cv::IMREAD_UNCHANGED);
    cv::Mat image1 = cv::imread("../configs/person.jpg", cv::IMREAD_UNCHANGED);
    std::vector<BatchResult> batch_res;
    Timer timer;
    for (;;) {
        // prepare batch data
        std::vector<cv::Mat> batch_img;
        cv::Mat temp0 = image0.clone();
        cv::Mat temp1 = image1.clone();
        batch_img.push_back(temp0);
        // batch_img.push_back(temp1);

        // detect
        timer.reset();
        detector->detect(batch_img, batch_res);
        timer.out("detect");

        // disp
        for (int i = 0; i < batch_img.size(); ++i) {
            for (const auto& r : batch_res[i]) {
                std::cout << "batch " << i << " id:" << r.id << " prob:" << r.prob << " rect:" << r.rect << std::endl;
                cv::rectangle(batch_img[i], r.rect, cv::Scalar(255, 0, 0), 2);
                std::stringstream stream;
                stream << std::fixed << std::setprecision(2) << "id:" << r.id << "  score:" << r.prob;
                cv::putText(batch_img[i], stream.str(), cv::Point(r.rect.x, r.rect.y - 5), 0, 0.5,
                            cv::Scalar(0, 0, 255), 2);
            }
            cv::namedWindow("image" + std::to_string(i), cv::WINDOW_NORMAL);
            cv::imshow("image" + std::to_string(i), batch_img[i]);
        }
        cv::waitKey(10);
    }
}
