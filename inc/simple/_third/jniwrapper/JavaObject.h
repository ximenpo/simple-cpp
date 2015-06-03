#ifndef JAVAOBJECT_H
#define JAVAOBJECT_H

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

#include <string>

class CJavaObject;

// A class that encapsulates a java object's fields or methods
class CJavaMember
{
public:
    CJavaMember(JNIEnv * env,jclass classTypeReflector,jobject iObject, std::string name) : 
        mEnvironment(env) , mClassTypeReflector(classTypeReflector), mObject(iObject), mName(name)
    {

    }

    CJavaMember(JNIEnv * env,jclass classTypeReflector,jobject iObject, std::string name, std::string javaObjectType) : 
        mEnvironment(env) , mClassTypeReflector(classTypeReflector), mObject(iObject), mName(name), mType(javaObjectType)
    {

    }

    CJavaMember(CJavaObject* javaobj, std::string name, std::string javaObjectType); //implemented at the bottom under CJavaObject definition

    CJavaMember(CJavaObject* javaobj, std::string name); //implemented at the bottom under CJavaObject definition



    CJavaMember operator [](std::string  fieldAndType) 
    { 
        return CJavaMember(mEnvironment, mClassTypeReflector,mObject,  mName, fieldAndType); 
    }

    //////////////////////////////////////////////////////////////////////////
    //Set fields

    template <typename ASSIGN_TYPE>
    CJavaMember& operator = (ASSIGN_TYPE value)
    {
        static_assert(sizeof(ASSIGN_TYPE) == -1, "CJavaMember assignment type not implemented");
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    //Get fields (cast)

    template <typename CAST_TYPE>
    operator CAST_TYPE() const
    { 
        return cast<CAST_TYPE>();
    }

    template  <typename CAST_TYPE>
    CAST_TYPE cast() const
    {
        static_assert(sizeof(CAST_TYPE) == -1, "CJavaMember cast not implemented");
    }

    //////////////////////////////////////////////////////////////////////////
    // Functions

    template <typename... ARGS>
    void operator() (ARGS... args )
    {
       	jmethodID mid = mEnvironment->GetMethodID(mClassTypeReflector, mName.c_str(), mType.c_str());
    	JavaCallFunctionToList(mid, args...);
    }

    template <typename JAVA_RETURN_TYPE, typename... ARGS>
    void operator() (JAVA_RETURN_TYPE* returnValue, ARGS... args)
    { 
    	jmethodID mid = mEnvironment->GetMethodID(mClassTypeReflector, mName.c_str(), mType.c_str());
        *returnValue = JavaCallFunctionToList<JAVA_RETURN_TYPE>(mid, args...);
    }

    JNIEnv * mEnvironment;
    jclass mClassTypeReflector;
    jobject mObject;
    std::string mName;
    std::string mType;

private:
    template <typename JAVA_RETURN_TYPE>
    JAVA_RETURN_TYPE JavaCallFunction (jmethodID  mid, va_list iParams)
    {
        static_assert(sizeof(JAVA_RETURN_TYPE) == -1, "Java Function call not implemented for this type");
        return JAVA_RETURN_TYPE();
    }

    template <typename JAVA_RETURN_TYPE>
    JAVA_RETURN_TYPE JavaCallFunctionToList (jmethodID  mid, ...)
    {
    	JAVA_RETURN_TYPE ret;
        va_list vaList;
        va_start (vaList, mid);
        ret  = JavaCallFunction<JAVA_RETURN_TYPE>(mid, vaList);
        va_end(vaList);
        return ret;
    }

    void JavaCallFunctionToList (jmethodID  mid, ...);
};


// A class that encapsulates a jobject
class CJavaObject
{
public:

    CJavaObject(JNIEnv * env, std::string typeName, jobject iClassLoader = NULL): mEnvironment(env), mClassLoader(iClassLoader), clearClass(true)
    {
    	if (iClassLoader == NULL)
    	{
			mClassTypeReflector = env->FindClass(typeName.c_str());
    	}
    	else
    	{
    	    jstring javaString = mEnvironment->NewStringUTF(typeName.c_str());
    		CJavaObject reflector(env, iClassLoader);
    		jobject reflectorClassObject;
    		reflector["findClass"]["(Ljava/lang/String;)Ljava/lang/Class;"](&reflectorClassObject, javaString);
    		mClassTypeReflector = (jclass)reflectorClassObject;
     		mEnvironment->DeleteLocalRef(javaString);
    	}
		jmethodID classConstructor = env->GetMethodID(mClassTypeReflector, "<init>", "()V");
		mActualObject = env->NewObject(mClassTypeReflector, classConstructor);
    }

    CJavaObject(JNIEnv * env, jobject objectReference) : mEnvironment(env), mActualObject(objectReference), mClassLoader(NULL), clearClass(true)
    {
    	mClassTypeReflector = mEnvironment->GetObjectClass(mActualObject);
    }

    CJavaObject(JNIEnv * env, jclass classType) : mEnvironment(env), mClassTypeReflector(classType), mClassLoader(NULL), clearClass(false)
    {
        jmethodID classConstructor = env->GetMethodID(mClassTypeReflector, "<init>", "()V");
        mActualObject = env->NewObject(mClassTypeReflector, classConstructor);
    }

    virtual ~CJavaObject()
    {
    	if (clearClass)
    	{
    		mEnvironment->DeleteLocalRef(mClassTypeReflector);
    	}
    }

    CJavaMember operator [](std::string memberName) { return CJavaMember(this, memberName); }

    // member name and member object type pair
    CJavaMember operator [](std::pair<std::string /*memberName*/, std::string /*javaObjectType*/> fieldAndType) 
    { 
        return CJavaMember(this, fieldAndType.first, fieldAndType.second); 
    }

    operator jobject() 
    {
        return mActualObject;
    }

    JNIEnv * mEnvironment;
    jclass mClassTypeReflector;
    jobject mActualObject;
    jobject mClassLoader;
    bool clearClass;
};

//Include specializations after declaration
#include "JavaObjectSpec.h"

#endif
