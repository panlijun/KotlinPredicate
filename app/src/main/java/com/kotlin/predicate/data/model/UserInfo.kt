package com.kotlin.predicate.data.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize


@Parcelize
data class UserInfo(
    var id: Long = 0,
    var name: String = "",
    var nickname: String = "",
    var gender: Int = 0,
    var role: Int = 0,
    var IDNum: String = "",
    var school: String = "",
    var classId: String = "",
    var validate: Long = 0,
    var accountId: Long = 0,
    var choose: Int = 0,
    var gmtCreate: Long = 0,
    var gmtModified: String = "",
    var level: Int = 0,
    var expr: Int = 0,
    var schoolId: Int = 0,

    ) : Parcelable
