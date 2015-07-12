package com.ryangame.pet;

import com.ryangame.pet.jni.Helper;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Helper.init(this);
		setContentView(R.layout.activity_main);

		Button btn_showPet = (Button) findViewById(R.id.show_pet);
		btn_showPet.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				Intent intent = new Intent(MainActivity.this, FloatWindowService.class);
				startService(intent);
				finish();
			}
		});
		Button btn_hidePet = (Button) findViewById(R.id.hide_pet);
		btn_hidePet.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				Intent intent = new Intent(MainActivity.this, FloatWindowService.class);
				stopService(intent);
				finish();
			}
		});
		
//		GameWindowManager.createPetView(getApplicationContext());
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
		// TODO Auto-generated method stub
		super.onDestroy();
	}
}
