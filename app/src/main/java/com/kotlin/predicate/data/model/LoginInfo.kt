package com.kotlin.predicate.data.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class LoginInfo(
    var token: String = "",
    var accountDTO: AccountDTO = AccountDTO(),
    var userDTO: UserInfo? = null
) : Parcelable
