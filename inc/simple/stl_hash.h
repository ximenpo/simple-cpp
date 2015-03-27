#ifndef STL_HASH_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define STL_HASH_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#if	defined(_MSC_VER)
#	if	 _MSC_VER < 1310	//VS2003
#		include <hash_map>
#		include <hash_set>
using	std::hash_map;
using	std::hash_set;
#	else
#		include <hash_map>
#		include <hash_set>
using	stdext::hash_map;
using	stdext::hash_set;
#	endif
#endif

#if	defined(__GNUC__)
#	if __GNUC__ < 3
#		include <hash_map.h>
#		include <hash_set.h>
using	::hash_map; // inherit globals
using	::hash_set;
#	else
#		include <ext/hash_map>
#		include <ext/hash_set>
#		if __GNUC__ == 3 && __GNUC_MINOR__ == 0
using	std::hash_map; // GCC 3.0
using	std::hash_set; // GCC 3.0
#		else
using	__gnu_cxx::hash_map;       // GCC 3.1 and later
using	__gnu_cxx::hash_set;
#		endif
#	endif
#endif

#if	defined(__GNUC__)
#	include	<string>
#	if	(__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 3)
// Implement string hash functions so that strings of various flavors can
// be used as keys in STL maps and sets.
#include <tr1/functional>
namespace __gnu_cxx {
template<>
struct hash<std::string> {
    size_t operator()(const std::string& s) const {
        return std::tr1::hash<std::string>()(s);
    }
};

template<>
struct hash<const std::string> {
    size_t operator()(const std::string& s) const {
        return std::tr1::hash<std::string>()(s);
    }
};

template<typename T>
struct hash<T*> {
    size_t operator()(const T* s) const {
        return std::tr1::hash<T*>()(s);
    }
};

template<typename T>
struct hash<const T*> {
    size_t operator()(const T* s) const {
        return std::tr1::hash<const T*>()(s);
    }
};
};
#	else
#include	<ext/hash_fun.h>
namespace __gnu_cxx {
template<>
struct hash<std::string> {
    size_t operator()(const std::string& s) const {
        return hash<const char*>()(s.c_str());
    }
};

template<>
struct hash<const std::string> {
    size_t operator()(const std::string& s) const {
        return hash<const char*>()(s.c_str());
    }
};

template<typename T>
struct hash<T*> {
    size_t operator()(const T* s) const {
        return hash<size_t>()(reinterpret_cast<size_t>(s));
    }
};

template<typename T>
struct hash<const T*> {
    size_t operator()(const T* s) const {
        return hash<size_t>()(reinterpret_cast<size_t>(s));
    }
};
};
#	endif
#endif

#endif
