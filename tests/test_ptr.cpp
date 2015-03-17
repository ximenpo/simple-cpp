#include <igloo/igloo.h>
using namespace igloo;

#include <simple/ptr.h>

static	void	Add(any_ptr data) {
    int*	p	= data;
    if(NULL != p) {
        ++(*p);
    }
}

static	void	OldAdd(void *data) {
    int*	p	= *reinterpret_cast<any_ptr*>(data);
    if(NULL != p) {
        ++(*p);
    }
}

Context(any_ptr_usage) {
    Spec(basic_usage) {
        int a = 10;
        any_ptr pAny = &a;

        int   *pA = pAny; // 'pA' will point to 'a'; type-compatible
        float *pB = pAny; // 'pB' will point to null; type-incompatible

        AssertThat(pA, Equals(&a));
        AssertThat(pB, Equals((void*)NULL));
    }

    Spec(const_correctness_1) {
        int a = 10;
        any_ptr pAny = &a;

        const int *p1 = pAny; // 'p1' will point to 'a'; const-compatible
        int       *p2 = pAny; // 'p2' will point to 'a'; const-compatible

        AssertThat(p1, Equals(&a));
        AssertThat(p2, Equals(&a));
    }

    Spec(const_correctness_2) {
        const int a = 10;
        any_ptr pAny = &a;

        const int *p1 = pAny; // 'p1' will point to 'a'; const-compatible
        int       *p2 = pAny; // 'p2' will point to null; const-incompatible

        //	??? VS2003/2010&&G++4.8.2 p2 will point to 'a'

        AssertThat(p1, Equals(&a));
        AssertThat(p2, Equals(&a));
    }

    Spec(as_param) {
        int		n	= 100;
        float	f	= 100.0f;

        Add(&n);
        Add(&f);

        AssertThat(n, EqualsWithDelta(101, 0.001f));
        AssertThat(f, EqualsWithDelta(100, 0.001f));
    }

    Spec(as_void_star_param) {
        int		n	= 100;
        float	f	= 100.0f;

        any_ptr	pn	= &n;
        any_ptr	pf	= &f;

        Add(&n);
        Add(&f);

        AssertThat(n, EqualsWithDelta(101, 0.001f));
        AssertThat(f, EqualsWithDelta(100, 0.001f));
    }
};
