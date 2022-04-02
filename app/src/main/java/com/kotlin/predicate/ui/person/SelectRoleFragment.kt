package com.kotlin.predicate.ui.person

import android.os.Bundle
import com.mvvm.core.ext.nav
import com.mvvm.core.ext.navigateAction
import com.kotlin.predicate.R
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.databinding.SelectRoleFragmentBinding
import com.kotlin.predicate.ui.person.SelectRoleViewModel

class SelectRoleFragment:BaseFragment<SelectRoleViewModel,SelectRoleFragmentBinding> (){
    override fun initView(savedInstanceState: Bundle?) {
        mDatabind.tvStudents.setOnClickListener {
            nav().navigateAction(R.id.action_selectRoleFragment_to_addUserInfoFragment)
        }
        mDatabind.tvTeacher.setOnClickListener {
            nav().navigateAction(R.id.action_selectRoleFragment_to_addUserInfoFragment)
        }
    }
}