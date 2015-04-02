#include "simple/igloo.h"
using namespace igloo;

#include	"simple/string.h"
#include	"simple/stringify.h"

#include	<stdexcept>

using	namespace	stringify;

struct	testSimpleVisitor	: public stringify_data_visitor {
    virtual		bool	visit_begin() {
        os.str(std::string());
        return	true;
    }
    virtual		bool	visit_value(const std::string& value, const std::string* name) {
        if(0 != name) {
            os << *name << "=";
        }
        os	<<	value	<<	std::endl;
        return	true;
    }
    virtual		bool	visit_array_begin(const std::string* name) {
        if(0 != name) {
            os << *name << "=";
        }
        os	<<	"["	<<	std::endl;
        return	true;
    }
    virtual		bool	visit_array_end() {
        os	<<	"]"	<<	std::endl;
        return	true;
    }
    virtual		bool	visit_object_begin(const std::string* name) {
        if(0 != name) {
            os << *name << "=";
        }
        os	<<	"{"	<<	std::endl;
        return	true;
    }
    virtual		bool	visit_object_end() {
        os	<<	"}"	<<	std::endl;
        return	true;
    }
    virtual		bool	visit_end() {
        return	true;
    }

    std::stringstream	os;
};

struct	testCommonStruct {
    int			age;
    std::string	name;
    std::deque<int>	queue;
};
struct	testSubStruct {
    testCommonStruct	common;
    bool		sex;
    std::map<std::string, std::string>	map;
};

template<typename NameType>
inline	stringify_data_builder&		stringify_build_field(stringify_data_builder& builder, const NameType& name, const testCommonStruct& value) {
    return	builder.object_begin(name).field("age", value.age).field("name", value.name).field("queue", value.queue).object_end();
}

template<typename NameType>
inline	stringify_data_fetcher&		stringify_fetch_field(stringify_data_fetcher& fetcher, const NameType& name, testCommonStruct& value) {
    return	fetcher.object_begin(name).field("age", value.age).field("name", value.name).field("queue", value.queue).object_end();
}

template<typename NameType>
inline	stringify_data_builder&		stringify_build_field(stringify_data_builder& builder, const NameType& name, const testSubStruct& value) {
    return	builder.object_begin(name).field("common", value.common).field("sex", value.sex).field("map", value.map).object_end();
}

template<typename NameType>
inline	stringify_data_fetcher&		stringify_fetch_field(stringify_data_fetcher& fetcher, const NameType& name, testSubStruct& value) {
    return	fetcher.object_begin(name).field("common", value.common).field("sex", value.sex).field("map", value.map).object_end();
}

