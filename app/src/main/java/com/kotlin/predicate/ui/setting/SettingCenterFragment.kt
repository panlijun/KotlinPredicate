package com.kotlin.predicate.ui.setting

import android.content.Intent
import android.os.Bundle
import com.kotlin.predicate.app.EmptyViewModel
import com.kotlin.predicate.app.appViewModel
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.app.ext.initClose
import com.kotlin.predicate.databinding.SettingCenterFragmentBinding
import com.kotlin.predicate.ui.LoginActivity
import com.mvvm.core.ext.nav

class SettingCenterFragment : BaseFragment<EmptyViewModel, SettingCenterFragmentBinding>() {
    override fun initView(savedInstanceState: Bundle?) {

        mDatabind.includeToolbar.toolbar.initClose() {
            nav().navigateUp()
        }
        mDatabind.includeToolbar.toolbarTitle.text = "设置中心"
        mDatabind.loginOutText.setOnClickListener {
            appViewModel.loginOut()
            activity?.let {
                it.startActivity(Intent(it, LoginActivity::class.java))
                it.finish()
            }
        }
    }
}