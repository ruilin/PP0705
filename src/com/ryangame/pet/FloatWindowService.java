package com.ryangame.pet;

import java.util.ArrayList;
import java.util.List;
import java.util.TimerTask;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningTaskInfo;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.os.Handler;
import android.os.IBinder;

import com.ryangame.pet.gl.GL2JNILib;
import com.ryangame.pet.gl.Platform;

public class FloatWindowService extends Service {
	public static FloatWindowService instance;
	public static Platform globalData = null;
	/**
	 * 用于在线程中创建或移除悬浮窗。
	 */
	private Handler handler = new Handler();

	@Override
	public void onCreate() {
		super.onCreate();
		instance = this;
	}
	
	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}
//	Log.e(null, "xxxxxxxxxxx");
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		globalData = new Platform();
		GL2JNILib.create(globalData);
		ViewMng.createPetView(getApplicationContext());
		return super.onStartCommand(intent, flags, startId);
	}

	private void end() {
		ViewMng.destroy(getApplicationContext());
		GL2JNILib.destroy(globalData);
		globalData = null;
	}
	
	@Override
	public void onDestroy() {
		super.onDestroy();
		end();
	}
	
	public void onTaskRemoved(Intent rootIntent) {
		end();
	};

	class RefreshTask extends TimerTask {

		@Override
		public void run() {
			// 当前界面是桌面，且没有悬浮窗显示，则创建悬浮窗。
			if (isHome() && !ViewMng.isWindowShowing()) {
				handler.post(new Runnable() {
					@Override
					public void run() {
						ViewMng.createSmallWindow(getApplicationContext());
					}
				});
			}
			// 当前界面不是桌面，且有悬浮窗显示，则移除悬浮窗。
			else if (!isHome() && ViewMng.isWindowShowing()) {
				handler.post(new Runnable() {
					@Override
					public void run() {
						ViewMng.removeSmallWindow(getApplicationContext());
						ViewMng.removeBigWindow(getApplicationContext());
					}
				});
			}
			// 当前界面是桌面，且有悬浮窗显示，则更新内存数据。
			else if (isHome() && ViewMng.isWindowShowing()) {
				handler.post(new Runnable() {
					@Override
					public void run() {
						ViewMng.updateUsedPercent(getApplicationContext());
					}
				});
			}
		}

	}

	/**
	 * 判断当前界面是否是桌面
	 */
	private boolean isHome() {
		ActivityManager mActivityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		List<RunningTaskInfo> rti = mActivityManager.getRunningTasks(1);
		return getHomes().contains(rti.get(0).topActivity.getPackageName());
	}

	/**
	 * 获得属于桌面的应用的应用包名称
	 * 
	 * @return 返回包含所有包名的字符串列表
	 */
	private List<String> getHomes() {
		List<String> names = new ArrayList<String>();
		PackageManager packageManager = this.getPackageManager();
		Intent intent = new Intent(Intent.ACTION_MAIN);
		intent.addCategory(Intent.CATEGORY_HOME);
		List<ResolveInfo> resolveInfo = packageManager.queryIntentActivities(intent,
				PackageManager.MATCH_DEFAULT_ONLY);
		for (ResolveInfo ri : resolveInfo) {
			names.add(ri.activityInfo.packageName);
		}
		return names;
	}
}
