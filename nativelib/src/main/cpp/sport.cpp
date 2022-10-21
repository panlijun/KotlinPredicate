//
// Created by yss on 2022/5/6.
//
#include "sport.h"

// 计算角度
/*
get angle ACB, point B is the center point
A(x1,y1)；B(x2,y2)；C(x3,y3)
*/
float get_angle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    float theta = atan2((x1 - x3), (y1 - y3)) - atan2((x2 - x3), (y2 - y3));
    if (theta > M_PI)
        theta -= 2 * M_PI;
    if (theta < -M_PI)
        theta += 2 * M_PI;

    theta = abs(theta * 180.0 / M_PI);
    return theta;
}

float LeastSquares(std::deque<float>& X, std::deque<float>& Y)
{
    if (X.empty() || Y.empty())
        return false;

    int vec_size = int(X.size());
    float sum1 = 0, sum2 = 0;
    float x_avg = std::accumulate(X.begin(), X.end(), 0.0) / vec_size;
    float y_avg = std::accumulate(Y.begin(), Y.end(), 0.0) / vec_size;

    for (int i = 0; i < vec_size; ++i) {
        sum1 += (X.at(i) * Y.at(i) - x_avg * y_avg);
        sum2 += (X.at(i) * X.at(i) - x_avg * x_avg);
    }

    float line_slope = sum1 / sum2;

    return line_slope;
}

static PoseNet* g_posenet = nullptr;


void Sport::load_model(AAssetManager *mgr, bool use_gpu)
{
    if (g_posenet == nullptr)
    {
        g_posenet = new PoseNet(mgr, use_gpu);
    }
}

void Sport::unload_model()
{
    if (g_posenet != nullptr)
    {
        delete g_posenet;
        g_posenet = nullptr;
    }
}

void Sport::ready_cache_clear()
{
    person_frame_cache.clear();
}


int Sport::person_ready(JNIEnv *env, jobject image, int mask_x, int mask_y, int mask_width, int mask_height, int sport_type=-1)
{
    // 定义是否准备好, 0:未准备好，1:已准备好，其他预留
    int ready_status = 0;

    // 检测关键点
    key_points = g_posenet->detect(env, image, mask_x, mask_y, mask_width, mask_height);

    // 第一次为空的情况
//    if (predict_key_points.empty())
//    {
//        predict_key_points = key_points;
//    }

    // 定长缓存队列
    if (person_frame_cache.size() > ready_frame_num)
    {
        person_frame_cache.pop_front();
    }

    int confidence_num = 0;
    int head_pos_num = 0;
    // 平滑处理防抖和满足条件筛选
    int br_x = mask_x + mask_width;
    int br_y = mask_y + mask_height;

    // 趣味运动，只检测半身情况
    if (sport_type == 11)
    {
        // 半身检测
        int include_num = 0;
        int key_point_index[6] = {1, 2, 5, 6, 7, 8};
        for (int i=0; i < 6; i++)
        {
            auto temp_kp = key_points[key_point_index[i]];

            // 根据包含和置信度筛选
            if (temp_kp.x >= mask_x && temp_kp.x <= br_x &&
                temp_kp.y >= mask_y && temp_kp.y <= br_y &&
                temp_kp.score > 0.3)
            {
                include_num += 1;
            }
        }

        if (include_num == 6)
        {
            return 1;
        } else
        {
            return 0;
        }

    }

    for (int i = 0; i < 5; i++)
    {
        // 根据包含和置信度筛选
        if (key_points[i].x >= (float)mask_x && key_points[i].x <= (float)br_x &&
            key_points[i].y >= (float)mask_y && key_points[i].y <= (float)br_y &&
            key_points[i].score > 0.3f)
        {
            head_pos_num = 1;
            break;
        }
    }

    for (int i = 5; i < 17; i++)
    {
        // 根据包含和置信度筛选
        if (key_points[i].x >= (float)mask_x && key_points[i].x <= (float)br_x &&
                key_points[i].y >= (float)mask_y && key_points[i].y <= (float)br_y &&
                key_points[i].score > 0.3f)
        {
            confidence_num += 1;
        }
    }

    confidence_num = confidence_num + head_pos_num;

    // 不同运动特有的判断
    int sport_pose_ready = 1;

    if (sport_type == 7)
    {
        sport_pose_ready = 0;
        // 左脚踝膝盖之间的距离阈值
        float refer_distance = pow((pow((key_points[13].x-key_points[15].x), 2) +
                                    pow((key_points[13].y-key_points[15].y), 2)), 0.5);

        // 身体处于躺着状态，手腕，手肘，肩关节在x轴上小于阈值
        if (key_points[12].x < key_points[13].x && key_points[12].x < key_points[14].x &&
            key_points[14].x < key_points[15].x && key_points[14].x < key_points[16].x)
        {
            // 两个脚踝的距离小于一定阈值
            if (abs(key_points[15].x-key_points[16].x) < 0.3f*refer_distance)
            {
                sport_pose_ready = 1;
            }
        }
    }

    if (sport_type == 8)
    {
        sport_pose_ready = 0;

        // 身体处于躺着状态，手腕，手肘，肩关节在x轴上小于阈值
        if (key_points[12].x < key_points[13].x && key_points[12].x < key_points[14].x &&
            key_points[14].x < key_points[15].x && key_points[14].x < key_points[16].x)
        {
            // 膝盖弯曲小于 95 度
            float knee_theta = get_angle(key_points[12].x, key_points[12].y,
                                         key_points[16].x, key_points[16].y, key_points[14].x, key_points[14].y);

            // 计算腰部夹角
            float waist_theta = get_angle(key_points[6].x, key_points[6].y,
                                          key_points[16].x, key_points[16].y, key_points[12].x, key_points[12].y);

            if (knee_theta < 95 && waist_theta > 155)
            {
                sport_pose_ready = 1;
            }
        }

    }

    if (sport_type == 9)
    {
        sport_pose_ready = 0;
        float left_wrist_x = key_points[9].x;
        float left_wrist_y = key_points[9].y;
        float left_shoulder_x = key_points[5].x;
        float left_shoulder_y = key_points[5].y;
        float left_hip_x = key_points[11].x;
        float left_hip_y = key_points[11].y;
        float left_knee_x = key_points[13].x;
        float left_knee_y = key_points[13].y;
        float left_ankle_x = key_points[15].x;
        float left_ankle_y = key_points[15].y;
        float left_elbow_x = key_points[7].x;
        float left_elbow_y = key_points[7].y;

        if (left_wrist_x < left_shoulder_x && left_shoulder_x < left_hip_x &&
            left_hip_x < left_knee_x && left_knee_x < left_ankle_x &&
            left_hip_y < left_ankle_y && left_shoulder_y < left_knee_y)
        {
            float elbow_theta = get_angle(left_wrist_x, left_wrist_y,
                                          left_shoulder_x, left_shoulder_y, left_elbow_x, left_elbow_y);

            float shoulder_theta = get_angle(left_elbow_x, left_elbow_y,
                                             left_hip_x, left_hip_y, left_shoulder_x, left_shoulder_y);

            if (elbow_theta > 70 && elbow_theta < 100 && shoulder_theta > 65 && shoulder_theta < 100)
            {
                sport_pose_ready = 1;
            }
        }
    }

    if (confidence_num == 13 && sport_pose_ready == 1)
    {
        person_frame_cache.push_back(1);
    }
    else
    {
        person_frame_cache.push_back(0);
    }

    // 统计 1 出现次数
    int ready_count_num = std::count(person_frame_cache.begin(), person_frame_cache.end(), 1);
    if (ready_count_num > (ready_frame_num - 4) && person_frame_cache[ready_frame_num-1] == 1)
    {
        ready_status = 1;
    }

    // 把新检测点更新，用于下次计算
//    predict_key_points = key_points;

    return ready_status;

}

