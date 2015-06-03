#ifndef JAVAMEMBER_H
#define JAVAMEMBER_H

#include "jni.h"
#include <string>


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

//////////////////////////////////////////////////////////////////////////
//Set fields

template <>
inline CJavaMember& CJavaMember::operator = <int>(int value) { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "I");
    mEnvironment->SetIntField(mObject,fid,value);
    return *this;
}

template <>
inline CJavaMember& CJavaMember::operator = <long long>(long long value) { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "J");
    mEnvironment->SetLongField(mObject,fid,value);
    return *this;
}

template <>
inline CJavaMember& CJavaMember::operator = <float>(float value) { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "F");
    mEnvironment->SetFloatField(mObject,fid,value);
    return *this;
}

template <>
inline CJavaMember& CJavaMember::operator = <double>(double value) { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "D");
    mEnvironment->SetDoubleField(mObject,fid,value);
    return *this;
}

template <>
inline CJavaMember& CJavaMember::operator = <unsigned short>(unsigned short value) { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "C");
    mEnvironment->SetCharField(mObject,fid,value);
    return *this;
}

template <>
inline CJavaMember& CJavaMember::operator = <short>(short value) { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "S");
    mEnvironment->SetShortField(mObject,fid,value);
    return *this;
}

template <>
inline CJavaMember& CJavaMember::operator = <bool>(bool value) { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "Z");
    mEnvironment->SetBooleanField(mObject,fid,value);
    return *this;
}

template <>
inline CJavaMember& CJavaMember::operator = <void*>(void* value) { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "J");
    mEnvironment->SetLongField(mObject,fid, (jlong)value); //cast pointer to jlong
    return *this;
}


template <>
inline CJavaMember& CJavaMember::operator = <jobject>(jobject value) { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), mType.c_str());
    mEnvironment->SetObjectField(mObject,fid, value); //cast pointer to jlong
    return *this;
}

template <>
inline CJavaMember& CJavaMember::operator = <std::string>(std::string value) { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), mType.c_str());
    jstring javaString = mEnvironment->NewStringUTF(value.c_str());
    mEnvironment->SetObjectField(mObject,fid, javaString);
    mEnvironment->DeleteLocalRef(javaString);
    return *this;
}

//////////////////////////////////////////////////////////////////////////
//Get fields (cast)

template <>
inline void* CJavaMember::cast<void*>() const  { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "J");
    jlong nativePointer = mEnvironment->GetLongField(mObject,fid);
    return (void*) nativePointer;
}


template <>
inline int CJavaMember::cast<int>() const  { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "I");
    return mEnvironment->GetIntField(mObject,fid);
}

template <>
inline long long CJavaMember::cast<long long>() const  { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "J");
    return mEnvironment->GetLongField(mObject,fid);
}

template <>
inline float CJavaMember::cast<float>() const  { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "F");
    return mEnvironment->GetFloatField(mObject,fid);
}

template <>
inline double CJavaMember::cast<double>() const  { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "D");
    return mEnvironment->GetDoubleField(mObject,fid);
}

template <>
inline unsigned short CJavaMember::cast<unsigned short>() const { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "C");
    return mEnvironment->GetCharField(mObject,fid);
}

template <>
inline short CJavaMember::cast<short>() const { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "S");
    return mEnvironment->GetShortField(mObject,fid);
}

template <>
inline bool CJavaMember::cast<bool>() const { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), "Z");
    return mEnvironment->GetBooleanField(mObject,fid) == JNI_TRUE;
}

template <>
inline jobject CJavaMember::cast<jobject>() const { 
    jfieldID fid = mEnvironment->GetFieldID(mClassTypeReflector, mName.c_str(), mType.c_str());
    return mEnvironment->GetObjectField(mObject,fid);
}

//////////////////////////////////////////////////////////////////////////
// Functions

template <>
inline void CJavaMember::JavaCallFunction<void> (jmethodID  mid, va_list iParams) //void no return value
{
    mEnvironment->CallVoidMethodV(mObject,mid, iParams);
}

inline void CJavaMember::JavaCallFunctionToList (jmethodID  mid, ...)
{
    va_list vaList;
    va_start (vaList, mid);
    JavaCallFunction<void>(mid, vaList);
    va_end(vaList);
}

template <>
inline jint CJavaMember::JavaCallFunction<jint> (jmethodID  mid, va_list iParams) 
{  
    return mEnvironment->CallIntMethodV(mObject,mid, iParams);
}

template <>
inline jlong CJavaMember::JavaCallFunction<jlong>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallLongMethodV(mObject,mid, iParams);
}

template <>
inline jfloat CJavaMember::JavaCallFunction<jfloat>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallFloatMethodV(mObject,mid, iParams);
}

template <>
inline jdouble CJavaMember::JavaCallFunction<jdouble>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallDoubleMethodV(mObject,mid, iParams);
}

template <>
inline jchar CJavaMember::JavaCallFunction<jchar>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallCharMethodV(mObject,mid, iParams);
}

template <>
inline jshort CJavaMember::JavaCallFunction<jshort>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallShortMethodV(mObject,mid, iParams);
}

template <>
inline jboolean CJavaMember::JavaCallFunction<jboolean>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallBooleanMethodV(mObject,mid, iParams);
}

template <>
inline jobject CJavaMember::JavaCallFunction<jobject>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallObjectMethodV(mObject,mid, iParams);
}


//inline constructor implementation

inline CJavaMember::CJavaMember (CJavaObject* javaobj, std::string name, std::string javaObjectType) :
mEnvironment(javaobj->mEnvironment) , mClassTypeReflector(javaobj->mClassTypeReflector), mObject(javaobj->mActualObject), mName(name), mType(javaObjectType)
{

}
inline CJavaMember::CJavaMember(CJavaObject* javaobj, std::string name):
mEnvironment(javaobj->mEnvironment) , mClassTypeReflector(javaobj->mClassTypeReflector), mObject(javaobj->mActualObject), mName(name)
{

}

#endif