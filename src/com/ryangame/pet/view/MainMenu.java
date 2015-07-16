package com.ryangame.pet.view;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

public class MainMenu extends View {
	private Paint paint = new Paint();
	
	public static int viewWidth = 300;
	public static int viewHeight = 200;
	
	public MainMenu(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		paint.setColor(0XFFFFFF);
		paint.setTextSize(50);
		
	}

	public MainMenu(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}

	public MainMenu(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		// TODO Auto-generated constructor stub
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
//		this.dispatchTouchEvent(event);
		return super.onTouchEvent(event);
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.onDraw(canvas);
		canvas.drawColor(Color.BLUE);
		paint.setColor(Color.WHITE);
		canvas.drawText("桌面宠物", 100, 100, paint);
		this.invalidate();
	}
}
