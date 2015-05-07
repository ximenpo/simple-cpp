
#include	<cassert>
#include	<deque>

#include	"string.h"
#include	"stringify.h"

#include	"third/pugixml.hpp"

struct	stringify_data_xml_visitor	:	public stringify_data_visitor {
    virtual		bool	visit_begin() {
        tags_.clear();
        tags_.push_back(anonymous_);
        return	os_.good();
    }
    virtual		bool	visit_value(const std::string& value, const std::string* name) {
        os_	<<	'<'		<<	(0 != name ? *name : anonymous_)	<<	'>'
            <<	string_xml_escape(value)
            <<	"</"	<<	(0 != name ? *name : anonymous_)	<<	'>';
        return	os_.good();
    }
    virtual		bool	visit_array_begin(const std::string* name) {
        tags_.push_back(0 != name ? *name : anonymous_);
        os_	<<	'<'		<<	tags_.back()	<<	'>';
        return	os_.good();
    }
    virtual		bool	visit_array_end() {
        os_	<<	"</"	<<	tags_.back()	<<	'>';
        tags_.pop_back();
        return	os_.good();
    }
    virtual		bool	visit_object_begin(const std::string* name) {
        tags_.push_back(0 != name ? *name : anonymous_);
        os_	<<	'<'		<<	tags_.back()	<<	'>';
        return	os_.good();
    }
    virtual		bool	visit_object_end() {
        os_	<<	"</"	<<	tags_.back()	<<	'>';
        tags_.pop_back();
        return	os_.good();
    }
    virtual		bool	visit_end() {
        assert(!tags_.empty());
        tags_.pop_back();
        assert(tags_.empty());
        return	os_.good();
    }

    stringify_data_xml_visitor(std::ostream& os, const std::string& anonymous_tag_name) : os_(os), anonymous_(anonymous_tag_name) {
    }

    std::ostream&		os_;
    const std::string&	anonymous_;
    std::deque<std::string>	tags_;
};

bool	stringify_to_xml_stream(stringify_data& data, std::ostream& os, const std::string& anonymous_tag_name) {
    stringify_data_xml_visitor	visitor(os, anonymous_tag_name);
    return	data.accept(visitor);
}

struct stringify_from_xml_stream_walker {
public:
    stringify_from_xml_stream_walker(stringify_data& data, const std::string& anonymous)
        :	data_(data), anonymous_(anonymous) {
    }

    bool begin(const pugi::xml_node& node) {
        stack_.clear();

        if(node.name() != anonymous_) {
            return	false;
        }

        stringify::node_container*	container	= 0;
        if(!data_.add_container("", &container, 0, "/")) {
            return	false;
        }
        container->is_array	= (anonymous_ == node.first_child().name());

        stack_.push_back(data_.root);
        return	true;
    }
    bool before(const pugi::xml_node& node) {
        if(pugi::node_element == node.type()) {
            switch(node.first_child().type()) {
            case pugi::node_element: {
                stringify::node_container*	container;
                stringify::node_id			container_id;
                if((node.name() != anonymous_)) {
                    data_.add_container(stack_.back(), node.name(), &container, &container_id);
                } else {
                    data_.add_container(stack_.back(), &container, &container_id);
                }
                container->is_array	= (node.first_child().name() == anonymous_);
                stack_.push_back(container_id);
            }
            break;
            case pugi::node_null: {
                if(node.name() == anonymous_) {
                    data_.add_value(stack_.back(), 0, 0);
                } else {
                    data_.add_value(stack_.back(), node.name(), 0, 0);
                }
            }
            break;
            default: {
                //ignore
            }
            break;
            }
        }
        return	true;
    }

    bool visit(const pugi::xml_node& node) {
        switch(node.type()) {
        case pugi::node_pcdata:
        case pugi::node_cdata: {
            std::string	name	= node.parent().name();
            stringify::node_value*	node_new	= 0;
            if(name == anonymous_) {
                data_.add_value(stack_.back(), &node_new, 0);
            } else {
                data_.add_value(stack_.back(), name, &node_new, 0);
            }
            if(0 != node.value()) {
                node_new->value	= node.value();
            }
        }
        break;
        default:
            break;
        }
        return true; // continue traversal
    }

    bool after(const pugi::xml_node& node) {
        if(pugi::node_element == node.type() && pugi::node_element == node.first_child().type() ) {
            stack_.pop_back();
        }
        return	true;
    }

    bool end(const pugi::xml_node& node) {
        assert(!stack_.empty());
        stack_.pop_back();
        assert(stack_.empty());
        return	true;
    }

    stringify_data& data_;
    const std::string& anonymous_;
    std::deque<stringify::node_id>	stack_;
};

bool	stringify_from_xml_stream(stringify_data& data, std::istream& is, const std::string& anonymous_tag_name) {
    data.clear();

    pugi::xml_document doc;
    try {
        if(doc.load(is, pugi::parse_default).status != pugi::status_ok) {
            return	false;
        }
    } catch(const std::exception&) {
        return	false;
    }

    using	namespace	pugi;
    xml_node root = doc.first_child();
    if(root.name() != anonymous_tag_name) {
        return	false;
    }

    //	简单值的简单处理
    if(root.type() == pugi::node_element && root.first_child().type() != pugi::node_element) {
        stringify::node_value*	node;
        if(!data.add_root(&node, 0)) {
            return	false;
        }
        node->value	= root.first_child().value();
        return	true;
    }

    stringify_from_xml_stream_walker	walker(data, anonymous_tag_name);
    if (!walker.begin(root)) {
        return false;
    }

    xml_node cur = root.first_child();
    if (cur) {
        walker.before(cur);
        do {
            if (!walker.visit(cur)) {
                return false;
            }

            if (cur.first_child()) {
                cur = cur.first_child();
                walker.before(cur);
            } else if (cur.next_sibling()) {
                walker.after(cur);
                cur = cur.next_sibling();
                walker.before(cur);
            } else {
                // Borland C++ workaround
                while (!cur.next_sibling() && cur != root && !cur.parent().empty()) {
                    walker.after(cur);
                    cur = cur.parent();
                }

                if (cur != root) {
                    walker.after(cur);
                    cur = cur.next_sibling();
                    walker.before(cur);
                }
            }
        } while (cur && cur != root);
    }

    return walker.end(root);
}