std::vector<keypoint> Sport::detect_key_points(JNIEnv *env, jobject image, int mask_x, int mask_y, int mask_width, int mask_height)
{
    // 检测关键点
    key_points = g_posenet->detect(env, image, mask_x, mask_y, mask_width, mask_height);
    // 平滑处理后的关键点
//    std::vector<keypoint> smooth_key_points;
    // 第一次为空的情况
//    if (predict_key_points.empty())
//    {
//        predict_key_points = key_points;
//    }

    // 平滑处理防抖
//    for (int i = 0; i < 17; i++)
//    {
//        predict_key_points[i].x = 0.7f*key_points[i].x + 0.3f*predict_key_points[i].x;
//        predict_key_points[i].y = 0.7f*key_points[i].y + 0.3f*predict_key_points[i].y;
//        predict_key_points[i].score = 0.7f*key_points[i].score + 0.3f*predict_key_points[i].score;
//    }

    // 把新检测点更新，用于下次计算
//    smooth_key_points = predict_key_points;
//    predict_key_points = key_points;
//    smooth_key_points = key_points;

    return key_points;
}


// 跳绳
int SkippIng::count(std::vector<keypoint> key_points)
{
    // skipping_num 为 0 没计数，为 1 计数， 为 -1 纠正计数。
    int skipping_num = 0;

    // 计数判断（规则逻辑，同步亚楠修改）
    float nose_y = key_points[0].y;
    float left_eye_y = key_points[1].y;
    float left_shoulder_x = key_points[5].x;
    float left_shoulder_y = key_points[5].y;
    float right_shoulder_x = key_points[6].x;
    float right_shoulder_y = key_points[6].y;
    float left_ankle_y = key_points[15].y;
    float right_ankle_y = key_points[16].y;

    if (key_points[0].score < 0.3 || key_points[1].score < 0.3 || key_points[5].score < 0.3 ||
        key_points[6].score < 0.3 || key_points[15].score < 0.3 || key_points[16].score < 0.3)
    {
        return skipping_num;
    }

    // 手腕超过肩膀不算
    if (key_points[9].y < key_points[5].y || key_points[9].y < key_points[6].y ||
        key_points[10].y < key_points[5].y || key_points[10].y < key_points[6].y)
    {
        start_mark_first = true;
        mark_up = false;
        mark_down = false;
        mark_count = false;
        diff_up = -1;
        diff_down = -1;

        return skipping_num;
    }
    // 左右摆动判断
    if (key_points[10].x > key_points[8].x || key_points[9].x < key_points[7].x)
    {
        start_mark_first = true;
        mark_up = false;
        mark_down = false;
        mark_count = false;
        diff_up = -1;
        diff_down = -1;

        return skipping_num;
    }
    // 手表抬高一定高度也不算
    if (abs(key_points[9].y - key_points[5].y) < 0.3f*abs(left_shoulder_x - right_shoulder_x) ||
        abs(key_points[10].y - key_points[6].y) < 0.3f*abs(left_shoulder_x - right_shoulder_x))
    {

        start_mark_first = true;
        mark_up = false;
        mark_down = false;
        mark_count = false;
        diff_up = -1;
        diff_down = -1;

        return skipping_num;
    }

    float base_y = (left_shoulder_y + right_shoulder_y) * 0.5f;

    if (start_mark_first)
    {
        bottom_base = base_y;
        diff_up_limit = abs(nose_y-left_eye_y)*1.5f;
        start_mark_first = false;
    }

    if (!mark_up)
    {
        if (abs(bottom_base-base_y) >= diff_up)
        {
            up_y = base_y;
            diff_up = abs(bottom_base-base_y);
        } else
        {
            mark_up = true;
        }
    } else
    {
        if (abs(base_y-up_y) >= diff_down)
        {
            diff_down = abs(base_y - up_y);
            bottom_base = base_y;
        } else
        {
            mark_count = true;
        }
    }

    if (mark_count)
    {
        if (diff_up > diff_up_limit && diff_down > diff_up_limit)
        {
            skipping_num = 1;
        }
        // 更新 diff_up_limit
//            limit_tmp.push_back(diff_up);
//            if (limit_tmp.size() > 5)
//            {
//                limit_tmp.pop_front();
//            }
//            float diff_refer = abs(nose_y - left_eye_y);
//            float tmp_mean = (std::accumulate(limit_tmp.begin(), limit_tmp.end(), 0.0)) / limit_tmp.size();
//            diff_up_limit = std::max(tmp_mean*0.5f, diff_refer);

        mark_count = false;
        mark_up = false;
        diff_up = -1;
        diff_down = -1;

//        if ((left_ankle_y - right_ankle_y) > diff_up_limit)
//        {
//            skipping_num = -1;
//        }
    }

    return skipping_num;
}

void SkippIng::count_clear()
{
//    skipping_num = 0;
//    predict_key_points.clear();

    start_mark_first = true;
    mark_up = false;
    mark_down = false;
    mark_count = false;
    diff_up = -1;
    diff_down = -1;
}

// 深蹲
int Squat::count(std::vector<keypoint> key_points)
{
    int squat_num = 0;

    // 阈值不满足要求直接返回
    if (key_points[5].score < 0.3 || key_points[6].score < 0.3 || key_points[11].score < 0.3 ||
        key_points[12].score < 0.3 || key_points[15].score < 0.3 || key_points[16].score < 0.3)
    {
        return squat_num;
    }

    float left_shoulder_x = key_points[5].x;
    float right_shoulder_x = key_points[6].x;
    float left_ankle_y = key_points[15].y;
    float right_ankle_y = key_points[16].y;
    float left_hip_y = key_points[11].y;
    float right_hip_y = key_points[12].y;

    float shoulder_distance = abs(right_shoulder_x - left_shoulder_x);
    float hip_ankle_distance = abs(
            (left_hip_y+right_hip_y)*0.5f - (left_ankle_y+right_ankle_y)*0.5f);

    // 左右脚踝y坐标超过肩宽的1/3，说明有一只脚抬起来不计数
    float gap_ankle = abs(left_ankle_y - right_ankle_y);
    if (gap_ankle > shoulder_distance*0.33)
    {
        return squat_num;
    }


    if (hip_ankle_distance < 1.8*shoulder_distance)
    {
        mark_down = true;
    }

    if (hip_ankle_distance > 2*shoulder_distance)
    {
        mark_up = true;
        if (mark_down)
        {
            squat_num = 1;
            mark_down = false;
        } else
        {
            mark_up = false;
        }
    }

    return squat_num;


}

