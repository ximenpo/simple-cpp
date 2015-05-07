
#include	"stringify.h"
#include	"string.h"

#include	<stdexcept>

using	namespace	stringify;

/////////////////////////////////////////////////////////////////////////////
//
//	VISITOR
//
static	bool	stringify_data_visit_value(stringify_data& data, stringify_data_visitor& visitor, node_id id);
static	bool	stringify_data_visit_container(stringify_data& data, stringify_data_visitor& visitor, node_id id);
static	bool	stringify_data_visit_array(stringify_data& data, stringify_data_visitor& visitor, node_id id);
static	bool	stringify_data_visit_object(stringify_data& data, stringify_data_visitor& visitor, node_id id);

static	bool	stringify_data_visit_value(stringify_data& data, stringify_data_visitor& visitor, node_id id) {
    NODE_TYPE		type	= node_type(id);
    unsigned long	index	= node_index(id);
    switch(type) {
    case NODE_VALUE: {
        node_value&	node	= data.values[index];
        if(!visitor.visit_value(node.value, 0)) {
            return	false;
        }
    }
    break;
    case NODE_NAMED_VALUE: {
        node_named_value&	node	= data.named_values[index];
        if(!visitor.visit_value(node.value, &node.name)) {
            return	false;
        }
    }
    break;
    default: {
        // ignore.
    }
    break;
    }
    return	true;
}

static	bool	stringify_data_visit_container(stringify_data& data, stringify_data_visitor& visitor, node_id id) {
    NODE_TYPE		type	= node_type(id);
    unsigned long	index	= node_index(id);

    node_container*	node	= 0;
    switch(type) {
    case NODE_CONTAINER:
        node	= &data.containers[index];
        break;
    case NODE_NAMED_CONTAINER:
        node	= &data.named_containers[index];
        break;
    default:
        break;
    }

    if(0 == node) {
        return	false;
    }

    if(node->is_array && !stringify_data_visit_array(data, visitor, id)) {
        return	false;
    } else if(!node->is_array && !stringify_data_visit_object(data, visitor, id)) {
        return	false;
    }

    return	true;
}

static	bool	stringify_data_visit_array(stringify_data& data, stringify_data_visitor& visitor, node_id id) {
    NODE_TYPE		type	= node_type(id);
    unsigned long	index	= node_index(id);

    std::string*	name	= 0;
    node_container*	node	= 0;
    switch(type) {
    case NODE_CONTAINER:
        node	= &data.containers[index];
        break;
    case NODE_NAMED_CONTAINER:
        node	= &data.named_containers[index];
        name	= &data.named_containers[index].name;
        break;
    default:
        break;
    }

    if(0 == node) {
        return	false;
    }

    if(!visitor.visit_array_begin(name)) {
        return	false;
    }

    id_list::const_iterator	it		= node->items.begin();
    id_list::const_iterator	it_end	= node->items.end();
    for(; it != it_end; ++it) {
        switch(node_type(*it)) {
        case NODE_VALUE: {
            if(!stringify_data_visit_value(data, visitor, *it)) {
                return	false;
            }
        }
        break;
        case NODE_CONTAINER: {
            if(!stringify_data_visit_container(data, visitor, *it)) {
                return	false;
            }
        }
        break;
        default: {
            // ignore.
        }
        break;
        }
    }

    if(!visitor.visit_array_end()) {
        return	false;
    }

    return	true;
}

