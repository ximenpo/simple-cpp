
// Copyright (c) 2010 Francis Xavier Joseph Pulikotil
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef PTR_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define PTR_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include <cstddef>
#include <cstring>
#include <typeinfo>

class any_ptr {
private:
    const void *obj_;
    const void *type_id_;
    const char *type_name_;

    template <class T>
    const void* TypeId()const {
        //static const char typeId( 0 );
        //return reinterpret_cast<std::size_t>( &typeId );
        return &typeid(T);
    }

    template <class T>
    const char* TypeName()const {
        //static const char typeId( 0 );
        //return reinterpret_cast<std::size_t>( &typeId );
        return typeid(T).name();
    }

public:
    // Default Constructor
    any_ptr() :
        obj_( 0 ),
        type_id_( 0 ),
        type_name_( 0 ) {}

    // Construct from object pointer
    template <class T>
    any_ptr(T *const pObject) :
        obj_( pObject ),
        type_id_( TypeId<T>() ),
        type_name_( TypeName<T>() ) {}

    // Copy Constructor
    any_ptr(const any_ptr &other) :
        obj_( other.obj_ ),
        type_id_( other.type_id_ ),
        type_name_( other.type_name_ ) {}

    // Assignment Operator
    const any_ptr &operator =(const any_ptr &other) {
        obj_ = other.obj_;
        type_id_  = other.type_id_;
        type_name_ = other.type_name_;
        return (*this);
    }

    // Assign from object pointer
    template <class T>
    const any_ptr &operator =(T *const pObject) {
        obj_ = pObject;
        type_id_  = TypeId<T>();
        type_name_ = TypeName<T>();
        return (*this);
    }

    // Implicit cast to non-const object pointer
    template <class T>
    operator T *const() const {
        if( type_id_ == TypeId<T>() ) {
            return static_cast<T *const>( const_cast<void *const>(obj_) );
        }

        const char *name	= TypeName<T>();
        if( 0 != type_name_ && 0 != name ) {
            if( type_name_ == name || 0 == strcmp(type_name_, name) ) {
                return static_cast<T *const>( const_cast<void *const>(obj_) );
            }
        }

        return 0;
    }

    // Implicit cast to const object pointer
    template <class T>
    operator const T *const() const {
        if( (type_id_ == TypeId<const T>()) || (type_id_ == TypeId<T>()) ) {
            return static_cast<const T *const>( obj_ );
        }

        const char *name	= TypeName<T>();
        if( 0 != type_name_ && 0 != name ) {
            if( type_name_ == name || 0 == strcmp(type_name_, name) ) {
                return static_cast<const T *const>( obj_ );
            }
        }

        return 0;
    }

    // Test for emptiness
    const bool empty() const {
        return (obj_ == 0);
    }
};

#endif
