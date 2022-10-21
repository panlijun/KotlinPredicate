//
// Created by yss on 2022/4/18.
//
#ifndef INSPORTAPP_POSENET_H
#define INSPORTAPP_POSENET_H

#include <iostream>
#include <MNN/ImageProcess.hpp>
#include <MNN/Interpreter.hpp>
//#define MNN_OPEN_TIME_TRACE
//#include <MNN/AutoTime.hpp>
#include <MNN/Tensor.hpp>
#include <android/bitmap.h>
#include <android/asset_manager_jni.h>
#include <jni.h>
#include <cmath>


// 关键点存储结构体
struct keypoint
{
    float x;
    float y;
    float score;
};

// 前向迭代器, 64 X 64 拉平之后计算最大值的值及索引
template<class ForwardIterator>
inline static size_t argmax(ForwardIterator first, ForwardIterator last) {
    return std::distance(first, std::max_element(first, last));
}

// 显示元素
template<class ForwardIterator>
void display(ForwardIterator first, ForwardIterator last)
{
    while(first!=last)
    {
        std::cout<<*first<<" ";
        first++;
    }
}


class PoseNet
{
public:
    // Constructor
    PoseNet(AAssetManager *mgr, bool use_gpu);
    // Destructor
    ~PoseNet();
    // detect
    std::vector<keypoint> detect(JNIEnv *env, jobject image, int mask_x, int mask_y, int mask_width, int mask_height);

private:
    // post processing
    void post_processing(MNN::Tensor* centerHost, MNN::Tensor* heatmapHost, MNN::Tensor* offsetHost, MNN::Tensor* regressHost, std::vector<keypoint>& key_points);

    // MNN 相关
    std::shared_ptr<MNN::Interpreter> interpreter;
    MNN::Session* session = nullptr;
    MNN::Tensor* input_tensor = nullptr;
    MNN::Tensor* nchw_tensor = nullptr;
    MNN::CV::ImageProcess * process;

    // 前处理相关
    std::vector<std::vector<float>> dist_y, dist_x;
    float scale;
    // 模型输入输出大小
//    int model_input_size = 256;
//    int model_output_size = 64;
    int model_input_size = 192;
    int model_output_size = 48;
    // Rect 缩放比例
    float box_ratio = 1.1f;
    float resize_scale = 4.0f;

//    int wpad;
//    int hpad;

    // 归一化相关
    const float mean_vals[3] = {127.5f, 127.5f,  127.5f};
    const float norm_vals[3] = {1/ 127.5f, 1 / 127.5f, 1 / 127.5f};

    // 绘图相关
    float confidence = 0.3;

};

#endif //INSPORTAPP_POSENET_H