static	bool	stringify_data_visit_object(stringify_data& data, stringify_data_visitor& visitor, node_id id) {
    NODE_TYPE		type	= node_type(id);
    unsigned long	index	= node_index(id);

    std::string*	name	= 0;
    node_container*	node	= 0;
    switch(type) {
    case NODE_CONTAINER:
        node	= &data.containers[index];
        break;
    case NODE_NAMED_CONTAINER:
        node	= &data.named_containers[index];
        name	= &data.named_containers[index].name;
        break;
    default:
        break;
    }

    if(0 == node) {
        return	false;
    }

    if(!visitor.visit_object_begin(name)) {
        return	false;
    }

    id_list::const_iterator	it		= node->items.begin();
    id_list::const_iterator	it_end	= node->items.end();
    for(; it != it_end; ++it) {
        switch(node_type(*it)) {
        case NODE_NAMED_VALUE: {
            if(!stringify_data_visit_value(data, visitor, *it)) {
                return	false;
            }
        }
        break;
        case NODE_NAMED_CONTAINER: {
            if(!stringify_data_visit_container(data, visitor, *it)) {
                return	false;
            }
        }
        break;
        default: {
            // ignore.
        }
        break;
        }
    }

    if(!visitor.visit_object_end()) {
        return	false;
    }

    return	true;
}

//
/////////////////////////////////////////////////////////////////////////////

void	stringify_data::clear() {
    values.clear();
    containers.clear();
    named_values.clear();
    named_containers.clear();
    root	= make_node_id(NODE_NONE, 0);
}

bool	stringify_data::accept(stringify_data_visitor& visitor) {
    if(!visitor.visit_begin()) {
        return	false;
    }

    switch(node_type(root)) {
    case NODE_VALUE: {
        if(!stringify_data_visit_value(*this, visitor, root)) {
            return	false;
        }
    }
    break;
    case NODE_CONTAINER: {
        if(!stringify_data_visit_container(*this, visitor, root)) {
            return	false;
        }
    }
    break;
    default: {
        // ignore.
    }
    break;
    }

    return	visitor.visit_end();
}

bool	stringify_data::fetch(const std::string& path, stringify::node_id* node_id, const std::string& path_delimiter) {
    std::deque<std::string>	strs;
    string_split(path, path_delimiter, std::back_inserter(strs), false);

    stringify::node_id	id	= root, tmp_id;

    std::deque<std::string>::iterator	it		= strs.begin();
    std::deque<std::string>::iterator	it_end	= strs.end();
    // ���Ե�һ������[���Ա�ʾΪroot]
    if(!strs.empty() && string_trim(it->c_str()).empty()) {
        ++it;
    }
    for(; it != it_end; ++it) {
        string_trim(*it);
        if(!this->fetch(id, *it, &tmp_id)) {
            return	false;
        }
        id	= tmp_id;
    }

    if(0 != node_id) {
        *node_id	= id;
    }
    return	true;
}

bool	stringify_data::fetch(stringify::node_id node_id, std::string** value, std::string** name) {
    unsigned long	index	= node_index(node_id);
    switch(node_type(node_id)) {
    case NODE_VALUE: {
        if(index >= values.size()) {
            break;
        }
        if(0 != value) {
            *value	= &values[index].value;
        }
        if(0 != name) {
            *name	= 0;
        }
        return	true;
    }
    break;
    case NODE_NAMED_VALUE: {
        if(index >= named_values.size()) {
            break;
        }
        node_named_value& node	= named_values[index];
        if(0 != value) {
            *value	= &node.value;
        }
        if(0 != name) {
            *name	= &node.name;
        }
        return	true;
    }
    break;
    default: {
        // ignore.
    }
    break;
    }
    return	false;
}

bool	stringify_data::fetch(stringify::node_id node_id, stringify::node_container** container, std::string** name) {
    unsigned long	index	= node_index(node_id);
    switch(node_type(node_id)) {
    case NODE_CONTAINER: {
        if(index >= containers.size()) {
            break;
        }
        node_container&	node	= containers[index];
        if(0 != container) {
            *container	= &node;
        }
        if(0 != name) {
            *name	= 0;
        }
        return	true;
    }
    break;
    case NODE_NAMED_CONTAINER: {
        if(index >= named_containers.size()) {
            break;
        }
        node_named_container& node	= named_containers[index];
        if(0 != container) {
            *container	= &node;
        }
        if(0 != name) {
            *name	= &node.name;
        }
        return	true;
    }
    break;
    default: {
        // ignore.
    }
    break;
    }
    return	false;
}

