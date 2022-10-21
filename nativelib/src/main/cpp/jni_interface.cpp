#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include "sport.h"


static Sport *g_sport = nullptr;
static SkippIng* g_skipping = nullptr;
static Squat* g_squat = nullptr;
static SitBalloon* g_sit_ballon = nullptr;
static Run* g_run = nullptr;
static OpenCloseJump* g_open_close_jump = nullptr;
static JumpBoxGame* g_jump_box_game = nullptr;
static BounceGame* g_bounce_game = nullptr;
static PushUp* g_push_up = nullptr;
static SitUp* g_sit_up = nullptr;
static Plank* g_plank = nullptr;
static AgileSkillGame* g_agile_skill_game = nullptr;
static ReactionSpeed* g_reaction_speed_game = nullptr;
static TiaoYiTiaoGame* g_tiaoyitiao_game = nullptr;
static SittingForwardBend* g_sitting_forward_bend = nullptr;

// 运动类型全局变量
int global_sport_type = -1;


JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    if(g_sport != nullptr)
    {
        g_sport->unload_model();
        delete g_sport;
        g_sport = nullptr;
    }
    g_sport = new Sport;
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    if (g_sport != nullptr)
    {
        g_sport->unload_model();
        delete g_sport;
        g_sport = nullptr;
    }

    // 跳绳
    if (g_skipping != nullptr)
    {
        delete g_skipping;
        g_skipping = nullptr;
    }

    // 深蹲
    if (g_squat != nullptr)
    {
        delete g_squat;
        g_squat = nullptr;
    }

    // 坐气球
    if (g_sit_ballon != nullptr)
    {
        delete g_sit_ballon;
        g_sit_ballon = nullptr;
    }

    // 跑步项目
    if (g_run != nullptr)
    {
        delete g_run;
        g_run = nullptr;
    }

    // 开合跳项目
    if (g_open_close_jump != nullptr)
    {
        delete g_open_close_jump;
        g_open_close_jump = nullptr;
    }

    // 跳箱子
    if (g_jump_box_game != nullptr)
    {
        delete g_jump_box_game;
        g_jump_box_game = nullptr;
    }

    // 弹跳大侦探
    if (g_bounce_game != nullptr)
    {
        delete g_bounce_game;
        g_bounce_game = nullptr;
    }
    // 俯卧撑
    if (g_push_up != nullptr)
    {
        delete g_push_up;
        g_push_up = nullptr;
    }
    // 仰卧起坐
    if (g_sit_up != nullptr)
    {
        delete g_sit_up;
        g_sit_up = nullptr;
    }
    // 平板支撑
    if (g_plank != nullptr)
    {
        delete g_plank;
        g_plank = nullptr;
    }
    // 敏捷身手
    if (g_agile_skill_game != nullptr)
    {
        delete g_agile_skill_game;
        g_agile_skill_game = nullptr;
    }
    // 反应力挑战
    if (g_reaction_speed_game != nullptr)
    {
        delete g_reaction_speed_game;
        g_reaction_speed_game = nullptr;
    }
    // 跳一跳
    if (g_tiaoyitiao_game != nullptr)
    {
        delete g_tiaoyitiao_game;
        g_tiaoyitiao_game = nullptr;
    }
    // 坐位体前屈
    if (g_sitting_forward_bend != nullptr)
    {
        delete g_sitting_forward_bend;
        g_sitting_forward_bend = nullptr;
    }

    // 其他项目

}

