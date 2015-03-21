#ifndef STRINGIFY_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define STRINGIFY_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<cassert>
#include	<deque>
#include	<iosfwd>
#include	<iterator>
#include	<string>
#include	<sstream>
#include	<stdexcept>

namespace stringify {

//
//	节点类型
//
enum	NODE_TYPE {
    NODE_NONE			= 0,	//	空
    NODE_VALUE,					//	无名字值，		为[根]或[数组]的[叶子]节点
    NODE_CONTAINER,				//	无名字容器		为[根]节点或者[数组]成员
    NODE_NAMED_VALUE,			//	带名字的值，	为[对象]的[叶子]节点
    NODE_NAMED_CONTAINER,		//	带名字的容器，	为[数组]或[对象]
    NODE_TYPE_SUM,
};

//
//	节点ID
//
typedef	unsigned long	node_id;
//struct	node_id{
//	unsigned long	type:	8;
//	unsigned long	index:	24;
//};

///	生成ID
inline	node_id			make_node_id(NODE_TYPE type, unsigned long index) {
    return	((type & 0xFF) << 24) | (index & 0xFFFFFF);
}
///	获取节点类型
inline	NODE_TYPE		get_node_type(node_id id) {
    return	NODE_TYPE((id >> 24) & 0xFF);
}
///	获取节点数据索引
inline	unsigned long	get_node_index(node_id id) {
    return	id & 0xFFFFFF;
}

//
//	节点数据定义
//
///	名字
struct	node_name {
    std::string		name;
};
///	仅值
struct	node_value {
    std::string		value;
};
///	含子项
struct	node_container {
    bool	is_array;
    std::deque<node_id>	items;
};
///	带名字的值
struct	node_named_value : public node_value, public node_name {
};
///	含子项，带名字
struct	node_named_container : public node_container, public node_name {
};

///	无名字traits，仅用于支持generic编程
struct	no_name {
};

//
//	容器类型定义
//
typedef	std::deque<node_id>					id_list;
typedef	std::deque<node_value>				value_list;
typedef	std::deque<node_container>			container_list;
typedef	std::deque<node_named_value>		named_value_list;
typedef	std::deque<node_named_container>	named_container_list;

}

//
//	将 stringify_data 流化输出接口
//
struct	stringify_data_visitor {
    virtual		bool	visit_begin()	= 0;
    virtual		bool	visit_value(const std::string& value, const std::string* name)	= 0;
    virtual		bool	visit_array_begin(const std::string* name)	= 0;
    virtual		bool	visit_array_end()	= 0;
    virtual		bool	visit_object_begin(const std::string* name)	= 0;
    virtual		bool	visit_object_end()	= 0;
    virtual		bool	visit_end()	= 0;
};

//
//	字符串化数据
//
struct	stringify_data {
public:
    ///	root node.
    stringify::node_id					root;

    ///	containers.
    stringify::value_list				values;
    stringify::container_list			containers;
    stringify::named_value_list			named_values;
    stringify::named_container_list		named_containers;

public:
    //
    //	methods.
    //

    ///	清空
    void			clear();

    ///	遍历
    bool			accept(stringify_data_visitor& visitor);

    ///	获取带名字节点ID[split by '/', eg: a1/b1/c1 ]
    bool			fetch(const std::string& path, stringify::node_id* node_id, const std::string& path_delimiter = "/");
    ///	获取值，成功返回true
    bool			fetch(stringify::node_id node_id, std::string** value, std::string** name);
    ///	获取子项，成功返回true
    bool			fetch(stringify::node_id node_id, stringify::node_container** container, std::string** name);
    ///	获取指定位置的项，成功返回true
    bool			fetch(stringify::node_id node_id, unsigned long child_pos, stringify::node_id* child_id);
    bool			fetch(stringify::node_id node_id, unsigned long child_pos, std::string** value);
    bool			fetch(stringify::node_id node_id, unsigned long child_pos, stringify::node_container** container);
    ///	获取指定名字的项，成功返回true
    bool			fetch(stringify::node_id node_id, const std::string& child_name, stringify::node_id* child_id);
    bool			fetch(stringify::node_id node_id, const std::string& child_name, std::string** value);
    bool			fetch(stringify::node_id node_id, const std::string& child_name, stringify::node_container** container);

