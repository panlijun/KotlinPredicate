# 整体包含
- fragment和activity的base实现， 以及viewBinding和dataBinding支持
- viewModel和liveData支持
- lifecycle支持
- 增加了一些扩展函数
- okhttp和retrofit
- navigation支持


# base
-  BaseVmActivity/BaseVmDbActivity/BaseVmVbActivity:分别是  无viewBinding/dataBinding/viewBinding支持的Activity
-  BaseVmFragment/BaseVmDbFragment/BaseVmVbFragment:分别是  无viewBinding/dataBinding/viewBinding支持的Fragment
-  BaseViewModel 
-  Ktx/BaseApp方便统一处理获取Application

# callBack
特别处理了各数据类型的返回值，避免Null

# ext
扩展函数
- view（点击事件，文字）
- string
- navigation
- viewBinding
- Context（系统）

# network

