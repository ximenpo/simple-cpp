
#include	"simple/string.h"
#include	"simple/stringify.h"

stringify_config::stringify_config(stringify_data* data, const std::string& id, const std::string& group, const std::string& type, const std::string& path_delimeter)
    : data_(data), id_(id), group_(group), type_(type), delimeter_(path_delimeter) {
}

void	stringify_config::set_config_data(stringify_data* data) {
    data_	= data;
}
stringify_data*	stringify_config::get_config_data() {
    return	data_;
}

void	stringify_config::set_config_type(const std::string& type) {
    type_	= type;
}

const std::string&	stringify_config::get_config_type() {
    return	type_;
}

void	stringify_config::set_config_id(const std::string& id) {
    id_	= id;
}

const std::string&	stringify_config::get_config_id() {
    return	id_;
}

void	stringify_config::set_config_path_delimeter(const std::string& path_delimeter) {
    delimeter_	= path_delimeter;
}

const std::string&	stringify_config::get_config_path_delimeter() {
    return	delimeter_;
}

bool	stringify_config::has_section(const std::string& path) {
    return	0 == data_ ? false
            :	!id_.empty()	&& data_->has_container(path + delimeter_ + id_)
            ||	!group_.empty()	&& data_->has_container(path + delimeter_ + group_)
            ||	!type_.empty()	&& data_->has_container(path + delimeter_ + type_)
            ||	data_->has_container(path)
            ;
}

bool	stringify_config::has_section(const std::string& path, const std::string& child_name) {
    return	0 == data_ ? false
            :	!id_.empty()	&& data_->has_container(path + delimeter_ + id_, child_name)
            ||	!group_.empty()	&& data_->has_container(path + delimeter_ + group_, child_name)
            ||	!type_.empty()	&& data_->has_container(path + delimeter_ + type_, child_name)
            ||	data_->has_container(path, child_name)
            ;
}

bool	stringify_config::has_section(const std::string& path, unsigned long child_index) {
    return	0 == data_ ? false
            :	!id_.empty()	&& data_->has_container(path + delimeter_ + id_, child_index)
            ||	!group_.empty() && data_->has_container(path + delimeter_ + group_, child_index)
            ||	!type_.empty()	&& data_->has_container(path + delimeter_ + type_, child_index)
            ||	data_->has_container(path, child_index)
            ;
}

stringify::node_container*	stringify_config::get_section(const std::string& path) {
    return	0 == data_ ? 0
            :	!id_.empty()	&& data_->has_container(path + delimeter_ + id_)	?	data_->get_container(path + delimeter_ + id_)
            :	!group_.empty()	&& data_->has_container(path + delimeter_ + group_)	?	data_->get_container(path + delimeter_ + group_)
            :	!type_.empty()	&& data_->has_container(path + delimeter_ + type_)	?	data_->get_container(path + delimeter_ + type_)
            :	data_->get_container(path)
            ;
}

stringify::node_container*	stringify_config::get_section(const std::string& path, const std::string& child_name) {
    return	0 == data_ ? 0
            :	!id_.empty()	&& data_->has_container(path + delimeter_ + id_, child_name)	?	data_->get_container(path + delimeter_ + id_,	child_name)
            :	!group_.empty()	&& data_->has_container(path + delimeter_ + group_, child_name)	?	data_->get_container(path + delimeter_ + group_,child_name)
            :	!type_.empty()	&& data_->has_container(path + delimeter_ + type_, child_name)	?	data_->get_container(path + delimeter_ + type_,	child_name)
            :	data_->get_container(path,	child_name)
            ;
}

stringify::node_container*	stringify_config::get_section(const std::string& path, unsigned long child_index) {
    return	0 == data_ ? 0
            :	!id_.empty()	&& data_->has_container(path + delimeter_ + id_, child_index)	?	data_->get_container(path + delimeter_ + id_,	child_index)
            :	!group_.empty()	&& data_->has_container(path + delimeter_ + group_, child_index)?	data_->get_container(path + delimeter_ + group_,child_index)
            :	!type_.empty()	&& data_->has_container(path + delimeter_ + type_, child_index)	?	data_->get_container(path + delimeter_ + type_,	child_index)
            :	data_->get_container(path,	child_index)
            ;
}