    ///	添加根、容器、值[path为带名字节点]
    bool			add_root(stringify::node_value** new_root, stringify::node_id* new_id);
    bool			add_root(stringify::node_container** new_root, stringify::node_id* new_id);
    bool			add_container(stringify::node_id container_id, stringify::node_container** new_container, stringify::node_id* new_id);
    bool			add_container(stringify::node_id container_id, const std::string& child_name,	stringify::node_container** new_container, stringify::node_id* new_id);
    bool			add_container(const std::string& path,	stringify::node_container** new_container, stringify::node_id* new_id, const std::string& path_delimiter = "/");
    bool			add_value(stringify::node_id container_id, stringify::node_value** new_node, stringify::node_id* new_id);
    bool			add_value(stringify::node_id container_id, const std::string& child_name,	stringify::node_value** new_node, stringify::node_id* new_id);

    //	has/get/set[item must exist]
    bool			has(const std::string& path);
    bool			has(const std::string& path, const std::string& child_name);
    bool			has(const std::string& path, unsigned long child_index);

    bool			has_value(const std::string& path);
    bool			has_value(const std::string& path, const std::string& child_name);
    bool			has_value(const std::string& path, unsigned long child_index);
    std::string		get_value(const std::string& path, const std::string& default_value);
    std::string		get_value(const std::string& path, const std::string& child_name, const std::string& default_value);
    std::string		get_value(const std::string& path, unsigned long child_index, const std::string& default_value);
    bool			set_value(const std::string& path, const std::string& value);
    bool			set_value(const std::string& path, const std::string& child_name, const std::string& value);
    bool			set_value(const std::string& path, unsigned long child_index, const std::string& value);

    bool						has_container(const std::string& path);
    bool						has_container(const std::string& path, const std::string& child_name);
    bool						has_container(const std::string& path, unsigned long child_index);
    stringify::node_container*	get_container(const std::string& path);
    stringify::node_container*	get_container(const std::string& path, const std::string& child_name);
    stringify::node_container*	get_container(const std::string& path, unsigned long child_index);
};

//
//	字符串化数据获取器
//
class	stringify_data_fetcher;
template<typename NameType, typename ValueType>
stringify_data_fetcher&		stringify_fetch_field(stringify_data_fetcher& fetcher, const NameType& field_name, const ValueType& field_value);

class	stringify_data_fetcher {
public:
    stringify_data_fetcher(stringify_data& data) : data_(data) {
    }

public:
    template<typename ObjectType>
    bool	execute(ObjectType& obj, std::string* err_msg = 0) {
        try {
            this->begin();
            stringify_fetch_field(*this, stringify::no_name(), obj);
            this->end();
            return	true;
        } catch(const std::logic_error& err) {
            if(0 != err_msg) {
                *err_msg	= err.what();
            }
        }
        return	false;
    }

public:
    stringify_data_fetcher&		begin();

    bool						value(std::string** value);
    bool						value(std::string** value, const std::string& name);
    bool						value(std::string** value, unsigned long index);

    stringify_data_fetcher&		value(std::string& value);
    stringify_data_fetcher&		value(std::string& value, const std::string& name);
    stringify_data_fetcher&		value(std::string& value, unsigned long index);

    stringify_data_fetcher&		array_begin(size_t* size);
    stringify_data_fetcher&		array_begin(unsigned long index, size_t* size);
    stringify_data_fetcher&		array_begin(const std::string& name, size_t* size);
    stringify_data_fetcher&		array_begin(const stringify::no_name& name, size_t* size);
    stringify_data_fetcher&		array_end();

    stringify_data_fetcher&		object_begin();
    stringify_data_fetcher&		object_begin(unsigned long index);
    stringify_data_fetcher&		object_begin(const std::string& name);
    stringify_data_fetcher&		object_begin(const stringify::no_name& name);
    stringify_data_fetcher&		object_end();

    template<typename ValueType>
    stringify_data_fetcher&		field(unsigned long index, ValueType& value) {
        return	stringify_fetch_field(*this, index, value);
    }
    template<typename ValueType>
    stringify_data_fetcher&		field(const stringify::no_name& name, ValueType& value) {
        return	stringify_fetch_field(*this, name, value);
    }
    template<typename ValueType>
    stringify_data_fetcher&		field(const std::string& name, ValueType& value) {
        return	stringify_fetch_field(*this, name, value);
    }

