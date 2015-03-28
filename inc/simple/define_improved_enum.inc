
//////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANT NOTE:
// This is a "batch file of preprocessing directives" (because this cannot be done with a macro).
// Each time you include this file you are calling a batch file, it doesn't work as a macro include.
// If you want to declare several different enum types, you have to include this file several times.
// Do not use "#pragma once" directive, because it would have unexpected behaviour and results.
// Do not use directives like: #ifndef _IMPROVED_ENUM_H_ ; #define _IMPROVED_ENUM_H_ (same reason).
//////////////////////////////////////////////////////////////////////////////////////////////////////
// AUTHOR:      Hugo Gonz醠ez Castro
// EMAIL:       webmaster@protoidea.org
// TITLE:       Improving C++ Enum: Adding Serialization, Inheritance and Iteration.
// DESCRIPTION: A different approach to C++ enums: enum to string, enum extension and enum iteration.
// VERSION:     v5.0 - 2009/04/13
// LICENSE:     CPOL (Code Project Open License). Please, do not remove nor modify this header.
// URL:         http://www.codeproject.com/KB/cpp/ImprovedEnum.aspx
//////////////////////////////////////////////////////////////////////////////////////////////////////
// INPUT PARAMETERS:
// This file needs the following input parameters to be defined before including it:
// #define IMPROVED_ENUM_NAME [NameOfYourEnum] // Input parameter: the name of the enumeration
// #define IMPROVED_ENUM_FILE ["EnumItemFile"] // Input parameter: the file with the enum items
//////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPROVED_ENUM_ITEM FILE:
// The EnumItemFile is a list (one per line) of: IMPROVED_ENUM_ITEM(EnumItem) or IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value)
//////////////////////////////////////////////////////////////////////////////////////////////////////
// ALTERNATIVE TO IMPROVED_ENUM_ITEM FILE: IMPROVED_ENUM_LIST instead of IMPROVED_ENUM_FILE
// #define IMPROVED_ENUM_LIST  IMPROVED_ENUM_ITEM(Item1) IMPROVED_ENUM_ITEM(Item2) ... IMPROVED_ENUM_ITEM(LastItem)
// #define IMPROVED_ENUM_LIST  IMPROVED_ENUM_ITEM(Item1) \
//                             IMPROVED_ENUM_ITEM(Item2) \
//                             ...
//                             IMPROVED_ENUM_ITEM(LastItem)
//////////////////////////////////////////////////////////////////////////////////////////////////////
// OPTIONAL INPUT PARAMETERS:
// If you want to define a subclass instead of a namespace, you can #define IMPROVED_ENUM_SUBCLASS,
// or #define IMPROVED_ENUM_SUBCLASS_PARENT [ParentClass] to make subclass inherit from a ParentClass.
// If you want to extend an already defined ImprovedEnum, you have to define which type do you want
// to extend with IMPROVED_ENUM_INHERITED_NAME and IMPROVED_ENUM_INHERITED_FILE input parameters.
//////////////////////////////////////////////////////////////////////////////////////////////////////


// Checking IMPROVED_ENUM_ITEM and IMPROVED_ENUM_ITEM_VALUE macros are not already defined
#if defined(IMPROVED_ENUM_ITEM)
#error IMPROVED_ENUM_ITEM macro cannot be already defined
#elif defined(IMPROVED_ENUM_ITEM_VALUE)
#error IMPROVED_ENUM_ITEM_VALUE macro cannot be already defined
#endif

// Standard string class
#include <string>


#if defined(IMPROVED_ENUM_SUBCLASS_PARENT)

//! We define the IMPROVED_ENUM_NAME subclass (that
//! inherits from the specified parent class) which contains
//! the enum type and the static conversion methods from the
//! enum type to the string type and vice versa.
///////////////////////////////////////////////////////////
#define STATIC_METHOD static
class IMPROVED_ENUM_NAME : public IMPROVED_ENUM_SUBCLASS_PARENT {
public:

#elif defined(IMPROVED_ENUM_SUBCLASS)

//! We define the IMPROVED_ENUM_NAME subclass, which contains
//! the enum type and the static conversion methods from the
//! enum type to the string type and vice versa.
///////////////////////////////////////////////////////////
#define STATIC_METHOD static
class IMPROVED_ENUM_NAME {
public:

#else // IMPROVED_ENUM_SUBCLASS || IMPROVED_ENUM_SUBCLASS_PARENT

//! We define the IMPROVED_ENUM_NAME namespace, which contains
//! the enum type and the conversion functions from the
//! enum type to the string type and vice versa.
///////////////////////////////////////////////////////////
#define STATIC_METHOD
namespace IMPROVED_ENUM_NAME {

#endif // IMPROVED_ENUM_SUBCLASS || IMPROVED_ENUM_SUBCLASS_PARENT

