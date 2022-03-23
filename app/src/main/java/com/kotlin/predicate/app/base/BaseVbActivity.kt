package com.kotlin.predicate.app.base

import android.os.Bundle
import androidx.databinding.ViewDataBinding
import com.kotlin.predicate.app.ext.dismissLoadingExt
import com.kotlin.predicate.app.ext.showLoadingExt
import com.mvvm.core.base.activity.BaseVmDbActivity
import com.mvvm.core.base.activity.BaseVmVbActivity
import com.mvvm.core.base.viewmodel.BaseViewModel

abstract class BaseVbActivity<VM : BaseViewModel, DB : ViewDataBinding> :
    BaseVmVbActivity<VM, DB>() {

    abstract override fun initView(savedInstanceState: Bundle?);

    override fun showLoading(message: String) {
        showLoadingExt(message);
    }

    override fun dismissLoading() {
        dismissLoadingExt();
    }
}