    void						end();

public:
    template<typename ValueType>
    stringify_data_fetcher&		value(ValueType& value) {
        std::string*	v;
        if(this->value(&v)) {
            std::istringstream	is(*v);
            is	>> value;
        }
        return	*this;
    }
    template<typename ValueType>
    stringify_data_fetcher&		value(ValueType& value, const std::string& name) {
        std::string*	v;
        if(this->value(&v, name)) {
            std::istringstream	is(*v);
            is	>> value;
        }
        return	*this;
    }
    template<typename ValueType>
    stringify_data_fetcher&		value(ValueType& value, unsigned long index) {
        std::string*	v;
        if(this->value(&v, index)) {
            std::istringstream	is(*v);
            is	>> value;
        }
        return	*this;
    }

private:
    stringify_data&			data_;
    std::istringstream		is_;
    stringify::id_list		stack_;
};

//
//	字符串化数据生成器
//
class	stringify_data_builder;
template<typename NameType, typename ValueType>
stringify_data_builder&		stringify_build_field(stringify_data_builder& builder, const NameType& field_name, const ValueType& field_value);

class	stringify_data_builder {
public:
    stringify_data_builder(stringify_data& data) : data_(data) {
    }

public:
    template<typename ObjectType>
    bool	execute(const ObjectType& obj, std::string* err_msg = 0) {
        try {
            this->begin();
            stringify_build_field(*this, stringify::no_name(), obj);
            this->end();
            return	true;
        } catch(const std::logic_error& err) {
            if(0 != err_msg) {
                *err_msg	= err.what();
            }
        }
        return	false;
    }

public:
    ///	builder methods. throw std::invliad_arguments if error found.
    stringify_data_builder&		begin();

    stringify_data_builder&		value(const std::string& value);
    stringify_data_builder&		value(const std::string& value, const std::string& name);

    stringify_data_builder&		array_begin();
    stringify_data_builder&		array_begin(const std::string& name);
    stringify_data_builder&		array_end();

    stringify_data_builder&		object_begin();
    stringify_data_builder&		object_begin(const std::string& name);
    stringify_data_builder&		object_end();

    template<typename NameType, typename ValueType>
    stringify_data_builder&		field(const NameType& name, const ValueType& value) {
        return	field_(name, value);
    }

    void						end();

public:
    ///	generic support.
    template<typename Type>
    stringify_data_builder&		value(const Type& value) {
        os_.str(std::string());
        os_	<< value;
        return	this->value(os_.str());
    }
    template<typename Type>
    stringify_data_builder&		value(const Type& value, const std::string& name) {
        os_.str(std::string());
        os_	<< value;
        return	this->value(os_.str(), name);
    }
    template<typename Type>
    stringify_data_builder&		value(const Type& value, const stringify::no_name& name) {
        return	this->value(value);
    }
    stringify_data_builder&		array_begin(const stringify::no_name& name) {
        return	this->array_begin();
    }
    stringify_data_builder&		object_begin(const stringify::no_name& name) {
        return	this->object_begin();
    }

private:
    template<typename ValueType>
    stringify_data_builder&		field_(const stringify::no_name& name, const ValueType& value) {
        return	stringify_build_field(*this, name, value);
    }
    template<typename ValueType>
    stringify_data_builder&		field_(const std::string& name, const ValueType& value) {
        return	stringify_build_field(*this, name, value);
    }

private:
    void	do_before_add_value(const char* err_msg);
    void	do_after_add_value(stringify::node_id new_id);
    void	do_before_add_named_value(const char* err_msg);
    void	do_after_add_named_value(stringify::node_id new_id);

private:
    stringify_data&			data_;
    std::ostringstream		os_;
    stringify::id_list		stack_;
};

//////////////////////////////////////////////////////////////////////////////////
//
//	stringify_data_builder genric support: stringify_build_field
//

#define	STRINGIFY_SIMPLE_FIELD_TYPE(TYPE)																								\
inline	stringify_data_builder&		stringify_build_field(stringify_data_builder& builder, const std::string& name, TYPE value)	{		\
	return	builder.value(value, name);																									\
}																																		\
inline	stringify_data_builder&		stringify_build_field(stringify_data_builder& builder, const stringify::no_name& name, TYPE value) {\
	return	builder.value(value);																										\
}																																		\
inline	stringify_data_fetcher&		stringify_fetch_field(stringify_data_fetcher& fetcher, const std::string& name, TYPE& value) {		\
	return	fetcher.value(value, name);																									\
}																																		\
inline	stringify_data_fetcher&		stringify_fetch_field(stringify_data_fetcher& fetcher, const stringify::no_name& name, TYPE& value){\
	return	fetcher.value(value);																										\
}																																		\
inline	stringify_data_fetcher&		stringify_fetch_field(stringify_data_fetcher& fetcher, unsigned long index, TYPE& value) {			\
	return	fetcher.value(value, index);																								\
}


