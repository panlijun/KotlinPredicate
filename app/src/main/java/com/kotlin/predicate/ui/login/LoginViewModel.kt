package com.kotlin.predicate.ui.login

import com.kotlin.predicate.app.appViewModel
import com.mvvm.core.base.viewmodel.BaseViewModel
import com.mvvm.core.callback.databind.BooleanObservableField
import com.mvvm.core.callback.databind.StringObservableField

class LoginViewModel : BaseViewModel() {
    var phone = StringObservableField("18368187701")

    var code = StringObservableField("")

    var password = StringObservableField("")
    var read = BooleanObservableField(false)


    fun login() {
        // mock  by plj
        appViewModel.simulateLogin(phone = phone.get())
    }
}