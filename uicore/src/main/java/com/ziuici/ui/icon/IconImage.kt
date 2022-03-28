package com.ziuici.ui.icon

import android.content.Context
import android.util.AttributeSet
import android.util.TypedValue
import android.view.LayoutInflater
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.TextView
import com.ziuici.ui.R
import com.ziuici.ui.dpToPx

class IconImage : LinearLayout {

    @JvmOverloads
    constructor(
        context: Context,
        attrs: AttributeSet,
        defStyleAttr: Int = 0,
    ) : super(context, attrs, defStyleAttr) {
        initView(context, attrs)
    }

    private fun initView(context: Context?, attrs: AttributeSet?) {
        if (context == null || attrs == null) return
        val view = LayoutInflater.from(context).inflate(R.layout.layout_icon_image, this, true)
        val a = context.obtainStyledAttributes(attrs, R.styleable.IconImage)

        val iconSrc = a.getResourceId(R.styleable.IconImage_icon_src, 0)
        val iconWidth = a.getDimensionPixelSize(R.styleable.IconImage_icon_width, 0)
        val iconText = a.getString(R.styleable.IconImage_icon_text)
        val iconTextColor = a.getColor(R.styleable.IconImage_icon_text_color, 0);
        val iconTextSize = a.getDimensionPixelSize(R.styleable.IconImage_icon_text_size, dpToPx(11))
        val iconMargin = a.getDimensionPixelSize(R.styleable.IconImage_icon_margin, dpToPx(10))

        val iconImageView = view.findViewById<ImageView>(R.id.iconImage)
        val iconTextView = view.findViewById<TextView>(R.id.iconTxt)

        if (iconSrc > 0) {
            iconImageView.setImageResource(iconSrc)
        }
        if (iconWidth > 0) {
            val layoutParams = iconImageView.layoutParams
            layoutParams.width = iconWidth
            layoutParams.height = iconWidth
            iconImageView.layoutParams = layoutParams
        }
        iconTextView.text = iconText
        iconTextView.setTextSize(TypedValue.COMPLEX_UNIT_PX, iconTextSize.toFloat())
        if (iconTextColor > 0) {
            iconTextView.setTextColor(iconTextColor)
        }
        iconTextView.setPadding(0, iconMargin, 0, 0)


        a.recycle()

    }


}