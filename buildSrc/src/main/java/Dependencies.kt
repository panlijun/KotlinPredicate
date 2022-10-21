object Versions {
    const val compileSdk = 31
    const val minSdk = 24
    const val targetSdk = 31
    const val buildToolsVersion = "31.0.0"


    const val core_ktx_version = "1.7.0"
    const val androidx_appcompat = "1.7.0"
    const val material = "1.5.0"
    const val multidex = "2.0.1"
    const val exifinter = "1.3.3"
    const val fragment_ktx = "1.4.1"

    const val androidx_lifecycle = "2.4.1"
    const val lifecycle_extensions = "2.2.0"

    const val camerax_version = "1.0.2"
    const val camerax_view = "1.0.0-alpha32"

    const val constraint_version = "2.1.3"
    const val retrofit = "2.9.0"
    const val rxjava = "2.1.9"
    const val glide = "4.11.0"
    const val material_dialogs = "3.3.0"
    const val picture_selector = "3.0.9"
    const val agent_web = "v4.1.9-androidx"
    const val ffmeng = "1.0.15"
    const val picker = "4.1.7"
    const val unpeek_livedata = "4.4.1-beta1"
}

object Libs {
    const val ktxCore = "androidx.core:core-ktx:${Versions.core_ktx_version}"
    const val androidx_appcompat = "androidx.appcompat:appcompat:${Versions.androidx_appcompat}"
    const val androidx_material = "com.google.android.material:material:${Versions.material}"
    const val androidx_multidex = "androidx.multidex:multidex:${Versions.multidex}"
    const val androidx_exifinterface = "androidx.exifinterface:exifinterface:${Versions.exifinter}"

    const val fragment_ktx = "androidx.fragment:fragment-ktx:${Versions.fragment_ktx}"

    //    lifecycle  viewmodel liveData
    const val lifecycle_ktx =
        "androidx.lifecycle:lifecycle-runtime-ktx:${Versions.androidx_lifecycle}"
    const val lifecycle_java =
        "androidx.lifecycle:lifecycle-common-java8:${Versions.androidx_lifecycle}"
    const val lifecycle_extensions =
        "androidx.lifecycle:lifecycle-extensions:${Versions.lifecycle_extensions}"
    const val viewmodel_ktx =
        "androidx.lifecycle:lifecycle-viewmodel-ktx:${Versions.androidx_lifecycle}"
    const val liveData_ktx =
        "androidx.lifecycle:lifecycle-livedata-ktx:${Versions.androidx_lifecycle}"

    //camerax
    const val camera_core = "androidx.camera:camera-core:${Versions.camerax_version}"
    const val camera_camera2 = "androidx.camera:camera-camera2:${Versions.camerax_version}"
    const val camera_lifecycle = "androidx.camera:camera-lifecycle:${Versions.camerax_version}"
    const val camera_view = "androidx.camera:camera-view:${Versions.camerax_view}"

    const val constraint_layout =
        "androidx.constraintlayout:constraintlayout:${Versions.constraint_version}"


    //阿里云 OSS
    const val aliyun_oss = "com.aliyun.dpa:oss-android-sdk:2.9.9"

    // glide
    const val glide = "com.github.bumptech.glide:glide:${Versions.glide}"

    // FFMMR  相机解码相关 https://github.com/wseemann/FFmpegMediaMetadataRetriever
    const val ffmeng_core =
        "com.github.wseemann:FFmpegMediaMetadataRetriever-core:${Versions.ffmeng}"
    const val ffmeng_native =
        "com.github.wseemann:FFmpegMediaMetadataRetriever-native:${Versions.ffmeng}"

    //dialog  https://github.com/afollestad/material-dialogs
    const val dialogs_lifecycle =
        "com.afollestad.material-dialogs:lifecycle:${Versions.material_dialogs}"
    const val dialogs_core = "com.afollestad.material-dialogs:core:${Versions.material_dialogs}"
    const val dialogs_color = "com.afollestad.material-dialogs:color:${Versions.material_dialogs}"
    const val dialogs_datetime =
        "com.afollestad.material-dialogs:datetime:${Versions.material_dialogs}"
    const val dialogs_bottom =
        "com.afollestad.material-dialogs:bottomsheets:${Versions.material_dialogs}"


