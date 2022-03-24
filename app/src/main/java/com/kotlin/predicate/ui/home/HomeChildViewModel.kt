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
        normalItem.add(HomeProject(R.mipmap.ic_launcher, "跳绳"))
        normalItem.add(HomeProject(R.mipmap.ic_launcher, "仰卧起坐"))
        normalItem.add(HomeProject(R.mipmap.ic_launcher, "原地跑步"))
        normalItem.add(HomeProject(R.mipmap.ic_launcher, "深蹲"))
        normalItem.add(HomeProject(R.mipmap.ic_launcher, "坐位提体前屈"))
        normalItem.add(HomeProject(R.mipmap.ic_launcher, "开合跳"))
        normalItem.add(HomeProject(R.mipmap.ic_launcher, "俯卧撑"))
        normalItem.add(HomeProject(R.mipmap.ic_launcher, "跪姿俯卧撑"))


        funnyItems.add(HomeProject(R.mipmap.ic_launcher, "周游地球"))
        funnyItems.add(HomeProject(R.mipmap.ic_launcher, "跳格子游戏"))
        funnyItems.add(HomeProject(R.mipmap.ic_launcher, "跳框游戏"))
        funnyItems.add(HomeProject(R.mipmap.ic_launcher, "坐西瓜"))

    }
}