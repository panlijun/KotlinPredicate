package com.kotlin.predicate.ui.home

import com.chad.library.adapter.base.BaseQuickAdapter
import com.chad.library.adapter.base.viewholder.BaseViewHolder
import com.kotlin.predicate.R
import com.kotlin.predicate.data.model.HomeProject

class HomeChildAdapter() :
    BaseQuickAdapter<HomeProject, BaseViewHolder>(R.layout.item_home_child, ArrayList()) {


    override fun convert(holder: BaseViewHolder, item: HomeProject) {

        if (item.imgRes > 0) {
            holder.setBackgroundResource(R.id.image, item.imgRes)
        }
        holder.setText(R.id.nameTxt, item.name)
    }
}