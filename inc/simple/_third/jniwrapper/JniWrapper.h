#ifndef JNIWRAPPER_H
#define JNIWRAPPER_H

/*
 *  (C) Copyright 2015 Yochai Timmer
 *
 * Distributed under the Code Project Open License (CPOL) (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.codeproject.com/info/cpol10.aspx
 *
 */


#include "jni.h"
#include "JavaClass.h"

#include "JavaObject.h"


// RAII Wrapper for java strings
class CJavaToString
{
public:
    CJavaToString(JNIEnv * env, jstring iJavaString) : mEnvironment(env), mJString(iJavaString)
    {
        mString = env->GetStringUTFChars(iJavaString, NULL);
    }

    ~CJavaToString()
    {
        mEnvironment->ReleaseStringUTFChars( mJString, mString);
    }

    operator const char*()
    {
        return mString;
    }

    JNIEnv *    mEnvironment;
    jstring     mJString;
    const char* mString;
};

// RAII Wrapper for java global references
class CJavaGlobalRef
{
public:
    CJavaGlobalRef(JNIEnv * env, const jobject iObject) : mEnvironment(env)
    {
        mGlobalRef = env->NewGlobalRef(iObject);
    }

    ~CJavaGlobalRef()
    {
        mEnvironment->DeleteGlobalRef( mGlobalRef );
    }

    operator jobject()
    {
        return mGlobalRef;
    }

    JNIEnv *    mEnvironment;
    jobject     mGlobalRef;
};


// RAII Wrapper for attaching a java thread
class CJavaThreadEnv
{
public:
    CJavaThreadEnv(JavaVM* iVM) : mVM(iVM)
    {
#ifdef __ANDROID__
        mVM->AttachCurrentThread(&mEnvironment,NULL);
#else
        mVM->AttachCurrentThread((void**)&mEnvironment,NULL);
#endif
    }

    ~CJavaThreadEnv()
    {
        mVM->DetachCurrentThread();
    }

    operator JNIEnv*()
    {
        return mEnvironment;
    }

    JNIEnv*    mEnvironment;
    JavaVM*     mVM;
};

#endif
