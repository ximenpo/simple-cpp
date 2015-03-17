#ifndef STRINGIFY_CONFIG_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define STRINGIFY_CONFIG_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	"stringify.h"


//
//	基于字符串化数据的配置文件, 概念上为 stringify_data 里边一个子集，包括 类型、ID 两个属性
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

    ///
    ///	判断是否存在
    ///
    bool			has(const std::string& path);
    bool			has(const std::string& path, const std::string& child_name);
    bool			has(const std::string& path, unsigned long child_index);

    ///
    ///	简单值访问方法[从root开始]
    ///
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

#endif
