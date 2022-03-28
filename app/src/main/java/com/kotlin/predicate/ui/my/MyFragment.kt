package com.kotlin.predicate.ui.my

import android.os.Bundle
import com.afollestad.materialdialogs.MaterialDialog
import com.afollestad.materialdialogs.actions.getActionButton
import com.kotlin.predicate.R
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.app.ext.initCommon
import com.kotlin.predicate.databinding.MyFragmentBinding

class MyFragment : BaseFragment<MyViewModel, MyFragmentBinding>() {

    var dialog: MaterialDialog? = null;

    companion object {
        fun newInstance() = MyFragment()
    }

    override fun initView(savedInstanceState: Bundle?) {

        mDatabind.fromSecond.setOnClick {

            if (dialog == null) {
                dialog = this@MyFragment.context?.let { it1 -> MaterialDialog(it1) }
                dialog?.initCommon("提醒", "删除成员后，所绑定的学校班级、记录 等都将删除，是否确认删除",
                    null, null)
            }
            dialog?.show { }
//            dialog.set
//            mDatabind.fromName.checkRequired()
//            mDatabind.fromHead.setContent("https://img1.baidu.com/it/u=3485632422,576867562&fm=253&fmt=auto&app=138&f=JPEG?w=750&h=500")
        }
    }


}