void Squat::count_clear()
{
    mark_up = false;
    mark_down = false;
}


// 坐气球项目
int SitBalloon::count(std::vector<keypoint> key_points, int obj_x, int obj_y, int obj_width, int obj_height)
{
    int sit_ballon_num = 0;

    // 腰部位置
    float left_hip_y = key_points[11].y;
    float right_hip_y = key_points[12].y;
    float left_hip_x = key_points[11].x;
    float right_hip_x = key_points[12].x;

    // 鼻子
//    float nose_x = key_points[0].x;
//    float nose_y = key_points[0].y;

    // 两个耳位置
    float left_ear_x = key_points[3].x;
//    float left_ear_y = key_points[3].y;
    float right_ear_x = key_points[4].x;
//    float right_ear_y = key_points[4].y;

    // 肩膀位置
    float left_shoulder_x = key_points[5].x;
//    float left_shoulder_y = key_points[5].y;
    float right_shoulder_x = key_points[6].x;
//    float right_shoulder_y = key_points[6].y;

    // 膝盖位置
    float left_knee_y = key_points[13].y;
    float right_knee_y = key_points[14].y;

    float left_knee_x = key_points[13].x;
    float right_knee_x = key_points[14].x;


    // 脚踝位置
    float left_ankle_y = key_points[15].y;
    float right_ankle_y = key_points[16].y;

    float left_ankle_x = key_points[15].x;
    float right_ankle_x = key_points[16].x;


    // 置信度控制

    // 根据关键点的评分确定 x 中心点和参考基准点
    // 人站立中心点
    float center_x;
    if (key_points[13].score > 0.3 && key_points[14].score > 0.3)
    {
        center_x = (left_knee_x + right_knee_x) * 0.5f;
    }
    else if (key_points[15].score > 0.3 && key_points[16].score > 0.3)
    {
        center_x = (left_ankle_x + right_ankle_x) * 0.5f;
    }
    else if(key_points[11].score > 0.3 && key_points[12].score > 0.3)
    {
        center_x = (left_hip_x + right_hip_x) * 0.5f;
    } else
    {
        return sit_ballon_num;
    }

    // 参考距离
    float ref_base_size;
    if (key_points[11].score > 0.3 && key_points[12].score > 0.3)
    {
        ref_base_size = abs(left_hip_x - right_hip_x) * 0.96f;
    }
    else if (key_points[5].score > 0.3 && key_points[6].score > 0.3)
    {
        ref_base_size = abs(left_shoulder_x - right_shoulder_x) * 0.7f;
    }
    else if (key_points[3].score > 0.3 && key_points[4].score > 0.3)
    {
        ref_base_size = abs(left_ear_x - right_ear_x) * 1.4f;
    } else
    {
        return sit_ballon_num;
    }

    // x 轴位置判断
    if (makr_back)
    {
        // x 轴匹配
        if ((center_x > (obj_x + obj_width*0.1f)) && (center_x < (obj_x + obj_width*0.9f)))
        {
            mark_x = true;
        } else
        {
            mark_x = false;
        }

        // 蹲下匹配
        if ((key_points[11].score > 0.3) && (key_points[13].score > 0.3) && (key_points[15].score > 0.3) &&
            ((left_knee_y - left_hip_y) < ref_base_size*0.7f) &&
            (left_knee_y > left_hip_y) && (left_ankle_y > left_knee_y) &&
            ((left_ankle_y - left_knee_y) > (left_knee_y - left_hip_y)))
        {
            if ((key_points[16].score > 0.3) && (abs(left_ankle_y-right_ankle_y) < ref_base_size*0.6f))
            {
                mark_y = true;
            } else
            {
                mark_y = false;
            }
        }
        else if (key_points[12].score > 0.3 && key_points[14].score > 0.3 && key_points[16].score > 0.3 &&
                 ((right_knee_y - right_hip_y) < ref_base_size*0.7f) &&
                 (right_knee_y > right_hip_y) && (right_ankle_y > right_knee_y) &&
                 ((right_ankle_y - right_knee_y) > (right_knee_y - right_hip_y)))
        {
            if ((key_points[15].score > 0.3) && (abs(left_ankle_y-right_ankle_y) < ref_base_size*0.6f))
            {
                mark_y = true;
            } else
            {
                mark_y = false;
            }
        }
        else
        {
            mark_y = false;
        }

        if (mark_x && mark_y)
        {
            sit_ballon_num = 1;
            mark_x = false;
            mark_y = false;
            makr_back = false;
        }

    } else
    {
        // 站起来判断
        if ((key_points[11].score > 0.3) && (key_points[13].score > 0.3) && (key_points[15].score > 0.3) &&
            ((left_knee_y - left_hip_y) > 0.8f*ref_base_size) &&
            (left_knee_y > left_hip_y) && (left_ankle_y > left_knee_y) &&
            ((left_ankle_y - left_knee_y) > (left_knee_y - left_hip_y)))
        {
            makr_back = true;
        }
        else if (key_points[12].score > 0.3 && key_points[14].score > 0.3 && key_points[16].score > 0.3 &&
                 ((right_knee_y - right_hip_y) > 0.8f*ref_base_size) &&
                 (right_knee_y > right_hip_y) && (right_ankle_y > right_knee_y) &&
                 ((right_ankle_y - right_knee_y) > abs(right_knee_y - right_hip_y)))
        {
            makr_back = true;
        }
    }

    return sit_ballon_num;

}

void SitBalloon::count_clear()
{
    mark_x = false;
    mark_y = false;
    makr_back = true;
}


// 跑步
int Run::count(std::vector<keypoint> key_points)
{
    int run_num = 0;

    float left_ankle_y = key_points[15].y;
    float right_ankle_y = key_points[16].y;
    float left_knee_y = key_points[13].y;

    // 阈值不满足要求直接返回
    if (key_points[13].score < 0.3 || key_points[14].score < 0.3 || key_points[15].score < 0.3 ||
        key_points[16].score < 0.3)
    {
        return run_num;
    }

    if (start_mark)
    {
        bottom_left_ankle = left_ankle_y;
        bottom_right_ankle = right_ankle_y;
        start_mark = false;
        diff_up_limit = abs(left_knee_y-left_ankle_y);
    }
    if (!mark_up_left_ankle)
    {
        if (abs(left_ankle_y-bottom_left_ankle) > diff_up_left_ankle)
        {
            up_left_ankle = left_ankle_y;
            diff_up_left_ankle = abs(left_ankle_y-bottom_left_ankle);
        } else
        {
            mark_up_left_ankle = true;
        }
    } else
    {
        if (abs(left_ankle_y-up_left_ankle) > diff_down_left_ankle)
        {
            bottom_left_ankle = left_ankle_y;
            diff_down_left_ankle = abs(left_ankle_y-up_left_ankle);
        } else
        {
            mark_count_left_ankle = true;
        }
    }

    if (!mark_up_right_ankle)
    {
        if (abs(right_ankle_y-bottom_right_ankle) > diff_up_right_ankle)
        {
            up_right_ankle = right_ankle_y;
            diff_up_right_ankle = abs(right_ankle_y-bottom_right_ankle);
        } else
        {
            mark_up_right_ankle = true;
        }
    } else
    {
        if (abs(right_ankle_y-up_right_ankle) > diff_down_right_ankle)
        {
            bottom_right_ankle = right_ankle_y;
            diff_down_right_ankle = abs(right_ankle_y-up_right_ankle);
        } else
        {
            mark_count_right_ankle = true;
        }
    }

    if (mark_count_left_ankle)
    {
        if (diff_up_left_ankle > diff_up_limit * 0.1f)
        {
            run_num = 1;
        }
        mark_count_left_ankle = false;
        mark_up_left_ankle = false;
        diff_up_left_ankle = -1;
        diff_down_left_ankle = -1;
    }

    if (mark_count_right_ankle)
    {
        if (diff_up_right_ankle > diff_up_limit * 0.1f)
        {
            run_num = 1;
        }
        mark_count_right_ankle = false;
        mark_up_right_ankle = false;
        diff_up_right_ankle = -1;
        diff_down_right_ankle = -1;
    }

    return run_num;

}