extern "C" JNIEXPORT void JNICALL
Java_com_zjuici_nativelib_SportAi_init(JNIEnv *env, jclass clazz, jobject assetManager,
                                       jint sport_type, jboolean useGPU) {
    if (g_sport == nullptr)
    {
        g_sport = new Sport;
    }
    // 加载模型
    AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
    g_sport->load_model(mgr, useGPU);

    global_sport_type = sport_type;

    // 跳绳项目
    if (sport_type == 0)
    {
        if (g_skipping != nullptr)
        {
            delete g_skipping;
            g_skipping = nullptr;
        }
        g_skipping = new SkippIng;
    }

    // 深蹲项目
    if (sport_type == 1)
    {
        if (g_squat != nullptr)
        {
            delete g_squat;
            g_squat = nullptr;
        }
        g_squat = new Squat;
    }

    // 坐气球
    if (sport_type == 2)
    {
        if (g_sit_ballon != nullptr)
        {
            delete g_sit_ballon;
            g_sit_ballon = nullptr;
        }
        g_sit_ballon = new SitBalloon;
    }

    // 跑步项目
    if (sport_type == 3)
    {
        if (g_run != nullptr)
        {
            delete g_run;
            g_run = nullptr;
        }
        g_run = new Run;
    }

    // 开合跳项目
    if (sport_type == 4)
    {
        if (g_open_close_jump != nullptr)
        {
            delete g_open_close_jump;
            g_open_close_jump = nullptr;
        }
        g_open_close_jump = new OpenCloseJump;
    }

    // 跳箱子
    if (sport_type == 5)
    {
        if (g_jump_box_game != nullptr)
        {
            delete g_jump_box_game;
            g_jump_box_game = nullptr;
        }
        g_jump_box_game = new JumpBoxGame;
    }

    // 弹跳大侦探
    if (sport_type == 6)
    {
        if (g_bounce_game != nullptr)
        {
            delete g_bounce_game;
            g_bounce_game = nullptr;
        }
        g_bounce_game = new BounceGame;
    }

    // 俯卧撑
    if (sport_type == 7)
    {
        if (g_push_up != nullptr)
        {
            delete g_push_up;
            g_push_up = nullptr;
        }
        g_push_up = new PushUp;
    }

    // 仰卧起坐
    if (sport_type == 8)
    {
        if (g_sit_up != nullptr)
        {
            delete g_sit_up;
            g_sit_up = nullptr;
        }
        g_sit_up = new SitUp;
    }

    // 平板支撑
    if (sport_type == 9)
    {
        if (g_plank != nullptr)
        {
            delete g_plank;
            g_plank = nullptr;
        }
        g_plank = new Plank;
    }

    // 敏捷身手
    if (sport_type == 10)
    {
        if (g_agile_skill_game != nullptr)
        {
            delete g_agile_skill_game;
            g_agile_skill_game = nullptr;
        }
        g_agile_skill_game = new AgileSkillGame;
    }

    // 反应力挑战
    if (sport_type == 11)
    {
        if (g_reaction_speed_game != nullptr)
        {
            delete g_reaction_speed_game;
            g_reaction_speed_game = nullptr;
        }
        g_reaction_speed_game = new ReactionSpeed;
    }

    // 跳一跳
    if (sport_type == 12)
    {
        if (g_tiaoyitiao_game != nullptr)
        {
            delete g_tiaoyitiao_game;
            g_tiaoyitiao_game = nullptr;
        }
        g_tiaoyitiao_game = new TiaoYiTiaoGame;
    }

    // 坐位体前屈
    if (sport_type == 13)
    {
        if (g_sitting_forward_bend != nullptr)
        {
            delete g_sitting_forward_bend;
            g_sitting_forward_bend = nullptr;
        }
        g_sitting_forward_bend = new SittingForwardBend;
    }


    // 其他项目依次添加
    // ......



}

extern "C" JNIEXPORT jint JNICALL
Java_com_zjuici_nativelib_SportAi_ready(JNIEnv *env, jclass clazz, jobject image, jint mask_x,
                                        jint mask_y, jint mask_width, jint mask_height) {
    jint ready_status = g_sport->person_ready(env, image, mask_x, mask_y, mask_width, mask_height, global_sport_type);
    return ready_status;
}


extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_zjuici_nativelib_SportAi_count(JNIEnv *env, jclass clazz, jobject image, jint sport_type,
                                        jint mask_x, jint mask_y, jint mask_width,
                                        jint mask_height) {

    auto keypoints = g_sport->detect_key_points(env, image, mask_x, mask_y, mask_width, mask_height);
    // 运动项目计数
    jint count_num = 0;

    // 跳绳项目
    if (sport_type == 0)
    {
        count_num = g_skipping->count(keypoints);
    }

    // 深蹲项目
    if (sport_type == 1)
    {
        count_num = g_squat->count(keypoints);
    }

    // 坐气球
    if (sport_type == 2)
    {
        count_num = g_sit_ballon->count(keypoints, mask_x, mask_y, mask_width, mask_height);
    }

    // 跑步
    if (sport_type == 3)
    {
        count_num = g_run->count(keypoints);
    }

    // 开合跳
    if (sport_type == 4)
    {
        count_num = g_open_close_jump->count(keypoints);
    }

    // 跳箱子
    if (sport_type == 5)
    {
        count_num = g_jump_box_game->count(keypoints, mask_x, mask_y, mask_width, mask_height);
    }

    // 弹跳大侦探
    if (sport_type == 6)
    {
        count_num = g_bounce_game->count(keypoints, mask_x, mask_y, mask_width, mask_height);
    }

    // 俯卧撑
    if (sport_type == 7)
    {
        count_num = g_push_up->count(keypoints);
    }
    // 仰卧起坐
    if (sport_type == 8)
    {
        count_num = g_sit_up->count(keypoints);
    }
    // 平板支撑
    if (sport_type == 9)
    {
        // count_num = g_plank->count(key_points);
        count_num = g_plank->cache_count(keypoints);
    }
    // 敏捷身手
    if (sport_type == 10)
    {
        count_num = g_agile_skill_game->count(keypoints, mask_x, mask_y, mask_width, mask_height);
    }
    // 反应力挑战
    if (sport_type == 11)
    {
        count_num = g_reaction_speed_game->count(keypoints, mask_x, mask_y, mask_width, mask_height);
    }

    // 跳一跳趣味运动
    if (sport_type == 12)
    {
        count_num = g_tiaoyitiao_game->count(keypoints, mask_x, mask_y, mask_width, mask_height);
    }
    // 坐位体前屈
    if (sport_type == 13)
    {
        count_num = g_sitting_forward_bend->count(keypoints);
    }


    auto sport_cls = env->FindClass("com/zjuici/nativelib/SportPoint");
    auto cid = env->GetMethodID(sport_cls, "<init>", "(FFFI)V");

    jobjectArray ret = env->NewObjectArray(keypoints.size(), sport_cls, nullptr);

    int i = 0;
    int KEY_NUM = 17;
    for (auto &key_point : keypoints) {
        env->PushLocalFrame(1);
        jobject obj = env->NewObject(
                sport_cls, cid, key_point.x, key_point.y, key_point.score, count_num);
        obj = env->PopLocalFrame(obj);

        env->SetObjectArrayElement(ret, i++, obj);
    }

    return ret;
}


extern "C" JNIEXPORT void JNICALL
Java_com_zjuici_nativelib_SportAi_clear(JNIEnv *env, jclass clazz) {
    if (g_sport)
    {
        g_sport->ready_cache_clear();
    }
    // 跳绳项目缓存清理
    if (g_skipping)
    {
        g_skipping->count_clear();
    }

    // 深蹲项目
    if (g_squat)
    {
        g_squat->count_clear();
    }

    // 坐气球项目
    if (g_sit_ballon)
    {
        g_sit_ballon->count_clear();
    }

    // 跑步项目
    if (g_run != nullptr)
    {
        g_run->count_clear();
    }

    // 开合跳项目
    if (g_open_close_jump != nullptr)
    {
        g_open_close_jump->count_clear();
    }

    // 跳箱子
    if (g_jump_box_game != nullptr)
    {
        g_jump_box_game->count_clear();
    }

    // 弹跳大侦探
    if (g_bounce_game != nullptr)
    {
        g_bounce_game->count_clear();
    }
    // 俯卧撑
    if (g_push_up != nullptr)
    {
        g_push_up->count_clear();
    }
    // 仰卧起坐
    if (g_sit_up != nullptr)
    {
        g_sit_up->count_clear();
    }
    // 平板支撑
    if (g_plank != nullptr)
    {
        g_plank->count_clear();
    }
    // 坐位体前屈
    if (g_sitting_forward_bend != nullptr)
    {
        g_sitting_forward_bend->count_clear();
    }
    // 敏捷身手
    if (g_agile_skill_game != nullptr)
    {
        g_agile_skill_game->count_clear();
    }
    // 反应力挑战
    if (g_reaction_speed_game != nullptr)
    {
        g_reaction_speed_game->count_clear();
    }
    // 跳一跳
    if (g_tiaoyitiao_game != nullptr)
    {
        g_tiaoyitiao_game->count_clear();
    }

}