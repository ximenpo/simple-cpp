#include <simple/igloo.h>
using namespace igloo;

#include <simple/creator.h>

class	CreatorTest {
    bool	valid_;
public:
    CreatorTest() : valid_(false) {
    }

    virtual	void	set_valid(bool valid) {
        valid_	= valid;
    }

    virtual	bool	is_valid() {
        return	valid_;
    }
};

class	MyCreatorTest : public CreatorTest {
public:
    virtual	bool	is_valid() {
        return	!CreatorTest::is_valid();
    }
};

Context(creator_usage) {
    Spec(creator_null_usage) {
        creator_null<CreatorTest>	creator;
        CreatorTest*	p	= creator.create();
        AssertThat(p,		Equals((void*)0));
        creator.destroy(p);
    }

    Spec(creator_static_usage) {
        creator_static<CreatorTest>	creator;
        CreatorTest*	p	= creator.create();
        AssertThat(p,		!Equals((void*)0));
        AssertThat(p->is_valid(),	IsFalse());
        p->set_valid(true);
        AssertThat(p->is_valid(),	IsTrue());
        creator.destroy(p);
    }

    Spec(creator_new_usage) {
        creator_new<CreatorTest>	creator;
        CreatorTest*	p	= creator.create();
        AssertThat(p,		!Equals((void*)0));
        AssertThat(p->is_valid(),	IsFalse());
        p->set_valid(true);
        AssertThat(p->is_valid(),	IsTrue());
        creator.destroy(p);
    }

    Spec(creator_new_intf_usage) {
        creator_new<CreatorTest, MyCreatorTest>	creator;
        CreatorTest*	p	= creator.create();
        AssertThat(p,		!Equals((void*)0));
        AssertThat(p->is_valid(),	!IsFalse());
        p->set_valid(true);
        AssertThat(p->is_valid(),	!IsTrue());
        creator.destroy(p);
    }

    Spec(creator_placement_new_usage) {
        creator_placement_new<CreatorTest>	creator;
        CreatorTest*	p	= creator.create();
        AssertThat(p,		!Equals((void*)0));
        AssertThat(p->is_valid(),	IsFalse());
        p->set_valid(true);
        AssertThat(p->is_valid(),	IsTrue());
        creator.destroy(p);
    }

    Spec(creator_placement_new_intf_usage) {
        creator_placement_new<CreatorTest, MyCreatorTest>	creator;
        CreatorTest*	p	= creator.create();
        AssertThat(p,		!Equals((void*)0));
        AssertThat(p->is_valid(),	!IsFalse());
        p->set_valid(true);
        AssertThat(p->is_valid(),	!IsTrue());
        creator.destroy(p);
    }
};
