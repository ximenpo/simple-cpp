#include "simple/igloo.h"
using namespace igloo;

#include "simple/any.h"

Context(any_usage) {
    Spec(basic_usage) {
        any a = 42;
        AssertThat(a.cast<int>(), Equals(42));

        a = 13;
        AssertThat(a.cast<int>(), Equals(13));

        a = "hello";
        AssertThat(a.cast<const char*>(), Equals("hello"));

        a = std::string("1234567890");
        AssertThat(a.cast<std::string>(), Equals(std::string("1234567890")));

        int n = 42;
        a = &n;
        AssertThat(a.cast<int*>(), Equals(&n));

        any b = true;
        AssertThat(b.cast<bool>(), IsTrue());

        std::swap(a, b);
        AssertThat(a.cast<bool>(), IsTrue());

        a.cast<bool>() = false;
        AssertThat(a.cast<bool>(), IsFalse());
    }
};