bool	stringify_data::fetch(stringify::node_id node_id, unsigned long child_pos, stringify::node_id* child_id) {
    node_container*	container	= 0;
    if(!fetch(node_id, &container, 0)) {
        return	false;
    }
    if(child_pos >= container->items.size()) {
        return	false;
    }
    if(0 != child_id) {
        *child_id	= container->items.at(child_pos);
    }
    return	true;
}

bool	stringify_data::fetch(stringify::node_id node_id, unsigned long child_pos, std::string** value) {
    stringify::node_id	id	= 0;
    if(!fetch(node_id, child_pos, &id)) {
        return	false;
    }
    return	fetch(id, value, 0);
}

bool	stringify_data::fetch(stringify::node_id node_id, unsigned long child_pos, stringify::node_container** container) {
    stringify::node_id	id	= 0;
    if(!fetch(node_id, child_pos, &id)) {
        return	false;
    }
    return	fetch(id, container, 0);
}

bool	stringify_data::fetch(stringify::node_id node_id, const std::string& child_name, stringify::node_id* child_id) {
    node_container*	container	= 0;
    if(!fetch(node_id, &container, 0)) {
        return	false;
    }
    if(container->is_array || container->items.empty()) {
        return	false;
    }
    id_list::const_iterator		it		= container->items.begin();
    id_list::const_iterator		it_end	= container->items.end();
    for(; it != it_end; ++it) {
        switch(node_type(*it)) {
        case NODE_NAMED_VALUE: {
            if(named_values[node_index(*it)].name == child_name) {
                if(0 != child_id) {
                    *child_id	= *it;
                }
                return	true;
            }
        }
        break;
        case NODE_NAMED_CONTAINER: {
            if(named_containers[node_index(*it)].name == child_name) {
                if(0 != child_id) {
                    *child_id	= *it;
                }
                return	true;
            }
        }
        break;
        default: {
            // ignore.
        }
        break;
        }
    }
    return	false;
}

bool	stringify_data::fetch(stringify::node_id node_id, const std::string& child_name, std::string** value) {
    stringify::node_id	id	= 0;
    if(!fetch(node_id, child_name, &id)) {
        return	false;
    }
    return	fetch(id, value, 0);
}

bool	stringify_data::fetch(stringify::node_id node_id, const std::string& child_name, stringify::node_container** container) {
    stringify::node_id	id	= 0;
    if(!fetch(node_id, child_name, &id)) {
        return	false;
    }
    return	fetch(id, container, 0);
}

bool	stringify_data::add_root(stringify::node_value** new_root, stringify::node_id* new_id_ptr) {
    if(NODE_NONE != node_type(root)) {
        return	false;
    }
    this->values.push_back(node_value());
    node_id		new_id		= make_node_id(NODE_VALUE, this->values.size() - 1);
    node_value&	new_node	= this->values.back();
    root	= new_id;
    if(0 != new_root) {
        *new_root	= &new_node;
    }
    if(0 != new_id_ptr) {
        *new_id_ptr	= new_id;
    }
    return	true;
}

bool	stringify_data::add_root(stringify::node_container** new_root, stringify::node_id* new_id_ptr) {
    if(NODE_NONE != node_type(root)) {
        return	false;
    }
    this->containers.push_back(node_container());
    node_id			new_id		= make_node_id(NODE_CONTAINER, this->containers.size() - 1);
    node_container&	new_node	= this->containers.back();
    new_node.is_array	= false;
    root	= new_id;
    if(0 != new_root) {
        *new_root	= &new_node;
    }
    if(0 != new_id_ptr) {
        *new_id_ptr	= new_id;
    }
    return	true;
}

bool	stringify_data::add_container(stringify::node_id container_id, stringify::node_container** new_container, stringify::node_id* new_id_ptr) {
    node_container*	container	= 0;
    if(!fetch(container_id, &container, 0) || !container->is_array) {
        return	false;
    }
    this->containers.push_back(node_container());
    node_id			new_id		= make_node_id(NODE_CONTAINER, this->containers.size() - 1);
    node_container&	new_node	= this->containers.back();
    container->items.push_back(new_id);
    new_node.is_array	= false;
    if(0 != new_container) {
        *new_container	= &new_node;
    }
    if(0 != new_id_ptr) {
        *new_id_ptr	= new_id;
    }
    return	true;
}