void Run::count_clear()
{
    start_mark = true;
    mark_up_left_ankle = false;
    mark_up_right_ankle = false;

    mark_count_left_ankle = false;
    mark_count_right_ankle = false;

    bottom_left_ankle = -1;
    bottom_right_ankle = -1;
    bottom_left_ankle_limit = -1;

    diff_up_left_ankle = -1;
    diff_down_left_ankle = -1;
    diff_up_right_ankle = -1;
    diff_down_right_ankle = -1;
}

// 开合跳
int OpenCloseJump::count(std::vector<keypoint> key_points)
{
    int jump_num = 0;

    float left_shoulder_x = key_points[5].x;
    float right_shoulder_x = key_points[6].x;
    float left_ankle_x = key_points[15].x;
    float right_ankle_x = key_points[16].x;

    float left_wrist_y = key_points[9].y;
    float right_wrist_y = key_points[10].y;
//    float left_wrist_x = key_points[9].x;
//    float right_wrist_x = key_points[10].x;
    float nose_y = key_points[0].y;

    // 阈值过滤
    if (key_points[5].score < 0.3 || key_points[6].score < 0.3 || key_points[15].score < 0.3 ||
        key_points[16].score < 0.3 || key_points[9].score < 0.3 || key_points[10].score < 0.3 ||
        key_points[0].score < 0.3)
    {
        return jump_num;
    }

    if (!first_key)
    {
        if (abs(left_ankle_x-right_ankle_x) < abs(left_shoulder_x-right_shoulder_x))
        {
            first_key = true;
        }
    } else if (!second_key)
    {
        if (abs(left_ankle_x-right_ankle_x) >= diff_up)
        {
            diff_up = abs(left_ankle_x-right_ankle_x);
        } else if (diff_up > abs(left_shoulder_x-right_shoulder_x))
        {
            if ((left_wrist_y<nose_y) && (right_wrist_y<nose_y))
            {
                second_key = true;
            }
        }
    } else if (second_key)
    {
        if (abs(left_ankle_x-right_ankle_x) < abs(left_shoulder_x-right_shoulder_x))
        {
            first_key = false;
            second_key = false;
            jump_num = 1;
            diff_up = -1;
        }
    }

    return jump_num;

}

void OpenCloseJump::count_clear()
{
    start_mark = true;
    first_key = false;
    second_key = false;
    diff_up = -1;
}

// 跳箱子游戏
int JumpBoxGame::count(std::vector<keypoint> key_points, int obj_x, int obj_y, int obj_width, int obj_height)
{
    int jump_box_num = 0;

//    float left_ankle_y = key_points[15].y;
//    float right_ankle_y = key_points[16].y;

    float left_ankle_x = key_points[15].x;
    float right_ankle_x = key_points[16].x;

    // 缓存机制，超过限制 frame num 删除
    if (jump_num_cache.size() > cache_num)
    {
        jump_num_cache.pop_front();
    }

    if (jump_out_cache.size() > cache_num)
    {
        jump_out_cache.pop_front();
    }


    if (key_points[15].score < 0.3 || key_points[16].score < 0.3)
    {
        return jump_box_num;
    }

    if (mark_back)
    {
        if ((left_ankle_x > obj_x) && (left_ankle_x < (obj_x + obj_width)) &&
            (right_ankle_x > obj_x) && (right_ankle_x < (obj_x + obj_width)))
        {
            jump_num_cache.push_back(1);
        } else
        {
            jump_num_cache.push_back(0);
        }
        int jump_count_num = int(std::count(jump_num_cache.begin(), jump_num_cache.end(), 1));
        if (jump_count_num > (cache_num-1) && jump_num_cache[cache_num-1] == 1)
        {
            jump_box_num = 1;
            mark_back = false;
            last_obj_x = obj_x;
            last_obj_width = obj_width;
            // 清空缓存
            jump_num_cache.clear();
        }

    } else
    {
        jump_box_num = 0;
    }

    if (last_obj_x >= 0 || last_obj_width >= 0)
    {
        if ((left_ankle_x < last_obj_x) || (left_ankle_x > (last_obj_x + last_obj_width)) ||
            (right_ankle_x < last_obj_x) || (right_ankle_x > (obj_x + last_obj_width)))
        {
            jump_out_cache.push_back(1);
        } else
        {
            jump_out_cache.push_back(0);
        }

        int jump_count_num = int(std::count(jump_out_cache.begin(), jump_out_cache.end(), 1));
        if (jump_count_num > (cache_num-1) && jump_out_cache[cache_num-1] == 1)
        {
            mark_back = true;
            jump_out_cache.clear();
        }
    }

    return jump_box_num;

}

void JumpBoxGame::count_clear()
{
    mark_back = true;
    last_obj_x = -1;
    last_obj_width = -1;
}

// 弹跳大侦探
int BounceGame::count(std::vector<keypoint> key_points, int obj_x, int obj_y, int obj_width, int obj_height)
{
    int bounce_num = 0;

    // 不满足条件直接返回
    if (key_points[11].score < 0.3 || key_points[12].score < 0.3 ||
        key_points[15].score < 0.3 || key_points[16].score < 0.3)
    {
        return bounce_num;
    }

    float left_hip_x = key_points[11].x;
    float rigth_hip_x = key_points[12].x;

    float left_ankle_y = key_points[15].y;
    float right_ankle_y = key_points[16].y;

    float center_x = (left_hip_x + rigth_hip_x)*0.5f;

    if (start_mark)
    {
        if ((center_x >= obj_x) && (center_x <= (obj_x + obj_width)))
        {
            threshold = abs(rigth_hip_x - left_hip_x)*0.5f;
            if (abs(left_ankle_y - right_ankle_y) < threshold*0.5f)
            {
                base_ankle = (left_ankle_y + right_ankle_y)*0.5f;
                start_mark = false;
                last_position = base_ankle;
            } else
            {
                return bounce_num;
            }

        }
    }

    if ((center_x < obj_x) || (center_x > (obj_x + obj_width)))
    {
        start_mark = true;
        first_key = false;
    }
    else
    {
        ankle = (left_ankle_y + right_ankle_y)*0.5f;
        // 滤波,阈值=threshold/4
        if (abs(ankle - last_position) < threshold*0.25f)
        {
            return bounce_num;
        }
        last_position = ankle;
        // 左右脚踝是否处在同一水平,阈值=threshold/2
        if (abs(left_ankle_y - right_ankle_y) > threshold*0.5f)
        {
            first_key = false;
//            diff_up = -1;
            return bounce_num;
        }

        if ((abs(base_ankle - ankle) > diff_up) && (ankle < base_ankle))
        {
            diff_up = abs(base_ankle - ankle);
            up = ankle;
        } else
        {
            if (diff_up > threshold)
            {
                first_key = true;
            } else
            {
                diff_up = -1;
            }
        }

        if ((abs(ankle - up) > threshold*0.5f) && (first_key))
//        if (first_key)
        {
            bounce_num = 1;
            first_key = false;
            start_mark = true;
            diff_up = -1;
            up = 9999;
        }
    }

    return bounce_num;
}

