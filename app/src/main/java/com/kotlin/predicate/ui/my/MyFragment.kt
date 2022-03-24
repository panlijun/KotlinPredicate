package com.kotlin.predicate.ui.my

import android.os.Bundle
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.databinding.MyFragmentBinding

class MyFragment : BaseFragment<MyViewModel,MyFragmentBinding>() {

    companion object {
        fun newInstance() = MyFragment()
    }

    override fun initView(savedInstanceState: Bundle?) {
    }


}