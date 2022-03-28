package com.kotlin.predicate.ui.my

import android.os.Bundle
import android.view.View
import com.afollestad.materialdialogs.MaterialDialog
import com.afollestad.materialdialogs.actions.getActionButton
import com.blankj.utilcode.util.ToastUtils
import com.kotlin.predicate.R
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.app.ext.initCommon
import com.kotlin.predicate.databinding.MyFragmentBinding

class MyFragment : BaseFragment<MyViewModel, MyFragmentBinding>() {


    private val notOpenClick : View.OnClickListener = View.OnClickListener {
        ToastUtils.showShort("开发中，敬请期待！")
    }
    companion object {
        fun newInstance() = MyFragment()
    }

    override fun initView(savedInstanceState: Bundle?) {
        mDatabind.iconHomework.setOnClickListener(notOpenClick)
        mDatabind.iconHomework.setOnClickListener(notOpenClick)
        mDatabind.iconHomework.setOnClickListener(notOpenClick)


        mDatabind.fromHealth.setOnClick(notOpenClick)
        mDatabind.fromRecommend.setOnClick(notOpenClick)
        mDatabind.fromAchievement.setOnClick(notOpenClick)
        mDatabind.fromHelp.setOnClick(notOpenClick)
        mDatabind.fromSetting.setOnClick(notOpenClick)

    }


}