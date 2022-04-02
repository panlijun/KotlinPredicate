package com.kotlin.predicate.ui.login

import android.os.Bundle
import androidx.fragment.app.viewModels
import androidx.lifecycle.Observer
import com.kotlin.predicate.R
import com.kotlin.predicate.app.appViewModel
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.databinding.LoginWithCodeFragmentBinding
import com.mvvm.core.ext.nav
import com.mvvm.core.ext.navigateAction

class LoginWithCodeFragment : BaseFragment<LoginViewModel, LoginWithCodeFragmentBinding>() {

    val viewModel: LoginViewModel by viewModels()

    companion object {
        fun newInstance() = LoginWithCodeFragment()
    }

    override fun initView(savedInstanceState: Bundle?) {
        mDatabind.vm = viewModel
        mDatabind.btnLogin.setOnClickListener {
            viewModel.login()
        }

        mDatabind.imageRead.setOnClickListener { viewModel.read.set(!viewModel.read.get()) }
        mDatabind.toLoginPassword.setOnClickListener {
            nav()
                .navigateAction(R.id.action_loginWithCodeFragment_to_loginWithPasswordFragment)
        }
    }

    override fun createObserver() {
        appViewModel.loginInfo.observeInFragment(this, Observer {
            if (it != null && it.userDTO == null) {
                nav().navigateAction(
                    R.id.action_loginWithCodeFragment_to_selectRoleFragment
                )
            }
        })
    }

}