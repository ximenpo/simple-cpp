#include <simple/igloo.h>
using namespace igloo;

#include <simple/stl_hash.h>

Context(stl_hash_usage) {
    Spec(hash_map_basic_usage) {
        hash_map<int, int>	m;

        m.insert(std::make_pair(10, 10));
        m.insert(std::make_pair(100, 100));
        m[1000]	= 1000;

        AssertThat(m.size(),	Equals(3));
        AssertThat(m.find(10) != m.end(),	IsTrue());
        AssertThat(m.find(1) == m.end(),	IsTrue());
    }
    Spec(hash_set_basic_usage) {
        hash_set<int>	m;

        m.insert(10);
        m.insert(100);
        m.insert(1000);

        AssertThat(m.size(),	Equals(3));
        AssertThat(m.find(10) != m.end(),	IsTrue());
        AssertThat(m.find(1) == m.end(),	IsTrue());
    }

    Spec(string_test) {
        hash_map<std::string, int>	m;

        m.insert(std::make_pair("10", 10));
        m.insert(std::make_pair("100", 100));
        m["1000"]	= 1000;

        AssertThat(m.size(),	Equals(3));
        AssertThat(m.find("10") != m.end(),	IsTrue());
        AssertThat(m.find("1") == m.end(),	IsTrue());
    }
};
