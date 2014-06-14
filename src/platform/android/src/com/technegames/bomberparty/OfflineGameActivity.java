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

		rendererWrapper.onChatReceived("{\"breakableBlockPowerUpFlags\": \"0,0,0,1,4,2,4,0,2,2,3,0,0,2,2,4,0,2,4,2,4,3,4,0,0,0,4,4,2,0,4,4,2,0,0,3,0,3,0,3,4,2,4,1,2,4,2,4,0,0,0,3,1,3,4,2,0,0,4,0,0,0,2,0,2,0,0,4,4,3,0,3,2,4,1,1,0,3,0,0,1,0,0,4,4,4,1,0,0,4,0,4,3,0,4,1,1,3,4,2,0,3,3,4,2,0,0,3,0,2,1,0,0,2\",\"breakableBlockXValues\": \"3,4,5,6,7,9,11,12,13,2,4,6,8,12,14,2,3,5,6,8,11,12,14,0,4,6,0,2,5,6,7,9,10,11,12,13,14,0,4,10,12,14,0,2,4,6,7,8,9,11,12,14,2,6,8,10,12,4,6,9,10,11,12,13,2,4,8,10,12,4,5,7,9,10,12,13,4,6,12,14,1,3,4,5,9,10,12,14,15,0,6,8,12,3,4,5,6,9,10,11,13,15,0,4,10,2,4,5,6,11,12,13,14,15\",\"breakableBlockYValues\": \"0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,8,8,8,8,8,8,8,9,9,9,9,9,10,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,12,12,12,13,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,16,16,16,16,16,16,16,16,16\",\"eventType\": 1337,\"numBreakableBlocks\": 114,\"numClientBots\": 7,\"numPlayers\": 1,\"playerIndex0\": \"Player_Offline\",\"playerIndex0Direction\": 0,\"playerIndex0X\": 1.8865671157836914,\"playerIndex0Y\": 1.3058645725250244,\"playerIndex1\": \"Bot 1\",\"playerIndex1Direction\": 2,\"playerIndex1X\": 21.397016525268555,\"playerIndex1Y\": 12.768550872802734,\"playerIndex2\": \"Bot 2\",\"playerIndex2Direction\": 0,\"playerIndex2X\": 1.8865671157836914,\"playerIndex2Y\": 15.634222030639648,\"playerIndex3\": \"Bot 3\",\"playerIndex3Direction\": 2,\"playerIndex3X\": 21.397016525268555,\"playerIndex3Y\": 23.873029708862305,\"playerIndex4\": \"Bot 4\",\"playerIndex4Direction\": 1,\"playerIndex4X\": 21.397016525268555,\"playerIndex4Y\": 1.3058645725250244,\"playerIndex5\": \"Bot 5\",\"playerIndex5Direction\": 1,\"playerIndex5X\": 21.397016525268555,\"playerIndex5Y\": 15.634222030639648,\"playerIndex6\": \"Bot 6\",\"playerIndex6Direction\": 3,\"playerIndex6X\": 1.8865671157836914,\"playerIndex6Y\": 12.768550872802734,\"playerIndex7\": \"Bot 7\",\"playerIndex7Direction\": 3,\"playerIndex7X\": 1.8865671157836914,\"playerIndex7Y\": 23.873029708862305}");
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