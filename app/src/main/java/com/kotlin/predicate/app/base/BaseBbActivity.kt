package com.kotlin.predicate.app.base

import android.os.Bundle
import androidx.databinding.ViewDataBinding
import com.mvvm.core.base.activity.BaseVmDbActivity
import com.mvvm.core.base.viewmodel.BaseViewModel

abstract class BaseBbActivity<VM : BaseViewModel, DB : ViewDataBinding> :
    BaseVmDbActivity<VM, DB>() {

    abstract override fun initView(savedInstanceState: Bundle?);

    override fun showLoading(message: String) {
        TODO("Not yet implemented")
    }

    override fun dismissLoading() {
        TODO("Not yet implemented")
    }
}