package com.kotlin.predicate.ui.main

import android.os.Bundle
import androidx.fragment.app.Fragment
import androidx.viewpager2.adapter.FragmentStateAdapter
import com.kotlin.predicate.R
import com.kotlin.predicate.app.base.BaseFragment
import com.kotlin.predicate.app.ext.interceptLongClick
import com.kotlin.predicate.databinding.MainFragmentBinding
import com.kotlin.predicate.ui.home.HomeFragment
import com.kotlin.predicate.ui.my.MyFragment

class MainFragment : BaseFragment<MainViewModel, MainFragmentBinding>() {

    override fun initView(savedInstanceState: Bundle?) {


        mDatabind.viewPager2.isUserInputEnabled =false
        mDatabind.viewPager2.adapter = object : FragmentStateAdapter(this) {
            override fun getItemCount(): Int {
                return 2
            }

            override fun createFragment(position: Int): Fragment {
                return when (position) {

                    0 -> {
                        HomeFragment()
                    }
                    1 -> {
                        MyFragment()
                    }
                    else -> {
                        HomeFragment()
                    }
                }
            }

        }

        mDatabind.mainBottomView.setOnNavigationItemSelectedListener {
            when (it.itemId) {
                R.id.menu_main -> mDatabind.viewPager2.setCurrentItem(0, false);
                R.id.menu_me -> mDatabind.viewPager2.setCurrentItem(1, false);
                else -> {
                }
            }
            true
        }
    }


}