package com.kotlin.predicate.ui

import android.content.Intent
import android.os.Bundle
import android.os.CountDownTimer
import androidx.activity.viewModels
import com.kotlin.predicate.app.base.BaseActivity
import com.kotlin.predicate.app.util.CacheUtil
import com.kotlin.predicate.app.util.StatusBarUtil
import com.kotlin.predicate.databinding.WelcomeActivityBinding

class WelcomeActivity : BaseActivity<WelcomeViewModel, WelcomeActivityBinding>() {

    var countDownTimer: CountDownTimer? = null

    private val welcomeViewModel: WelcomeViewModel by viewModels()
    override fun initView(savedInstanceState: Bundle?) {
        mDatabind.vm = welcomeViewModel
        StatusBarUtil.initStatusView(this)
        StatusBarUtil.setLightMode(this)
    }

    override fun createObserver() {
        countDownTimer = object : CountDownTimer(welcomeViewModel.countTime.get() * 1000L, 1000) {
            override fun onTick(millisUntilFinished: Long) {
                welcomeViewModel.countTime.set((millisUntilFinished / 1000).toInt())
            }

            override fun onFinish() {

                if (CacheUtil.isLogin()) {
                    this@WelcomeActivity.startActivity(
                        Intent(
                            this@WelcomeActivity,
                            MainActivity::class.java
                        )
                    )
                } else {
                    this@WelcomeActivity.startActivity(
                        Intent(
                            this@WelcomeActivity,
                            LoginActivity::class.java
                        )
                    )
                }
                this@WelcomeActivity.finish()

            }

        }.start()
    }

    override fun onDestroy() {
        super.onDestroy()
        countDownTimer?.cancel()
        countDownTimer = null
    }

}