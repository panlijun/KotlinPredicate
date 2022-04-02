package com.kotlin.predicate.data.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class AccountDTO(
    var id: Long = 0,
    var phone: String = "",
    var password: String = "",
    var freshman: Int = 0,
    var validate: Long = 0,
    var gmtCreate: Long = 0,
    var gmtModified: Boolean = false
) : Parcelable