void BounceGame::count_clear()
{
    start_mark = true;
    first_key = false;
    diff_up = -1;
    up = 9999;
}

// 俯卧撑
int PushUp::count(std::vector<keypoint> key_points)
{
    int push_up_num = 0;

    // std::cout << "进来了" << std::endl;

    // 阈值过滤
    if (key_points[5].score < 0.3 || key_points[7].score < 0.3 || key_points[9].score < 0.3 ||
        key_points[11].score < 0.3 || key_points[12].score < 0.3 || key_points[13].score < 0.3 ||
        key_points[14].score < 0.3 || key_points[15].score < 0.3 || key_points[16].score < 0.3)
    {
        return push_up_num;
    }

    // std::cout << "阈值过滤" << std::endl;

    // 躺下判断过滤
    if (key_points[5].x < key_points[11].x && key_points[5].x < key_points[12].x &&
        key_points[11].x < key_points[13].x && key_points[11].x < key_points[14].x &&
        key_points[13].x < key_points[15].x && key_points[13].x < key_points[16].x)
    {
        // do nothing
        ;
    } else
    {
        return push_up_num;
    }

    // std::cout << "躺下判断过滤" << std::endl;

    float left_wrist_y = key_points[9].y;
    float left_elbow_y = key_points[7].y;
    float left_shoulder_y = key_points[5].y;

    float left_hip_x = key_points[11].x;
    float left_hip_y = key_points[11].y;
    float left_ankle_x = key_points[15].x;
    float left_ankle_y = key_points[15].y;
    float left_knee_x = key_points[13].x;
    float left_knee_y = key_points[13].y;

    float right_hip_x = key_points[12].x;
    float right_hip_y = key_points[12].y;
    float right_ankle_x = key_points[16].x;
    float right_ankle_y = key_points[16].y;
    float right_knee_x = key_points[14].x;
    float right_knee_y = key_points[14].y;

    // 左右膝盖部分的夹角小于 90 度
    float left_knee_theta = get_angle(left_hip_x, left_hip_y,
                                      left_ankle_x, left_ankle_y, left_knee_x, left_knee_y);

    float right_knee_theta = get_angle(right_hip_x, right_hip_y,
                                       right_ankle_x, right_ankle_y, right_knee_x, right_knee_y);

    if (left_knee_theta < 165 || right_knee_theta < 165)
    {
        return push_up_num;
    }

    // y 轴方向上
    if (left_wrist_y < left_elbow_y || left_wrist_y < left_shoulder_y)
    {
        return push_up_num;
    }

    if ((left_elbow_y - left_shoulder_y) > 1.4f*(left_wrist_y - left_elbow_y))
    {
        return push_up_num;
    }

    // 下去之后上来判断
    if (mark_down)
    {
        // 站起来判断
        if ((left_elbow_y - left_shoulder_y) > 0.7f*(left_wrist_y - left_elbow_y))
        {
            mark_up_cache.push_back(1);
            // mark_up = true;
        } else
        {
            mark_up_cache.push_back(0);
        }
        // 超过限制数量自动删除
        if (mark_up_cache.size() > cache_num)
        {
            mark_up_cache.pop_front();
        }

        int mark_up_num = int(std::count(mark_up_cache.begin(), mark_up_cache.end(), 1));

        if (mark_up_num > (cache_num - 1) && mark_up_cache[cache_num-1] == 1)
        {
            // mark_up = true;
            push_up_num = 1;
            mark_down = false;
            mark_up = false;
            mark_up_cache.clear();
        }
    } else
    {
        if ((left_elbow_y - left_shoulder_y) < 0.4f*(left_wrist_y - left_elbow_y))
        {
            // mark_down = true;
            mark_down_cache.push_back(1);
        } else
        {
            mark_down_cache.push_back(0);
            // mark_down = false;
        }
        // 超过缓存数，自动删除
        if (mark_down_cache.size() > cache_num)
        {
            mark_down_cache.pop_front();
        }

        int mark_down_num = int(std::count(mark_down_cache.begin(), mark_down_cache.end(), 1));

        if (mark_down_num > (cache_num - 1) && mark_down_cache[cache_num-1] == 1)
        {
            // push_up_num = 1;
            mark_down = true;
            // mark_down = false;
            // mark_up = false;
            // 清空缓存
            mark_down_cache.clear();
        }
    }

    return push_up_num;
}

void PushUp::count_clear()
{
    mark_down = false;
    mark_up = true;
}

