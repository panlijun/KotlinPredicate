package com.kotlin.predicate.app.widget

import android.content.Context
import android.util.AttributeSet as AttributeSet1


class WEqualsHImageView : androidx.appcompat.widget.AppCompatImageView {


    constructor(context: Context) : this(context, null)
    constructor(context: Context, attrs: AttributeSet1?) : this(context, attrs, 0)
    constructor(context: Context, attrs: AttributeSet1?, defStyleAttr: Int) : super(
        context,
        attrs,
        defStyleAttr
    )

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        // 设置View宽高的测量值
        setMeasuredDimension(
            getDefaultSize(0, widthMeasureSpec),
            getDefaultSize(0, heightMeasureSpec)
        );
        // 只有setMeasuredDimension调用之后，才能使用getMeasuredWidth()和getMeasuredHeight()来获取视图测量出的宽高，以此之前调用这两个方法得到的值都会是0
        val childWidthSize = measuredWidth

        // 高度和宽度一样
        val width = MeasureSpec.makeMeasureSpec(
            childWidthSize, MeasureSpec.EXACTLY
        );
        super.onMeasure(width, width);
    }
}