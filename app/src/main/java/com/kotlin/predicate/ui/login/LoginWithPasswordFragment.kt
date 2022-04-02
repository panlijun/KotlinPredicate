package com.kotlin.predicate.ui.login

import android.os.Bundle
import android.text.method.HideReturnsTransformationMethod
import android.text.method.PasswordTransformationMethod
import androidx.fragment.app.viewModels
import androidx.lifecycle.Observer
import com.kotlin.predicate.R
import com.kotlin.predicate.app.appViewModel
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.databinding.LoginWithPasswordFragmentBinding
import com.mvvm.core.ext.nav
import com.mvvm.core.ext.navigateAction

class LoginWithPasswordFragment : BaseFragment<LoginViewModel, LoginWithPasswordFragmentBinding>() {
    val viewModel: LoginViewModel by viewModels()

    override fun initView(savedInstanceState: Bundle?) {
        mDatabind.vm = viewModel

        mDatabind.imageRead.setOnClickListener {
            viewModel.read.set(!viewModel.read.get())
        }

        mDatabind.btnLogin.setOnClickListener {
            viewModel.login()
        }
        mDatabind.toLoginCode.setOnClickListener {
            nav().navigateAction(R.id.action_loginWithPasswordFragment_to_loginWithCodeFragment)
        }

        mDatabind.btnLogin.setOnClickListener {

        }

        mDatabind.imgShowPassword.setOnClickListener {
            if (mDatabind.edtPassword.transformationMethod == HideReturnsTransformationMethod.getInstance()) {
                mDatabind.edtPassword.transformationMethod =
                    PasswordTransformationMethod.getInstance()
            } else {
                mDatabind.edtPassword.transformationMethod =
                    HideReturnsTransformationMethod.getInstance()
            }
        }

    }

    override fun createObserver() {
        appViewModel.loginInfo.observeInFragment(this, Observer {
            if (it != null && it.userDTO == null) {
                nav().navigateAction(R.id.action_loginWithPasswordFragment_to_selectRoleFragment)
            }
        })
    }
}