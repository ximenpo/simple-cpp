#include "simple/igloo.h"
using namespace igloo;

#include "simple/type.h"

Context(type_inference_usage) {
    Spec(const_inference) {
        AssertThat(bool(type_inference<int>::is_const),			IsFalse());
        AssertThat(bool(type_inference<const int>::is_const),	IsTrue());
        AssertThat(bool(type_inference<int const>::is_const),	IsTrue());

        AssertThat(bool(type_inference<int&>::is_const),		IsFalse());
        AssertThat(bool(type_inference<const int&>::is_const),	IsTrue());
        AssertThat(bool(type_inference<int const&>::is_const),	IsTrue());
    }

    Spec(ref_inference) {
        AssertThat(bool(type_inference<int>::is_ref),			IsFalse());
        AssertThat(bool(type_inference<const int>::is_ref),		IsFalse());
        AssertThat(bool(type_inference<int const>::is_ref),		IsFalse());

        AssertThat(bool(type_inference<int>::is_ref),			IsFalse());
        AssertThat(bool(type_inference<const int&>::is_ref),	IsTrue());
        AssertThat(bool(type_inference<int const&>::is_ref),	IsTrue());
    }

    Spec(original_type_inference) {
        AssertThat(&typeid(type_inference<int>::original_type),			Equals(&typeid(int)));
        AssertThat(&typeid(type_inference<const int>::original_type),	Equals(&typeid(const int)));
        AssertThat(&typeid(type_inference<int const>::original_type),	Equals(&typeid(int const)));

        AssertThat(&typeid(type_inference<int&>::original_type),		Equals(&typeid(int&)));
        AssertThat(&typeid(type_inference<const int&>::original_type),	Equals(&typeid(const int&)));
        AssertThat(&typeid(type_inference<int const&>::original_type),	Equals(&typeid(int const&)));
    }

    Spec(undecorated_type_inference) {
        AssertThat(&typeid(type_inference<int>::undecorated_type),			Equals(&typeid(int)));
        AssertThat(&typeid(type_inference<const int>::undecorated_type),	Equals(&typeid(int)));
        AssertThat(&typeid(type_inference<int const>::undecorated_type),	Equals(&typeid(int)));

        AssertThat(&typeid(type_inference<int&>::undecorated_type),			Equals(&typeid(int)));
        AssertThat(&typeid(type_inference<const int&>::undecorated_type),	Equals(&typeid(int)));
        AssertThat(&typeid(type_inference<int const&>::undecorated_type),	Equals(&typeid(int)));
    }
};
