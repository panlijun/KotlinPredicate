package com.kotlin.predicate.ui.home

import android.widget.ImageView
import com.chad.library.adapter.base.BaseQuickAdapter
import com.chad.library.adapter.base.viewholder.BaseViewHolder
import com.kotlin.predicate.R
import com.kotlin.predicate.data.model.HomeProject
import com.mvvm.core.ext.util.dp2px

class HomeChildAdapter() :
    BaseQuickAdapter<HomeProject, BaseViewHolder>(R.layout.item_home_child, ArrayList()) {


    override fun convert(holder: BaseViewHolder, item: HomeProject) {

        if (item.imgRes > 0) {
            holder.setBackgroundResource(R.id.image, item.imgRes)
        }

        val img = holder.getView<ImageView>(R.id.image);
        val layoutParams = img.layoutParams;
        layoutParams.height = context.resources.displayMetrics.widthPixels / 2 - context.dp2px(20)
        img.layoutParams = layoutParams

        holder.setText(R.id.nameTxt, item.name)
    }
}