STRINGIFY_SIMPLE_FIELD_TYPE(bool);
STRINGIFY_SIMPLE_FIELD_TYPE(signed int);
STRINGIFY_SIMPLE_FIELD_TYPE(signed char);
STRINGIFY_SIMPLE_FIELD_TYPE(signed short);
STRINGIFY_SIMPLE_FIELD_TYPE(signed long);
STRINGIFY_SIMPLE_FIELD_TYPE(unsigned int);
STRINGIFY_SIMPLE_FIELD_TYPE(unsigned char);
STRINGIFY_SIMPLE_FIELD_TYPE(unsigned short);
STRINGIFY_SIMPLE_FIELD_TYPE(unsigned long);
STRINGIFY_SIMPLE_FIELD_TYPE(std::string);

///	const char*
template<typename NameType>
inline	stringify_data_builder&		stringify_build_field(stringify_data_builder& builder, const NameType& name, const char* value) {
    return	builder.value(std::string(value), name);
}

template<typename NameType>
inline	stringify_data_fetcher&		stringify_fetch_field(stringify_data_fetcher& fetcher, const NameType& name, const char*& value) {
    std::string*	v	= 0;
    fetcher.value(&v, name);
    if(v != 0) {
        value	= v->c_str();
    }
    return	fetcher;
}

template<typename NameType, class KEY, class VALUE>
inline stringify_data_builder&		stringify_build_field(stringify_data_builder& builder, const NameType& name, const std::pair<KEY, VALUE>& value) {
    builder.array_begin(name);
    stringify_build_field(builder, stringify::no_name(), value.first);
    stringify_build_field(builder, stringify::no_name(), value.second);
    return	builder.array_end();
}

template<typename NameType, class KEY, class VALUE>
inline	stringify_data_fetcher&		stringify_fetch_field(stringify_data_fetcher& fetcher, const NameType& name, std::pair<KEY, VALUE>& value) {
    size_t	size	= 0;
    fetcher.array_begin(name, &size);
    stringify_fetch_field(fetcher, (unsigned long)0, value.first);
    stringify_fetch_field(fetcher, (unsigned long)1, value.second);
    return	fetcher.array_end();
}

template<typename NameType, class KEY, class VALUE>
inline	stringify_data_fetcher&		stringify_fetch_field(stringify_data_fetcher& fetcher, const NameType& name, std::pair<const KEY, VALUE>& value) {
    size_t	size	= 0;
    fetcher.array_begin(name, &size);
    stringify_fetch_field(fetcher, (unsigned long)0, const_cast<KEY&>(value.first));
    stringify_fetch_field(fetcher, (unsigned long)1, value.second);
    return	fetcher.array_end();
}

template<typename NameType, typename STLContainer>
inline stringify_data_builder&		stringify_build_field(stringify_data_builder& builder, const NameType& name, const STLContainer& array) {
    builder.array_begin(name);
    typename STLContainer::const_iterator	it		= array.begin();
    typename STLContainer::const_iterator	it_end	= array.end();
    for(; it != it_end; ++it) {
        stringify_build_field(builder, stringify::no_name(), *it);
    }
    return	builder.array_end();
}

template<typename NameType, typename STLContainer>
inline stringify_data_fetcher&		stringify_fetch_field(stringify_data_fetcher& fetcher, const NameType& name, STLContainer& arr) {
    size_t	size	= 0;
    fetcher.array_begin(name, &size);
    std::insert_iterator<STLContainer>	it(arr, arr.end());
    for(unsigned long i = 0; i < size; ++i) {
        typename STLContainer::value_type	v;
        stringify_fetch_field(fetcher, (unsigned long)i, v);
        *it	= v;
    }
    return	fetcher.array_end();
}

