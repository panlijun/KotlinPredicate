package com.kotlin.predicate.app

import ErrorCallback
import android.view.Gravity
import android.widget.Toast
import androidx.multidex.MultiDex
import com.kingja.loadsir.callback.SuccessCallback
import com.kingja.loadsir.core.LoadSir
import com.kotlin.predicate.app.ext.getProcessName
import com.kotlin.predicate.app.widget.callBack.EmptyCallback
import com.kotlin.predicate.app.widget.callBack.LoadingCallback
import com.luck.picture.lib.utils.ToastUtils
import com.mvvm.core.base.BaseApp
import com.tencent.mmkv.MMKV


/**
 * 作者　: hegaojian
 * 时间　: 2019/12/23
 * 描述　:
 */

//Application全局的ViewModel，里面存放了一些账户信息，基本配置信息等
val appViewModel: AppViewModel by lazy { App.appViewModelInstance }

//Application全局的ViewModel，用于发送全局通知操作
val eventViewModel: EventViewModel by lazy { App.eventViewModelInstance }

class App : BaseApp() {

    companion object {
        lateinit var instance: App
        lateinit var eventViewModelInstance: EventViewModel
        lateinit var appViewModelInstance: AppViewModel
    }

    override fun onCreate() {
        super.onCreate()
        MMKV.initialize(this.filesDir.absolutePath + "/mmkv")
        instance = this
        eventViewModelInstance = getAppViewModelProvider().get(EventViewModel::class.java)
        appViewModelInstance = getAppViewModelProvider().get(AppViewModel::class.java)
        MultiDex.install(this)
//        ToastUtils.setGravity(Gravity.CENTER,0,0)

        //界面加载管理 初始化
        LoadSir.beginBuilder()
            .addCallback(LoadingCallback())//加载
            .addCallback(ErrorCallback())//错误
            .addCallback(EmptyCallback())//空
            .setDefaultCallback(SuccessCallback::class.java)//设置默认加载状态页
            .commit()
        //初始化Bugly
        val context = applicationContext
        // 获取当前包名
        val packageName = context.packageName
        // 获取当前进程名
        val processName = getProcessName(android.os.Process.myPid())
        // 设置是否为上报进程
//        val strategy = CrashReport.UserStrategy(context)
//        strategy.isUploadProcess = processName == null || processName == packageName

//        jetpackMvvmLog = BuildConfig.DEBUG


        //防止项目崩溃，崩溃后打开错误界面
//        CaocConfig.Builder.create()
//            .backgroundMode(CaocConfig.BACKGROUND_MODE_SILENT) //default: CaocConfig.BACKGROUND_MODE_SHOW_CUSTOM
//            .enabled(true)//是否启用CustomActivityOnCrash崩溃拦截机制 必须启用！不然集成这个库干啥？？？
//            .showErrorDetails(false) //是否必须显示包含错误详细信息的按钮 default: true
//            .showRestartButton(false) //是否必须显示“重新启动应用程序”按钮或“关闭应用程序”按钮default: true
//            .logErrorOnRestart(false) //是否必须重新堆栈堆栈跟踪 default: true
//            .trackActivities(true) //是否必须跟踪用户访问的活动及其生命周期调用 default: false
//            .minTimeBetweenCrashesMs(2000) //应用程序崩溃之间必须经过的时间 default: 3000
//            .restartActivity(WelcomeActivity::class.java) // 重启的activity
//            .errorActivity(ErrorActivity::class.java) //发生错误跳转的activity
//            .apply()
    }

}
