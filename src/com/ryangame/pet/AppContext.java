package com.ryangame.pet;

import android.app.Application;
import android.content.Context;
import android.util.DisplayMetrics;
import android.view.WindowManager;

public class AppContext extends Application {

	private static AppContext instance;
	private WindowManager wm = null;
	private DisplayMetrics dm = null;
	@Override
	public void onCreate() {
		super.onCreate();
		instance = this;
	}

	public static AppContext getInstance() {
		return (AppContext) (null != instance ? instance : FloatWindowService.instance.getApplication());
	}

	public int[] getScreenSize() {
		WindowManager windowManager = getWindowManager(this.getApplicationContext());
		if (null == dm) dm = new DisplayMetrics();
		windowManager.getDefaultDisplay().getMetrics(dm);
		return new int[]{dm.widthPixels, dm.heightPixels};
	}
	
	private WindowManager getWindowManager(Context context) {
		if (null == wm) {
			wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
		}
		return wm;
	}
}
