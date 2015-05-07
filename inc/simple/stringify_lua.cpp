
#include	<cassert>
#include	<deque>

#include	"simple/string.h"
#include	"simple/stringify.h"

struct	stringify_data_lua_visitor	:	public stringify_data_visitor {
    virtual		bool	visit_begin() {
        seqs_.clear();
        seqs_.push_back(0);
        return	os_.good();
    }
    virtual		bool	visit_value(const std::string& value, const std::string* name) {
        write_comma_();
        if(0 != name) {
            os_	<<	(*name)	<<	'='	;
        }
        if(string_isnumber(value)) {
            os_<<	value;
        } else {
            os_<<	'"'	<<	string_json_escape(value)	<<	'"';
        }
        return	os_.good();
    }
    virtual		bool	visit_array_begin(const std::string* name) {
        write_comma_();
        if(0 != name) {
            os_	<<	(*name)	<<	'='	;
        }
        os_	<<	'{';
        seqs_.push_back(0);
        return	os_.good();
    }
    virtual		bool	visit_array_end() {
        os_	<<	'}';
        seqs_.pop_back();
        return	os_.good();
    }
    virtual		bool	visit_object_begin(const std::string* name) {
        write_comma_();
        if(0 != name) {
            os_	<<	(*name)	<<	'='	;
        }
        os_	<<	'{';
        seqs_.push_back(0);
        return	os_.good();
    }
    virtual		bool	visit_object_end() {
        os_	<<	'}';
        seqs_.pop_back();
        return	os_.good();
    }
    virtual		bool	visit_end() {
        assert(!seqs_.empty());
        seqs_.pop_back();
        assert(seqs_.empty());
        return	os_.good();
    }

    stringify_data_lua_visitor(std::ostream& os) : os_(os) {
    }

    void	write_comma_() {
        assert(!seqs_.empty());
        if(seqs_.back() > 0) {
            os_	<<	',';
        }
        seqs_.back()++;
    }

    std::ostream&		os_;
    std::deque<size_t>	seqs_;
};

bool	stringify_to_lua_stream(stringify_data& data, std::ostream& os) {
    stringify_data_lua_visitor	visitor(os);
    return	data.accept(visitor);
}
