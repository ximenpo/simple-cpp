
#include	<cassert>
#include	<deque>

#include	"string.h"
#include	"stringify.h"

struct	stringify_data_ini_visitor	:	public stringify_data_visitor {
    virtual		bool	visit_begin() {
        sections_.clear();
        last_level_	= 0;
        return	true;
    }
    virtual		bool	visit_value(const std::string& value, const std::string* name) {
        if(last_level_ > sections_.size()) {
            os_	<<	"["	<<	string_join(sections_.begin(), sections_.end(), delimeter_.substr(0, 1))	<<	"]\n";
        }
        if(0 != name) {
            os_	<<	(*name)	<<	"="	;
        }
        os_<<	string_line_escape(value)	<<	"\n";
        last_level_	= sections_.size();
        return	true;
    }
    virtual		bool	visit_array_begin(const std::string* name) {
        if(!sections_.empty() || 0 != name) {
            sections_.push_back(0 == name ? "" : *name);
        }
        os_	<<	"["	<<	string_join(sections_.begin(), sections_.end(), delimeter_.substr(0, 1))	<<	"]\n";
        return	true;
    }
    virtual		bool	visit_array_end() {
        if(!sections_.empty()) {
            sections_.pop_back();
        }
        return	true;
    }
    virtual		bool	visit_object_begin(const std::string* name) {
        if(!sections_.empty() || 0 != name) {
            sections_.push_back(0 == name ? "" : *name);
        }
        os_	<<	"["	<<	string_join(sections_.begin(), sections_.end(), delimeter_.substr(0, 1))	<<	"]\n";
        return	true;
    }
    virtual		bool	visit_object_end() {
        if(!sections_.empty()) {
            sections_.pop_back();
        }
        return	true;
    }
    virtual		bool	visit_end() {
        assert(sections_.empty());
        return	true;
    }

    stringify_data_ini_visitor(std::ostream& os, const std::string& path_delimeter) : os_(os), delimeter_(path_delimeter) {
    }

    std::ostream&	os_;
    std::string		delimeter_;
    std::deque<std::string>	sections_;
    size_t			last_level_;
};

bool	stringify_to_ini_stream(stringify_data& data, std::ostream& os, const std::string& path_delimeter) {
    stringify_data_ini_visitor	visitor(os, path_delimeter);
    return	data.accept(visitor);
}

bool	stringify_from_ini_stream(stringify_data& data, std::istream& is, const std::string& path_delimeter) {
    using	namespace	stringify;
    std::string		line;
    node_container*	container	= 0;
    node_id			container_id= data.root;
    // root container.
    if(!data.add_container("", &container, &container_id, path_delimeter)) {
        return	false;
    }
    //	content
    while(std::getline(is, line)) {
        string_trim(line);
        if(line.empty() || line[0]==';' || line[0]=='#') {
            continue;
        }
        // section
        if(line[0]=='['&& *(--line.end())==']') {
            std::string	full_section	= line.substr(1, line.size() - 2);
            string_trim(full_section);
            //	[object/array//] means object:{array:[[[1,2,3]...],
            size_t	pos	= full_section.find_last_not_of(path_delimeter);
            std::string	section			= full_section.substr(0, pos + 1);
            std::string	nonleaf_arrays	= full_section.substr(pos + 1);
            if(!nonleaf_arrays.empty()) {
                // non-leaf array is not support.
                return	false;
            }
            if(!data.fetch(section, &container_id, path_delimeter)) {
                if(!data.add_container(section, &container, &container_id, path_delimeter)) {
                    return	false;
                }
            } else {
                if(!data.fetch(container_id, &container, 0)) {
                    return	false;
                }
            }
            continue;
        }
        //	value
        size_t	pos	= line.find('=');
        if(std::string::npos != pos) {
            node_value*		value;
            if(container->is_array) {
                if(!container->items.empty()) {
                    return	false;
                } else {
                    container->is_array	= false;
                }
            }
            if(!data.add_value(container_id, string_trim(line.substr(0, pos).c_str()), &value, 0)) {
                return	false;
            }
            value->value	= string_trim(line.substr(pos+1, line.length()-pos-1).c_str());
        } else {
            node_value*		value;
            if(!container->is_array) {
                if(!container->items.empty()) {
                    return	false;
                } else {
                    container->is_array	= true;
                }
            }
            if(!data.add_value(container_id, &value, 0)) {
                return	false;
            }
            value->value	= line;
        }
    }
    return	true;
}