//仰卧起坐
int SitUp::count(std::vector<keypoint> key_points)
{
    int sit_up_num = 0;

    // 阈值过滤
    if (key_points[6].score < 0.3 || key_points[12].score < 0.3 ||
        key_points[14].score < 0.3 || key_points[16].score < 0.3)
    {
        return sit_up_num;
    }
    // 需要用到的坐标点
    float right_shoulder_x = key_points[6].x;
    float right_shoulder_y = key_points[6].y;

    float right_hip_x = key_points[12].x;
    float right_hip_y = key_points[12].y;

    float left_knee_x = key_points[13].x;

    float right_knee_x = key_points[14].x;
    float right_knee_y = key_points[14].y;

    float left_ankle_x = key_points[15].x;

//    float left_wrist_y = key_points[9].y;
    float right_wrist_y = key_points[10].y;
    float right_wrist_x = key_points[10].x;

    float right_ankle_y = key_points[16].y;
    float right_ankle_x = key_points[16].x;

//    float right_elbow_y = key_points[8].y;
    float right_elbow_x = key_points[8].x;


    // 躺下判断过滤
    if (right_hip_x < left_knee_x && right_hip_x < right_knee_x &&
        right_knee_x < left_ankle_x && right_knee_x < right_ankle_x)
    {
        // do nothing
        ;
    } else
    {
        mark_sit = false;
        mark_lie = false;
        return sit_up_num;
    }

    // 膝盖部分的夹角小于 90 度
    float knee_theta = get_angle(right_hip_x, right_hip_y,
                                 right_ankle_x, right_ankle_y, right_knee_x, right_knee_y);

    // std::cout << "knee_theta: " << knee_theta << std::endl;
    if (knee_theta > 95)
    {
        return sit_up_num;
    }

    // 计算腰部夹角
    float waist_theta = get_angle(right_shoulder_x, right_shoulder_y,
                                  right_ankle_x, right_ankle_y, right_hip_x, right_hip_y);

    //参考距离
    float refer_distance = pow((pow((right_ankle_x-right_knee_x), 2) +
                                pow((right_ankle_y-right_knee_y), 2)), 0.5);

    // std::cout << "waist_theta: " << waist_theta << std::endl;

    // std::cout << "mark_lie: " << mark_lie << " mark_sit: " << mark_sit << std::endl;

    if (mark_lie)
    {
        if (waist_theta < 135)
        {
            if (abs(right_wrist_x-right_knee_x) < 0.2f*refer_distance && right_wrist_x > right_elbow_x &&
                key_points[10].score > 0.3)
            {
                mark_sit_cache.push_back(1);
            }
                // std::cout << "right_hip_x: " << right_hip_x << std::endl;
                // std::cout << "right_elbow_x: " << right_elbow_x << std::endl;
                // std::cout << "refer_distance: " << refer_distance << std::endl;

            else if ((right_hip_x-right_elbow_x) < 0.2f*refer_distance)
            {
                mark_sit_cache.push_back(1);
            } else
            {
                mark_sit_cache.push_back(0);
            }

        } else
        {
            mark_sit_cache.push_back(0);
        }

        // 超过缓存数，自动删除
        if (mark_sit_cache.size() > cache_num)
        {
            mark_sit_cache.pop_front();
        }

        int mark_sit_num = int(std::count(mark_sit_cache.begin(), mark_sit_cache.end(), 1));

        // std::cout << "躺下判断判断完成: " << mark_sit_num << std::endl;

        if (mark_sit_num > (cache_num - 1) && mark_sit_cache[cache_num-1] == 1)
        {
            sit_up_num = 1;
            mark_sit = false;
            mark_lie = false;
            // 清空缓存
            mark_sit_cache.clear();
        }
    } else
    {
        //躺下判断
        if (waist_theta > 150)
        {
            if (abs(right_wrist_x-right_knee_x) > 0.4f*refer_distance && right_wrist_y > right_knee_y &&
                right_hip_y > right_wrist_y)
            {
                mark_lie_cache.push_back(1);
            } else
            {
                mark_lie_cache.push_back(0);
            }
        } else
        {
            mark_lie_cache.push_back(0);
        }

        // 超过限制数量自动删除
        if (mark_lie_cache.size() > cache_num)
        {
            mark_lie_cache.pop_front();
        }

        int mark_lie_num = int(std::count(mark_lie_cache.begin(), mark_lie_cache.end(), 1));

        if (mark_lie_num > (cache_num - 1) && mark_lie_cache[cache_num-1] == 1)
        {
            mark_lie = true;
            mark_lie_cache.clear();
        }

    }

    return sit_up_num;

}

void SitUp::count_clear()
{
    mark_sit = false;
    mark_lie = true;
}

// 平板支撑
/*
-1: 代表不确定状态，
0: 代表不规范动作不计时间
1：代表有效
*/
int Plank::count(std::vector<keypoint> key_points)
{
    int plank_status = -1;

    // 阈值过滤
    if (key_points[9].score < 0.3 || key_points[7].score < 0.3 || key_points[5].score < 0.3 ||
        key_points[11].score < 0.3 || key_points[13].score < 0.3 || key_points[15].score < 0.3)
    {
        // plank_status_cache.push_back(-1);
        return plank_status;
    }
    // 需要用到的坐标点
    // 左手腕
    float left_wrist_x = key_points[9].x;
    float left_wrist_y = key_points[9].y;
    // 左手肘
    float left_elbow_x = key_points[7].x;
    float left_elbow_y = key_points[7].y;
    // 左肩膀
    float left_shoulder_x = key_points[5].x;
    float left_shoulder_y = key_points[5].y;
    // 左腰
    float left_hip_x = key_points[11].x;
    float left_hip_y = key_points[11].y;
    // 左膝盖
    float left_knee_x = key_points[13].x;
    float left_knee_y = key_points[13].y;
    // 左脚踝
    float left_ankle_x = key_points[15].x;
    float left_ankle_y = key_points[15].y;

    float right_hip_x = key_points[12].x;
    float right_hip_y = key_points[12].y;
    float right_ankle_x = key_points[16].x;
    float right_ankle_y = key_points[16].y;
    float right_knee_x = key_points[14].x;
    float right_knee_y = key_points[14].y;

    // 左右膝盖部分的夹角小于 90 度
    float left_knee_theta = get_angle(left_hip_x, left_hip_y,
                                      left_ankle_x, left_ankle_y, left_knee_x, left_knee_y);

    float right_knee_theta = get_angle(right_hip_x, right_hip_y,
                                       right_ankle_x, right_ankle_y, right_knee_x, right_knee_y);

    // 没有躺平状态不计数
    if (left_wrist_x < left_shoulder_x && left_shoulder_x < left_hip_x &&
        left_hip_x < left_knee_x && left_knee_x < left_ankle_x &&
        left_hip_y < left_ankle_y && left_shoulder_y < left_knee_y)
    {
        // do nothing
        ;
    } else
    {
        plank_status = 0;
        // plank_status_cache.push_back(0);
        return plank_status;
    }
    // 角度判断
    // 计算手腕，手肘，肩膀之间的夹角
    float elbow_theta = get_angle(left_wrist_x, left_wrist_y,
                                  left_shoulder_x, left_shoulder_y, left_elbow_x, left_elbow_y);

    float shoulder_theta = get_angle(left_elbow_x, left_elbow_y,
                                     left_hip_x, left_hip_y, left_shoulder_x, left_shoulder_y);

    if (elbow_theta > 75 && elbow_theta < 100 && shoulder_theta > 65 && shoulder_theta < 100 &&
        left_knee_theta > 165 && right_knee_theta > 165)
    {
        plank_status = 1;
    } else
    {
        plank_status = 0;
    }

    return plank_status;

}

int Plank::cache_count(std::vector<keypoint> key_points)
{
    int plank_status = count(key_points);
    plank_status_cache.push_back(plank_status);

    // 超过限制数量自动删除
    if (plank_status_cache.size() > cache_num)
    {
        plank_status_cache.pop_front();
    }
    // 统计 1 出现的数量
    int mark_true_num = int(std::count(plank_status_cache.begin(), plank_status_cache.end(), 1));

    // 统计 0 出现的个数
    int mark_false_num = int(std::count(plank_status_cache.begin(), plank_status_cache.end(), 0));

    int threshold_num =  int(cache_num*0.5f);
    if (mark_true_num > threshold_num)
    {
        return 1;
    }
    else if (mark_false_num > threshold_num)
    {
        return 0;
    } else
    {
        return -1;
    }

}

void Plank::count_clear()
{
    plank_status_cache.clear();
}

