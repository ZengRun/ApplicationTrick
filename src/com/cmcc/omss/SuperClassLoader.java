package com.cmcc.omss;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.Enumeration;

import dalvik.system.PathClassLoader;
import dalvik.system.DexFile;

import android.annotation.SuppressLint;
import android.app.Application;
import android.util.Log;

@SuppressLint("NewApi")
public class SuperClassLoader extends ClassLoader {
	
	private final SuperDexPathList pathList;
	private static PathClassLoader pp;
	private final String TAG = "SuperClassLoader";

	public SuperClassLoader(String libraryPath, ClassLoader parent,
			Application application, PathClassLoader pp) throws IOException {
		super(parent);
		this.pp = pp;
		this.pathList = new SuperDexPathList(this, libraryPath, application);
	}

	@Override
	protected Class<?> findClass(String name) throws ClassNotFoundException {
		Log.i(TAG, "before findClass name is "+name);
		Class c = findLoadedClass(name);
		if(c == null){
			c = pathList.findClass(name);
		}
		if (c == null) {
			Log.e(TAG, "Class not found ");
			throw new ClassNotFoundException(name);
		}
		return c;
	}

	@Override
	protected URL findResource(String name) {
		Log.i(TAG, "in findResource name is " + name);
		Object pathlist = RefInvoke.getFieldOjbect(
				"dalvik.system.BaseDexClassLoader", pp, "pathList");
		URL rezlt = (URL) RefInvoke.invokeMethod("dalvik.system.DexPathList",
				"findResource", pathlist, new Class[] { String.class },
				new Object[] { name });
		if(rezlt == null)
		Log.i(TAG, "resource got failed " + name);
		return rezlt;
	}

	@Override
	protected Enumeration<URL> findResources(String name) {
		Log.i(TAG, "in findResources name is " + name);
		Object pathlist = RefInvoke.getFieldOjbect(
				"dalvik.system.BaseDexClassLoader", pp, "pathList");
		@SuppressWarnings("unchecked")
		Enumeration<URL> rezlt = (Enumeration<URL>) RefInvoke.invokeMethod(
				"dalvik.system.DexPathList", "findResources", pathlist,
				new Class[] { String.class }, new Object[] { name });
		if (rezlt != null)
			Log.i(TAG, "findResources got " + name);
		return rezlt;
	}

	@Override
	public String findLibrary(String name) {
		Log.i(TAG, "in findLibrary name is " + name);
		return pathList.findLibrary(name);
	}

	@Override
	protected synchronized Package getPackage(String name) {
		Log.i(TAG, "in getPackage name is " + name);
		if (name != null && !name.isEmpty()) {
			Package pack = super.getPackage(name);

			if (pack == null) {
				pack = definePackage(name, "Unknown", "0.0", "Unknown",
						"Unknown", "0.0", "Unknown", null);
			}
			return pack;
		}
		return null;
	}

	/**
	 * @hide
	 */
	public String getLdLibraryPath() {
		Log.i(TAG, "in getLdLibraryPath");
		StringBuilder result = new StringBuilder();
		for (File directory : pathList.getNativeLibraryDirectories()) {
			if (result.length() > 0) {
				result.append(':');
			}
			result.append(directory);
		}
		return result.toString();
	}

	@Override
	public String toString() {
		return getClass().getName() + "[" + pathList + "]";
	}

}
