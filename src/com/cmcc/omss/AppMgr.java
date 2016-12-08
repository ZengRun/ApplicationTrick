package com.cmcc.omss;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Application;
import android.app.Instrumentation;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Build;
import android.os.Bundle;
import android.util.ArrayMap;
import android.util.Log;
import dalvik.system.DexClassLoader;
import dalvik.system.PathClassLoader;
import android.content.res.Configuration;

public class AppMgr extends Application {
	private String libPath;
	private final static String TAG = "App2";
	private Application application = null;
	String appClassName = null;
	int sysVersion;
	String st[] = null;
	SuperClassLoader dLoader;
	private static Application application1 = null;
    
	//private long total_consume_pre;
	//private long total_consume_post;

	@Override
	protected void attachBaseContext(Context base) {
		super.attachBaseContext(base);
		//total_consume_pre = System.currentTimeMillis();
		application = this;
		sysVersion = getAndroidSDKVersion();
		if(sysVersion>=14)
			CheckUnshellTools(base);
		if(android.os.Debug.isDebuggerConnected()){
			android.os.Process.killProcess(android.os.Process.myPid());
			System.exit(0);
		}
		//Log.i(TAG, "pid is " + android.os.Process.myPid());
		prepare(application);
	}

	@TargetApi(Build.VERSION_CODES.CUPCAKE)
	@SuppressLint("NewApi")
	public void prepare(Application application) {
		libPath = Config.PKGNAME + application.getPackageName() + "/lib";
		String pkname = application.getPackageName();
		String pkpath = application.getPackageCodePath();
		Log.i(TAG, "pkname is " + pkname);
		Log.i(TAG, "pkpath is " + pkpath);
		try {
			if (this.getClass().getClassLoader() instanceof DexClassLoader) {
				//Log.i(TAG, "this is DexClassLoader");
				android.os.Process.killProcess(android.os.Process.myPid());
				System.exit(0);
			}
			//long pre_LoadOriDex = System.currentTimeMillis();
			Object currentActivityThread = RefInvoke.invokeStaticMethod(
					Config.MTHREAD, "currentActivityThread", new Class[] {},
					new Object[] {});
			//Log.w(TAG, "currentActivityThread _________");
			String packageName = application.getPackageName();
			WeakReference wr=null;
			
			if (sysVersion>=19) {
			ArrayMap mPackages=(ArrayMap) RefInvoke.getFieldOjbect(
			Config.MTHREAD, currentActivityThread, "mPackages");
				 wr = (WeakReference) mPackages.get(packageName);
				//Log.w(TAG, " ArrayMap WeakReference ____ _________");
			}else{
			HashMap mPackages = (HashMap) RefInvoke.getFieldOjbect(
					Config.MTHREAD, currentActivityThread, "mPackages");
			//Log.w(TAG, "mPackages ____ _________");
			 wr = (WeakReference) mPackages.get(packageName);
			//Log.w(TAG, "HashMap WeakReference ____ _________");
			}
			
			if (sysVersion >8) {
				PathClassLoader fakeloader = (PathClassLoader) RefInvoke.getFieldOjbect(Config.LOADP1, wr.get(), "mClassLoader");
				dLoader = new SuperClassLoader(libPath, (ClassLoader) RefInvoke.getFieldOjbect(Config.LOADP1, wr.get(), "mClassLoader"),
						application, fakeloader);
				RefInvoke.setFieldOjbect(Config.LOADP1, "mClassLoader",wr.get(), dLoader);
			}
			else{
				PathClassLoader fakeloader = (PathClassLoader) RefInvoke.getFieldOjbect(Config.LOADP2, wr.get(), "mClassLoader");
				dLoader = new SuperClassLoader(libPath, (ClassLoader) RefInvoke.getFieldOjbect(Config.LOADP2, wr.get(), "mClassLoader"),
						application, fakeloader);
				RefInvoke.setFieldOjbect(Config.LOADP2, "mClassLoader",wr.get(), dLoader);
			}
		//	long post_LoadOriDex = System.currentTimeMillis();
			//Log.w(TAG, "3. Classloader LoadOriDex time is "
					//+ (post_LoadOriDex - pre_LoadOriDex) + "ms");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static int getAndroidSDKVersion() {
		int version = 0;
		try {
			version = Integer.valueOf(android.os.Build.VERSION.SDK);
		} catch (NumberFormatException e) {
			Log.e("004", e.toString());
		}
		return version;
	}

	public static void CheckUnshellTools(Context base){		//检测脱壳工具
		StackTraceElement[] stacktrace = Thread.currentThread().getStackTrace();
//		for(int i=0;i<stacktrace.length;i++){
//			System.out.println(i+": " +"toString:"+stacktrace[i].toString()+" MethodName:"+stacktrace[i].getMethodName()+";"+stacktrace[i].getClassName());
//		}
		if((stacktrace[stacktrace.length-1].getClassName().equalsIgnoreCase("dalvik.system.NativeStart"))){
			if(!(stacktrace[stacktrace.length-2].getClassName().equalsIgnoreCase("com.android.internal.os.ZygoteInit"))){
				Log.e(TAG,"unshell tool detected!");
				android.os.Process.killProcess(android.os.Process.myPid());
				System.exit(0);
			}
		}else{
			if(!(stacktrace[stacktrace.length-1].getClassName().equalsIgnoreCase("com.android.internal.os.ZygoteInit"))){
				Log.e(TAG,"unshell tool detected!");
				android.os.Process.killProcess(android.os.Process.myPid());
				System.exit(0);
			}
		}
		
		PackageManager packageManager=base.getPackageManager();
		try {
            ApplicationInfo info =packageManager.getApplicationInfo("com.saurik.substrate",0);
    		Log.e(TAG,"unshell tool substrate detected!");
    		Log.e(TAG,info.packageName);
    		android.os.Process.killProcess(android.os.Process.myPid());
			System.exit(0);
        } catch (NameNotFoundException e) {
        	Log.i(TAG, "no unshell tool detected!");
        }	
	}

	public void onCreate() {
		//long pre_onCreate = System.currentTimeMillis();
		initApplication();
		//long post_onCreate = System.currentTimeMillis();
		//Log.w(TAG, "4. Classloader onCreate time is "
				//+ (post_onCreate - pre_onCreate) + "ms");
	}

	String str;

	@SuppressLint("NewApi")
	private void initApplication() {
		if(android.os.Debug.isDebuggerConnected()){
			android.os.Process.killProcess(android.os.Process.myPid());
		    System.exit(0);
		}
		try {
			if (this.getClass().getClassLoader() instanceof DexClassLoader) {
				Log.i(TAG, str);
			}
			ApplicationInfo ai = application.getPackageManager()
					.getApplicationInfo(application.getPackageName(),
							PackageManager.GET_META_DATA);
			Bundle bundle = ai.metaData;
			if (bundle != null && bundle.containsKey(Config.APPKEY)) {
				appClassName = bundle.getString(Config.APPKEY);
			} else {
				return;
			}
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		Object currentActivityThread = RefInvoke.invokeStaticMethod(
				Config.MTHREAD, "currentActivityThread", new Class[] {},
				new Object[] {});
		Object mBoundApplication = RefInvoke.getFieldOjbect(Config.MTHREAD,
				currentActivityThread, "mBoundApplication");
		Object loadedApkInfo = RefInvoke.getFieldOjbect(Config.MTHREAD
				+ "$AppBindData", mBoundApplication, "info");
		if (sysVersion > 8) {
			setVersionClass(Config.LOADP1, loadedApkInfo);
		} else {
			setVersionClass(Config.LOADP2, loadedApkInfo);
		}
		Object oldApplication = RefInvoke.getFieldOjbect(Config.MTHREAD,
				currentActivityThread, "mInitialApplication");
		ArrayList<Application> mAllApplications = (ArrayList<Application>) RefInvoke
				.getFieldOjbect(Config.MTHREAD, currentActivityThread,
						"mAllApplications");
		mAllApplications.remove(oldApplication);
		ApplicationInfo appinfo_In_LoadedApk;
		if (sysVersion > 8) {
			appinfo_In_LoadedApk = inLoadedApk(Config.LOADP1, loadedApkInfo);

		} else {
			appinfo_In_LoadedApk = inLoadedApk(Config.LOADP2, loadedApkInfo);
		}

		ApplicationInfo appinfo_In_AppBindData = (ApplicationInfo) RefInvoke
				.getFieldOjbect(Config.MTHREAD + "$AppBindData",
						mBoundApplication, "appInfo");
		appinfo_In_LoadedApk.className = appinfo_In_AppBindData.className = appClassName;
		Application app;
		if (sysVersion > 8) {
			app = getppApplaction(Config.LOADP1, loadedApkInfo);
		} else {
			app = getppApplaction(Config.LOADP2, loadedApkInfo);
		}

		RefInvoke.setFieldOjbect(Config.MTHREAD, "mInitialApplication",
				currentActivityThread, app);
		Iterator it = null;
		if (sysVersion>=19) {
			ArrayMap  mProviderMap = (ArrayMap) RefInvoke.getFieldOjbect(
					Config.MTHREAD, currentActivityThread, "mProviderMap");
			
			it = mProviderMap.values().iterator();
		}else{
			HashMap mProviderMap = (HashMap) RefInvoke.getFieldOjbect(
					Config.MTHREAD, currentActivityThread, "mProviderMap");
			it = mProviderMap.values().iterator();
		}
		
		while (it.hasNext()) {
			Object providerClientRecord = it.next();
			Object localProvider = null;
			if (sysVersion > 8) {
				localProvider = localPro(Config.MTHREAD
						+ "$ProviderClientRecord", providerClientRecord);

			} else if(sysVersion<=8){
				localProvider = localPro(Config.MTHREAD + "$ProviderRecord",
						providerClientRecord);
			}
			RefInvoke.setFieldOjbect("android.content.ContentProvider",
					"mContext", localProvider, app);
		}
		app.onCreate();
	//	total_consume_post = System.currentTimeMillis();
		//Log.w(TAG, "1. Classloader total time is "
				//+ (total_consume_post - total_consume_pre) + "ms");
	}

	private Object localPro(String className, Object providerClientRecord) {
		Object localProvider;
		localProvider = RefInvoke.getFieldOjbect(className,
				providerClientRecord, "mLocalProvider");
		return localProvider;
	}

	private ApplicationInfo inLoadedApk(String inLoaded, Object loadedApkInfo) {
		ApplicationInfo appinfo_In_LoadedApk;
		appinfo_In_LoadedApk = (ApplicationInfo) RefInvoke.getFieldOjbect(
				inLoaded, loadedApkInfo, "mApplicationInfo");
		return appinfo_In_LoadedApk;
	}

	private Application getppApplaction(String appLoad, Object loadedApkInfo) {
		Application app;
		app = (Application) RefInvoke.invokeMethod(appLoad, "makeApplication",
				loadedApkInfo, new Class[] { boolean.class,
						Instrumentation.class }, new Object[] { false, null });
		return app;
	}

	private void setVersionClass(String loadP, Object loadedApkInfo) {
		RefInvoke.setFieldOjbect(loadP, "mApplication", loadedApkInfo, null);
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
		if (application1 != null) {
			application1.onConfigurationChanged(newConfig);
		}
	}

	@Override
	public void onLowMemory() {
		// TODO Auto-generated method stub
		super.onLowMemory();
		if (application1 != null)
			application1.onLowMemory();

	}

	@Override
	public void onTerminate() {
		// TODO Auto-generated method stub
		super.onTerminate();
		if (application1 != null)
			application1.onTerminate();
	}

}