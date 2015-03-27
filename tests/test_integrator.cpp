#include <simple/igloo.h>
using namespace igloo;

#include <cmath>
#include <iomanip>
#include "simple/integrate.h"

class testIntegrator_Func1 {
public:
    double operator()(double x) const {
        return exp(-x/5.0)*(2.0 + sin(2.0*x));
    }
};

class testIntegrator_Func2 {
public:
    double operator()(double x) const {
        return pow(1.0 - x, 5.0)*pow(x, -1.0/3.0);
    }
};

Context(integrator_context) {
    Spec(f1_test) {
        testIntegrator_Func1 f1;
        AssertThat(integrate(f1, 0, 10, 1e-6),	EqualsWithDelta(9.10823960732284, 1e-6));
    }
    Spec(f2_test) {
        testIntegrator_Func2 f2;
        AssertThat(integrate(f2, 0, 1, 1e-6),	EqualsWithDelta(0.41768525570112, 1e-6));
    }
};