    //! Some stuff to get the string of the IMPROVED_ENUM_NAME
    ///////////////////////////////////////////////////////////
#define GET_MACRO_STRING_EXPANDED(Macro)  #Macro
#define GET_MACRO_STRING(Macro)  GET_MACRO_STRING_EXPANDED(Macro)
#define ENUM_SEPARATOR  "::"
#define ENUM_TYPE_NAME  GET_MACRO_STRING(IMPROVED_ENUM_NAME)
    STATIC_METHOD inline const std::string enum_separator() {
        return ENUM_SEPARATOR;
    }
    STATIC_METHOD inline const std::string enum_type_name() {
        return ENUM_TYPE_NAME;
    }
#ifdef  IMPROVED_ENUM_INHERITED_NAME
#define PARENT_ENUM_TYPE_NAME  GET_MACRO_STRING(IMPROVED_ENUM_INHERITED_NAME)
#define FULL_ENUM_TYPE_NAME    PARENT_ENUM_TYPE_NAME  ENUM_SEPARATOR  ENUM_TYPE_NAME
#else //IMPROVED_ENUM_INHERITED_NAME
#define PARENT_ENUM_TYPE_NAME  ""
#define FULL_ENUM_TYPE_NAME    ENUM_TYPE_NAME
#endif//IMPROVED_ENUM_INHERITED_NAME
    STATIC_METHOD inline const std::string enum_parent_type_name() {
        return PARENT_ENUM_TYPE_NAME;
    }
    STATIC_METHOD inline const std::string enum_full_type_name() {
        return FULL_ENUM_TYPE_NAME;
    }


    //! This defines the enumerated type:
    //////////////////////////////////////////
    typedef enum EnumTypeTag {
        //////////////////////////////////////////
        // With this mini-macro we make IMPROVED_ENUM_ITEM file/s
        // a list of items separated by commas:
#define  IMPROVED_ENUM_ITEM(EnumItem) EnumItem,
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) EnumItem = Value,
#ifdef   IMPROVED_ENUM_INHERITED_FILE
#include IMPROVED_ENUM_INHERITED_FILE
#endif// IMPROVED_ENUM_INHERITED_FILE
#ifdef   IMPROVED_ENUM_FILE
#include IMPROVED_ENUM_FILE
#else // IMPROVED_ENUM_LIST
        IMPROVED_ENUM_LIST
#endif// IMPROVED_ENUM_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
        //////////////////////////////////////////
        enum_item_invalid // We add this item to all enums
    } enum_type, Type;

    //! Conversion from enum to string:
    //////////////////////////////////////////
    STATIC_METHOD inline const std::string enum_to_string(const enum_type& t) {
        switch (t) {
            //////////////////////////////////////////
            // With this mini-macro we make IMPROVED_ENUM_ITEM file/s
            // a CASE list which returns the stringized value:
#define  IMPROVED_ENUM_ITEM(EnumItem) case EnumItem : return #EnumItem;
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#ifdef   IMPROVED_ENUM_INHERITED_FILE
#include IMPROVED_ENUM_INHERITED_FILE
#endif// IMPROVED_ENUM_INHERITED_FILE
#ifdef   IMPROVED_ENUM_FILE
#include IMPROVED_ENUM_FILE
#else // IMPROVED_ENUM_LIST
            IMPROVED_ENUM_LIST
#endif// IMPROVED_ENUM_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
        //////////////////////////////////////////
        default:
            break;
        }
        return ""; // enum_item_invalid
    }

    //! Conversion from enum to full string (namespace::string):
    /////////////////////////////////////////////////////////////
    STATIC_METHOD inline const std::string enum_to_full_string(const enum_type& t) {
        switch (t) {
            //////////////////////////////////////////
            // With this mini-macro we make IMPROVED_ENUM_ITEM file/s
            // a CASE list which returns the stringized value:
#define  IMPROVED_ENUM_ITEM(EnumItem) case EnumItem : return  FULL_ENUM_TYPE_NAME  ENUM_SEPARATOR  #EnumItem;
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#ifdef   IMPROVED_ENUM_INHERITED_FILE
#include IMPROVED_ENUM_INHERITED_FILE
#endif// IMPROVED_ENUM_INHERITED_FILE
#ifdef   IMPROVED_ENUM_FILE
#include IMPROVED_ENUM_FILE
#else // IMPROVED_ENUM_LIST
            IMPROVED_ENUM_LIST
#endif// IMPROVED_ENUM_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
        //////////////////////////////////////////
        default:
            break;
        }
        return ""; // enum_item_invalid
    }

