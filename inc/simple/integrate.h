#ifndef INTEGRATE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define INTEGRATE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	"third/DEIntegrator.h"

template<typename Functor>
double	integrate(Functor func,		//!< [in] integrand
                  double from,                    //!< [in] left limit of integration
                  double to,						//!< [in] right limit of integration
                  double targetAbsoluteError     //!< [in] desired bound on error
                ) {
    return	DEIntegrator<Functor>::Integrate(func, from, to, targetAbsoluteError);
}

#endif