bool	stringify_data::add_container(stringify::node_id container_id, const std::string& child_name,	stringify::node_container** new_container, stringify::node_id* new_id_ptr) {
    node_container*	container	= 0;
    if(!fetch(container_id, &container, 0) || container->is_array) {
        return	false;
    }
    this->named_containers.push_back(node_named_container());
    node_id					new_id		= make_node_id(NODE_NAMED_CONTAINER, this->named_containers.size() - 1);
    node_named_container&	new_node	= this->named_containers.back();
    new_node.is_array	= false;
    new_node.name		= child_name;
    container->items.push_back(new_id);
    if(0 != new_container) {
        *new_container	= &new_node;
    }
    if(0 != new_id_ptr) {
        *new_id_ptr	= new_id;
    }
    return	true;
}

bool	stringify_data::add_container(const std::string& path,	stringify::node_container** new_container, stringify::node_id* new_id, const std::string& path_delimiter) {
    std::deque<std::string>	strs;
    string_split(path, path_delimiter, std::back_inserter(strs), false);

    stringify::node_id	id	= root, tmp_id;
    node_container*	container	= 0;
    if(NODE_NONE == node_type(root) && !this->add_root(&container, &id)) {
        return	false;
    } else {
        if(NODE_CONTAINER != node_type(id) || node_index(id) >= containers.size()) {
            return	false;
        }
        container	= &containers[node_index(id)];
    }

    std::deque<std::string>::iterator	it		= strs.begin();
    std::deque<std::string>::iterator	it_end	= strs.end();
    // ���Ե�һ������[���Ա�ʾΪroot]
    if(!strs.empty() && string_trim(it->c_str()).empty()) {
        ++it;
    }
    for(; it != it_end; ++it) {
        string_trim(*it);
        if(!this->fetch(id, *it, &tmp_id)) {
            // ���Ӷ���
            this->add_container(id, *it, &container, &tmp_id);
        } else if(node_type(tmp_id) != NODE_NAMED_CONTAINER) {
            return	false;
        }
        id	= tmp_id;
    }
    if(0 != new_container) {
        *new_container	= container;
    }
    if(0 != new_id) {
        *new_id	= id;
    }
    return	true;
}

bool	stringify_data::add_value(stringify::node_id container_id, stringify::node_value** new_node_ptr, stringify::node_id* new_id_ptr) {
    node_container*	container	= 0;
    if(!fetch(container_id, &container, 0) || !container->is_array) {
        return	false;
    }
    this->values.push_back(node_value());
    node_id		new_id		= make_node_id(NODE_VALUE, this->values.size() - 1);
    node_value&	new_node	= this->values.back();
    container->items.push_back(new_id);
    if(0 != new_node_ptr) {
        *new_node_ptr	= &new_node;
    }
    if(0 != new_id_ptr) {
        *new_id_ptr	= new_id;
    }
    return	true;
}

bool	stringify_data::add_value(stringify::node_id container_id, const std::string& child_name,	stringify::node_value** new_node_ptr, stringify::node_id* new_id_ptr) {
    node_container*	container	= 0;
    if(!fetch(container_id, &container, 0) || container->is_array) {
        return	false;
    }
    this->named_values.push_back(node_named_value());
    node_id				new_id		= make_node_id(NODE_NAMED_VALUE, this->named_values.size() - 1);
    node_named_value&	new_node	= this->named_values.back();
    new_node.name		= child_name;
    container->items.push_back(new_id);
    if(0 != new_node_ptr) {
        *new_node_ptr	= &new_node;
    }
    if(0 != new_id_ptr) {
        *new_id_ptr	= new_id;
    }
    return	true;
}

bool	stringify_data::has(const std::string& path) {
    stringify::node_id	id;
    return	this->fetch(path, &id);
}

