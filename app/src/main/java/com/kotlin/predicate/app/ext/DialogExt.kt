package com.kotlin.predicate.app.ext

import android.view.View
import android.widget.TextView
import com.afollestad.materialdialogs.MaterialDialog
import com.kotlin.predicate.R


fun MaterialDialog.initCommon(
    title: String,
    message: String,
    cancel: View.OnClickListener?,
    confirm: View.OnClickListener?,
) {
    val contentView = this.layoutInflater.inflate(R.layout.dialog_common_message, null)
    val titleView = contentView.findViewById<TextView>(R.id.title)
    val messageView = contentView.findViewById<TextView>(R.id.message)
    val cancelView = contentView.findViewById<TextView>(R.id.cancel)
    val confirmView = contentView.findViewById<TextView>(R.id.confirm)

    cancelView.setOnClickListener {
        cancel?.onClick(it)
        dismiss()
    }

    confirmView.setOnClickListener {
        confirm?.onClick(it)
        dismiss()
    }
    titleView.text = title
    messageView.text = message

    setContentView(contentView)
}