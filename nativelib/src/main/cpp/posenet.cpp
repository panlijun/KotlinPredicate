//
// Created by yss on 2022/4/18.
//
#include "posenet.h"


// 构造函数
PoseNet::PoseNet(AAssetManager *mgr, bool use_gpu)
{
    // 从内存加载模型
//    AAsset *assetFile = AAssetManager_open(mgr, "model_fp16.mnn", AASSET_MODE_BUFFER);
    AAsset *assetFile = AAssetManager_open(mgr, "light_model_nchw.mnn", AASSET_MODE_BUFFER);

    size_t fileLength = AAsset_getLength(assetFile);
    char *dataBuffer2 = (char *) malloc(fileLength);

    AAsset_read(assetFile, dataBuffer2, fileLength);
    AAsset_close(assetFile);
//    MNN_PRINT("model read success");

    // 创建 interpreter
    interpreter = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromBuffer(dataBuffer2, fileLength));
//    MNN_PRINT("interpreter_createFromBuffer_sucess %s", interpreter->bizCode());

    // 创建 session
    MNN::ScheduleConfig netConfig;

    if (use_gpu)
    {
        netConfig.type = MNN_FORWARD_OPENCL;
    }
    else
    {
        netConfig.type = MNN_FORWARD_CPU;
    }

//    netConfig.type = MNN_FORWARD_OPENCL;
//    netConfig.type = MNN_FORWARD_USER_0;
//    if (use_gpu){
//        netConfig.type = MNN_FORWARD_OPENCL;
////        netConfig.type = MNN_FORWARD_VULKAN;
//        netConfig.backupType = MNN_FORWARD_CPU;
////        netConfig.mode = MNN_GPU_TUNING_NORMAL | MNN_GPU_MEMORY_IMAGE;
////        netConfig.mode = MNN_GPU_TUNING_HEAVY | MNN_GPU_MEMORY_IMAGE;
//
//        MNN::BackendConfig backendConfig;
////        backendConfig.memory = MNN::BackendConfig::Memory_Normal;  // 内存
//        backendConfig.power = MNN::BackendConfig::Power_Normal;  // 功耗
//        backendConfig.precision = MNN::BackendConfig::Precision_Low;
//        netConfig.backendConfig = &backendConfig;
//    }
//    else
//    {
//        netConfig.type = MNN_FORWARD_CPU;
//        netConfig.backupType = MNN_FORWARD_CPU;
//        netConfig.numThread = 2;
//        MNN::BackendConfig backendConfig;
////        backendConfig.memory = MNN::BackendConfig::Memory_Normal;  // 内存
////        backendConfig.power = MNN::BackendConfig::Power_Normal;  // 功耗
////        backendConfig.precision = MNN::BackendConfig::Precision_Low;  // 精度
//        netConfig.backendConfig = &backendConfig;
//    }

    // 缓存，模型加载较慢问题
    interpreter->setCacheFile(".tempcache");

    session = interpreter->createSession(netConfig);

    // 输入数据初始化
    input_tensor = interpreter->getSessionInput(session, nullptr);
    nchw_tensor = new MNN::Tensor(input_tensor, MNN::Tensor::CAFFE);

    //前处理初始化
    MNN::CV::ImageProcess::Config preProcessConfig;
    ::memcpy(preProcessConfig.mean, mean_vals, sizeof(mean_vals));
    ::memcpy(preProcessConfig.normal, norm_vals, sizeof(norm_vals));

    preProcessConfig.sourceFormat = MNN::CV::RGBA;
//    preProcessConfig.destFormat   = MNN::CV::BGR;
    preProcessConfig.destFormat   = MNN::CV::RGB;
    preProcessConfig.filterType   = MNN::CV::BILINEAR;
    preProcessConfig.wrap         = MNN::CV::ZERO;

    process = MNN::CV::ImageProcess::create(preProcessConfig);

    // 构建 64 X 64 索引矩阵，后面使用
    for (int i = 0; i < model_output_size; i++)
    {
        std::vector<float> x, y;
        for (int j = 0; j < model_output_size; j++)
        {
            x.push_back(j);
            y.push_back(i);
        }
        dist_y.push_back(y);
        dist_x.push_back(x);
    }
}