bool	stringify_data::has(const std::string& path, const std::string& child_name) {
    stringify::node_id	id;
    return	this->fetch(path, &id) && this->fetch(id, child_name, &id);
}

bool	stringify_data::has(const std::string& path, unsigned long child_index) {
    stringify::node_id	id;
    return	this->fetch(path, &id) && this->fetch(id, child_index, &id);
}

bool	stringify_data::has_value(const std::string& path) {
    stringify::node_id	id;
    return	this->fetch(path, &id) && (node_type(id) == NODE_VALUE || node_type(id) == NODE_NAMED_VALUE);
}

bool	stringify_data::has_value(const std::string& path, const std::string& child_name) {
    stringify::node_id	id;
    return	this->fetch(path, &id) && this->fetch(id, child_name, &id) && (node_type(id) == NODE_VALUE || node_type(id) == NODE_NAMED_VALUE);
}

bool	stringify_data::has_value(const std::string& path, unsigned long child_index) {
    stringify::node_id	id;
    return	this->fetch(path, &id) && this->fetch(id, child_index, &id) && (node_type(id) == NODE_VALUE || node_type(id) == NODE_NAMED_VALUE);
}

std::string		stringify_data::get_value(const std::string& path, const std::string& default_value) {
    stringify::node_id	id;
    std::string*	str = 0;
    if(!this->fetch(path, &id) || !this->fetch(id, &str, 0)) {
        return	default_value;
    }

    return	*str;
}

std::string		stringify_data::get_value(const std::string& path, const std::string& child_name, const std::string& default_value) {
    stringify::node_id	id;
    std::string*	str = 0;
    if(!this->fetch(path, &id) || !this->fetch(id, child_name, &str)) {
        return	default_value;
    }

    return	*str;
}

std::string		stringify_data::get_value(const std::string& path, unsigned long child_index, const std::string& default_value) {
    stringify::node_id	id;
    std::string*	str = 0;
    if(!this->fetch(path, &id) || !this->fetch(id, child_index, &str)) {
        return	default_value;
    }

    return	*str;
}

bool	stringify_data::set_value(const std::string& path, const std::string& value) {
    stringify::node_id	id;
    std::string*	str = 0;
    if(!this->fetch(path, &id) || !this->fetch(id, &str, 0)) {
        return	false;
    }
    *str	= value;
    return	true;
}

bool	stringify_data::set_value(const std::string& path, const std::string& child_name, const std::string& value) {
    stringify::node_id	id;
    std::string*	str = 0;
    if(!this->fetch(path, &id) || !this->fetch(id, child_name, &str)) {
        return	false;
    }
    *str	= value;
    return	true;
}

bool	stringify_data::set_value(const std::string& path, unsigned long child_index, const std::string& value) {
    stringify::node_id	id;
    std::string*	str = 0;
    if(!this->fetch(path, &id) || !this->fetch(id, child_index, &str)) {
        return	false;
    }
    *str	= value;
    return	true;
}

bool	stringify_data::has_container(const std::string& path) {
    stringify::node_id	id;
    return	this->fetch(path, &id) && (node_type(id) == NODE_CONTAINER || node_type(id) == NODE_NAMED_CONTAINER);
}

bool	stringify_data::has_container(const std::string& path, const std::string& child_name) {
    stringify::node_id	id;
    return	this->fetch(path, &id) && this->fetch(id, child_name, &id) && (node_type(id) == NODE_CONTAINER || node_type(id) == NODE_NAMED_CONTAINER);
}

bool	stringify_data::has_container(const std::string& path, unsigned long child_index) {
    stringify::node_id	id;
    return	this->fetch(path, &id) && this->fetch(id, child_index, &id) && (node_type(id) == NODE_CONTAINER || node_type(id) == NODE_NAMED_CONTAINER);
}

stringify::node_container*	stringify_data::get_container(const std::string& path) {
    stringify::node_id	id;
    stringify::node_container*	ret	= 0;
    if(		this->fetch(path, &id)
            && (node_type(id) == NODE_CONTAINER || node_type(id) == NODE_NAMED_CONTAINER)
            &&	fetch(id, &ret, 0)) {
        return	ret;
    }
    return	0;
}