// 敏捷身手
int AgileSkillGame::count(std::vector<keypoint> key_points, int obj_x, int obj_y, int obj_width, int obj_height)
{
    int is_touch_status = 0;

    float lines[][4] =
            {{key_points[1].x, key_points[1].y, key_points[3].x, key_points[3].y},
             {key_points[1].x, key_points[1].y, key_points[0].x, key_points[0].y},
             {key_points[0].x, key_points[0].y, key_points[2].x, key_points[2].y},
             {key_points[2].x, key_points[2].y, key_points[4].x, key_points[4].y},
             {key_points[0].x, key_points[0].y, key_points[5].x, key_points[5].y},
             {key_points[0].x, key_points[0].y, key_points[6].x, key_points[6].y},
             {key_points[7].x, key_points[7].y, key_points[5].x, key_points[5].y},
             {key_points[7].x, key_points[7].y, key_points[9].x, key_points[9].y},
             {key_points[8].x, key_points[8].y, key_points[6].x, key_points[6].y},
             {key_points[8].x, key_points[8].y, key_points[10].x, key_points[10].y},
             {key_points[11].x, key_points[11].y, key_points[5].x, key_points[5].y},
             {key_points[12].x, key_points[12].y, key_points[6].x, key_points[6].y},
             {key_points[11].x, key_points[11].y, key_points[13].x, key_points[13].y},
             {key_points[12].x, key_points[12].y, key_points[14].x, key_points[14].y},
             {key_points[15].x, key_points[15].y, key_points[13].x, key_points[13].y},
             {key_points[16].x, key_points[16].y, key_points[14].x, key_points[14].y},
             {key_points[11].x, key_points[11].y, key_points[12].x, key_points[12].y}};

    int rec[4] = {obj_x, obj_y, obj_width, obj_height};

    for (int i=1; i<17; i++) {
        if (is_intersect_rec(rec, lines[i])) {
            is_touch_status = 1;
            break;
        }
    }

    touch_status_cache.push_back(is_touch_status);
    // 超过限制数量自动删除
    if (touch_status_cache.size() > cache_num)
    {
        touch_status_cache.pop_front();
    }

    // 统计 1 出现的数量
    int mark_touch_num = int(std::count(touch_status_cache.begin(), touch_status_cache.end(), 1));

    if (mark_touch_num > (cache_num - 1) && touch_status_cache[cache_num-1] == 1)
    {
        touch_status_cache.clear();
        return 1;
    } else
    {
        return 0;
    }
}
void AgileSkillGame::count_clear()
{
    touch_status_cache.clear();;
}

bool AgileSkillGame::is_intersect_line(int rec[], float line[]) {
    float x00 = rec[0], y00 = rec[1];
    float x10 = line[0], y10 = line[1];
    float x01 = rec[2] - rec[0], y01 = rec[3] - rec[1];
    float x11 = line[2] - line[0], y11 = line[3] - line[1];
    float d = x11*y01 - x01*y11;
    float s = ((x00 - x10) * y01 - (y00 - y10) * x01)/d;
    float t = ((x00 - x10) * y11 - (y00 - y10) * x11)/d;
    if (s >= 0 and s <= 1 and t >= 0 and t <= 1) {
        // std::cout << rec[0] << ',' << rec[1] << ',' << rec[2] << ',' << rec[3] << ' ' << line[0] << ',' << line[1] << ',' << line[2] << ',' << line[3] ;
        return true;
    }
    return false;
}

bool AgileSkillGame::is_intersect_rec(int rec[], float line[]) {
    int lines[4][4] = {{rec[0], rec[1], rec[0]+rec[2], rec[1]},
                       {rec[0], rec[1], rec[0], rec[1]+rec[3]},
                       {rec[0]+rec[2], rec[1], rec[0]+rec[2], rec[1]+rec[3]},
                       {rec[0], rec[1]+rec[3], rec[0]+rec[2], rec[1]+rec[3]}};

    for (int i=0; i<4; i++){
        if (is_intersect_line(lines[i], line)) {
            return true;
        }
    }
    return false;
}

// 反应力挑战
int ReactionSpeed::count(std::vector<keypoint> key_points, int obj_x, int obj_y, int obj_width, int obj_height)
{
    int is_touch_status = 0;

    // 需要用到的坐标点
    // 左手腕
    float left_wrist_x = key_points[9].x;
    float left_wrist_y = key_points[9].y;
    // 左手肘
    float left_elbow_x = key_points[7].x;
    float left_elbow_y = key_points[7].y;

    // 右手腕
    float right_wrist_x = key_points[10].x;
    float right_wrist_y = key_points[10].y;
    // 右手肘
    float right_elbow_x = key_points[8].x;
    float right_elbow_y = key_points[8].y;

//    int br_x = obj_x + obj_width;
//    int br_y = obj_y + obj_height;

    // 矩形中心点位置
    float center_x = obj_x + obj_width*0.5f;
    float center_y = obj_y + obj_height*0.5f;

    // 补偿关键点到手掌位置
    float left_extend_x = (left_wrist_x-left_elbow_x)*0.2f;
    float left_extend_y = (left_wrist_y-left_elbow_y)*0.2f;
    float left_palm_x = left_wrist_x + left_extend_x;
    float left_palm_y = left_wrist_y + left_extend_y;

    float right_extend_x = (right_wrist_x-right_elbow_x)*0.2f;
    float right_extend_y = (right_wrist_y-right_elbow_y)*0.2f;
    float right_palm_x = right_wrist_x + right_extend_x;
    float right_palm_y = right_wrist_y + right_extend_y;

    float left_ins = pow((pow(abs(left_palm_x-center_x), 2) + pow(abs(left_palm_y-center_y), 2)), 0.5);
    float right_ins = pow((pow(abs(right_palm_x-center_x), 2) + pow(abs(right_palm_y-center_y), 2)), 0.5);

    if ((left_ins <= 0.8f*obj_width && key_points[9].score >= 0.3 &&
         key_points[7].score >= 0.3) || (right_ins <= 0.8f*obj_width &&
                                         key_points[10].score >= 0.3 && key_points[8].score >= 0.3))

//    if ((left_palm_x >= obj_x && left_palm_x <= br_x &&
//         left_palm_y >= obj_y && left_palm_y <= br_y &&
//         key_points[9].score >= 0.3) ||
//        (right_palm_x >= obj_x && right_palm_x <= br_x &&
//         right_palm_y >= obj_y && right_palm_y <= br_y &&
//         key_points[10].score >= 0.3))
    {
        is_touch_status = 1;
        touch_status_cache.push_back(is_touch_status);
    } else
    {
        is_touch_status = 0;
        touch_status_cache.push_back(is_touch_status);
    }
    // 超过限制数量自动删除
    if (touch_status_cache.size() > cache_num)
    {
        touch_status_cache.pop_front();
    }

    int mark_touch_num = int(std::count(touch_status_cache.begin(), touch_status_cache.end(), 1));

    if (mark_touch_num > 0)
//    if (mark_touch_num > (cache_num - 1) && touch_status_cache[cache_num - 1] == 1)
    {
        touch_status_cache.clear();
        return 1;
    } else
    {
        return 0;
    }
}

void ReactionSpeed::count_clear()
{
    touch_status_cache.clear();;
}