    //! Conversion from string to enum:
    //////////////////////////////////////////
    STATIC_METHOD inline const enum_type enum_from_string(const std::string& s) {
        if (s == "") return enum_item_invalid;
        //////////////////////////////////////////
        // With this mini-macro we make IMPROVED_ENUM_ITEM file/s
        // an IF list which returns the enum item:
#define  IMPROVED_ENUM_ITEM(EnumItem) if (s == #EnumItem) return EnumItem;
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#ifdef   IMPROVED_ENUM_INHERITED_FILE
#include IMPROVED_ENUM_INHERITED_FILE
#endif// IMPROVED_ENUM_INHERITED_FILE
#ifdef   IMPROVED_ENUM_FILE
#include IMPROVED_ENUM_FILE
#else // IMPROVED_ENUM_LIST
        IMPROVED_ENUM_LIST
#endif// IMPROVED_ENUM_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
        //////////////////////////////////////////
        return enum_item_invalid;
    }

    //! Conversion from full string (namespace::string) to enum:
    /////////////////////////////////////////////////////////////
    STATIC_METHOD inline const enum_type enum_from_full_string(const std::string& s) {
        if (s == "") return enum_item_invalid;
        //////////////////////////////////////////
        // With this mini-macro we make IMPROVED_ENUM_ITEM file/s
        // an IF list which returns the enum item:
#define  IMPROVED_ENUM_ITEM(EnumItem) if (s ==  FULL_ENUM_TYPE_NAME  ENUM_SEPARATOR  #EnumItem) return EnumItem;
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#ifdef   IMPROVED_ENUM_INHERITED_FILE
#include IMPROVED_ENUM_INHERITED_FILE
#endif// IMPROVED_ENUM_INHERITED_FILE
#ifdef   IMPROVED_ENUM_FILE
#include IMPROVED_ENUM_FILE
#else // IMPROVED_ENUM_LIST
        IMPROVED_ENUM_LIST
#endif// IMPROVED_ENUM_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
        //////////////////////////////////////////
        return enum_item_invalid;
    }

    //! Enum iteration to next:
    //////////////////////////////////////////
    STATIC_METHOD inline const enum_type enum_next_item(const enum_type& t) {
        switch (t) {
        case enum_item_invalid :
            //////////////////////////////////////////
            // With this mini-macro we make IMPROVED_ENUM_ITEM file/s
            // a CASE list which returns the next item:
#define  IMPROVED_ENUM_ITEM(EnumItem) return EnumItem; case EnumItem :
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#ifdef   IMPROVED_ENUM_INHERITED_FILE
#include IMPROVED_ENUM_INHERITED_FILE
#endif// IMPROVED_ENUM_INHERITED_FILE
#ifdef   IMPROVED_ENUM_FILE
#include IMPROVED_ENUM_FILE
#else // IMPROVED_ENUM_LIST
            IMPROVED_ENUM_LIST
#endif// IMPROVED_ENUM_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
            //////////////////////////////////////////
            return enum_item_invalid; // (This indentation is intentional)
        }
        return enum_item_invalid; // (This line is intentional too, do not remove)
    }

    //! Enum iteration to previous:
    //////////////////////////////////////////
    STATIC_METHOD inline const enum_type enum_previous_item(const enum_type& t) {
        enum_type tprev = enum_item_invalid;
        //////////////////////////////////////////
        // With this mini-macro we make IMPROVED_ENUM_ITEM file/s
        // an IF list which returns the previous item:
#define  IMPROVED_ENUM_ITEM(EnumItem) if (t == EnumItem) return tprev; else tprev = EnumItem;
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#ifdef   IMPROVED_ENUM_INHERITED_FILE
#include IMPROVED_ENUM_INHERITED_FILE
#endif// IMPROVED_ENUM_INHERITED_FILE
#ifdef   IMPROVED_ENUM_FILE
#include IMPROVED_ENUM_FILE
#else // IMPROVED_ENUM_LIST
        IMPROVED_ENUM_LIST
#endif// IMPROVED_ENUM_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
        //////////////////////////////////////////
        return tprev;
    }

    //! The first and the last Enums:
    //////////////////////////////////////////
    STATIC_METHOD inline const enum_type enum_first_item() {
        return enum_next_item(enum_item_invalid);
    }
    STATIC_METHOD inline const enum_type enum_last_item() {
        return enum_previous_item(enum_item_invalid);
    }

