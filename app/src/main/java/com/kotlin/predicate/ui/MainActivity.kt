package com.kotlin.predicate.ui

import android.os.Bundle
import androidx.activity.OnBackPressedCallback
import androidx.navigation.Navigation
import com.blankj.utilcode.util.ToastUtils
import com.kotlin.predicate.R
import com.kotlin.predicate.app.base.BaseActivity
import com.kotlin.predicate.databinding.MainActivityBinding

class MainActivity : BaseActivity<MainActivityViewModel, MainActivityBinding>() {

    var lastExitTime = 0L
    override fun initView(savedInstanceState: Bundle?) {

        onBackPressedDispatcher.addCallback(
            this,
            object : OnBackPressedCallback(true) {
                override fun handleOnBackPressed() {
                    val nav = Navigation.findNavController(this@MainActivity, R.id.host_fragment);
                    if (nav.currentDestination != null && nav.currentDestination!!.id != R.id.mainFragment) {
                        nav.navigateUp();
                    } else {
                        if (System.currentTimeMillis() - lastExitTime > 2_000) {
                            ToastUtils.showShort("再按一次退出程序");
                            lastExitTime = System.currentTimeMillis();
                        } else {
                            finish()
                        }
                    }
                }
            });
    }

    override fun createObserver() {
    }

}