stringify::node_container*	stringify_data::get_container(const std::string& path, const std::string& child_name) {
    stringify::node_id	id;
    stringify::node_container*	ret	= 0;
    if(		this->fetch(path, &id)
            &&	this->fetch(id, child_name, &id)
            &&	(node_type(id) == NODE_CONTAINER || node_type(id) == NODE_NAMED_CONTAINER)
            &&	fetch(id, &ret, 0)) {
        return	ret;
    }
    return	0;
}

stringify::node_container*	stringify_data::get_container(const std::string& path, unsigned long child_index) {
    stringify::node_id	id;
    stringify::node_container*	ret	= 0;
    if(		this->fetch(path, &id)
            &&	this->fetch(id, child_index, &id)
            &&	(node_type(id) == NODE_CONTAINER || node_type(id) == NODE_NAMED_CONTAINER)
            &&	fetch(id, &ret, 0)) {
        return	ret;
    }
    return	0;
}

void	stringify_data_builder::do_before_add_value(const char* err_msg) {
    if(stack_.empty()) {
        throw	std::invalid_argument("found empty context when add_value");
    }

    node_id	curr_id	= stack_.back();

    bool	context_valid	= false;
    switch(node_type(curr_id)) {
    case NODE_NONE://root
        context_valid	= true;
        break;
    case NODE_CONTAINER: //array item
        context_valid	= (data_.containers[node_index(curr_id)].is_array);
        break;
    case NODE_NAMED_CONTAINER: //object item
        context_valid	= (data_.named_containers[node_index(curr_id)].is_array);
        break;
    default:
        break;
    }
    if(!context_valid) {
        throw	std::invalid_argument(err_msg);
    }
}

void	stringify_data_builder::do_after_add_value(node_id new_id) {
    node_id	curr_id	= stack_.back();

    switch(node_type(curr_id)) {
    case NODE_NONE:// change root value
        data_.root	= new_id, stack_.pop_back();
        break;
    case NODE_CONTAINER: //	append
        data_.containers[node_index(curr_id)].items.push_back(new_id);
        break;
    case NODE_NAMED_CONTAINER: //append
        data_.named_containers[node_index(curr_id)].items.push_back(new_id);
        break;
    default:
        break;
    }
}

void	stringify_data_builder::do_before_add_named_value(const char* err_msg) {
    if(stack_.empty()) {
        throw	std::invalid_argument("found empty context when add_named_value");
    }

    node_id	curr_id	= stack_.back();

    bool	context_valid	= false;
    switch(node_type(curr_id)) {
    case NODE_CONTAINER: //object item
        context_valid	= !(data_.containers[node_index(curr_id)].is_array);
        break;
    case NODE_NAMED_CONTAINER: //object item
        context_valid	= !(data_.named_containers[node_index(curr_id)].is_array);
        break;
    default:
        break;
    }
    if(!context_valid) {
        throw	std::invalid_argument(err_msg);
    }
}

void	stringify_data_builder::do_after_add_named_value(node_id new_id) {
    node_id	curr_id	= stack_.back();

    switch(node_type(curr_id)) {
    case NODE_CONTAINER: // append
        data_.containers[node_index(curr_id)].items.push_back(new_id);
        break;
    case NODE_NAMED_CONTAINER: // append
        data_.named_containers[node_index(curr_id)].items.push_back(new_id);
        break;
    default:
        break;
    }
}

stringify_data_builder&		stringify_data_builder::begin() {
    data_.clear();
    stack_.clear();
    stack_.push_back(make_node_id(NODE_NONE, 0));

    return	*this;
}

stringify_data_builder&		stringify_data_builder::value(const std::string& value) {
    do_before_add_value("require root or array context when add value");

    data_.values.push_back(node_value());
    node_value&	node	= data_.values.back();
    node.value	= value;

    node_id		new_id	= make_node_id(NODE_VALUE, data_.values.size() - 1);
    do_after_add_value(new_id);
    return	*this;
}

