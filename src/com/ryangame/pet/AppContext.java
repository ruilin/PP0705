package com.ryangame.pet;

import android.app.Application;

public class AppContext extends Application {

	private static AppContext instance;

	public static AppContext getInstance() {
		return instance;
	}

	@Override
	public void onCreate() {
		super.onCreate();
		instance = this;
	}

}
