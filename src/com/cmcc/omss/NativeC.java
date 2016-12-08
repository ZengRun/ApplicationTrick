package com.cmcc.omss;

public class NativeC{
	
	static{
		System.loadLibrary("cmcc_haze");
	}

	public native static int rustezedd(String DexPath, String apkpath);

	public native static int rustezedd_Lollipop(String DexPath, String apkpath);
	
	public native static void rustezehide(String DexPath);
	
	public native static int rustezedecoat(String DexPath);
}