stringify_data_builder&		stringify_data_builder::value(const std::string& value, const std::string& name) {
    do_before_add_named_value("require object context when add named value");

    data_.named_values.push_back(node_named_value());
    node_named_value&	node	= data_.named_values.back();
    node.value	= value;
    node.name	= name;

    node_id		new_id	= make_node_id(NODE_NAMED_VALUE, data_.named_values.size() - 1);
    do_after_add_named_value(new_id);
    return	*this;
}

stringify_data_builder&		stringify_data_builder::array_begin() {
    do_before_add_value("require root or array context when add array");

    data_.containers.push_back(node_container());
    node_container&	node	= data_.containers.back();
    node.is_array		= true;

    node_id		new_id	= make_node_id(NODE_CONTAINER, data_.containers.size() - 1);
    do_after_add_value(new_id);

    stack_.push_back(new_id);
    return	*this;
}

stringify_data_builder&		stringify_data_builder::array_begin(const std::string& name) {
    do_before_add_named_value("require object context when add named array");

    data_.named_containers.push_back(node_named_container());
    node_named_container&	node	= data_.named_containers.back();
    node.name		= name;
    node.is_array	= true;

    node_id		new_id	= make_node_id(NODE_NAMED_CONTAINER, data_.named_containers.size() - 1);
    do_after_add_named_value(new_id);

    stack_.push_back(new_id);
    return	*this;
}

stringify_data_builder&		stringify_data_builder::array_end() {
    stack_.pop_back();
    return	*this;
}

stringify_data_builder&		stringify_data_builder::object_begin() {
    do_before_add_value("require root or array/object context when add object");

    data_.containers.push_back(node_container());
    node_container&	node	= data_.containers.back();
    node.is_array		= false;

    node_id		new_id	= make_node_id(NODE_CONTAINER, data_.containers.size() - 1);
    do_after_add_value(new_id);

    stack_.push_back(new_id);
    return	*this;
}

stringify_data_builder&	stringify_data_builder::object_begin(const std::string& name) {
    do_before_add_named_value("require object context when add named object");

    data_.named_containers.push_back(node_named_container());
    node_named_container&	node	= data_.named_containers.back();
    node.name		= name;
    node.is_array	= false;

    node_id		new_id	= make_node_id(NODE_NAMED_CONTAINER, data_.named_containers.size() - 1);
    do_after_add_named_value(new_id);

    stack_.push_back(new_id);
    return	*this;
}

stringify_data_builder&	stringify_data_builder::object_end() {
    stack_.pop_back();
    return	*this;
}

void	stringify_data_builder::end() {
    if(!stack_.empty()) {
        stack_.pop_back();
    }
    assert(stack_.empty());
}

stringify_data_fetcher&		stringify_data_fetcher::begin() {
    stack_.clear();
    stack_.push_back(data_.root);

    return	*this;
}

bool	stringify_data_fetcher::value(std::string** value) {
    if(stack_.empty()) {
        throw	std::invalid_argument("found empty context when fetch value");
    }
    node_id	curr_id	= stack_.back();
    return	data_.fetch(curr_id, value, 0);
}

bool	stringify_data_fetcher::value(std::string** value, const std::string& name) {
    if(stack_.empty()) {
        throw	std::invalid_argument("found empty context when fetch value");
    }
    node_id	curr_id	= stack_.back();
    return	data_.fetch(curr_id, name, value);
}

bool	stringify_data_fetcher::value(std::string** value, unsigned long index) {
    if(stack_.empty()) {
        throw	std::invalid_argument("found empty context when fetch value");
    }
    node_id	curr_id	= stack_.back();
    return	data_.fetch(curr_id, index, value);
}

stringify_data_fetcher&		stringify_data_fetcher::value(std::string& value) {
    std::string*	v	= 0;
    if(this->value(&v)) {
        value	= *v;
    }
    return	*this;
}

stringify_data_fetcher&		stringify_data_fetcher::value(std::string& value, const std::string& name) {
    std::string*	v	= 0;
    if(this->value(&v, name)) {
        value	= *v;
    }
    return	*this;
}

