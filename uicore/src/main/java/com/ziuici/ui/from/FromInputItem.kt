package com.ziuici.ui.from

import android.content.Context
import android.util.AttributeSet
import android.util.TypedValue
import android.view.LayoutInflater
import android.view.View
import android.widget.*
import androidx.appcompat.widget.AppCompatEditText
import androidx.appcompat.widget.AppCompatImageView
import androidx.appcompat.widget.AppCompatTextView
import androidx.appcompat.widget.LinearLayoutCompat
import androidx.core.widget.doAfterTextChanged
import com.ziuici.ui.R
import com.ziuici.ui.dpToPx

class FromInputItem : RelativeLayout, FromItemListener {
    var leftTextView: TextView? = null
    var rightTextView: TextView? = null
    var line: View? = null
    var lLayout: LinearLayout? = null
    var showRed: Boolean = false
    var mustRequired: Boolean = true

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

        val a = context.obtainStyledAttributes(attrs, R.styleable.FromInputItem)
        val leftText = a.getString(R.styleable.FromInputItem_leftText)
        val leftSize = a.getDimensionPixelSize(R.styleable.FromInputItem_leftSize, dpToPx(15))
        val leftColor = a.getColor(
            R.styleable.FromInputItem_leftColor,
            resources.getColor(R.color.ui_title_color)
        )
        val rightText = a.getText(R.styleable.FromInputItem_rightText)
        val hintText = a.getText(R.styleable.FromInputItem_rightHint)
        val rightSize = a.getDimensionPixelSize(R.styleable.FromInputItem_rightSize, dpToPx(14))
        val rightColor = a.getColor(
            R.styleable.FromInputItem_rightColor,
            resources.getColor(R.color.ui_title_color)
        )
        val hintColor = a.getColor(
            R.styleable.FromInputItem_hintColor,
            resources.getColor(R.color.ui_hint_color)
        )
        val showLine = a.getBoolean(R.styleable.FromInputItem_showLine, true)
        val isSelect = a.getBoolean(R.styleable.FromInputItem_isSelect, false)

        val leftImage = a.getResourceId(R.styleable.FromInputItem_leftImage, 0);

        leftTextView = view.findViewById<AppCompatTextView>(R.id.text_left)
        val edtRight = view.findViewById<AppCompatEditText>(R.id.edt_right)
        val textRight = view.findViewById<AppCompatTextView>(R.id.text_right)

        lLayout = view.findViewById<LinearLayout>(R.id.lLayout)
        val arrow = view.findViewById<AppCompatImageView>(R.id.arrow)
        line = view.findViewById<View>(R.id.line)
        val leftImg = view.findViewById<ImageView>(R.id.leftImg);

        if (isSelect) {
            rightTextView = textRight
            edtRight.visibility = GONE
            textRight.visibility = VISIBLE
        } else {
            rightTextView = edtRight
            edtRight.visibility = VISIBLE
            textRight.visibility = GONE
        }
        rightTextView?.hint = hintText ?: ""
        rightTextView?.setHintTextColor(hintColor)
        rightTextView?.text = rightText
        rightTextView?.setTextColor(rightColor)
        rightTextView?.setTextSize(TypedValue.COMPLEX_UNIT_PX, rightSize.toFloat())
        leftTextView?.text = leftText
        leftTextView?.setTextSize(TypedValue.COMPLEX_UNIT_PX, leftSize.toFloat())
        leftTextView?.setTextColor(leftColor)
        rightTextView?.doAfterTextChanged {
            if (showRed && it?.length!! > 0) {
                showRed = false
                line?.setBackgroundColor(resources.getColor(R.color.ui_line_color))
                rightTextView?.setHintTextColor(hintColor)
            }
        }

        line?.visibility = if (showLine) VISIBLE else GONE
        if (isSelect) {
            arrow.visibility = VISIBLE
            rightTextView?.isEnabled = false
            rightTextView?.isClickable = false
            rightTextView?.isFocusableInTouchMode = false
        } else {
            arrow.visibility = GONE
            rightTextView?.isEnabled = true
        }

        if (leftImage > 0) {
            leftImg.visibility = VISIBLE
            leftImg.setImageResource(leftImage);
        } else {
            leftImg.visibility = GONE
        }

        a.recycle()
    }


    override fun checkRequired(): Boolean {
        if (!mustRequired) {
            return true
        }
        if (rightTextView?.text?.length!! > 0) {
            return true
        }
        showRed = true
        line?.setBackgroundColor(resources.getColor(R.color.ui_red_color))
        rightTextView?.setHintTextColor(resources.getColor(R.color.ui_red_color))
        return false
    }


    override fun setOnClick(click: OnClickListener) {
        lLayout?.setOnClickListener(click)
    }

    override fun getContent(): String {
        return rightTextView?.text?.toString() ?: ""
    }

    override fun setContent(content: CharSequence) {
        rightTextView?.text = content
    }
}

