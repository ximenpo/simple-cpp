
#pragma once

// adapt_cast is a function template which allows passing a function argument
// by reference, to a function which expects an argument of a different type
// See: http://www.codeproject.com/Articles/1042460/Adapt-Cast

#include <string>
#include <sstream>
#include <stdexcept>

template <class Source, class Target>
class AdaptCastRef
{
private:
    Source &d_source;
    Target d_target;

    Source (*d_convertTargetToSource)(const Target &);

public:
    explicit AdaptCastRef(
        Source &source,
        Target (*convertSourceToTarget)(const Source &),
        Source (*convertTargetToSource)(const Target &)) :
            d_source(source),
            d_target(convertSourceToTarget(source)),
            d_convertTargetToSource(convertTargetToSource)
    {
    }

    explicit AdaptCastRef(
        Source &source,
        Source (*convertTargetToSource)(const Target &)) :
            d_source(source),
            d_convertTargetToSource(convertTargetToSource)
    {
    }

    ~AdaptCastRef()
    {
        d_source = d_convertTargetToSource(d_target);
    }

    operator Target &()
    {
        return d_target;
    }

    AdaptCastRef &operator =(const Target &target)
    {
        d_target = target;
        return *this;
    }
};

template <class Source, class Target>
struct AdaptCastConverter
{
    static Target convert(const Source &source)
    {
        return static_cast<Target>(source);
    }
};

template <class Target>
struct AdaptCastConverter<std::string, Target>
{
    static Target convert(const std::string &source)
    {
        std::istringstream iss(source);

        Target value;
        if( iss >> value )
        {
            return value;
        }

        throw std::runtime_error(
            "AdaptCastConverter: failed to convert string to value");
    }
};

template <class Source>
struct AdaptCastConverter<Source, std::string>
{
    static std::string convert(const Source &source)
    {
        std::ostringstream oss;
        if( oss << source )
        {
            return oss.str();
        }

        throw std::runtime_error(
            "AdaptCastConverter: failed to convert value to string");
    }
};

// This is included here to allow us to work with pre-C++11 compilers.
// See: http://en.cppreference.com/w/cpp/types/remove_reference
namespace AdaptCastHelper
{
    template<class T> struct RemoveRef     { typedef T type; };
    template<class T> struct RemoveRef<T&> { typedef T type; };
}

template <class TargetRef, class Source>
AdaptCastRef<Source, typename AdaptCastHelper::RemoveRef<TargetRef>::type> adapt_cast(
    Source &source,
    typename AdaptCastHelper::RemoveRef<TargetRef>::type (*convertSourceToTarget)(const Source &),
    Source (*convertTargetToSource)(const typename AdaptCastHelper::RemoveRef<TargetRef>::type &))
{
    typedef typename AdaptCastHelper::RemoveRef<TargetRef>::type Target;

    return AdaptCastRef<Source, Target>(
        source,
        convertSourceToTarget,
        convertTargetToSource);
}

template <class TargetRef, class Source>
AdaptCastRef<Source, typename AdaptCastHelper::RemoveRef<TargetRef>::type> adapt_cast_out(
    Source &source,
    Source (*convertTargetToSource)(const typename AdaptCastHelper::RemoveRef<TargetRef>::type &))
{
    typedef typename AdaptCastHelper::RemoveRef<TargetRef>::type Target;

    return AdaptCastRef<Source, Target>(
        source,
        convertTargetToSource);
}

template <class TargetRef, class Source>
AdaptCastRef<Source, typename AdaptCastHelper::RemoveRef<TargetRef>::type> adapt_cast(Source &source)
{
    typedef typename AdaptCastHelper::RemoveRef<TargetRef>::type Target;

    return adapt_cast<TargetRef>(
        source,
        AdaptCastConverter<Source, Target>::convert,
        AdaptCastConverter<Target, Source>::convert);
}

template <class TargetRef, class Source>
AdaptCastRef<Source, typename AdaptCastHelper::RemoveRef<TargetRef>::type> adapt_cast_out(Source &source)
{
    typedef typename AdaptCastHelper::RemoveRef<TargetRef>::type Target;

    return adapt_cast_out<TargetRef>(
        source,
        AdaptCastConverter<Target, Source>::convert);
}
