
#include	<cassert>
#include	<deque>

#include	"string.h"
#include	"stringify.h"

#include	"_third/json.h"

struct	stringify_data_json_visitor	:	public stringify_data_visitor {
    virtual		bool	visit_begin() {
        seqs_.clear();
        seqs_.push_back(0);
        return	os_.good();
    }
    virtual		bool	visit_value(const std::string& value, const std::string* name) {
        write_comma_();
        if(0 != name) {
            os_	<<	'"'	<<	(*name)	<<	"\":"	;
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
            os_	<<	'"'	<<	(*name)	<<	"\":"	;
        }
        os_	<<	'[';
        seqs_.push_back(0);
        return	os_.good();
    }
    virtual		bool	visit_array_end() {
        os_	<<	']';
        seqs_.pop_back();
        return	os_.good();
    }
    virtual		bool	visit_object_begin(const std::string* name) {
        write_comma_();
        if(0 != name) {
            os_	<<	'"'	<<	(*name)	<<	"\":"	;
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

    stringify_data_json_visitor(std::ostream& os) : os_(os) {
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

bool	stringify_to_json_stream(stringify_data& data, std::ostream& os) {
    stringify_data_json_visitor	visitor(os);
    return	data.accept(visitor);
}

static	bool	load_from_json_array(stringify_data& data, stringify::node_id node_id, const json::Array& val);
static	bool	load_from_json_object(stringify_data& data, stringify::node_id node_id, const json::Object& val);

bool	stringify_from_json_stream(stringify_data& data, std::istream& is) {
    data.clear();

    json::Value	val;
    {
        std::string doc;
        doc.reserve(1024*4);
        std::getline(is, doc, (char)-1);// -1 means EOF

        val	= json::Deserialize(doc);
        if(val.GetType() == json::NULLVal) {
            return	false;
        }
    }

    //
    //	construct a stringify_data from json::Value
    //

    //	¸ù½Úµã
    bool	simple_value	= true;
    std::ostringstream		os;
    switch(val.GetType()) {
    case	json::StringVal:
        os << val.ToString();
        break;
    case	json::IntVal:
        os << val.ToInt();
        break;
    case	json::FloatVal:
        os << val.ToFloat();
        break;
    case	json::DoubleVal:
        os << val.ToDouble();
        break;
    case	json::BoolVal:
        os << val.ToBool();
        break;
    case	json::ObjectVal:
        simple_value	= false;
        break;
    case	json::ArrayVal:
        simple_value	= false;
        break;
    default:
        break;
    }

    if(simple_value) {
        stringify::node_value*	value	= 0;
        data.add_root(&value, 0);
        value->value	= os.str();
        return	true;
    }

    stringify::node_container*	container	= 0;
    stringify::node_id			container_id= 0;
    switch(val.GetType()) {
    case	json::ArrayVal:
        data.add_root(&container, &container_id);
        container->is_array	= true;
        return	load_from_json_array(data, container_id, val.ToArray());
    case	json::ObjectVal:
        data.add_root(&container, &container_id);
        container->is_array	= false;
        return	load_from_json_object(data, container_id, val.ToObject());
    default:
        break;
    }

    return	false;
}

static	bool	load_from_json_array(stringify_data& data, stringify::node_id node_id, const json::Array& val) {
    std::ostringstream		os;
    stringify::node_container*	container	= 0;
    stringify::node_id			container_id= 0;

    json::Array::ValueVector::const_iterator	it		= val.begin();
    json::Array::ValueVector::const_iterator	it_end	= val.end();
    for(; it != it_end; ++it) {
        os.str("");
        bool	is_value	= true;
        switch(it->GetType()) {
        case json::ObjectVal:
            is_value	= false;
            if(!data.add_container(node_id, &container, &container_id)) {
                return	false;
            }
            container->is_array	= false;
            if(!load_from_json_object(data, container_id, it->ToObject())) {
                return	false;
            }
            break;
        case json::ArrayVal:
            is_value	= false;
            if(!data.add_container(node_id, &container, &container_id)) {
                return	false;
            }
            container->is_array	= true;
            if(!load_from_json_array(data, container_id, it->ToArray())) {
                return	false;
            }
            break;
        case json::StringVal:
            os	<<	it->ToString();
            break;
        case json::IntVal:
            os	<<	it->ToInt();
            break;
        case json::FloatVal:
            os	<<	it->ToFloat();
            break;
        case json::DoubleVal:
            os	<<	it->ToDouble();
            break;
        case json::BoolVal:
            os	<<	it->ToBool();
            break;
        default:
            is_value	= false;
            break;
        }

        if(is_value) {
            stringify::node_value*	value	= 0;
            data.add_value(node_id, &value, 0);
            value->value	= os.str();
        }
    }

    return	true;
}

static	bool	load_from_json_object(stringify_data& data, stringify::node_id node_id, const json::Object& val) {
    std::ostringstream		os;
    stringify::node_container*	container	= 0;
    stringify::node_id			container_id= 0;

    json::Object::ValueMap::const_iterator	it		= val.begin();
    json::Object::ValueMap::const_iterator	it_end	= val.end();
    for(; it != it_end; ++it) {
        os.str("");
        bool	is_value	= true;
        switch(it->second.GetType()) {
        case json::ObjectVal:
            is_value	= false;
            if(!data.add_container(node_id, it->first, &container, &container_id)) {
                return	false;
            }
            container->is_array	= false;
            if(!load_from_json_object(data, container_id, it->second.ToObject())) {
                return	false;
            }
            break;
        case json::ArrayVal:
            is_value	= false;
            if(!data.add_container(node_id, it->first, &container, &container_id)) {
                return	false;
            }
            container->is_array	= true;
            if(!load_from_json_array(data, container_id, it->second.ToArray())) {
                return	false;
            }
            break;
        case json::StringVal:
            os	<<	it->second.ToString();
            break;
        case json::IntVal:
            os	<<	it->second.ToInt();
            break;
        case json::FloatVal:
            os	<<	it->second.ToFloat();
            break;
        case json::DoubleVal:
            os	<<	it->second.ToDouble();
            break;
        case json::BoolVal:
            os	<<	it->second.ToBool();
            break;
        default:
            is_value	= false;
            break;
        }

        if(is_value) {
            stringify::node_value*	value	= 0;
            data.add_value(node_id, it->first, &value, 0);
            value->value	= os.str();
        }
    }

    return	true;
}
