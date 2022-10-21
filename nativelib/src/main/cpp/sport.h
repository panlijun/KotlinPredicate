//
// Created by yss on 2022/5/6.
//

#ifndef INSPORTAPP_SPORT_H
#define INSPORTAPP_SPORT_H

#include <deque>
#include <numeric>
#include <string>
#include "posenet.h"

class Sport
{
public:
    // 加载模型
    void load_model(AAssetManager *mgr, bool use_gpu);
    // 卸载模型
    void unload_model();
    // 准备判断
    int person_ready(JNIEnv *env, jobject image, int mask_x, int mask_y, int mask_width, int mask_height, int sport_type);
    // 关键点检测
    std::vector<keypoint> detect_key_points(JNIEnv *env, jobject image, int mask_x, int mask_y, int mask_width, int mask_height);
//    int sport_count(JNIEnv *env, jobject image, int sport_type, int mask_x, int mask_y, int mask_width, int mask_height);
    // 准备判断，清理缓存
    void ready_cache_clear();
    // 获取关键点
//    std::vector<keypoint> get_key_points();
private:
    // 准备阶段连续判断帧率
    int ready_frame_num = 10;
    // 准备阶段缓存
    std::deque<int> person_frame_cache;

    // 关键点检测
//    std::vector<keypoint> predict_key_points;
    std::vector<keypoint> key_points;
};


// 跳绳
class SkippIng{
public:
    int count(std::vector<keypoint> key_points);
    // 清空统计信息，重新计数
    void count_clear();

private:
    // 跳绳阶段
//    int skipping_num = 0;
    bool start_mark_first = true;
    bool mark_up = false;
    bool mark_down = false;
    bool mark_count = false;
    float diff_up = -1;
    float diff_down = -1;
    std::deque<float> limit_tmp;

    float bottom_base;
    float diff_up_limit;
    float up_y;
};

// 深蹲
class Squat{
public:
    int count(std::vector<keypoint> key_points);
    void count_clear();

private:
    bool mark_up = false;
    bool mark_down = false;
};

// 坐气球
class SitBalloon
{
public:
    int count(std::vector<keypoint> key_points, int obj_x, int obj_y, int obj_width, int obj_height);
    void count_clear();
private:
    bool mark_x = false;
    bool mark_y = false;
    bool makr_back = true;
};

// 跑步
class Run
{
public:
    int count(std::vector<keypoint> key_points);
    void count_clear();
private:
    bool start_mark = true;
    bool mark_up_left_ankle = false;
    bool mark_up_right_ankle = false;

    bool mark_count_left_ankle = false;
    bool mark_count_right_ankle = false;

    float bottom_left_ankle = -1;
    float bottom_right_ankle = -1;
    float bottom_left_ankle_limit = -1;

    float diff_up_left_ankle = -1;
    float diff_down_left_ankle = -1;
    float diff_up_right_ankle = -1;
    float diff_down_right_ankle = -1;

    float diff_up_limit;
    float up_left_ankle;
    float up_right_ankle;

};

// 开合跳
class OpenCloseJump {
public:
    int count(std::vector<keypoint> key_points);
    void count_clear();
private:
    bool start_mark = true;
    bool first_key = false;
    bool second_key = false;
    float diff_up = -1;
};

// 调框游戏
class JumpBoxGame {
public:
    int count(std::vector<keypoint> key_points, int obj_x, int obj_y, int obj_width, int obj_height);
    void count_clear();
private:
    bool mark_back = true;
    int last_obj_x = -1;
    int last_obj_width = -1;
    std::deque<int> jump_num_cache;
    std::deque<int> jump_out_cache;
    int cache_num = 5;
};

// 弹跳大侦探
class BounceGame {
public:
    int count(std::vector<keypoint> key_points, int obj_x, int obj_y, int obj_width, int obj_height);
    void count_clear();
private:
    bool start_mark = true;
    bool first_key = false;
    float diff_up = -1;
    float up = 9999;

    float threshold;
    float last_position;
    float ankle;
    float base_ankle;

};

// 俯卧撑
class PushUp {
public:
    int count(std::vector<keypoint> key_points);
    void count_clear();
private:
    std::deque<int> mark_down_cache;
    std::deque<int> mark_up_cache;
    int cache_num = 3;

    bool mark_down = false;
    bool mark_up = false;
};

// 仰卧起坐
class SitUp {
public:
    int count(std::vector<keypoint> key_points);
    void count_clear();
private:
    std::deque<int> mark_sit_cache;
    std::deque<int> mark_lie_cache;
    int cache_num = 3;

    bool mark_sit = false;
    bool mark_lie = false;

};

// 平板支撑
class Plank {
public:
    int count(std::vector<keypoint> key_points);
    int cache_count(std::vector<keypoint> key_points);
    void count_clear();
private:
    std::deque<int> plank_status_cache;
    int cache_num = 4;

};

// 敏捷身手
class AgileSkillGame {
public:
    int count(std::vector<keypoint> key_points, int obj_x, int obj_y, int obj_width, int obj_height);
    void count_clear();
private:
    std::deque<int> touch_status_cache;
    int cache_num = 3;
    bool is_intersect_line(int rec[], float line[]);
    bool is_intersect_rec(int rec[], float line[]);

};

// 反应力挑战
class ReactionSpeed {
public:
    int count(std::vector<keypoint> key_points, int obj_x, int obj_y, int obj_width, int obj_height);
    void count_clear();
private:
    std::deque<int> touch_status_cache;
    int cache_num = 2;
};

// 跳一跳
class TiaoYiTiaoGame {
public:
    int count(std::vector<keypoint> key_points, int obj_x, int obj_y, int obj_width, int obj_height);
    void count_clear();
private:
    std::deque<int> mark_down_cache;
    // std::deque<int> mark_up_cache;
    int cache_num = 2;
};

// 坐位体前屈
class SittingForwardBend {
public:
    int count(std::vector<keypoint> key_points);
    void count_clear();
private:
    std::deque<float> wrist_kpx_cache;
    std::deque<float> kpx_index_cache { 0, 1, 2 };
    std::deque<int> mark_cache;
    // std::deque<int> mark_backward_cache;
    int kpx_cache_num = 3;
    int mark_cache_num = 3;
    float max_wrist_kpx = -9999;

    bool mark_forward = false;
    bool mark_backward = false;
    bool mark_max = false;
};


#endif //INSPORTAPP_SPORT_H