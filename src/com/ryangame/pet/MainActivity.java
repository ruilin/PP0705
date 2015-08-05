package com.ryangame.pet;

import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningServiceInfo;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

import com.ryangame.pet.jni.Helper;

public class MainActivity extends Activity {
	public static Activity mActivity;
	boolean isClickable = true;
	Timer clickTimer = null;
	TimerTask task = null;
	Toast toast = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mActivity = this;
		Helper.init(this);
		clickTimer = new Timer(true);
		setContentView(R.layout.activity_main);

		Button btn_showPet = (Button) findViewById(R.id.show_pet);
		btn_showPet.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				if (!isServiceWork(mActivity, mActivity.getPackageName() + ".FloatWindowService")) {
					Intent intent = new Intent(MainActivity.this,
							FloatWindowService.class);
					startService(intent);
					setTimer();
				}
			}
		});
		Button btn_hidePet = (Button) findViewById(R.id.hide_pet);
		btn_hidePet.setOnClickListener(new OnClickListener() {
			@SuppressLint("ShowToast")
			@Override
			public void onClick(View arg0) {
				if (!isClickable) {
					if (null != toast) toast.cancel();
					toast = Toast.makeText(getApplicationContext(), "游戏加载中...请稍等!!", 500);
					toast.show();
				} else if (isServiceWork(mActivity, mActivity.getPackageName() + ".FloatWindowService")) {
					Intent intent = new Intent(MainActivity.this,
							FloatWindowService.class);
					stopService(intent);
					setTimer();
				}
			}
		});
	}

	private void setTimer() {
		isClickable = false;
		if (null != task) {
			task.cancel();
		}
		task = new TimerTask() {
			@Override
			public void run() {
				isClickable = true;
			}
		};
		clickTimer.schedule(task, 500);
	}
	/**
	 * 判断某个服务是否正在运行的方法
	 * 
	 * @param mContext
	 * @param serviceName
	 *            是包名+服务的类名（例如：net.loonggg.testbackstage.TestService）
	 *            mActivity.getPackageName() + ".FloatWindowService"
	 * @return true代表正在运行，false代表服务没有正在运行
	 */
	public boolean isServiceWork(Context mContext, String serviceName) {
		boolean isWork = false;
		ActivityManager myAM = (ActivityManager) mContext
				.getSystemService(Context.ACTIVITY_SERVICE);
		List<RunningServiceInfo> myList = myAM.getRunningServices(40);
		if (myList.size() <= 0) {
			return false;
		}
		for (int i = 0; i < myList.size(); i++) {
			String mName = myList.get(i).service.getClassName().toString();
			if (mName.equals(serviceName)) {
				isWork = true;
				break;
			}
		}
		return isWork;
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
	}
}