    //! Number of enum items:
    //////////////////////////////////////////
    STATIC_METHOD inline const int enum_valid_item_sum() {
        return 0
               //////////////////////////////////////////
               // With this mini-macro we make IMPROVED_ENUM_ITEM file/s
               // a counter list:
#define  IMPROVED_ENUM_ITEM(EnumItem) +1
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#ifdef   IMPROVED_ENUM_INHERITED_FILE
#include IMPROVED_ENUM_INHERITED_FILE
#endif// IMPROVED_ENUM_INHERITED_FILE
#ifdef   IMPROVED_ENUM_FILE
#include IMPROVED_ENUM_FILE
#else // IMPROVED_ENUM_LIST
               IMPROVED_ENUM_LIST
#endif// IMPROVED_ENUM_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
               //////////////////////////////////////////
               ;
    }


    // This is only needed when working with inherited/extended enums:
    ////////////////////////////////////////////////////////////////////
#ifdef IMPROVED_ENUM_INHERITED_NAME
    //! Conversion from inherited enums:
    //! The same class items are returned without change, but
    //! other items are converted from one namespace to the other:
    //////////////////////////////////////////
    STATIC_METHOD inline const enum_type enum_from_inherited(const enum_type& t) {
        return t;
    }
    STATIC_METHOD inline const enum_type enum_from_inherited(const IMPROVED_ENUM_INHERITED_NAME::enum_type& t) {
        switch (t) {
            //////////////////////////////////////////
            // With this mini-macro we make IMPROVED_ENUM_ITEM file
            // a CASE list which returns the converted value
            // from one namespace to the other:
#define  IMPROVED_ENUM_ITEM(EnumItem) case IMPROVED_ENUM_INHERITED_NAME::EnumItem : return EnumItem;
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#ifdef   IMPROVED_ENUM_INHERITED_FILE
#include IMPROVED_ENUM_INHERITED_FILE
#endif// IMPROVED_ENUM_INHERITED_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
        //////////////////////////////////////////
        default:
            break;
        }
        return enum_item_invalid;
    }

    //! Conversion to inherited enums:
    //! The same class items are returned without change, but
    //! other items are converted from one namespace to the other:
    //////////////////////////////////////////
    STATIC_METHOD inline const IMPROVED_ENUM_INHERITED_NAME::enum_type enum_to_inherited(const IMPROVED_ENUM_INHERITED_NAME::enum_type& t) {
        return t;
    }
    STATIC_METHOD inline const IMPROVED_ENUM_INHERITED_NAME::enum_type enum_to_inherited(const enum_type& t) {
        switch (t) {
            //////////////////////////////////////////
            // With this mini-macro we make IMPROVED_ENUM_ITEM file
            // a CASE list which returns the converted value
            // from one namespace to the other:
#define  IMPROVED_ENUM_ITEM(EnumItem) case EnumItem : return IMPROVED_ENUM_INHERITED_NAME::EnumItem;
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#ifdef   IMPROVED_ENUM_INHERITED_FILE
#include IMPROVED_ENUM_INHERITED_FILE
#endif// IMPROVED_ENUM_INHERITED_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
        //////////////////////////////////////////
        default:
            break;
        }
        return IMPROVED_ENUM_INHERITED_NAME::enum_item_invalid;
    }


    //! Enum iteration to next extended (not inherited):
    //////////////////////////////////////////
    STATIC_METHOD inline const enum_type enum_next_extended_item(const enum_type& t) {
        switch (t) {
        case enum_item_invalid :
            //////////////////////////////////////////
            // With this mini-macro we make IMPROVED_ENUM_ITEM file/s
            // a CASE list which returns the next item:
#define  IMPROVED_ENUM_ITEM(EnumItem) return EnumItem; case EnumItem :
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#ifdef   IMPROVED_ENUM_FILE
#include IMPROVED_ENUM_FILE
#else // IMPROVED_ENUM_LIST
            IMPROVED_ENUM_LIST
#endif// IMPROVED_ENUM_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
            //////////////////////////////////////////
            return enum_item_invalid;
        default:
            break;
        }
        return enum_item_invalid;
    }

    //! Enum iteration to previous extended (not inherited):
    //////////////////////////////////////////
    STATIC_METHOD inline const enum_type enum_previous_extended_item(const enum_type& t) {
        enum_type tprev = enum_item_invalid;
        //////////////////////////////////////////
        // With this mini-macro we make IMPROVED_ENUM_ITEM file/s
        // an IF list which returns the previous item:
#define  IMPROVED_ENUM_ITEM(EnumItem) if (t == EnumItem) return tprev; else tprev = EnumItem;
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#ifdef   IMPROVED_ENUM_FILE
#include IMPROVED_ENUM_FILE
#else // IMPROVED_ENUM_LIST
        IMPROVED_ENUM_LIST
#endif// IMPROVED_ENUM_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
        //////////////////////////////////////////
        return tprev;
    }

