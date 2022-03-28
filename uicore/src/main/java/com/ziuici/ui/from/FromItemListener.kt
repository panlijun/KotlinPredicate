package com.ziuici.ui.from

import android.view.View

interface FromItemListener {

    fun checkRequired(): Boolean

    fun getContent(): String

    fun setContent(content: String) {}

    fun setOnClick(click: View.OnClickListener)
}