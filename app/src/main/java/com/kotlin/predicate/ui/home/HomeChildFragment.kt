package com.kotlin.predicate.ui.home

import android.os.Bundle
import androidx.fragment.app.viewModels
import androidx.lifecycle.Observer
import androidx.recyclerview.widget.GridLayoutManager
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.databinding.HomeChildFragmentBinding

class HomeChildFragment : BaseFragment<HomeChildViewModel, HomeChildFragmentBinding>() {

    private val viewModel: HomeChildViewModel by viewModels()


    private val adapter: HomeChildAdapter by lazy {
        HomeChildAdapter()
    }

    override fun initView(savedInstanceState: Bundle?) {

        mDatabind.recycleView.adapter = adapter
        mDatabind.recycleView.layoutManager = GridLayoutManager(context, 2);

        viewModel.items.observe(viewLifecycleOwner, Observer {
            adapter.setNewInstance(it)
        })

        viewModel.loadData((arguments?.get("type") ?: TYPE_SPORT_NORMAL) as Int)
    }

    companion object {
        fun newInstance(type: Int): HomeChildFragment {
            val args = Bundle()
            args.putInt("type", type)
            val fragment = HomeChildFragment()
            fragment.arguments = args
            return fragment
        }
    }
}