PoseNet::~PoseNet() {
    interpreter->releaseModel();
    interpreter->releaseSession(session);
    delete input_tensor;
    delete nchw_tensor;
    delete process;
}


std::vector<keypoint> PoseNet::detect(JNIEnv *env, jobject image, int mask_x, int mask_y, int mask_width, int mask_height)
{
    // 读取数据
    void* rgba;
    AndroidBitmap_lockPixels(env, image, &rgba);

    AndroidBitmapInfo img_size;
    AndroidBitmap_getInfo(env, image, &img_size);
    AndroidBitmap_unlockPixels(env, image);

    //前处理
    int roi_w = img_size.width;
    int roi_h = img_size.height;

    if (roi_w > roi_h)
    {
        scale = (float)((roi_w - 1) / (model_input_size - 1));
        roi_w = model_input_size;
        roi_h = roi_h / scale;
    }
    else {
        scale = (float) ((roi_h - 1) / (model_input_size - 1));

        roi_h = model_input_size;
        roi_w = roi_w / scale;
    }

//    int wpad = model_input_size - roi_w;
//    int hpad = model_input_size - roi_h;

    MNN::CV::Matrix trans;
    trans.postScale(scale, scale);
    process->setMatrix(trans);

    //复制内存数据到 input_tensor
    input_tensor->copyFromHostTensor(nchw_tensor);

    const auto rgbaPtr = reinterpret_cast<uint8_t*>(rgba);
    // TODO
    process->convert(rgbaPtr, img_size.width, img_size.height, 0, input_tensor);
//    process->convert(rgbaPtr, img_size.width, img_size.height, img_size.stride, input_tensor);

    // 执行推理
//    MNN_PRINT("AUTOTIME");
    interpreter->runSession(session);

    auto center  = interpreter->getSessionOutput(session, "center");
    auto heatmap = interpreter->getSessionOutput(session, "heatmap");
    auto offset  = interpreter->getSessionOutput(session, "offset");
    auto regress = interpreter->getSessionOutput(session, "regress");

    MNN::Tensor centerHost(center, MNN::Tensor::CAFFE);
    MNN::Tensor heatmapHost(heatmap, MNN::Tensor::CAFFE);
    MNN::Tensor offsetHost(offset, MNN::Tensor::CAFFE);
    MNN::Tensor regressHost(regress, MNN::Tensor::CAFFE);

    center->copyToHostTensor(&centerHost);
    heatmap->copyToHostTensor(&heatmapHost);
    offset->copyToHostTensor(&offsetHost);
    regress->copyToHostTensor(&regressHost);

    // 后处理
    std::vector<keypoint> key_points;
    post_processing(&centerHost, &heatmapHost, &offsetHost, &regressHost, key_points);

    return key_points;

}

