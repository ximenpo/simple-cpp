#ifndef JAVASTATICMEMBER_H
#define JAVASTATICMEMBER_H

#include "jni.h"
#include <string>

class CJavaClass; 

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
inline CJavaStaticMember& CJavaStaticMember::operator = <int>(int value) { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "I");
    mEnvironment->SetStaticIntField(mClassTypeReflector,fid,value);
    return *this;
}

template <>
inline CJavaStaticMember& CJavaStaticMember::operator = <long long>(long long value) { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "J");
    mEnvironment->SetStaticLongField(mClassTypeReflector,fid,value);
    return *this;
}

template <>
inline CJavaStaticMember& CJavaStaticMember::operator = <float>(float value) { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "F");
    mEnvironment->SetStaticFloatField(mClassTypeReflector,fid,value);
    return *this;
}

template <>
inline CJavaStaticMember& CJavaStaticMember::operator = <double>(double value) { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "D");
    mEnvironment->SetStaticDoubleField(mClassTypeReflector,fid,value);
    return *this;
}

template <>
inline CJavaStaticMember& CJavaStaticMember::operator = <unsigned short>(unsigned short value) { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "C");
    mEnvironment->SetStaticCharField(mClassTypeReflector,fid,value);
    return *this;
}

template <>
inline CJavaStaticMember& CJavaStaticMember::operator = <short>(short value) { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "S");
    mEnvironment->SetStaticShortField(mClassTypeReflector,fid,value);
    return *this;
}

template <>
inline CJavaStaticMember& CJavaStaticMember::operator = <bool>(bool value) { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "Z");
    mEnvironment->SetStaticBooleanField(mClassTypeReflector,fid,value);
    return *this;
}

template <>
inline CJavaStaticMember& CJavaStaticMember::operator = <void*>(void* value) { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "J");
    mEnvironment->SetStaticLongField(mClassTypeReflector,fid, (jlong)value); //cast pointer to jlong
    return *this;
}


template <>
inline CJavaStaticMember& CJavaStaticMember::operator = <jobject>(jobject value) { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), mType.c_str());
    mEnvironment->SetStaticObjectField(mClassTypeReflector,fid, value); //cast pointer to jlong
    return *this;
}

template <>
inline CJavaStaticMember& CJavaStaticMember::operator = <std::string>(std::string value) { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), mType.c_str());
    jstring javaString = mEnvironment->NewStringUTF(value.c_str());
    mEnvironment->SetStaticObjectField(mClassTypeReflector,fid, javaString);
    mEnvironment->DeleteLocalRef(javaString);
    return *this;
}

//////////////////////////////////////////////////////////////////////////
//Get fields (cast)

template <>
inline void* CJavaStaticMember::cast<void*>() const { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "J");
    jlong nativePointer = mEnvironment->GetStaticLongField(mClassTypeReflector,fid);
    return (void*) nativePointer;
}


template <>
inline int CJavaStaticMember::cast<int>() const  { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "I");
    return mEnvironment->GetStaticIntField(mClassTypeReflector,fid);
}

template <>
inline long long CJavaStaticMember::cast<long long>() const  { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "J");
    return mEnvironment->GetStaticLongField(mClassTypeReflector,fid);
}

template <>
inline float CJavaStaticMember::cast<float>() const  { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "F");
    return mEnvironment->GetStaticFloatField(mClassTypeReflector,fid);
}

template <>
inline double CJavaStaticMember::cast<double>() const  { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "D");
    return mEnvironment->GetStaticDoubleField(mClassTypeReflector,fid);
}

template <>
inline unsigned short CJavaStaticMember::cast<unsigned short>() const { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "C");
    return mEnvironment->GetStaticCharField(mClassTypeReflector,fid);
}

template <>
inline short CJavaStaticMember::cast<short>() const { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "S");
    return mEnvironment->GetStaticShortField(mClassTypeReflector,fid);
}

template <>
inline bool CJavaStaticMember::cast<bool>() const { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), "Z");
    return mEnvironment->GetStaticBooleanField(mClassTypeReflector,fid) == JNI_TRUE;
}

template <>
inline jobject CJavaStaticMember::cast<jobject>() const { 
    jfieldID fid = mEnvironment->GetStaticFieldID(mClassTypeReflector, mName.c_str(), mType.c_str());
    return mEnvironment->GetStaticObjectField(mClassTypeReflector,fid);
}


//////////////////////////////////////////////////////////////////////////
// Functions

template <>
inline void CJavaStaticMember::JavaCallFunction<void> (jmethodID  mid, va_list iParams) //void no return value
{
    mEnvironment->CallStaticVoidMethodV(mClassTypeReflector,mid, iParams);
}

inline void CJavaStaticMember::JavaCallFunctionToList (jmethodID  mid, ...)
{
    va_list vaList;
    va_start (vaList, mid);
    JavaCallFunction<void>(mid, vaList);
    va_end(vaList);
}

template <>
inline jint CJavaStaticMember::JavaCallFunction<jint> (jmethodID  mid, va_list iParams) 
{  
    return mEnvironment->CallStaticIntMethodV(mClassTypeReflector,mid, iParams);
}

template <>
inline jlong CJavaStaticMember::JavaCallFunction<jlong>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallStaticLongMethodV(mClassTypeReflector,mid, iParams);
}

template <>
inline jfloat CJavaStaticMember::JavaCallFunction<jfloat>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallStaticFloatMethodV(mClassTypeReflector,mid, iParams);
}

template <>
inline jdouble CJavaStaticMember::JavaCallFunction<jdouble>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallStaticDoubleMethodV(mClassTypeReflector,mid, iParams);
}

template <>
inline jchar CJavaStaticMember::JavaCallFunction<jchar>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallStaticCharMethodV(mClassTypeReflector,mid, iParams);
}

template <>
inline jshort CJavaStaticMember::JavaCallFunction<jshort>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallStaticShortMethodV(mClassTypeReflector,mid, iParams);
}

template <>
inline jboolean CJavaStaticMember::JavaCallFunction<jboolean>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallStaticBooleanMethodV(mClassTypeReflector,mid, iParams);
}

template <>
inline jobject CJavaStaticMember::JavaCallFunction<jobject>(jmethodID  mid, va_list iParams)  { 
    return mEnvironment->CallStaticObjectMethodV(mClassTypeReflector,mid, iParams);
}

//inline constructor implementation

inline CJavaStaticMember::CJavaStaticMember(CJavaClass * javaClass, std::string name) : 
mEnvironment(javaClass->mEnvironment) , mClassTypeReflector(javaClass->mClassTypeReflector),  mName(name)
{

}

inline CJavaStaticMember::CJavaStaticMember(CJavaClass * javaClass, std::string name, std::string javaObjectType) : 
mEnvironment(javaClass->mEnvironment) , mClassTypeReflector(javaClass->mClassTypeReflector), mName(name), mType(javaObjectType)
{

}

#endif