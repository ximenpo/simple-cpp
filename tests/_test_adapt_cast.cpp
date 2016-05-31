#include "simple/igloo.h"
using namespace igloo;

#include "simple/adapt_cast.h"

static    void multiplyByTwo(double &value) {
    value *= 2;
}

static    void getMagicNumber(float &value) {
    value = 7.2f;
}
static    double toDouble(const int &value) {
    return static_cast<double>(value);
}

static    int toInt(const double &value) {
    return static_cast<int>(value);
}

Context(adapt_cast_usage) {
    Spec(basic_usage) {
        int value = 10;
        multiplyByTwo(adapt_cast<double&>(value));
        AssertThat(value, Equals(20));
    }

    Spec(output_param) {
        std::string str;
        getMagicNumber(adapt_cast_out<float&>(str));
        AssertThat(str, Equals("7.2"));
    }

    Spec(explicitly_specifying_converters) {
        int value = 10;
        multiplyByTwo(adapt_cast<double&>(value, toDouble, toInt));
        AssertThat(value, Equals(20));
    }
};
