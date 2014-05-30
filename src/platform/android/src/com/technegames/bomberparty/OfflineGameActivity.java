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

		rendererWrapper.onChatReceived("{\"breakableBlockPowerUpFlags\":\"2,1,3,1,1,3,3,4,0,4,3,0,0,2,0,0,2,4,2,4,4,2,0,0,0,1,0,1,3,2,3,1,2,3,0,1,1,0,4,0,3,0,0,2,2,2,4,4,1,4,0,1,0,0,0,0,0,0,4,1,0,0,3,0,0,1,1,4,1,0,1,1,0,1,1,1,2,2,4,0,0,1,0,0,1,0,2,1,0,0,2,1,0,1,4,0,0,3,0,2,0,0,0,0,4,3,0,0,3,2,1,0,4,1,0,3,0,0,4,3,2,2,0,0,1,1,4,3,2,1,0,0,1,0,1,2,3,0,0,1,1,0,4,0,3\",\"breakableBlockXValues\":\"2,3,5,6,7,8,9,10,13,15,16,2,8,10,12,0,1,3,6,7,9,14,16,17,4,6,8,0,1,3,4,5,7,10,11,14,15,17,18,0,2,4,12,14,18,0,1,2,3,6,8,10,11,12,13,15,16,17,18,2,4,6,12,14,16,4,5,6,7,8,9,11,12,14,15,2,4,6,10,12,16,2,3,5,6,7,8,9,11,14,15,4,6,8,1,2,4,5,6,9,10,11,13,16,17,2,6,8,10,12,14,16,18,0,1,2,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,6,10,16,2,4,5,6,7,8,10,11,13,15,16\",\"breakableBlockYValues\":\"0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,11,11,11,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,16,16,16,16,16,16,16,16,16,16,16\",\"eventType\":1337,\"numBreakableBlocks\":145,\"numPlayers\":1,\"numClientBots\":7,\"playerIndex0\":\"Player_Offline\",\"playerIndex0Direction\":0,\"playerIndex0X\":3.747559070587158,\"playerIndex0Y\":3.418919086456299,\"playerIndex1\":\"Bot1\",\"playerIndex1Direction\":2,\"playerIndex1X\":22.08089256286621,\"playerIndex1Y\":13.41891860961914,\"playerIndex2\":\"Bot2\",\"playerIndex2Direction\":0,\"playerIndex2X\":3.747559070587158,\"playerIndex2Y\":15.91891860961914,\"playerIndex3\":\"Bot3\",\"playerIndex3Direction\":2,\"playerIndex3X\":22.08089256286621,\"playerIndex3Y\":24.16891860961914,\"playerIndex4\":\"Bot4\",\"playerIndex4Direction\":1,\"playerIndex4X\":22.08089256286621,\"playerIndex4Y\":3.418919086456299,\"playerIndex5\":\"Bot5\",\"playerIndex5Direction\":1,\"playerIndex5X\":22.08089256286621,\"playerIndex5Y\":15.91891860961914,\"playerIndex6\":\"Bot6\",\"playerIndex6Direction\":3,\"playerIndex6X\":3.747559070587158,\"playerIndex6Y\":13.41891860961914,\"playerIndex7\":\"Bot7\",\"playerIndex7Direction\":3,\"playerIndex7X\":3.747559070587158,\"playerIndex7Y\":24.16891860961914}");
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