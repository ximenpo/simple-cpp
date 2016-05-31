#ifndef ADAPT_CAST_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define ADAPT_CAST_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2


// adapt_cast is a function template which allows passing a function argument
// by reference, to a function which expects an argument of a different type
// See: http://www.codeproject.com/Articles/1042460/Adapt-Cast

#include <string>
#include <sstream>
#include <stdexcept>

template <class Source, class Target>
class adapt_cast_ref {
private:
    Source &d_source;
    Target d_target;

    Source (*d_convertTargetToSource)(const Target &);

public:
    explicit adapt_cast_ref(
        Source &source,
        Target (*convertSourceToTarget)(const Source &),
        Source (*convertTargetToSource)(const Target &)) :
        d_source(source),
        d_target(convertSourceToTarget(source)),
        d_convertTargetToSource(convertTargetToSource) {
    }

    explicit adapt_cast_ref(
        Source &source,
        Source (*convertTargetToSource)(const Target &)) :
        d_source(source),
        d_convertTargetToSource(convertTargetToSource) {
    }

    ~adapt_cast_ref() {
        d_source = d_convertTargetToSource(d_target);
    }

    operator Target &() {
        return d_target;
    }

    adapt_cast_ref &operator =(const Target &target) {
        d_target = target;
        return *this;
    }
};

template <class Source, class Target>
struct adapt_cast_converter {
    static Target convert(const Source &source) {
        return static_cast<Target>(source);
    }
};

template <class Target>
struct adapt_cast_converter<std::string, Target> {
    static Target convert(const std::string &source) {
        std::istringstream iss(source);

        Target value;
        if( iss >> value ) {
            return value;
        }

        throw std::runtime_error(
            "adapt_cast_converter: failed to convert string to value");
    }
};

template <class Source>
struct adapt_cast_converter<Source, std::string> {
    static std::string convert(const Source &source) {
        std::ostringstream oss;
        if( oss << source ) {
            return oss.str();
        }

        throw std::runtime_error(
            "adapt_cast_converter: failed to convert value to string");
    }
};

// This is included here to allow us to work with pre-C++11 compilers.
// See: http://en.cppreference.com/w/cpp/types/remove_reference
namespace adapt_cast_helper {
template<class T> struct RemoveRef     {
    typedef T type;
};
template<class T> struct RemoveRef<T&> {
    typedef T type;
};
}

template <class TargetRef, class Source>
adapt_cast_ref<Source, typename adapt_cast_helper::RemoveRef<TargetRef>::type> adapt_cast(
    Source &source,
    typename adapt_cast_helper::RemoveRef<TargetRef>::type (*convertSourceToTarget)(const Source &),
    Source (*convertTargetToSource)(const typename adapt_cast_helper::RemoveRef<TargetRef>::type &)) {
    typedef typename adapt_cast_helper::RemoveRef<TargetRef>::type Target;

    return adapt_cast_ref<Source, Target>(
               source,
               convertSourceToTarget,
               convertTargetToSource);
}

template <class TargetRef, class Source>
adapt_cast_ref<Source, typename adapt_cast_helper::RemoveRef<TargetRef>::type> adapt_cast_out(
    Source &source,
    Source (*convertTargetToSource)(const typename adapt_cast_helper::RemoveRef<TargetRef>::type &)) {
    typedef typename adapt_cast_helper::RemoveRef<TargetRef>::type Target;

    return adapt_cast_ref<Source, Target>(
               source,
               convertTargetToSource);
}

template <class TargetRef, class Source>
adapt_cast_ref<Source, typename adapt_cast_helper::RemoveRef<TargetRef>::type> adapt_cast(Source &source) {
    typedef typename adapt_cast_helper::RemoveRef<TargetRef>::type Target;

    return adapt_cast<TargetRef>(
               source,
               adapt_cast_converter<Source, Target>::convert,
               adapt_cast_converter<Target, Source>::convert);
}

template <class TargetRef, class Source>
adapt_cast_ref<Source, typename adapt_cast_helper::RemoveRef<TargetRef>::type> adapt_cast_out(Source &source) {
    typedef typename adapt_cast_helper::RemoveRef<TargetRef>::type Target;

    return adapt_cast_out<TargetRef>(
               source,
               adapt_cast_converter<Target, Source>::convert);
}

#endif
