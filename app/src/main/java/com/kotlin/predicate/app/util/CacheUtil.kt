package com.kotlin.predicate.app.util

import android.text.TextUtils
import com.google.gson.Gson
import com.kotlin.predicate.data.model.LoginInfo
import com.tencent.mmkv.MMKV


object CacheUtil {

    /**
     * 获取保存的账户信息
     */
    fun getLoginInfo(): LoginInfo? {
        val kv = MMKV.mmkvWithID(Key.APP)
        val loginInfo = kv.decodeString(Key.LOGIN_INFO)
        return if (TextUtils.isEmpty(loginInfo)) {
            null
        } else {
            Gson().fromJson(loginInfo, LoginInfo::class.java)
        }
    }

    /**
     * 设置账户信息
     */
    fun setLoginInfo(loginInfo: LoginInfo?) {
        val kv = MMKV.mmkvWithID(Key.APP)
        if (loginInfo == null) {
            kv.encode(Key.LOGIN_INFO, "")
            setIsLogin(false)
        } else {
            kv.encode(Key.LOGIN_INFO, Gson().toJson(loginInfo))
            setIsLogin(true)
        }

    }


    /**
     * 是否已经登录
     */
    fun isLogin(): Boolean {
        val kv = MMKV.mmkvWithID(Key.APP)
        return kv.decodeBool(Key.LOGIN, false)
    }

    /**
     * 设置是否已经登录
     */
    fun setIsLogin(isLogin: Boolean) {
        val kv = MMKV.mmkvWithID(Key.APP)
        kv.encode(Key.LOGIN, isLogin)
    }

    /**
     * 是否是第一次登陆
     */
    fun isFirst(): Boolean {
        val kv = MMKV.mmkvWithID(Key.APP)
        return kv.decodeBool(Key.FIRST, true)
    }

    /**
     * 是否是第一次登陆
     */
    fun setFirst(first: Boolean): Boolean {
        val kv = MMKV.mmkvWithID(Key.APP)
        return kv.encode(Key.FIRST, first)
    }

}