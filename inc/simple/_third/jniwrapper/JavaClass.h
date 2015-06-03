#ifndef JAVACLASS_H
#define JAVACLASS_H

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
#include "JavaObject.h"
class CJavaClass; 

// A class that encapsulates a java class' static fields or methods
class CJavaStaticMember
{
public:
    CJavaStaticMember(JNIEnv * env,jclass classTypeReflector, std::string name) : 
        mEnvironment(env) , mClassTypeReflector(classTypeReflector),  mName(name)
    {

    }

    CJavaStaticMember(JNIEnv * env,jclass classTypeReflector, std::string name, std::string javaObjectType) : 
        mEnvironment(env) , mClassTypeReflector(classTypeReflector), mName(name), mType(javaObjectType)
    {

    }

    CJavaStaticMember(CJavaClass * javaClass, std::string name);  //implemented at the bottom under CJavaObject definition

    CJavaStaticMember(CJavaClass * javaClass, std::string name, std::string javaObjectType);  //implemented at the bottom under CJavaObject definition


    CJavaStaticMember operator [](std::string  fieldAndType) 
    { 
        return CJavaStaticMember(mEnvironment, mClassTypeReflector, mName, fieldAndType); 
    }

    //////////////////////////////////////////////////////////////////////////
    //Set fields

    template <typename ASSIGN_TYPE>
    CJavaStaticMember& operator = (ASSIGN_TYPE value)
    {
        static_assert(sizeof(ASSIGN_TYPE) == -1, "CJavaStaticMember assignment type not implemented");
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
        static_assert(sizeof(CAST_TYPE) == -1, "CJavaStaticMember cast not implemented");
    }

    //////////////////////////////////////////////////////////////////////////
    // Functions

    template <typename... ARGS>
     void operator() (ARGS... args )
     {
    	jmethodID mid = mEnvironment->GetStaticMethodID(mClassTypeReflector, mName.c_str(), mType.c_str());
     	JavaCallFunctionToList(mid, args...);
     }

     template <typename JAVA_RETURN_TYPE, typename... ARGS>
     void operator() (JAVA_RETURN_TYPE* returnValue, ARGS... args)
     {
     	jmethodID mid = mEnvironment->GetStaticMethodID(mClassTypeReflector, mName.c_str(), mType.c_str());
     	*returnValue = JavaCallFunctionToList<JAVA_RETURN_TYPE>(mid, args...);
     }

    JNIEnv * mEnvironment;
    jclass mClassTypeReflector;
    std::string mName;
    std::string mType;

private:

    template <typename JAVA_RETURN_TYPE>
    JAVA_RETURN_TYPE JavaCallFunction (jmethodID  mid, va_list iParams)
    {
        static_assert(sizeof(JAVA_RETURN_TYPE) == -1, "Java Static Function call not implemented for this type");
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


// A class that encapsulates a jclass
class CJavaClass
{
public:

    CJavaClass(JNIEnv * env, std::string typeName, jobject iClassLoader = NULL): mEnvironment(env)
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
    }

    CJavaClass(JNIEnv * env, jobject objectReference) : mEnvironment(env)
    {
        mClassTypeReflector = mEnvironment->GetObjectClass(objectReference);
    }

    CJavaClass(JNIEnv * env, jclass classType) : mEnvironment(env), mClassTypeReflector(classType)
    {
    }

    virtual ~CJavaClass() {}

    CJavaStaticMember operator [](std::string memberName) { return CJavaStaticMember(this, memberName); }

    // member name and member object type pair
    CJavaStaticMember operator [](std::pair<std::string /*memberName*/, std::string /*javaObjectType*/> fieldAndType) 
    { 
        return CJavaStaticMember(this, fieldAndType.first, fieldAndType.second); 
    }

    operator jclass() 
    {
        return mClassTypeReflector;
    }

    JNIEnv * mEnvironment;
    jclass mClassTypeReflector;
};

//Include specializations after declaration
#include "JavaClassSpec.h"

#endif
