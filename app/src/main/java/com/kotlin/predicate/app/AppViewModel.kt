package com.kotlin.predicate.app

import com.kotlin.predicate.app.util.CacheUtil
import com.kotlin.predicate.app.util.SettingUtil
import com.kotlin.predicate.data.model.AccountDTO
import com.kotlin.predicate.data.model.LoginInfo
import com.kotlin.predicate.data.model.UserInfo
import com.kunminx.architecture.ui.callback.UnPeekLiveData
import com.mvvm.core.base.viewmodel.BaseViewModel
import com.mvvm.core.callback.livedata.event.EventLiveData

/**
 * 描述　:APP全局的ViewModel，可以存放公共数据，当他数据改变时，所有监听他的地方都会收到回调,也可以做发送消息
 * 比如 全局可使用的 地理位置信息，账户信息,App的基本配置等等，
 */
class AppViewModel : BaseViewModel() {

    //App的账户信息
    var loginInfo = UnPeekLiveData.Builder<LoginInfo>().setAllowNullValue(true).create()

    //当前用户信息，单账号对应多用户
    var userInfo = UnPeekLiveData.Builder<UserInfo>().setAllowNullValue(true).create()


    //App 列表动画
    var appAnimation = EventLiveData<Int>()

    init {
        val loginValue = CacheUtil.getLoginInfo()
        if (loginValue != null && loginValue.userDTO == null) {
            //登录了但是没有用户信息 重新登录
            loginInfo.value = null
            userInfo.value = null
            CacheUtil.setLoginInfo(null)
        } else {
            loginInfo.value = loginValue
            userInfo.value = loginInfo.value?.userDTO
        }

        //初始化列表动画
        appAnimation.value = SettingUtil.getListMode()
    }

    fun loginOut() {
        CacheUtil.setLoginInfo(null)
        appViewModel.loginInfo.value = null
        appViewModel.userInfo.value = null
    }

    fun loginIn(loginInfo: LoginInfo) {
        CacheUtil.setLoginInfo(loginInfo)
        appViewModel.loginInfo.value = loginInfo
        appViewModel.userInfo.value = appViewModel.loginInfo.value?.userDTO
    }

    fun switchUser(userInfo: UserInfo) {
        appViewModel.loginInfo.value?.userDTO = userInfo
        appViewModel.userInfo.value = userInfo
        CacheUtil.setLoginInfo(appViewModel.loginInfo.value)
    }


    fun simulateLogin(phone: String) {
        // mock  by plj
        loginIn(LoginInfo(accountDTO = AccountDTO(phone = phone)))
    }
}