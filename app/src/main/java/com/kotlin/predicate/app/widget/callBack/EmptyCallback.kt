package com.kotlin.predicate.app.widget.callBack


import com.kingja.loadsir.callback.Callback
import com.kotlin.predicate.R


class EmptyCallback : Callback() {

    override fun onCreateView(): Int {
        return R.layout.layout_empty
    }

}