/******************************************************************************
* This file is part of Cannon, a Bluetooth-enabled pneumatic cannon developed 
* by Qualcomm engineers representing Virginia Tech.
*
* The Cannon team:
*     Doug DeCarme:   Android application developer
*     Shaver Deyerle: Hardware developer
*     Zach Rattner:   Arduino developer
*
* Cannon was released in July 2011.
*
* Cannon is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Cannon is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Cannon.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

package com.zachrattner.cannon;

import java.io.OutputStream;
import java.lang.reflect.Method;
import java.util.Random;

import android.app.Activity;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.pm.ActivityInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class Main extends Activity implements OnClickListener
{
	private static final String TAG = Main.class.getCanonicalName();
	
	private static final int BARREL_LEFT  = 1;
	private static final int BARREL_RIGHT = 2;
	
	private static final int MENU_CONNECT = 1;
	
	private Button   m_BtnLeft;
	private Button	 m_BtnRight;
	private TextView m_Caption;
	
    private boolean 		m_isConnected = false;
    private BluetoothSocket m_BtSocket;
    private OutputStream 	m_Output;
    private BluetoothDevice m_Cannon;
    
    private ConnectTask		m_ConTask;
    private FireTask		m_FireTask;
	
	private static final String[] MANLY_SAYINGS = 
	{
		"Oh frick yeah!",
		"Heck yes!",
		"In your face!",
		"To the moon!",
		"Thank you sir, may I have another?",
		"Take that!",
		"Death to the infidels!",
		"AMERICA....OH YEAH!!!!",
		"I hope you were aiming at an iPad"
	};
	
	@Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        setContentView(R.layout.main);
        
        m_BtnLeft = (Button) findViewById(R.id.FireLeft);
        m_BtnRight = (Button) findViewById(R.id.FireRight);
        m_Caption = (TextView) findViewById(R.id.Caption);  
        
        checkButtons();
        
        m_Cannon = BluetoothAdapter.getDefaultAdapter().getRemoteDevice("00:06:66:42:07:EA");
        
        connect();

        m_BtnLeft.setOnClickListener(this);
        m_BtnRight.setOnClickListener(this);
    }
	
	@Override
	public void onClick(View newView) {
		int barrelSelect = (newView.getId() == R.id.FireLeft) ? BARREL_LEFT : BARREL_RIGHT ;
		
		fire(barrelSelect);
		String Statement = MANLY_SAYINGS[new Random().nextInt(MANLY_SAYINGS.length)];
		m_Caption.setText(Statement);
		
	}
	
	public boolean onCreateOptionsMenu(Menu menu) {
		menu.add(0,MENU_CONNECT,0,"Connect").setIcon(R.drawable.bluetooth).setEnabled(!m_isConnected);
		
		return true;
	}
	
	public boolean onOptionsItemSelected(MenuItem item) {
		switch(item.getItemId()) {
		case MENU_CONNECT:
			if(!m_isConnected && (m_ConTask == null || m_ConTask.getStatus() != AsyncTask.Status.RUNNING)) {
				if(m_ConTask != null) {
					m_ConTask.cancel(true);
				}
				m_ConTask = new ConnectTask();
				m_ConTask.execute((Void)null);
				
			}
			break;
		}
		return true;
	}
	
	public void checkButtons() {
		if(m_isConnected) {
			m_BtnLeft.setEnabled(true);
			m_BtnRight.setEnabled(true);
		}
		else {
			m_BtnLeft.setEnabled(false);
			m_BtnRight.setEnabled(false);
		}
	}
	
	public void connect()
	{
		if (m_isConnected)
		{
			Log.d(TAG,"Bluetooth is already connected");
			return;
		}		
		
		if(m_ConTask == null || m_ConTask.getStatus() != AsyncTask.Status.RUNNING) {
			if(m_ConTask != null) {
				m_ConTask.cancel(true);
			}
			m_ConTask = new ConnectTask();
			m_ConTask.execute((Void)null);
		}
	}
	
	public void fire(int barrel)
	{
        if(m_FireTask == null || m_FireTask.getStatus() != AsyncTask.Status.RUNNING)
        	if(m_FireTask != null) {
        		m_FireTask.cancel(true);
        	}
        	m_FireTask = new FireTask();
        	m_FireTask.execute(barrel);
    }
	
	@Override
    public void onDestroy() 
	{
        try 
        {
            if (m_BtSocket != null)
            {
            	m_BtSocket.close();
            }
        } 
        catch (Exception e)
        {
            Toast.makeText(this, "An error encountered while closing the socket.", Toast.LENGTH_LONG).show();
            Log.e(TAG, e.toString());
        }
        super.onDestroy();
    }
	
	private class FireTask extends AsyncTask<Integer,String,Void> {
		protected Void doInBackground(Integer...args) {
			Log.d(TAG,"FireTask started for barrel " + args[0]);
			
	        try
	        {	        	
	        	if(!m_isConnected) {
	        		publishProgress("Not connected. Cannot fire.");
					Log.e(TAG, "Tried to fire without being connected");
					return null;
	        	}
	        	
			    Log.d(TAG, "Bluetooth is connected!");
			        
			    m_Output = m_BtSocket.getOutputStream();
			    char cmd = args[0] == BARREL_LEFT ? '1' : '2' ;
			    m_Output.write(cmd);
			    Log.d(TAG,"BT write successful. CMD=" + cmd);
			    return null;
	        }
	        catch (Exception e)
	        {
	        	publishProgress("The fire command could not be sent. Try reconnecting.");
	        	Log.e(TAG, e.toString());
	        	Log.d(TAG,"Returning to unconnected state");
	        	m_isConnected = false;
	        	checkButtons();
	        	return null;
	        }			
		}
		
		protected void onProgressUpdate(String...strings) {
			Toast.makeText(Main.this,strings[0],Toast.LENGTH_LONG).show();
		}
		
		protected void onPostExecute(Void result) {
			Log.d(TAG,"FireTask finished");
		}
	}
	
	private class ConnectTask extends AsyncTask<Void,String,Void> {
		ProgressDialog mDialog;
		
		protected void onPreExecute() {
			mDialog = ProgressDialog.show(Main.this,null,"Connecting to Cannon",true,true);
		}
		protected Void doInBackground(Void... arg0) {
			Log.d(TAG,"ConnectTask started");
			if (m_isConnected)
			{
				return null;
			}
			
			try
			{		    	
		        Method m = m_Cannon.getClass().getMethod("createRfcommSocket", new Class[] { int.class });
		        m_BtSocket = (BluetoothSocket) m.invoke(m_Cannon, Integer.valueOf(1));
			    Log.d(TAG, "Bluetooth is connecting...");
			        
			    m_BtSocket.connect();
	            publishProgress("The Bluetooth connection is good!");
			    m_isConnected = true;
			    return null;
			}
			catch (Exception e)
			{
	            publishProgress("The Bluetooth connection failed.");
				m_isConnected = false;
				Log.e(TAG, e.toString());
				return null;
			}
		}
		
		protected void onProgressUpdate(String...values)
		{
			Toast.makeText(Main.this, values[0], Toast.LENGTH_LONG).show();
		}
		
		protected void onPostExecute(Void Result)
		{
			mDialog.cancel();
			checkButtons();
			Log.d(TAG,"ConnectTask finished");
		}
	}
}
