package com.kotlin.predicate.ui.my

import android.os.Bundle
import android.view.View
import com.blankj.utilcode.util.ToastUtils
import com.kotlin.predicate.R
import com.kotlin.predicate.app.appViewModel
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.databinding.MyFragmentBinding
import com.mvvm.core.ext.nav
import com.mvvm.core.ext.navigateAction

class MyFragment : BaseFragment<MyViewModel, MyFragmentBinding>() {


    private val notOpenClick: View.OnClickListener = View.OnClickListener {
        ToastUtils.showShort("开发中，敬请期待！")
    }

    companion object {
        fun newInstance() = MyFragment()
    }

    override fun initView(savedInstanceState: Bundle?) {
        mDatabind.iconSportReport.setOnClickListener(notOpenClick)
        mDatabind.iconHomework.setOnClickListener(notOpenClick)
        mDatabind.iconPlan.setOnClickListener(notOpenClick)


        mDatabind.fromHealth.setOnClick(notOpenClick)
        mDatabind.fromRecommend.setOnClick(notOpenClick)
        mDatabind.fromAchievement.setOnClick(notOpenClick)
        mDatabind.fromHelp.setOnClick(notOpenClick)
        mDatabind.fromSetting.setOnClick {
            nav().navigateAction(R.id.action_mainFragment_to_SettingFragment)
        }

        appViewModel.userInfo?.let {
            mDatabind.nameTxt.text = it.value?.nickname
            mDatabind.classText.text = it.value?.classId
        }

    }


}