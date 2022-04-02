package com.kotlin.predicate.ui

import android.content.Intent
import android.os.Bundle
import androidx.activity.OnBackPressedCallback
import androidx.lifecycle.Observer
import androidx.navigation.Navigation
import com.kotlin.predicate.R
import com.kotlin.predicate.app.EmptyViewModel
import com.kotlin.predicate.app.appViewModel
import com.kotlin.predicate.app.base.BaseActivity
import com.kotlin.predicate.app.util.StatusBarUtil
import com.kotlin.predicate.databinding.LoginActivityBinding

class LoginActivity : BaseActivity<EmptyViewModel, LoginActivityBinding>() {
    override fun initView(savedInstanceState: Bundle?) {
        onBackPressedDispatcher.addCallback(
            this,
            object : OnBackPressedCallback(true) {
                override fun handleOnBackPressed() {
                    val nav = Navigation.findNavController(this@LoginActivity, R.id.host_fragment);
                    if (!nav.navigateUp()) {
                        finish()
                    }
                }
            });

        StatusBarUtil.initStatusView(this)
        StatusBarUtil.setLightMode(this)

    }

    override fun createObserver() {
        appViewModel.userInfo.observeInActivity(this, Observer {
            if (it != null) {
                this.startActivity(
                    Intent(
                        this,
                        MainActivity::class.java
                    )
                )
                this.finish()
            }
        })
    }
}