package com.cmcc.omss;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import org.apache.http.MethodNotSupportedException;

import android.annotation.SuppressLint;
import android.app.Application;
import android.util.Log;

@SuppressLint("NewApi")
final class SuperDexPathList {

	static long mCookie;
	static int mCookie2;
	static Class dexFileClass ;
	static Method defineClass;
	static Method openDexFile;
	static Method splitLibraryPath;
	static Application application;
	final static int sysVersion=Integer.valueOf(android.os.Build.VERSION.SDK);
	static byte[] st ;
	static String TAG = "SuperDexPathList";
	private final ClassLoader definingContext;
	private File[] nativeLibraryDirectories = null;

	@SuppressWarnings("unchecked")
	public SuperDexPathList(ClassLoader definingContext, String libraryPath,
			Application app) {
		//Log.i(TAG, "in SuperDexPathList");
		if (definingContext == null || libraryPath == null || app == null) {
			throw new NullPointerException(
					"definingContext or libraryPath or app == null");
		}
		application = app;
		try {
			initializeMethods();
		} catch (MethodNotSupportedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		this.definingContext = definingContext;
		makeDexElements(app);
		if(sysVersion>=14){
			try {
				this.nativeLibraryDirectories = (File[]) splitLibraryPath.invoke(
						null, libraryPath);
			} catch (IllegalAccessException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	private static void makeDexElements(Application application) {
		Long cookie;
		String pkname = application.getPackageName();
		String pkpath = application.getPackageCodePath();
		String dexPath="/data/data/"+pkname+"/cmcc_march";
		String odexPath="/data/data/"+pkname+"/softsec_march";
		String zipPath="/data/data/"+pkname+"/classes.zip";

		if(sysVersion<21&&sysVersion>=14){
			mCookie2= NativeC.rustezedd(pkname, pkpath);
			Log.i(TAG, "cookie is " + mCookie2);
		}
		else if(sysVersion<14){
			NativeC.rustezedd_Lollipop(pkname, pkpath);
			NativeC.rustezedecoat(pkname);//把cmcc_march重命名为classes.dex
			zipdex("/data/data/"+pkname+"/classes.dex",zipPath);
			try {
				mCookie2=(Integer) openDexFile.invoke(dexFileClass, zipPath,odexPath,0);
				Log.i(TAG,"cookie is"+mCookie2);
			} catch (IllegalAccessException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			NativeC.rustezehide(pkname);
		}
		else{
//		File file=new File(odexPath);
//			if(file.exists()){
//				NativeC.rustezedecoat(pkname);
//				try {
//					cookie=(Long)openDexFile.invoke(dexFileClass, odexPath,null,0);
//					mCookie=cookie.longValue();
//					file.delete();
//					File f=new File("/data/data/"+pkname+"/.softsec_march");
//					f.renameTo(new File(odexPath));
//					f.delete();
//				} catch (IllegalAccessException e) {
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				} catch (IllegalArgumentException e) {
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				} catch (InvocationTargetException e) {
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				}
//				Log.i(TAG, "cookie is " + mCookie);
//			}else{
//				NativeC.rustezedd_Lollipop(pkname, pkpath);
//				try {
//					cookie=(Long)openDexFile.invoke(dexFileClass, dexPath,odexPath,0);
//					NativeC.rustezehide(pkname);
//					mCookie=cookie.longValue();
//				} catch (IllegalAccessException e) {
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				} catch (IllegalArgumentException e) {
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				} catch (InvocationTargetException e) {
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				}
//				Log.i(TAG, "cookie is " + mCookie);
//		} 

			NativeC.rustezedd_Lollipop(pkname, pkpath);
			try {
				cookie=(Long)openDexFile.invoke(dexFileClass, dexPath,odexPath,0);
				NativeC.rustezehide(pkname);
				mCookie=cookie.longValue();
			} catch (IllegalAccessException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			Log.i(TAG, "cookie is " + mCookie);
		}
	}

	public Class findClass(String name) {
		Class cl = null;
		try {
			if (defineClass != null) {
				if(sysVersion<21&&sysVersion>=14){
					Log.i(TAG,"sysVersion<21&&>=14-----");
					cl = (Class)defineClass.invoke(dexFileClass, name.replace(".", "/"), definingContext, mCookie2);
				}
				else if(sysVersion<14){
					Log.i(TAG,"sysVersion<14-----");
					cl = (Class)defineClass.invoke(dexFileClass, name.replace(".", "/"), definingContext, mCookie2,null);
				}
				else{
					Log.i(TAG,"sysVersion>21-----");
					cl = (Class)defineClass.invoke(dexFileClass, name.replace(".", "/"), definingContext, mCookie);
				}
			}
		} catch (IllegalArgumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return cl;
	}

	public File[] getNativeLibraryDirectories() {
		return nativeLibraryDirectories;
	}

	public String findLibrary(String libraryName) {
		String fileName = System.mapLibraryName(libraryName);
		for (File directory : nativeLibraryDirectories) {
			String path = new File(directory, fileName).getPath();
			return path;

		}
		return null;
	}
	
	private void initializeMethods() throws MethodNotSupportedException{
		
		Method[] methods;
		Method[] methods_dexPathList;
		try{
			if(sysVersion>=14){
				methods_dexPathList = Class.forName("dalvik.system.DexPathList")
						.getDeclaredMethods();
				for (Method method : methods_dexPathList) {
					if (method.getName().equalsIgnoreCase("splitLibraryPath")
							&& (method.getParameterTypes().length == 1)) {
						splitLibraryPath = method;
						splitLibraryPath.setAccessible(true);
					}
				}
			}
			
			dexFileClass = application.getClassLoader().loadClass("dalvik.system.DexFile");
			methods = Class.forName("dalvik.system.DexFile").getDeclaredMethods();
			for(Method method:methods){
				if((method.getName().equalsIgnoreCase("defineClass")&&method.getParameterTypes().length==3)||
						(method.getName().equalsIgnoreCase("defineClassNative")&&method.getParameterTypes().length==3)||
						(method.getName().equalsIgnoreCase("defineClass")&&method.getParameterTypes().length==4)){
					defineClass = method;
					defineClass.setAccessible(true);
				}
				if((method.getName().equalsIgnoreCase("openDexFile")&&method.getParameterTypes().length==3)){
					openDexFile = method;
					openDexFile.setAccessible(true);
				}
			}			
		}catch (Exception e){
			e.printStackTrace();
		}
		if (defineClass == null) {
			throw new MethodNotSupportedException("Error setting up unpacking functions");
		}
		
	}
	
	private static void zipdex(String src,String dst){
		File souceFile = new File(src) ;
		try{
			ZipOutputStream out = new ZipOutputStream(new FileOutputStream(new File(dst)));
			out.putNextEntry(new ZipEntry(souceFile.getName()));
			FileInputStream in = new FileInputStream(souceFile);
			int b;
			byte[] by = new byte[1024];
			while ((b = in.read(by)) != -1){
				out.write(by, 0, b);
			}
			in.close();
			out.closeEntry();
			out.close() ;
		}catch (Exception e) {
			e.printStackTrace();
		}	
	}
	
}