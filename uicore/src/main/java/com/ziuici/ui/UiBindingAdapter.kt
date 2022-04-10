package com.ziuici.ui

import android.text.Editable
import android.text.TextUtils
import android.text.TextWatcher
import androidx.databinding.BindingAdapter
import androidx.databinding.InverseBindingAdapter
import androidx.databinding.InverseBindingListener
import androidx.databinding.adapters.ListenerUtil
import androidx.databinding.adapters.TextViewBindingAdapter
import com.ziuici.ui.from.FromInputItem

object UiBindingAdapter {

    //实现了单向绑定，且已经监听了数据的变化
    @BindingAdapter(value = ["rightText"])
    @JvmStatic
    fun setRightText(fromInput: FromInputItem, text: CharSequence) {
        if (fromInput.rightTextView != null && !TextUtils.equals(//避免无限循环
                fromInput.rightTextView?.text,
                text
            )
        ) {
            fromInput.rightTextView?.text = text
        }
    }

    //反向绑定，但是单独这个不知晓view的变化，无法反应到具体的数据中区
    //需要绑定 rightTextAttrChanged   这个合成事件特性是自动生成的
    @InverseBindingAdapter(attribute = "rightText")
    @JvmStatic
    fun getRightText(fromInput: FromInputItem): String {
        if (fromInput.rightTextView != null) {
            return fromInput.rightTextView?.text.toString()
        }
        return ""
    }

    //配合绑定rightTextAttrChanged
    //rightTextAttrChanged是自定生成的合成事件特性，  属性名字+"AttrChanged"
    @BindingAdapter(value = ["app:rightTextAttrChanged"], requireAll = false)
    @JvmStatic
    fun setListeners(fromInput: FromInputItem, listener: InverseBindingListener) {
        val textWatcher: TextWatcher = object : TextWatcher {
            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {
            }

            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {
                listener.onChange()
            }

            override fun afterTextChanged(s: Editable?) {
            }
        }

        val old = ListenerUtil.trackListener(
            fromInput, textWatcher,
            androidx.databinding.library.baseAdapters.R.id.textWatcher
        )

        if (old != null) {
            fromInput.rightTextView?.removeTextChangedListener(old)
        }
        fromInput.rightTextView?.addTextChangedListener(textWatcher)
    }
}