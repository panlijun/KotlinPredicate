package com.mvvm.core.callback.livedata

import androidx.lifecycle.MutableLiveData

/**
 * 描述　:自定义的String型 MutableLiveData 提供了默认值，避免取值的时候还要判空
 */
class StringLiveData : MutableLiveData<String>() {

    override fun getValue(): String {
        return super.getValue() ?: ""
    }

}