    //util  https://github.com/Blankj/AndroidUtilCode
    const val utilcodex = "com.blankj:utilcodex:1.31.0"

    //微信开源项目，替代SP https://github.com/Tencent/MMKV
    const val mmkv = "com.tencent:mmkv:1.0.22"

    //管理界面状态库 https://github.com/KingJA/LoadSir
    const val loadsir = "com.kingja.loadsir:loadsir:1.3.8"

    //轮播图 https://github.com/zhpanvip/BannerViewPager
    const val bannerViewPager = "com.github.zhpanvip:BannerViewPager:3.1.5"

    //BaseAdapter https://github.com/CymChad/BaseRecyclerViewAdapterHelper
    const val baseAdapter = "com.github.CymChad:BaseRecyclerViewAdapterHelper:3.0.4"

    //第三方recyclerview https://github.com/yanzhenjie/SwipeRecyclerView
    const val swipeRecyclerView = "com.yanzhenjie.recyclerview:x:1.3.2"

    //GSYVideoPlayer视频播放器 https://github.com/CarGuo/GSYVideoPlayer
    const val GSYVideoPlayer =
        "com.github.CarGuo.GSYVideoPlayer:GSYVideoPlayer:v8.1.9-release-jitpack"

    //XXPermissions  https://github.com/getActivity/XXPermissions
    const val XXPermissions = "com.github.getActivity:XXPermissions:13.5"

    //progressbar    https://github.com/lopspower/CircularProgressBar
    const val circularprogressbar = "com.mikhaellopez:circularprogressbar:3.1.0"

    //webview       https://github.com/Justson/AgentWeb
    const val agentweb = "com.github.Justson.AgentWeb:agentweb-core:${Versions.agent_web}" // (必选)
    const val agentweb_filechooser =
        "com.github.Justson.AgentWeb:agentweb-filechooser:${Versions.agent_web}"
    const val agentweb_downloader = "com.github.Justson:Downloader:${Versions.agent_web}"

    // PictureSelector basic (Necessary) https://github.com/LuckSiege/PictureSelector
    const val pictureselector = "io.github.lucksiege:pictureselector:${Versions.picture_selector}"
    const val pictureselector_compress = "io.github.lucksiege:compress:${Versions.picture_selector}"
    const val pictureselector_crop = "io.github.lucksiege:ucrop:${Versions.picture_selector}"
    const val pictureselector_camerax = "io.github.lucksiege:camerax:${Versions.picture_selector}"

    //BasePopup  https://github.com/razerdp/BasePopup
    const val basePopup = "io.github.razerdp:BasePopup:3.2.0"

    //指示器库 https://github.com/hackware1993/MagicIndicator
    const val magicIndicator = "com.github.hackware1993:MagicIndicator:1.7.0"

    //Android UI 快速开发 圆角，边框，Gradient背景渐变，控件State各个状态UI样式，阴影，水波纹
    //https://github.com/RuffianZhong/RWidgetHelper
    const val RWidgetHelper = "com.github.RuffianZhong:RWidgetHelper:androidx.v0.0.9"

    //AndroidPicker   https://github.com/gzu-liyujiang/AndroidPicker
    const val picker_common = "com.github.gzu-liyujiang.AndroidPicker:Common:${Versions.picker}"
    const val picker_wheelview =
        "com.github.gzu-liyujiang.AndroidPicker:WheelView:${Versions.picker}"
    const val picker_wheel = "com.github.gzu-liyujiang.AndroidPicker:WheelPicker:${Versions.picker}"
    const val picker_address =
        "com.github.gzu-liyujiang.AndroidPicker:AddressPicker:${Versions.picker}"
    const val picker_calendar =
        "com.github.gzu-liyujiang.AndroidPicker:CalendarPicker${Versions.picker}"
    const val picker_image = "com.github.gzu-liyujiang.AndroidPicker:ImagePicker:${Versions.picker}"
    const val picker_file = "com.github.gzu-liyujiang.AndroidPicker:FilePicker:${Versions.picker}"
    const val picker_color = "com.github.gzu-liyujiang.AndroidPicker:ColorPicker:${Versions.picker}"

    const val unpeek_livedata = "com.kunminx.archi:unpeek-livedata:${Versions.unpeek_livedata}"
}
