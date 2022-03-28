package com.kotlin.predicate.ui.home

import androidx.lifecycle.MutableLiveData
import com.kotlin.predicate.R
import com.kotlin.predicate.data.model.HomeProject
import com.mvvm.core.base.viewmodel.BaseViewModel


const val TYPE_SPORT_NORMAL = 1
const val TYPE_SPORT_FUNNY = 2


class HomeChildViewModel : BaseViewModel() {


    var normalItem: ArrayList<HomeProject> = ArrayList<HomeProject>()
    var funnyItems: ArrayList<HomeProject> = ArrayList()
    var items: MutableLiveData<ArrayList<HomeProject>> = MutableLiveData<ArrayList<HomeProject>>()


    fun loadData(type: Int = TYPE_SPORT_NORMAL) {
        if (type == TYPE_SPORT_NORMAL) {
            items.value = normalItem
        } else if (type == TYPE_SPORT_FUNNY) {
            items.value = funnyItems
        }
    }

    init {
        normalItem.add(HomeProject(R.drawable.icon_sport_jump, "跳绳"))
        normalItem.add(HomeProject(R.drawable.icon_sport_sit_up, "仰卧起坐"))
        normalItem.add(HomeProject(R.drawable.ico_sport_run, "原地跑步"))
        normalItem.add(HomeProject(R.drawable.icon_squat_down, "深蹲"))
        normalItem.add(HomeProject(R.drawable.icon_sport_flexion, "坐位提体前屈"))
        normalItem.add(HomeProject(R.drawable.icon_sport_jumping_jacks, "开合跳"))
        normalItem.add(HomeProject(R.drawable.icon_sport_push_up, "俯卧撑"))
        normalItem.add(HomeProject(R.drawable.icon_sport_push_up2, "跪姿俯卧撑"))


        funnyItems.add(HomeProject(R.drawable.icon_travel_earth, "周游地球"))
        funnyItems.add(HomeProject(R.drawable.icon_jump_frame, "跳格子游戏"))
        funnyItems.add(HomeProject(R.drawable.icon_jumping_grid, "跳框游戏"))
        funnyItems.add(HomeProject(R.drawable.icon_sport_flexion, "坐西瓜"))

    }
}