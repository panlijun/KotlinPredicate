package com.ziuici.ui

import android.content.Context
import android.graphics.drawable.Drawable
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import android.view.View.OnTouchListener
import androidx.appcompat.widget.AppCompatEditText


class EditTextWithClearIcon : AppCompatEditText, OnTouchListener {
    // 删除符号
    private val deleteImage: Drawable = resources.getDrawable(R.drawable.ico_dele, null)
    var icon: Drawable? = null

    constructor(context: Context?, attrs: AttributeSet?, defStyle: Int) : super(
        context!!, attrs, defStyle
    ) {
        init()
    }

    constructor(context: Context?, attrs: AttributeSet?) : super(
        context!!, attrs
    ) {
        init()
    }

    constructor(context: Context?) : super(context!!) {
        init()
    }

    private fun init() {
        setOnTouchListener(this)
        deleteImage.setBounds(0, 0, deleteImage.intrinsicWidth, deleteImage.intrinsicHeight)
        manageClearButton()
    }


    private fun manageClearButton() {
        if (this.text.toString() == "") removeClearButton() else addClearButton()
    }

    private fun removeClearButton() {
        setCompoundDrawables(icon, this.compoundDrawables[1], null, this.compoundDrawables[3])
    }

    private fun addClearButton() {
        setCompoundDrawables(
            icon, this.compoundDrawables[1], deleteImage,
            this.compoundDrawables[3]
        )
    }

    override fun onTouch(v: View, event: MotionEvent): Boolean {
        val et = this@EditTextWithClearIcon
        if (et.compoundDrawables[2] == null) return false
        if (event.action != MotionEvent.ACTION_UP) return false
        if (event.x > et.width - et.paddingRight - deleteImage.intrinsicWidth) {
            et.setText("")
            removeClearButton()
        }
        return false
    }
}