stringify_data_fetcher&		stringify_data_fetcher::value(std::string& value, unsigned long index) {
    std::string*	v	= 0;
    if(this->value(&v, index)) {
        value	= *v;
    }
    return	*this;
}

stringify_data_fetcher&		stringify_data_fetcher::array_begin(size_t* size) {
    if(stack_.empty()) {
        throw	std::invalid_argument("found empty context when fetch array");
    }
    node_id		id	= stack_.back();
    node_container*	container	= 0;
    if(id == data_.root && data_.fetch(id, &container, 0) && container->is_array) {
        *size	= container->items.size();
    } else {
        throw	std::invalid_argument("can't fetch array");
    }
    return	*this;
}

stringify_data_fetcher&		stringify_data_fetcher::array_begin(unsigned long index, size_t* size) {
    if(stack_.empty()) {
        throw	std::invalid_argument("found empty context when fetch array");
    }
    node_id		id	= stack_.back();
    node_id		fid;
    node_container*	container	= 0;
    if(data_.fetch(id, index, &fid) && data_.fetch(fid, &container, 0) && container->is_array) {
        stack_.push_back(fid);
        *size	= container->items.size();
    } else {
        throw	std::invalid_argument("can't fetch array");
    }
    return	*this;
}

stringify_data_fetcher&		stringify_data_fetcher::array_begin(const std::string& name, size_t* size) {
    if(stack_.empty()) {
        throw	std::invalid_argument("found empty context when fetch array");
    }
    node_id		id	= stack_.back();
    node_id		fid;
    node_container*	container	= 0;
    if(data_.fetch(id, name, &fid) && data_.fetch(fid, &container, 0) && container->is_array) {
        stack_.push_back(fid);
        *size	= container->items.size();
    } else {
        throw	std::invalid_argument("can't fetch array");
    }
    return	*this;
}

stringify_data_fetcher&		stringify_data_fetcher::array_begin(const stringify::no_name& name, size_t* size) {
    return	array_begin(size);
}

stringify_data_fetcher&		stringify_data_fetcher::array_end() {
    stack_.pop_back();
    return	*this;
}

stringify_data_fetcher&		stringify_data_fetcher::object_begin() {
    if(stack_.empty()) {
        throw	std::invalid_argument("found empty context when fetch object");
    }
    node_id		id	= stack_.back();
    node_container*	container	= 0;
    if(id == data_.root && data_.fetch(id, &container, 0) && !container->is_array) {
    } else {
        throw	std::invalid_argument("can't fetch object");
    }
    return	*this;
}

stringify_data_fetcher&		stringify_data_fetcher::object_begin(unsigned long index) {
    if(stack_.empty()) {
        throw	std::invalid_argument("found empty context when fetch object");
    }
    node_id		id	= stack_.back();
    node_id		fid;
    node_container*	container	= 0;
    if(data_.fetch(id, index, &fid) && data_.fetch(fid, &container, 0) && !container->is_array) {
        stack_.push_back(fid);
    } else {
        throw	std::invalid_argument("can't fetch object");
    }
    return	*this;
}

stringify_data_fetcher&		stringify_data_fetcher::object_begin(const std::string& name) {
    if(stack_.empty()) {
        throw	std::invalid_argument("found empty context when fetch object");
    }
    node_id		id	= stack_.back();
    node_id		fid;
    node_container*	container	= 0;
    if(data_.fetch(id, name, &fid) && data_.fetch(fid, &container, 0) && !container->is_array) {
        stack_.push_back(fid);
    } else {
        throw	std::invalid_argument("can't fetch object");
    }
    return	*this;
}

stringify_data_fetcher&		stringify_data_fetcher::object_begin(const stringify::no_name& name) {
    return	object_begin();
}

stringify_data_fetcher&		stringify_data_fetcher::object_end() {
    stack_.pop_back();
    return	*this;
}

void	stringify_data_fetcher::end() {
    if(!stack_.empty()) {
        stack_.pop_back();
    }
    assert(stack_.empty());
}
