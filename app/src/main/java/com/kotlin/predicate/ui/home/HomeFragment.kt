package com.kotlin.predicate.ui.home

import android.os.Bundle
import androidx.fragment.app.Fragment
import androidx.viewpager2.adapter.FragmentStateAdapter
import com.google.android.material.tabs.TabLayoutMediator
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.app.ext.setSelectBold
import com.kotlin.predicate.databinding.HomeFragmentBinding

class HomeFragment : BaseFragment<HomeViewModel, HomeFragmentBinding>() {

    var titleData = arrayListOf("体育运动", "趣味运动")

    private var fragments: ArrayList<Fragment> = arrayListOf()

    init {
        fragments.add(HomeChildFragment.newInstance(TYPE_SPORT_NORMAL))
        fragments.add(HomeChildFragment.newInstance(TYPE_SPORT_FUNNY))
    }

    override fun initView(savedInstanceState: Bundle?) {
        mDatabind.viewPage2.isUserInputEnabled = false
        mDatabind.viewPage2.adapter = object : FragmentStateAdapter(this) {
            override fun getItemCount(): Int {
                return fragments.size
            }

            override fun createFragment(position: Int): Fragment {
                return fragments[position]
            }
        }
        TabLayoutMediator(mDatabind.tabLayout, mDatabind.viewPage2) { tab, position ->
            tab.text = titleData[position]
        }.attach()

        mDatabind.tabLayout.setSelectBold()
    }


    override fun createObserver() {
    }
}