Context(stringify_context) {
    static	std::string	serialize_(value_list& values) {
        std::ostringstream	os;
        value_list::const_iterator	it		= values.begin();
        value_list::const_iterator	it_end	= values.end();
        for(; it != it_end; ++it) {
            os	<<	string_line_escape(it->value)	<<	"\n";
        }
        return	os.str();
    }

    static	std::string	serialize_(container_list& containers) {
        std::ostringstream	os;
        container_list::const_iterator	it		= containers.begin();
        container_list::const_iterator	it_end	= containers.end();
        for(; it != it_end; ++it) {
            os	<<	(it->is_array?"[":"{");
            id_list::const_iterator	it2		= it->items.begin();
            id_list::const_iterator	it2_end	= it->items.end();
            for(; it2 != it2_end; ++it2) {
                os	<<	"("	<<	get_node_type(*it2) << "," <<  get_node_index(*it2)	<<	")";
            }
            os	<<	(it->is_array?"]":"}")	<<	"\n";
        }
        return	os.str();
    }

    static	std::string	serialize_(named_value_list& named_values) {
        std::ostringstream	os;
        named_value_list::const_iterator	it		= named_values.begin();
        named_value_list::const_iterator	it_end	= named_values.end();
        for(; it != it_end; ++it) {
            os	<<	it->name	<< "="	<<	it->value	<<	"\n";
        }
        return	os.str();
    }

    static	std::string	serialize_(named_container_list& named_containers) {
        std::ostringstream	os;
        named_container_list::const_iterator	it		= named_containers.begin();
        named_container_list::const_iterator	it_end	= named_containers.end();
        for(; it != it_end; ++it) {
            os	<<	it->name	<<	(it->is_array?"=[":"={");
            id_list::const_iterator	it2		= it->items.begin();
            id_list::const_iterator	it2_end	= it->items.end();
            for(; it2 != it2_end; ++it2) {
                os	<<	"("	<<	get_node_type(*it2) << "," <<  get_node_index(*it2)	<<	")";
            }
            os	<<	(it->is_array?"]":"}")	<<	"\n";
        }
        return	os.str();
    }

    stringify_data	data;

    void	SetUp() {
        data.clear();
    }

    Spec(root_is_value_usage) {
        stringify_data_builder(data).begin()
        .value("value")
        .end();

        AssertThat(data.root,	Equals(make_node_id(NODE_VALUE, 0)));
        AssertThat(serialize_(data.values),				Equals("value\n"));
        AssertThat(serialize_(data.containers),			Equals(""));
        AssertThat(serialize_(data.named_values),		Equals(""));
        AssertThat(serialize_(data.named_containers),	Equals(""));

        AssertThrows(std::invalid_argument,
                     stringify_data_builder(data)
                     .begin()
                     .value("value")
                     .value("value2")
                     .end()
                    );
    }

    Spec(basic_array_usage) {
        stringify_data_builder(data).begin()
        .array_begin()
        .value("s1")
        .value("s2")
        .value(101)
        .array_end()
        .end();

        AssertThat(data.root,	Equals(make_node_id(NODE_CONTAINER, 0)));
        AssertThat(serialize_(data.values),				Equals("s1\n" "s2\n" "101\n"));
        AssertThat(serialize_(data.containers),			Equals("[(1,0)(1,1)(1,2)]\n"));
        AssertThat(serialize_(data.named_values),		Equals(""));
        AssertThat(serialize_(data.named_containers),	Equals(""));
    }

    Spec(basic_object_usage) {
        stringify_data_builder(data).begin()
        .object_begin()
        .value("s1", "v1")
        .value("s2", "v2")
        .value(101, "v3")
        .object_end()
        .end();

        AssertThat(data.root,	Equals(make_node_id(NODE_CONTAINER, 0)));
        AssertThat(serialize_(data.values),				Equals(""));
        AssertThat(serialize_(data.containers),			Equals("{(3,0)(3,1)(3,2)}\n"));
        AssertThat(serialize_(data.named_values),		Equals("v1=s1\n" "v2=s2\n" "v3=101\n"));
        AssertThat(serialize_(data.named_containers),	Equals(""));
    }

    Spec(basic_object_array_usage) {
        stringify_data_builder(data).begin()
        .array_begin()
        .object_begin()
        .value("s1", "v")
        .object_end()
        .object_begin()
        .value("s2", "v")
        .object_end()
        .object_begin()
        .value(101, "v")
        .object_end()
        .array_end()
        .end();

        AssertThat(data.root,	Equals(make_node_id(NODE_CONTAINER, 0)));
        AssertThat(serialize_(data.values),				Equals(""));
        AssertThat(serialize_(data.containers),			Equals("[(2,1)(2,2)(2,3)]\n" "{(3,0)}\n" "{(3,1)}\n" "{(3,2)}\n"));
        AssertThat(serialize_(data.named_values),		Equals("v=s1\n" "v=s2\n" "v=101\n"));
        AssertThat(serialize_(data.named_containers),	Equals(""));
    }

    Spec(simple_struct_usage) {
        struct	S {
            int			age;
            std::string	name;
            bool		sex;
            std::pair<std::string, std::string>	data;
        };

        S	s1	= {
            34,	"XiMenPo", true
        };
        s1.data	= std::make_pair("tech", "cpp");

        stringify_data_builder(data).begin()
        .object_begin()
        .field("age",	s1.age)
        .field("name",	s1.name)
        .field("sex",	s1.sex)
        .field("data",	s1.data)
        .object_end()
        .end();

        AssertThat(data.root,	Equals(make_node_id(NODE_CONTAINER, 0)));
        AssertThat(serialize_(data.values),				Equals("tech\n" "cpp\n"));
        AssertThat(serialize_(data.containers),			Equals("{(3,0)(3,1)(3,2)(4,0)}\n"));
        AssertThat(serialize_(data.named_values),		Equals("age=34\n" "name=XiMenPo\n" "sex=1\n"));
        AssertThat(serialize_(data.named_containers),	Equals("data=[(1,0)(1,1)]\n"));
    }

    Spec(normal_struct_usage) {
        struct	S {
            int			age;
            std::string	name;
            bool		sex;
            std::deque<int>	queue;
            std::map<std::string, std::string>	map;
        };

        S	s1	= {
            34,	"XiMenPo", true
        };
        s1.queue.push_back(100);
        s1.queue.push_back(101);
        s1.queue.push_back(102);
        s1.map["tech"]	= "cpp";

        stringify_data_builder(data).begin()
        .object_begin()
        .field("age",	s1.age)
        .field("name",	s1.name)
        .field("sex",	s1.sex)
        .field("queue",	s1.queue)
        .field("map",	s1.map)
        .object_end()
        .end();

        std::string	str_values			= serialize_(data.values);
        std::string	str_containers		= serialize_(data.containers);
        std::string	str_named_values	= serialize_(data.named_values);
        std::string	str_named_containers= serialize_(data.named_containers);

        //std::cout	<< str_values << "--------------" << std::endl;
        //std::cout	<< str_containers << "--------------" << std::endl;
        //std::cout	<< str_named_values << "--------------" << std::endl;
        //std::cout	<< str_named_containers << "--------------" << std::endl;

        AssertThat(data.root,	Equals(make_node_id(NODE_CONTAINER, 0)));
        AssertThat(str_values,				Equals("100\n" "101\n" "102\n" "tech\n" "cpp\n"));
        AssertThat(str_containers,			Equals("{(3,0)(3,1)(3,2)(4,0)(4,1)}\n" "[(1,3)(1,4)]\n"));
        AssertThat(str_named_values,		Equals("age=34\n" "name=XiMenPo\n" "sex=1\n"));
        AssertThat(str_named_containers,	Equals("queue=[(1,0)(1,1)(1,2)]\n" "map=[(2,1)]\n"));
    }

    Spec(simple_visitor_test1) {
        stringify_data_builder(data).begin()
        .value("str")
        .end();

        testSimpleVisitor	visitor;
        AssertThat(data.accept(visitor),	IsTrue());
        AssertThat(visitor.os.str(),		Equals("str\n"));
    }

    Spec(simple_visitor_test2) {
        stringify_data_builder(data).begin()
        .array_begin()
        .object_begin()
        .value("s1", "v")
        .object_end()
        .object_begin()
        .value("s2", "v")
        .object_end()
        .object_begin()
        .value(101, "v")
        .object_end()
        .array_end()
        .end();

        testSimpleVisitor	visitor;
        AssertThat(data.accept(visitor),	IsTrue());
        AssertThat(visitor.os.str(),		Equals(
                       "[\n"
                       "{\n"
                       "v=s1\n"
                       "}\n"
                       "{\n"
                       "v=s2\n"
                       "}\n"
                       "{\n"
                       "v=101\n"
                       "}\n"
                       "]\n"
                   ));
    }

    Spec(simple_visitor_struct_usage) {
        struct	S {
            int			age;
            std::string	name;
            bool		sex;
            std::deque<int>	queue;
            std::map<std::string, std::string>	map;
        };

        S	s1	= {
            34,	"XiMenPo", true
        };
        s1.queue.push_back(100);
        s1.queue.push_back(101);
        s1.queue.push_back(102);
        s1.map["tech"]	= "cpp";

        stringify_data_builder(data).begin()
        .object_begin()
        .field("age",	s1.age)
        .field("name",	s1.name)
        .field("sex",	s1.sex)
        .field("queue",	s1.queue)
        .field("map",	s1.map)
        .object_end()
        .end();

        testSimpleVisitor	visitor;
        AssertThat(data.accept(visitor),	IsTrue());
        AssertThat(visitor.os.str(),		Equals(
                       "{\n"
                       "age=34\n"
                       "name=XiMenPo\n"
                       "sex=1\n"
                       "queue=[\n"
                       "100\n"
                       "101\n"
                       "102\n"
                       "]\n"
                       "map=[\n"
                       "[\n"
                       "tech\n"
                       "cpp\n"
                       "]\n"
                       "]\n"
                       "}\n"
                   ));
    }

    Spec(data_fetch_test) {
        stringify_data_builder(data).begin()
        .array_begin()
        .object_begin()
        .value("s1", "v")
        .object_end()
        .object_begin()
        .value("s2", "v")
        .object_end()
        .object_begin()
        .value(101, "v")
        .object_end()
        .array_end()
        .end();

        std::string*	name;
        std::string*	value;
        node_container*	container	= 0;
        AssertThat(data.fetch(data.root, &value, &name),	IsFalse());
        AssertThat(data.fetch(data.root, &container, &name),	IsTrue());
        AssertThat(name,	Equals((void*)0));
        AssertThat(container->items,	HasLength(3));
        AssertThat(container->is_array,	IsTrue());
    }

    Spec(named_value_fectch_test) {
        stringify_data_builder(data).begin()
        .object_begin()
        .field("age",	int(34))
        .field("name",	"XiMenPo")
        .field("sex",	true)
        .field("sname",	std::string("XiMenPo"))
        .array_begin("data")
        .value(101)
        .value(102)
        .value(103)
        .array_end()
        .object_end()
        .end();

        std::string*	name;
        std::string*	value;
        node_container*	container	= 0;
        AssertThat(data.fetch(data.root, &value, &name),		IsFalse());
        AssertThat(data.fetch(data.root, &container, &name),	IsTrue());
        AssertThat(name,		Equals((void*)0));
        AssertThat(container->items,	HasLength(5));
        AssertThat(container->is_array,	IsFalse());

        AssertThat(data.fetch(data.root, "age", &value), IsTrue());
        AssertThat(*value,		Equals("34"));

        AssertThat(data.fetch(data.root, "name", &value), IsTrue());
        AssertThat(*value,		Equals("XiMenPo"));

        AssertThat(data.fetch(data.root, "Sex", &value), IsFalse());
        //	value is undefined AssertThat(value,		Equals((void*)0));

        AssertThat(data.fetch(data.root, "data", &container), IsTrue());
        AssertThat(name,		Equals((void*)0));
        AssertThat(container->items,	HasLength(3));
        AssertThat(container->is_array,	IsTrue());
    }

    Spec(struct_fetcher_test) {
        struct	S {
            int			age;
            std::string	name;
            bool		sex;
            std::deque<int>	queue;
            std::map<std::string, std::string>	map;
        };

        S	s1	= {
            34,	"XiMenPo", true
        };
        s1.queue.push_back(100);
        s1.queue.push_back(101);
        s1.queue.push_back(102);
        s1.map["tech"]	= "cpp";

        stringify_data_builder(data).begin()
        .object_begin()
        .field("age",	s1.age)
        .field("name",	s1.name)
        .field("sex",	s1.sex)
        .field("queue",	s1.queue)
        .field("map",	s1.map)
        .object_end()
        .end();

        S	s2;
        stringify_data_fetcher(data).begin()
        .object_begin()
        .field("age",	s2.age)
        .field("name",	s2.name)
        .field("sex",	s2.sex)
        .field("queue",	s2.queue)
        .field("map",	s2.map)
        .object_end()
        .end();

        AssertThat(s1.age,		Equals(s2.age));
        AssertThat(s1.name,		Equals(s2.name));
        AssertThat(s1.sex,		Equals(s2.sex));
        AssertThat(s1.queue,	EqualsContainer(s2.queue));
        AssertThat(s1.map,		EqualsContainer(s2.map));
    }

    Spec(complex_struct_fetcher_test) {
        testSubStruct	s1, s2;
        s1.common.age	= 34;
        s1.common.name	= "XiMenPo";
        s1.common.queue.push_back(101);
        s1.common.queue.push_back(102);
        s1.common.queue.push_back(103);
        s1.sex	= true;
        s1.map["tech"]	= "cpp";

        stringify_data_builder(data).execute(s1);
        stringify_data_fetcher(data).execute(s2);

        AssertThat(s1.common.age,		Equals(s2.common.age));
        AssertThat(s1.common.name,		Equals(s2.common.name));
        AssertThat(s1.common.queue,		EqualsContainer(s2.common.queue));
        AssertThat(s1.sex,		Equals(s2.sex));
        AssertThat(s1.map,		EqualsContainer(s2.map));

        //testSimpleVisitor	visitor;
        //data.accept(visitor);
        //std::cout << visitor.os.str() << std::endl;
    }

    Spec(has_get_set_test) {
        stringify_data_builder(data).begin()
        .object_begin()
        .field("age",	"34")
        .field("name",	"XiMenPo")
        .object_begin("object")
        .field("sex",	"true")
        .field("sname",	std::string("XiMenPo"))
        .object_end()
        .array_begin("array")
        .value("101")
        .value("102")
        .value("103")
        .array_end()
        .object_end()
        .end();

        AssertThat(data.has(""),							IsTrue());//root
        AssertThat(data.has_value(""),						IsFalse());//root
        AssertThat(data.has_value("no"),					IsFalse());
        AssertThat(data.get_value("", "Nothing!"),			Equals("Nothing!"));
        AssertThat(data.has_value("age"),					IsTrue());
        AssertThat(data.get_value("age", "21"),				Equals("34"));
        AssertThat(data.has_value("object/sex"),			IsTrue());
        AssertThat(data.get_value("object/sex", "false"),	Equals("true"));
        AssertThat(data.has_value("object/sname"),			IsTrue());
        AssertThat(data.get_value("object/sname", ""),		Equals("XiMenPo"));
        AssertThat(data.get_value("object", "sex", "false"),Equals("true"));
        AssertThat(data.get_value("object", "sname", ""),	Equals("XiMenPo"));
        AssertThat(data.has_value("array", 0),				IsTrue());
        AssertThat(data.has_value("array", 1),				IsTrue());
        AssertThat(data.has_value("array", 2),				IsTrue());
        AssertThat(data.has_value("array", 3),				IsFalse());
        AssertThat(data.get_value("array", 0, "0"),			Equals("101"));
        AssertThat(data.get_value("array", 1, "0"),			Equals("102"));
        AssertThat(data.get_value("array", 2, "0"),			Equals("103"));
        AssertThat(data.get_value("array", 3, "0"),			Equals("0"));

        AssertThat(data.set_value("age", "21"),				IsTrue());
        AssertThat(data.get_value("age", "34"),				Equals("21"));
        AssertThat(data.set_value("array", 1, "10"),		IsTrue());
        AssertThat(data.get_value("array", 1, "0"),			Equals("10"));
        AssertThat(data.set_value("object", "sex", "false"),IsTrue());
        AssertThat(data.get_value("object", "sex", "true"),	Equals("false"));
    }
};
