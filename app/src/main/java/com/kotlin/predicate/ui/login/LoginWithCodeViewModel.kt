package com.kotlin.predicate.ui.login

import com.mvvm.core.base.viewmodel.BaseViewModel
import com.mvvm.core.callback.databind.BooleanObservableField
import com.mvvm.core.callback.databind.StringObservableField

class LoginWithCodeViewModel : BaseViewModel() {
    var phone = StringObservableField("18368187701")

    var code = StringObservableField("")

    var read = BooleanObservableField(false)

}