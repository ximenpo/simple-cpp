#include "simple/igloo.h"
using namespace igloo;

#include	<sstream>

#include	"simple/delegate.h"
#include	"simple/tree_node.h"

//
//	simple tree
//
TREE_NODE_SET_TYPE(test_tree, int,  std::string);

Context(tree_context) {
    std::ostringstream	os;
    void print_tree(int &_data, std::string &_key, long _deep) {
        int ident =_deep;
        while(ident--)
            os << "\t";

        os <<  _key << "=[" << _data << "]\n";
    }

    void	SetUp() {
        os.str("");
    }

    Spec(tree_node_basic_usage) {
        test_tree root;

        root.set_data_and_key(0, "root");
        root.create_node(1, "A");
        root.create_node(3, "C");
        root.create_node(2, "B");

        test_tree_ref sub1 = root.create_node(4, "D");
        sub1.create_node(10, "A");
        sub1.create_node(20, "B");

        test_tree_ref sub2 = sub1.create_node(30, "C");
        sub2.create_node(100, "A");
        sub2.create_node(200, "B");
        sub2.create_node(300, "C");

        root.create_node(5, "E");
        root.create_node(6, "F");

        root.accept(bind(&tree_context::print_tree, this));

        AssertThat(os.str(),	Equals(
                       "root=[0]\n"
                       "\tA=[1]\n"
                       "\tB=[2]\n"
                       "\tC=[3]\n"
                       "\tD=[4]\n"
                       "\t\tA=[10]\n"
                       "\t\tB=[20]\n"
                       "\t\tC=[30]\n"
                       "\t\t\tA=[100]\n"
                       "\t\t\tB=[200]\n"
                       "\t\t\tC=[300]\n"
                       "\tE=[5]\n"
                       "\tF=[6]\n"
                   ));
    }
};