bool	stringify_config::has(const std::string& path) {
    size_t	dpos	= path.find_last_of(delimeter_);
    return	(std::string::npos == dpos)
            ?	this->has("", path)
            :	this->has(path.substr(0, dpos), path.substr(dpos + 1))
            ;
}

bool	stringify_config::has(const std::string& path, const std::string& child_name) {
    return	0 == data_ ? false
            :	!id_.empty()	&& data_->has_value(path + delimeter_ + id_, child_name)
            ||	!group_.empty()	&& data_->has_value(path + delimeter_ + group_, child_name)
            ||	!type_.empty()	&& data_->has_value(path + delimeter_ + type_, child_name)
            ||	data_->has_value(path, child_name)
            ;
}

bool	stringify_config::has(const std::string& path, unsigned long child_index) {
    return	0 == data_ ? false
            :	!id_.empty()	&& data_->has_value(path + delimeter_ + id_, child_index)
            ||	!group_.empty() && data_->has_value(path + delimeter_ + group_, child_index)
            ||	!type_.empty()	&& data_->has_value(path + delimeter_ + type_, child_index)
            ||	data_->has_value(path, child_index)
            ;
}

std::string		stringify_config::get(const std::string& path, const std::string& default_value) {
    size_t	dpos	= path.find_last_of(delimeter_);
    return	(std::string::npos == dpos)
            ?	this->get("", path, default_value)
            :	this->get(path.substr(0, dpos), path.substr(dpos + 1), default_value)
            ;
}

std::string		stringify_config::get(const std::string& path, const std::string& child_name, const std::string& default_value) {
    return	0 == data_ ? default_value
            :	!id_.empty()	&& data_->has_value(path + delimeter_ + id_, child_name)	?	data_->get_value(path + delimeter_ + id_,		child_name,	default_value)
            :	!group_.empty()	&& data_->has_value(path + delimeter_ + group_, child_name)	?	data_->get_value(path + delimeter_ + group_,	child_name,	default_value)
            :	!type_.empty()	&& data_->has_value(path + delimeter_ + type_, child_name)	?	data_->get_value(path + delimeter_ + type_,	child_name,	default_value)
            :	data_->get_value(path,	child_name,	default_value)
            ;
}

std::string		stringify_config::get(const std::string& path, unsigned long child_index, const std::string& default_value) {
    return	0 == data_ ? default_value
            :	!id_.empty()	&& data_->has_value(path + delimeter_ + id_, child_index)	?	data_->get_value(path + delimeter_ + id_,		child_index,	default_value)
            :	!group_.empty()	&& data_->has_value(path + delimeter_ + group_, child_index)?	data_->get_value(path + delimeter_ + group_,	child_index,	default_value)
            :	!type_.empty()	&& data_->has_value(path + delimeter_ + type_, child_index)	?	data_->get_value(path + delimeter_ + type_,	child_index,	default_value)
            :	data_->get_value(path,	child_index,	default_value)
            ;
}

std::string	stringify_config::get(const std::string& path, const char* default_value) {
    if(0 == default_value) {
        default_value	= "";
    }
    return	this->get(path, std::string(default_value));
}

std::string	stringify_config::get(const std::string& path, const std::string& child_name, const char* default_value) {
    if(0 == default_value) {
        default_value	= "";
    }
    return	this->get(path, child_name, std::string(default_value));
}

std::string	stringify_config::get(const std::string& path, unsigned long child_index, const char* default_value) {
    if(0 == default_value) {
        default_value	= "";
    }
    return	this->get(path, child_index, std::string(default_value));
}

bool	stringify_config::get(const std::string& path, bool default_value) {
    bool	v;
    return	string_tobool(this->get(path, std::string()), v) ? v : default_value;
}

bool	stringify_config::get(const std::string& path, const std::string& child_name, bool default_value) {
    bool	v;
    return	string_tobool(this->get(path, child_name, std::string()), v) ? v : default_value;
}

bool	stringify_config::get(const std::string& path, unsigned long child_index, bool default_value) {
    bool	v;
    return	string_tobool(this->get(path, child_index, std::string()), v) ? v : default_value;
}
