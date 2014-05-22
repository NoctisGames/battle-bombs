package com.technegames.bomberparty;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Point;
import android.media.AudioManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
import android.widget.LinearLayout;

public final class OfflineGameActivity extends Activity
{
	private static final Logger logger = new Logger(OfflineGameActivity.class);

	public static void startActivity(Activity activity)
	{
		Intent i = new Intent(activity, OfflineGameActivity.class);
		activity.startActivity(i);
	}

	private RendererWrapper rendererWrapper;
	private GLSurfaceView glSurfaceView;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_game);

		Point size = ViewUtils.getScreenSize(this);

		if (Logger.isDebugEnabled())
		{
			logger.debug("dimension " + size.x + " x " + size.y);
		}

		rendererWrapper = new RendererWrapper(this, size.x, size.y, "Player_Offline");
		glSurfaceView = new GLSurfaceView(this);
		glSurfaceView.setEGLContextClientVersion(1);
		glSurfaceView.setRenderer(rendererWrapper);

		LinearLayout gameContainer = (LinearLayout) findViewById(R.id.game);
		gameContainer.addView(glSurfaceView);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED);
		setVolumeControlStream(AudioManager.STREAM_MUSIC);

		glSurfaceView.setOnTouchListener(new OnTouchListener()
		{
			@Override
			public boolean onTouch(View v, final MotionEvent event)
			{
				synchronized (this)
				{
					int action = event.getAction() & MotionEvent.ACTION_MASK;
					int pointerIndex = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;

					switch (action)
					{
						case MotionEvent.ACTION_DOWN:
						case MotionEvent.ACTION_POINTER_DOWN:
							rendererWrapper.handleTouchDown(event.getX(pointerIndex), event.getY(pointerIndex));
							break;
						case MotionEvent.ACTION_UP:
						case MotionEvent.ACTION_POINTER_UP:
						case MotionEvent.ACTION_CANCEL:
							rendererWrapper.handleTouchUp(event.getX(pointerIndex), event.getY(pointerIndex));
							break;
						case MotionEvent.ACTION_MOVE:
							for (int i = 0; i < event.getPointerCount(); i++)
							{
								pointerIndex = i;
								rendererWrapper.handleTouchDragged(event.getX(pointerIndex), event.getY(pointerIndex));
							}
							break;
					}

					return true;
				}
			}
		});
	}

	@Override
	protected void onResume()
	{
		super.onResume();

		glSurfaceView.onResume();
		rendererWrapper.onResume();

		rendererWrapper.onChatReceived("{\"playerIndex0\":\"Player_Offline\",\"numPlayers\":1,\"breakableBlockPowerUpFlags\":\"4,1,1,0,0,4,0,0,3,4,0,4,0,3,0,3,4,4,4,0,0,2,4,4,0,1,3,4,2,0,1,2,4,0,3,4,0,3,1,0,3,4,0,2,0,0,4,3,3,1,1,4,3,3,4,4,3,2,4,0,0,1,1,0,4,3,0,3,0,0,0,0,4,4,0,2,1,0,1,1,0,4,0,4,1,2,2,2,2,2,4,4,0,4,1,1,0,0,1,0,3,3,0,4,0,0,0,3,0,0,0,2,3,2,3,0,0,0,4,4,1,0,4,2,0,4,3,1,0,3,3,0,1,0,0,1,4,4\",\"breakableBlockYValues\":\"0,0,0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,7,7,7,7,8,8,8,8,8,8,8,8,8,9,9,9,10,10,10,10,10,10,10,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16\",\"breakableBlockXValues\":\"2,3,4,6,8,9,10,15,16,2,8,10,14,16,0,1,3,4,5,6,9,12,14,15,16,17,0,2,4,6,8,10,12,16,1,4,5,6,7,8,9,10,12,13,14,15,17,0,4,6,8,10,12,16,1,5,6,8,10,13,14,16,17,6,10,12,16,2,5,7,8,10,12,14,15,16,2,8,16,2,5,6,9,12,14,16,4,10,16,0,1,2,4,5,6,7,8,11,12,13,14,15,2,6,8,10,12,16,0,1,2,5,6,7,8,9,10,12,14,15,16,18,2,6,8,10,12,14,16,2,3,5,6,7,9,14,15,16\",\"eventType\":1337,\"playerIndex0X\":3.747559070587158,\"numBreakableBlocks\":138,\"playerIndex0Direction\":0,\"playerIndex0Y\":3.418919086456299}");
	}

	@Override
	protected void onPause()
	{
		rendererWrapper.onPause();
		glSurfaceView.onPause();

		finish();

		super.onPause();
	}

	@Override
	public void onBackPressed()
	{
		if (rendererWrapper.handleOnBackPressed())
		{
			return;
		}

		super.onBackPressed();
	}
}