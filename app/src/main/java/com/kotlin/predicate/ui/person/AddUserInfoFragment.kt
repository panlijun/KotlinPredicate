package com.kotlin.predicate.ui.person

import android.os.Bundle
import androidx.fragment.app.viewModels
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.databinding.AddUserInfoFragmentBinding

class AddUserInfoFragment : BaseFragment<AddUserInfoViewModel, AddUserInfoFragmentBinding>() {

    val viewModel: AddUserInfoViewModel by viewModels()
    override fun initView(savedInstanceState: Bundle?) {
        mDatabind.btnSubmit.setOnClickListener {
            viewModel.name.set(mDatabind.name.getContent())
            viewModel.nickname.set(mDatabind.nickName.getContent())
            viewModel.cardNo.set(mDatabind.cardNo.getContent())
            viewModel.classId.set(mDatabind.classId.getContent())
            viewModel.addUser()
        }
    }
}