    //! The first and the last extended Enums:
    //////////////////////////////////////////
    STATIC_METHOD inline const enum_type enum_first_extended_item() {
        return enum_next_extended_item(enum_item_invalid);
    }
    STATIC_METHOD inline const enum_type enum_last_extended_item() {
        return enum_previous_extended_item(enum_item_invalid);
    }

    //! Number of extended enum items:
    //////////////////////////////////////////
    STATIC_METHOD inline const int enum_valid_extended_item_sum() {
        return 0
               //////////////////////////////////////////
               // With this mini-macro we make IMPROVED_ENUM_ITEM file
               // a counter list:
#define  IMPROVED_ENUM_ITEM(EnumItem) +1
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#ifdef   IMPROVED_ENUM_FILE
#include IMPROVED_ENUM_FILE
#else // IMPROVED_ENUM_LIST
               IMPROVED_ENUM_LIST
#endif// IMPROVED_ENUM_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
               //////////////////////////////////////////
               ;
    }


    //! Enum iteration to next inherited:
    //////////////////////////////////////////
    STATIC_METHOD inline const enum_type enum_next_inherited_item(const enum_type& t) {
        switch (t) {
        case enum_item_invalid :
            //////////////////////////////////////////
            // With this mini-macro we make IMPROVED_ENUM_ITEM file/s
            // a CASE list which returns the next item:
#define  IMPROVED_ENUM_ITEM(EnumItem) return EnumItem; case EnumItem :
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#include IMPROVED_ENUM_INHERITED_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
            //////////////////////////////////////////
            return enum_item_invalid;
        default:
            break;
        }
        return enum_item_invalid;
    }

    //! Enum iteration to previous inherited:
    //////////////////////////////////////////
    STATIC_METHOD inline const enum_type enum_previous_inherited_item(const enum_type& t) {
        enum_type tprev = enum_item_invalid;
        //////////////////////////////////////////
        // With this mini-macro we make IMPROVED_ENUM_ITEM file/s
        // an IF list which returns the previous item:
#define  IMPROVED_ENUM_ITEM(EnumItem) if (t == EnumItem) return tprev; else tprev = EnumItem;
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#include IMPROVED_ENUM_INHERITED_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
        //////////////////////////////////////////
        return tprev;
    }

    //! The first and the last inherited Enums:
    //////////////////////////////////////////
    STATIC_METHOD inline const enum_type enum_first_inherited_item() {
        return enum_next_inherited_item(enum_item_invalid);
    }
    STATIC_METHOD inline const enum_type enum_last_inherited_item() {
        return enum_previous_inherited_item(enum_item_invalid);
    }

    //! Number of inherited enum items:
    //////////////////////////////////////////
    STATIC_METHOD inline const int enum_valid_inherited_item_sum() {
        return 0
               //////////////////////////////////////////
               // With this mini-macro we make IMPROVED_ENUM_ITEM file
               // a counter list:
#define  IMPROVED_ENUM_ITEM(EnumItem) +1
#define  IMPROVED_ENUM_ITEM_VALUE(EnumItem, Value) IMPROVED_ENUM_ITEM(EnumItem)
#include IMPROVED_ENUM_INHERITED_FILE
#undef   IMPROVED_ENUM_ITEM_VALUE
#undef   IMPROVED_ENUM_ITEM
               //////////////////////////////////////////
               ;
    }

#endif // IMPROVED_ENUM_INHERITED_NAME

    // Free temporary macros:
    ///////////////////////////
#undef STATIC_METHOD
#undef ENUM_SEPARATOR
#undef ENUM_TYPE_NAME
#undef PARENT_ENUM_TYPE_NAME
#undef FULL_ENUM_TYPE_NAME
#undef GET_MACRO_STRING
#undef GET_MACRO_STRING_EXPANDED
}
#if defined(IMPROVED_ENUM_SUBCLASS) || defined(IMPROVED_ENUM_SUBCLASS_PARENT)
;
#endif

// Free this file's parameters:
////////////////////////////////
#undef IMPROVED_ENUM_NAME
#undef IMPROVED_ENUM_FILE
#undef IMPROVED_ENUM_LIST
#undef IMPROVED_ENUM_SUBCLASS
#undef IMPROVED_ENUM_SUBCLASS_PARENT
#undef IMPROVED_ENUM_INHERITED_NAME
#undef IMPROVED_ENUM_INHERITED_FILE
// Do not use directives like: #endif // _IMPROVED_ENUM_H_ (reason above)