int TiaoYiTiaoGame::count(std::vector<keypoint> key_points, int obj_x, int obj_y, int obj_width, int obj_height)
{
    // 阈值不满足返回 -1 不确定
    if (key_points[5].score < 0.3 || key_points[6].score < 0.3 || key_points[11].score < 0.3 ||
        key_points[12].score < 0.3 || key_points[15].score < 0.3 || key_points[16].score < 0.3)
    {
        return -1;
    }

    float left_shoulder_x = key_points[5].x;
    float right_shoulder_x = key_points[6].x;
    float left_ankle_y = key_points[15].y;
    float right_ankle_y = key_points[16].y;
    float left_hip_y = key_points[11].y;
    float right_hip_y = key_points[12].y;

    float shoulder_distance = abs(right_shoulder_x - left_shoulder_x);
    float hip_ankle_distance = abs(
            (left_hip_y+right_hip_y)*0.5f - (left_ankle_y+right_ankle_y)*0.5f);

    if (hip_ankle_distance < 1.5*shoulder_distance)
    {
        mark_down_cache.push_back(1);
    }
    else if (hip_ankle_distance > 1.8*shoulder_distance)
    {
        mark_down_cache.push_back(0);
    } else
    {
        mark_down_cache.push_back(-1);
    }

    // 超过限制数量自动删除
    if (mark_down_cache.size() > cache_num)
    {
        mark_down_cache.pop_front();
    }

    // 统计 1 出现的数量
    int mark_down_num = int(std::count(mark_down_cache.begin(), mark_down_cache.end(), 1));

    // 统计 0 出现的个数
    int mark_up_num = int(std::count(mark_down_cache.begin(), mark_down_cache.end(), 0));

    int threshold_num =  int(cache_num*0.5f);

    if (mark_down_num > threshold_num)
    {
        return 1;
    }
    else if (mark_up_num > threshold_num)
    {
        return 0;
    } else
    {
        return -1;
    }
}

void TiaoYiTiaoGame::count_clear()
{
    mark_down_cache.clear();
}


// 坐位体前屈
int SittingForwardBend::count(std::vector<keypoint> key_points)
{
    int count_status = 0;

    //阈值过滤
    if (key_points[6].score < 0.3 || key_points[8].score < 0.3 || key_points[10].score < 0.3 ||
        key_points[12].score < 0.3 || key_points[14].score < 0.3 || key_points[14].score < 0.3)
    {
        return count_status;
    }

    // 鼻子
    float nose_y = key_points[0].y;
    // 右手腕
    float right_wrist_y = key_points[10].y;
    float right_wrist_x = key_points[10].x;
    // 右手肘
    float right_elbow_y = key_points[8].y;
    float right_elbow_x = key_points[8].x;
    // 右肩膀
    float right_shoulder_x = key_points[6].x;
    float right_shoulder_y = key_points[6].y;
    // 右膝盖
    float right_knee_x = key_points[14].x;
    float right_knee_y = key_points[14].y;
    // 右臀
    float right_hip_x = key_points[12].x;
    float right_hip_y = key_points[12].y;
    // 右脚踝
    float right_ankle_y = key_points[16].y;
    float right_ankle_x = key_points[16].x;

    // 膝盖夹角
    float knee_theta = get_angle(right_hip_x, right_hip_y,
                                 right_ankle_x, right_ankle_y, right_knee_x, right_knee_y);
    // 手肘夹角
    float elbow_theta = get_angle(right_shoulder_x, right_shoulder_y,
                                  right_wrist_x, right_wrist_y, right_elbow_x, right_elbow_y);

    if (knee_theta < 135)
    {
        mark_forward = false;
        mark_backward = false;
        mark_max = false;
        max_wrist_kpx = -9999;
        wrist_kpx_cache.clear();
        mark_cache.clear();
    }

    if (elbow_theta < 135 || right_wrist_y < nose_y)
    {
        return count_status;
    }

    //参考距离--脚踝到膝盖的距离
    float refer_distance = pow((pow((right_ankle_x-right_knee_x), 2) +
                                pow((right_ankle_y-right_knee_y), 2)), 0.5);

    wrist_kpx_cache.push_back(right_wrist_x);

    // 超过缓存数，自动删除
    if (wrist_kpx_cache.size() > kpx_cache_num)
    {
        wrist_kpx_cache.pop_front();
    }
    else if (wrist_kpx_cache.size() < kpx_cache_num)
    {
        count_status = 0;
        return count_status;
    }

    // 判断：前进(>0)，后退(<0)，不确定
    float line_slope = LeastSquares(kpx_index_cache, wrist_kpx_cache);


    if (line_slope > 8)
    {
        mark_cache.push_back(1);
    }
    else if (line_slope < -3)
    {
        mark_cache.push_back(-1);
    }
    else
    {
        mark_cache.push_back(0);
    }

    // 超过限制数量自动删除
    if (mark_cache.size() > mark_cache_num)
    {
        mark_cache.pop_front();
    }

    // 统计 1 出现数量
    int mark_forward_num = int(std::count(mark_cache.begin(), mark_cache.end(), 1));
    // 统计 -1 出现频率
    int mark_backward_num = int(std::count(mark_cache.begin(), mark_cache.end(), -1));

    int threshold_num =  int(mark_cache_num*0.5f) + 1;
    // std::cout << "mark_forward_num: " << mark_forward_num << std::endl;
    // std::cout << "mark_backward_num: " << mark_backward_num << std::endl;
    // std::cout << "threshold_num: " << threshold_num << std::endl;

    if (mark_forward_num > threshold_num)
    {
        mark_forward = true;
        mark_max = true;
    }

    if ((mark_backward_num > threshold_num) && mark_forward)
    {
        mark_backward = true;
    }

    if (mark_max)
    {
        if (max_wrist_kpx < right_wrist_x)
        {
            max_wrist_kpx = right_wrist_x;
        }
    }
    if (mark_forward && mark_backward)
    {

        // 判断 手腕最大距离和脚踝距离的关系
        float wrist_ankle_distance = max_wrist_kpx - right_ankle_x;
        // std::cout << "wrist_ankle_distance" << wrist_ankle_distance << std::endl;
        // std::cout << "refer_distance" << refer_distance << std::endl;

        if (wrist_ankle_distance < refer_distance*-0.1f)
        {
            count_status = 1;
        }
        else if (wrist_ankle_distance > refer_distance*-0.1f && wrist_ankle_distance < refer_distance*0.1f)
        {
            count_status = 2;
        }
        else if (wrist_ankle_distance > refer_distance*0.1f && wrist_ankle_distance < refer_distance*0.2f)
        {
            count_status = 3;
        }
        else if (wrist_ankle_distance > refer_distance*0.2f && wrist_ankle_distance < refer_distance*0.3f)
        {
            count_status = 4;
        }
        else if (wrist_ankle_distance > refer_distance*0.3f)
        {
            count_status = 5;
        }
        // 恢复状态
        mark_forward = false;
        mark_backward = false;
        mark_max = false;
        max_wrist_kpx = -9999;
        // mark_cache.clear();

        return count_status;
    } else
    {
        count_status = 0;
        return count_status;
    }

}

void SittingForwardBend::count_clear()
{
    mark_forward = false;
    mark_backward = false;
    mark_max = false;
    max_wrist_kpx = -9999;
    mark_cache.clear();
}
