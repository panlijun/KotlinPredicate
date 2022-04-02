package com.kotlin.predicate.ui.person

import com.mvvm.core.base.viewmodel.BaseViewModel
import com.mvvm.core.callback.databind.StringObservableField
import com.kotlin.predicate.app.appViewModel
import com.kotlin.predicate.app.util.CacheUtil
import com.kotlin.predicate.data.model.UserInfo

class AddUserInfoViewModel : BaseViewModel() {
    var name = StringObservableField("")
    var nickname = StringObservableField("")
    var gender = StringObservableField("")
    var cardNo = StringObservableField("")
    var school = StringObservableField("")
    var classId = StringObservableField("")


    fun addUser() {
        //mock by plj
        val userInfo =
            UserInfo(name = name.get(), nickname = nickname.get(), classId = classId.get())

        appViewModel.loginInfo.value?.let {
            it.userDTO = userInfo
            CacheUtil.setLoginInfo(it)
            appViewModel.loginInfo.value = it
            appViewModel.userInfo.value = userInfo
        }
    }
}