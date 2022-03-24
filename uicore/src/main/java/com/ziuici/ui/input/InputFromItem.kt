package com.ziuici.ui.input

import android.content.Context
import android.util.AttributeSet
import android.util.TypedValue
import android.view.LayoutInflater
import android.view.View
import android.widget.RelativeLayout
import androidx.appcompat.widget.AppCompatEditText
import androidx.appcompat.widget.AppCompatImageView
import androidx.appcompat.widget.AppCompatTextView
import androidx.appcompat.widget.LinearLayoutCompat
import com.ziuici.ui.R
import com.ziuici.ui.dpToPx

class InputFromItem : RelativeLayout {
    var leftTextView: AppCompatTextView? = null
    var rightEdt: AppCompatEditText? = null

    @JvmOverloads
    constructor(
        context: Context?,
        attrs: AttributeSet? = null,
        defStyleAttr: Int = 0,
        defStyleRes: Int = 0
    ) : super(context, attrs, defStyleAttr, defStyleRes) {
        initView(context, attrs)
    }


    private fun initView(context: Context?, attrs: AttributeSet? = null) {
        if (context == null || attrs == null) return
        val view = LayoutInflater.from(context).inflate(R.layout.layout_input_from_item, this, true)

        val a = context.obtainStyledAttributes(attrs, R.styleable.InputFromItem)
        val leftText = a.getString(R.styleable.InputFromItem_leftText)
        val leftSize = a.getDimensionPixelSize(R.styleable.InputFromItem_leftSize, dpToPx(15))
        val leftColor = a.getColor(
            R.styleable.InputFromItem_leftColor,
            resources.getColor(R.color.ui_text_color)
        )
        val rightText = a.getText(R.styleable.InputFromItem_rightText)
        val hintText = a.getText(R.styleable.InputFromItem_rightHint)
        val rightSize = a.getDimensionPixelSize(R.styleable.InputFromItem_rightSize, dpToPx(14))
        val rightColor = a.getColor(
            R.styleable.InputFromItem_rightColor,
            resources.getColor(R.color.ui_text_color)
        )
        val hintColor = a.getColor(
            R.styleable.InputFromItem_hintColor,
            resources.getColor(R.color.ui_hint_clor)
        )
        val showLine = a.getBoolean(R.styleable.InputFromItem_showLine, true)
        val isSelect = a.getBoolean(R.styleable.InputFromItem_isSelect, false)

        leftTextView = view.findViewById<AppCompatTextView>(R.id.text_left)
        rightEdt = view.findViewById<AppCompatEditText>(R.id.edt_right)
        val lLayout = view.findViewById<LinearLayoutCompat>(R.id.lLayout)
        val arrow = view.findViewById<AppCompatImageView>(R.id.arrow)
        val line = view.findViewById<View>(R.id.line)


        rightEdt?.hint = hintText ?: ""
        rightEdt?.setHintTextColor(hintColor)
        rightEdt?.setText(rightText)
        rightEdt?.setTextColor(rightColor)
        rightEdt?.setTextSize(TypedValue.COMPLEX_UNIT_PX, rightSize.toFloat())
        leftTextView?.text = leftText
        leftTextView?.setTextSize(TypedValue.COMPLEX_UNIT_PX, leftSize.toFloat())
        leftTextView?.setTextColor(leftColor)


        line.visibility = if (showLine) VISIBLE else GONE
        if (isSelect) {
            arrow.visibility = VISIBLE
            rightEdt?.isEnabled = false
        } else {
            arrow.visibility = GONE
            rightEdt?.isEnabled = true

        }
        a.recycle()
    }
}