//
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//
//	stringify_config
//
//	基于stringify_data的配置文件, 概念上为 stringify_data 里边一个子集，包括 类型、ID 两个属性
//
//	使用该类用来合并类似 item	type/item	group/item	id/item 这样的配置项，
//	读取优先级(以ini格式 ITEM 项为例)为:
//
//	[general/id]		item = xxx		某节点下的配置，通常与应用实例相关
//	[general/group]		item = xxx		某组别下的配置，通常与平台相关
//	[general/type]		item = xxx		某类型下的配置，通常与应用相关
//	[general]			item = xxx		系统缺省参数
//
class	stringify_config {
public:
    explicit			stringify_config(stringify_data* data,
                                         const std::string& id		= "",
                                         const std::string& group	= "",
                                         const std::string& type		= "",
                                         const std::string& path_delimeter	= "/");

public:
    //
    // properties.
    //
    void				set_config_data(stringify_data* data);
    stringify_data*		get_config_data();

    void				set_config_type(const std::string& type);
    const std::string&	get_config_type();

    void				set_config_group(const std::string& group);
    const std::string&	get_config_group();

    void				set_config_id(const std::string& id);
    const std::string&	get_config_id();

    void				set_config_path_delimeter(const std::string& path_delimeter);
    const std::string&	get_config_path_delimeter();

public:
    //
    //	methods.
    //

    //
    //	容器访问方法
    //
    bool			has_section(const std::string& path);
    bool			has_section(const std::string& path, const std::string& child_name);
    bool			has_section(const std::string& path, unsigned long child_index);

    stringify::node_container*	get_section(const std::string& path);
    stringify::node_container*	get_section(const std::string& path, const std::string& child_name);
    stringify::node_container*	get_section(const std::string& path, unsigned long child_index);

    //
    //	判断值是否存在
    //
    bool			has(const std::string& path);
    bool			has(const std::string& path, const std::string& child_name);
    bool			has(const std::string& path, unsigned long child_index);

    //
    //	值访问方法[从root开始]
    //
    //	string
    std::string		get(const std::string& path, const std::string& default_value);
    std::string		get(const std::string& path, const std::string& child_name, const std::string& default_value);
    std::string		get(const std::string& path, unsigned long child_index, const std::string& default_value);
    //	const char*
    std::string		get(const std::string& path, const char* default_value);
    std::string		get(const std::string& path, const std::string& child_name, const char* default_value);
    std::string		get(const std::string& path, unsigned long child_index, const char* default_value);
    //	bool
    bool			get(const std::string& path, bool default_value);
    bool			get(const std::string& path, const std::string& child_name, bool default_value);
    bool			get(const std::string& path, unsigned long child_index, bool default_value);
    //	number
    template<typename NumberType>
    NumberType		get(const std::string& path, NumberType default_value) {
        NumberType	num;
        std::istringstream is(get(path, std::string()));
        return	(is >> num) ? num : default_value;
    }
    template<typename NumberType>
    NumberType		get(const std::string& path, const std::string& child_name, const NumberType& default_value) {
        NumberType	num;
        std::istringstream is(get(path, child_name, std::string()));
        return	(is >> num) ? num : default_value;
    }
    template<typename NumberType>
    NumberType		get(const std::string& path, unsigned long child_index, const NumberType& default_value) {
        NumberType	num;
        std::istringstream is(get(path, child_index, std::string()));
        return	(is >> num) ? num : default_value;
    }

private:
    stringify_data*	data_;
    std::string		type_;
    std::string		group_;
    std::string		id_;
    std::string		delimeter_;
};

//
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//
//	stringify_data	序列化到文本流格式
//

///	ini
//
//	!	Note:	from func was not support non-leaf array.
//				读取操作不支持 中间数组节点，如 数组的数组，对象的数组。
//
bool	stringify_to_ini_stream(stringify_data& data, std::ostream& os, const std::string& path_delimeter = "/");
bool	stringify_from_ini_stream(stringify_data& data, std::istream& is, const std::string& path_delimeter = "/");

///	json
bool	stringify_to_json_stream(stringify_data& data, std::ostream& os);
bool	stringify_from_json_stream(stringify_data& data, std::istream& is);

///	xml
bool	stringify_to_xml_stream(stringify_data& data, std::ostream& os, const std::string& anonymous_tag_name = "_");
bool	stringify_from_xml_stream(stringify_data& data, std::istream& is, const std::string& anonymous_tag_name = "_");

//
/////////////////////////////////////////////////////////////////////////////////



#endif
