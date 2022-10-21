package com.zjuici.nativelib;

import android.content.res.AssetManager;
import android.graphics.Bitmap;


public class SportAi {
    static {
        System.loadLibrary("insportapp");
    }

    // sport_type 指的是 sportId
    public static native void init(AssetManager manager, int sport_type, boolean useGPU);

    public static native int ready(Bitmap bitmap, int mask_x, int mask_y, int mask_width, int mask_height);

    public static native SportPoint[] count(Bitmap bitmap, int sport_type, int mask_x, int mask_y, int mask_width, int mask_height);

    public static native void clear();

}