// MoveNet 后处理
void PoseNet::post_processing(MNN::Tensor* centerHost, MNN::Tensor* heatmapHost, MNN::Tensor* offsetHost, MNN::Tensor* regressHost, std::vector<keypoint>& key_points)
{
    float* center_data = centerHost->host<float>();
    float* heatmap_data = heatmapHost->host<float>();
    float* offset_data = offsetHost->host<float>();
    float* regress_data = regressHost->host<float>();

    key_points.clear();

    int top_index = 0;
    float top_score = 0;

    top_index = int(argmax(center_data, center_data+centerHost->height()));
    top_score = *std::max_element(center_data, center_data + centerHost->height());

    // 计算中心点坐标
    int ct_y = (top_index / model_output_size);
    int ct_x = top_index - ct_y * model_output_size;

    // 计算回归值
    std::vector<float> y_regress(17), x_regress(17);

    // 添加偏移量数据，得到准确的坐标值
    for (int i = 0; i < 17; i++)
    {
        // 根据内存排列得到每个关键点坐标的偏移量
        y_regress[i] = regress_data[i*model_output_size*model_output_size + ct_y*model_output_size + ct_x] + (float)ct_y;
        x_regress[i] = regress_data[(i+17)*model_output_size*model_output_size + ct_y*model_output_size + ct_x] + (float)ct_x;

//        y_regress[i] = regress_data[ct_y*model_output_size*17*2 + ct_x*17*2 + i] + (float)ct_y;
//        x_regress[i] = regress_data[ct_y*model_output_size*17*2 + ct_x*17*2 + i + 17] + (float)ct_x;
    }

    float* scores_data = new float[model_output_size*model_output_size*17*sizeof(float)];
    for (int i = 0; i < model_output_size; i++)
    {
        for (int j = 0; j < model_output_size; j++)
        {
            std::vector<float> score;
            for (int c = 0; c < 17; c++)
            {
                float y = (dist_y[i][j] - y_regress[c]) * (dist_y[i][j] - y_regress[c]);
                float x = (dist_x[i][j] - x_regress[c]) * (dist_x[i][j] - x_regress[c]);
                float dist_weight = std::sqrt(y + x) + 1.8;
                scores_data[c*model_output_size*model_output_size +i* model_output_size +j] =
                        heatmap_data[c * model_output_size * model_output_size + i * model_output_size + j] / dist_weight;
//                        heatmap_data[i * model_output_size * 17 + j * 17 + c] / dist_weight;
            }
        }
    }

    // 计算每个关键点最大得分的坐标值
    std::vector<int> kpts_ys, kpts_xs;
    for (int i = 0; i < 17; i++)
    {
        top_index = 0;
        top_score = 0;
        top_index = int(argmax(scores_data + model_output_size * model_output_size *i, scores_data + model_output_size * model_output_size *(i+1)));
        // top_score = *std::max_element(scores_data + model_output_size * model_output_size * i, scores_data + model_output_size * model_output_size * (i + 1));

        int top_y = (top_index / model_output_size);
        int top_x = top_index - top_y * model_output_size;
        kpts_ys.push_back(top_y);
        kpts_xs.push_back(top_x);
    }

    // 添加偏移量数据，得到准确的坐标值
    for (int i = 0; i < 17; i++)
    {
        // 根据内存排列得到每个关键点坐标的偏移量
//        float kpt_offset_x = offset_data[kpts_ys[i]*model_output_size*17*2 + kpts_xs[i]*17*2 + i*2];
//        float kpt_offset_y = offset_data[kpts_ys[i]*model_output_size*17*2 + kpts_xs[i]*17*2 + i*2 + 1];
        float kpt_offset_x = offset_data[i*2*model_output_size*model_output_size + kpts_ys[i]*model_output_size + kpts_xs[i]];
        float kpt_offset_y = offset_data[(i*2+1)*model_output_size*model_output_size + kpts_ys[i]*model_output_size + kpts_xs[i]];

        // 恢复到 256 X 256
        float x = (kpts_xs[i] + kpt_offset_y) * resize_scale;
        float y = (kpts_ys[i] + kpt_offset_x) * resize_scale;

        keypoint kpt;
//        kpt.x = (x - (wpad / 2)) / scale + new_box.x;
//        kpt.y = (y - (hpad / 2)) / scale + new_box.y;
        kpt.x = (x) * scale;
        kpt.y = (y) * scale;
//        kpt.score = heatmap_data[kpts_ys[i] * model_output_size * 17 + kpts_xs[i] * 17 + i];
        kpt.score = heatmap_data[i * model_output_size * model_output_size + kpts_ys[i] * model_output_size + kpts_xs[i]];
        key_points.push_back(kpt);
    }
    delete[] scores_data;

}


