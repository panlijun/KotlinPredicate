package com.kotlin.predicate.app

import com.mvvm.core.base.viewmodel.BaseViewModel
import com.mvvm.core.callback.livedata.event.EventLiveData

/**
 * 作者　: hegaojian
 * 时间　: 2019/5/2
 * 描述　:APP全局的ViewModel，可以在这里发送全局通知替代EventBus，LiveDataBus等
 */
class EventViewModel : BaseViewModel() {

    val shareArticleEvent = EventLiveData<Boolean>()

    val todoEvent = EventLiveData<Boolean>()

}