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

		rendererWrapper.onChatReceived("{\"breakableBlockPowerUpFlags\": \"4,4,2,3,4,0,0,0,2,4,0,0,1,1,0,2,1,2,0,1,0,0,3,2,0,4,4,0,0,1,0,0,4,1,0,0,0,0,1,4,0,0,3,1,1,3,2,3,2,1,0,4,2,0,2,3,3,1,0,3,0,0,4,1,0,0,0,4,3,0,3,1,2,0,0,2,2,0,0,4,0,1,2,4,0,0,3,1,3,3,4,4,2,0,0,4,4,2,2,1,4,4,0,0,0,1,2\", \"breakableBlockXValues\": \"8,4,5,7,7,11,0,1,2,3,4,6,7,8,9,11,12,13,14,0,6,8,10,12,14,1,2,3,4,5,6,8,10,11,12,13,14,2,4,8,10,12,4,5,6,7,9,10,12,2,6,12,2,4,10,3,4,5,6,9,10,11,2,6,10,12,0,1,2,3,4,7,8,9,11,12,13,14,0,2,6,8,12,14,0,1,2,3,5,7,9,10,12,2,4,6,8,10,12,4,5,6,7,8,9,10,12\", \"breakableBlockYValues\": \"0,1,1,1,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,9,9,9,10,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"numBreakableBlocks\": 107, \"numClientBots\": 7, \"numPlayers\": 1, \"playerIndex0\": \"Player_Offline\", \"playerIndex0Direction\": 0, \"playerIndex0X\": 6.447761058807373, \"playerIndex0Y\": 2.1492538452148438, \"playerIndex1\": \"Bot 1\", \"playerIndex1Direction\": 2, \"playerIndex1X\": 22.208955764770508, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Bot 2\", \"playerIndex2Direction\": 0, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 16.477611541748047, \"playerIndex3\": \"Bot 3\", \"playerIndex3Direction\": 2, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 25.074626922607422, \"playerIndex4\": \"Bot 4\", \"playerIndex4Direction\": 1, \"playerIndex4X\": 17.91044807434082, \"playerIndex4Y\": 2.1492538452148438, \"playerIndex5\": \"Bot 5\", \"playerIndex5Direction\": 3, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 12.179104804992676, \"playerIndex6\": \"Bot 6\", \"playerIndex6Direction\": 1, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 16.477611541748047, \"playerIndex7\": \"Bot 7\", \"playerIndex7Direction\": 3, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 25.074626922607422}");
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