package com.ziuici.ui.from

import android.content.Context
import android.util.AttributeSet
import android.util.TypedValue
import android.view.LayoutInflater
import android.view.View
import android.widget.ImageView
import android.widget.RelativeLayout
import android.widget.TextView
import androidx.appcompat.widget.AppCompatTextView
import androidx.appcompat.widget.LinearLayoutCompat
import com.bumptech.glide.Glide
import com.ziuici.ui.R
import com.ziuici.ui.dpToPx

class FromImageItem : RelativeLayout, FromItemListener {
    var leftTextView: TextView? = null
    var line: View? = null
    var lLayout: LinearLayoutCompat? = null
    var image: ImageView? = null

    var showRed: Boolean = false
    var mustRequired: Boolean = true
    var imgUrl: String = ""

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
        val view = LayoutInflater.from(context).inflate(R.layout.layout_image_from_item, this, true)

        val a = context.obtainStyledAttributes(attrs, R.styleable.FromImageItem)
        val leftText = a.getString(R.styleable.FromImageItem_leftText)
        val leftSize = a.getDimensionPixelSize(R.styleable.FromImageItem_leftSize, dpToPx(15))
        val leftColor = a.getColor(
            R.styleable.FromImageItem_leftColor,
            resources.getColor(R.color.ui_title_color)
        )

        val showLine = a.getBoolean(R.styleable.FromImageItem_showLine, true)

        leftTextView = view.findViewById<AppCompatTextView>(R.id.text_left)
        lLayout = view.findViewById<LinearLayoutCompat>(R.id.lLayout)
        image = view.findViewById<ImageView>(R.id.imageView)

        line = view.findViewById<View>(R.id.line)


        leftTextView?.text = leftText
        leftTextView?.setTextSize(TypedValue.COMPLEX_UNIT_PX, leftSize.toFloat())
        leftTextView?.setTextColor(leftColor)
        line?.visibility = if (showLine) VISIBLE else GONE
        a.recycle()
    }


    override fun checkRequired(): Boolean {
        if (!mustRequired) {
            return true
        }
        if (imgUrl.isNotEmpty()) {
            return true
        }
        showRed = true
        line?.setBackgroundColor(resources.getColor(R.color.ui_red_color))
        return false
    }


    override fun setOnClick(click: OnClickListener) {
        lLayout?.setOnClickListener(click)
    }

    override fun getContent(): String {
        return imgUrl
    }

    override fun setContent(content: String) {
        imgUrl = content
        image?.let { Glide.with(this).load(content).into(it) }
        if (showRed && content.isNotEmpty()) {
            showRed = false
            line?.setBackgroundColor(resources.getColor(R.color.ui_line_color))
        }
    }
}

