package com.kotlin.predicate.ui.my

import android.os.Bundle
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.databinding.MyFragmentBinding

class MyFragment : BaseFragment<MyViewModel, MyFragmentBinding>() {

    companion object {
        fun newInstance() = MyFragment()
    }

    override fun initView(savedInstanceState: Bundle?) {

        mDatabind.fromSecond.setOnClick {
            mDatabind.fromName.checkRequired()
            mDatabind.fromHead.setContent("https://img1.baidu.com/it/u=3485632422,576867562&fm=253&fmt=auto&app=138&f=JPEG?w=750&h=500")
        }
    }


}