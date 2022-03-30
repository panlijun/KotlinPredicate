package com.kotlin.predicate.ui.login

import android.os.Bundle
import androidx.fragment.app.viewModels
import com.blankj.utilcode.util.ToastUtils
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.databinding.LoginWithCodeFragmentBinding

class LoginWithCodeFragment : BaseFragment<LoginWithCodeViewModel, LoginWithCodeFragmentBinding>() {

    val viewModel :LoginWithCodeViewModel by viewModels()
    companion object {
        fun newInstance() = LoginWithCodeFragment()
    }

    override fun initView(savedInstanceState: Bundle?) {
        mDatabind.vm =viewModel
        mDatabind.btnLogin.setOnClickListener {
            ToastUtils.showShort("1111")
        }

        mDatabind.imageRead.setOnClickListener { viewModel.read.set(!viewModel.read.get()) }
    }


}