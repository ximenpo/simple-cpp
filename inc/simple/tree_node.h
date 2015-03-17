//
//   modified from third/TCNode.h
//
//   Implements a indexed multi-node data tree using STL containers
//
//   Copyright (C) 2013,  Ciro Sisman Pereira
// *-----------------------------------------------------------------------------------*
//
#ifndef TREE_NODE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define TREE_NODE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include <map>
#include <string>
#include <deque>
#include <algorithm>

#if	defined(_MSC_VER)
#	pragma warning( disable : 4311 )
#endif

using namespace std;

template <class _DATA, class _KEY, class _COMP = std::less<_KEY> >
class tree_node {

// ########################################################################
public:
// ########################################################################

    typedef _DATA													data_type;
    typedef _KEY													key_type;
    typedef	_COMP													compare_type;
    typedef tree_node<_DATA, _KEY, _COMP> *							node_ptr;
    typedef tree_node<_DATA, _KEY, _COMP> &							node_ref;
    typedef std::map<_KEY, tree_node<_DATA, _KEY, _COMP>, _COMP>	subnodes_type;
    typedef typename subnodes_type::iterator						subnodes_iterator;
    typedef std::map<std::string, std::deque<key_type> >			shortcuts_type;
    typedef typename shortcuts_type::iterator						shortcuts_iterator;

// ########################################################################
private:
// ########################################################################

    typedef bool (*_GenericCall)(node_ptr, node_ptr);

// ########################################################################
public:
// ########################################################################

    typedef _GenericCall SortPredCall;

// ########################################################################
private:
// ########################################################################

    template<class _SORTER>
    struct node_data_sorter {
        _SORTER sortpred;
        bool recursive;
        std::deque<node_ptr> refs;

        node_data_sorter() : recursive(false) {
            refs.clear();
        }
        ~node_data_sorter() {
            refs.clear();
        }

        node_data_sorter &operator=(node_data_sorter &_obj) { // just in case
            sortpred  = _obj.sortpred;
            recursive = _obj.recursive;
            refs      = _obj.refs;
            return *this;
        }
    };

// ########################################################################
private:
// ########################################################################

    long            m_id;
    key_type		m_key;
    data_type		m_data;
    subnodes_type	m_subnodes;
    node_ptr		m_parent;
    node_ptr		m_newnode;
    long            m_deep;

    std::map<std::string, node_data_sorter<_GenericCall> > m_sorters;
    typename std::map<std::string, node_data_sorter<_GenericCall> >::iterator m_ds_iter;
    std::deque<node_ptr> m_invalid_ds;
    shortcuts_type m_shortcuts;

// ########################################################################
private:
// ########################################################################

// *---------------------------------------------------------------*
    void init(void)
// *---------------------------------------------------------------*
    {
        this->m_parent = (node_ptr)NULL;
        this->m_id = (long)this;
        this->m_subnodes.clear();
        this->m_deep = 0;
        this->m_sorters.clear();
        this->m_invalid_ds.clear();
        this->m_newnode = NULL;
        this->m_shortcuts.clear();
    }

// *---------------------------------------------------------------*
    void release(void)
// *---------------------------------------------------------------*
    {
        this->m_subnodes.clear();
        this->m_sorters.clear();
        this->m_shortcuts.clear();
    }

// *---------------------------------------------------------------*
    void load_data_refs(typename std::map<std::string, node_data_sorter<_GenericCall> >::iterator _iter)
// *---------------------------------------------------------------*
    {
        subnodes_type &children1 = this->m_subnodes;
        for ( subnodes_iterator iter1 = children1.begin(); iter1 != children1.end(); iter1++ )
            _iter->second.refs.push_back(&iter1->second);

        if ( _iter->second.recursive ) {
            subnodes_type &children2 = this->m_subnodes;
            for ( subnodes_iterator iter2 = children2.begin(); iter2 != children2.end(); iter2++ )
                iter2->second.load_data_refs(_iter);
        }
    }

// *---------------------------------------------------------------*
    long get_count(long _ref)
// *---------------------------------------------------------------*
    {
        long count = _ref;

        subnodes_type &children = this->m_subnodes;
        for ( subnodes_iterator iter = children.begin(); iter != children.end(); iter++ )
            count += iter->second.get_count(1);

        return count;
    }

// *---------------------------------------------------------------*
    std::deque<node_ptr> &invalidate_data_sorter(void)
// *---------------------------------------------------------------*
    {
        this->m_invalid_ds.clear();
        return this->m_invalid_ds;
    }


// ########################################################################
protected:
// ########################################################################

// *---------------------------------------------------------------*
    void set_parent(node_ptr _obj)
// *---------------------------------------------------------------*
    {
        this->m_parent = _obj;
    }

// *---------------------------------------------------------------*
    void set_deep(long _deep)
// *---------------------------------------------------------------*
    {
        this->m_deep = _deep;
    }

// *---------------------------------------------------------------*
    node_ptr get_root_ref(void)
// *---------------------------------------------------------------*
    {
        return this->m_parent;
    }

// ########################################################################
public:
// ########################################################################

// *---------------------------------------------------------------*
    tree_node()
// *---------------------------------------------------------------*
    {
        this->init();
    }

// *---------------------------------------------------------------*
    tree_node(const tree_node &_obj)
// *---------------------------------------------------------------*
    {
        this->operator=(_obj);
    }

// *---------------------------------------------------------------*
    ~tree_node()
// *---------------------------------------------------------------*
    {
        this->release();
    }

// *---------------------------------------------------------------*
    bool is_root(void)
// *---------------------------------------------------------------*
    {
        return (this->m_parent == NULL);
    }

// *---------------------------------------------------------------*
    subnodes_type &get_subnodes(void)
// *---------------------------------------------------------------*
    {
        return this->m_subnodes;
    }

// *---------------------------------------------------------------*
    node_ref get_parent(void)
// *---------------------------------------------------------------*
    {
        if ( this->m_parent == NULL )
            return *this;
        return *this->m_parent;
    }

// *---------------------------------------------------------------*
    node_ref get_root(void)
// *---------------------------------------------------------------*
    {
        if ( this->m_parent == NULL )
            return *this;

        node_ptr parent = this->m_parent->get_root_ref();
        node_ptr last   = this->m_parent;

        while ( parent ) {
            last   = parent;
            parent = parent->get_root_ref();
        }

        return *last;
    }

// *---------------------------------------------------------------*
    bool has_subnodes(void)
// *---------------------------------------------------------------*
    {
        return this->m_subnodes.size() ? true : false;
    }

// *---------------------------------------------------------------*
    data_type &get_data(void)
// *---------------------------------------------------------------*
    {
        return this->m_data;
    }

// *---------------------------------------------------------------*
    key_type &get_key(void)
// *---------------------------------------------------------------*
    {
        return this->m_key;
    }

// *---------------------------------------------------------------*
    node_ref set_data( data_type _data)
// *---------------------------------------------------------------*
    {
        this->m_data = _data;
        return *this;
    }

// *---------------------------------------------------------------*
    node_ref set_key( key_type _key)
// *---------------------------------------------------------------*
    {
        this->m_key = _key;
        return *this;
    }

// *---------------------------------------------------------------*
    node_ref set_data_and_key( data_type _data,  key_type _key)
// *---------------------------------------------------------------*
    {
        this->m_data = _data;
        this->m_key  = _key;
        return *this;
    }

// *---------------------------------------------------------------*
    node_ref create_node( data_type _data,  key_type _key)
// *---------------------------------------------------------------*
    {
        subnodes_iterator it = this->m_subnodes.find(_key);
        if ( it != this->m_subnodes.end() ) {
            it->second.set_data(_data);
            return this->m_subnodes[_key];
        }

        this->m_newnode = new tree_node<data_type, key_type, compare_type>();
        this->m_newnode->set_data_and_key(_data, _key);

        this->m_newnode->set_parent(this);
        this->m_newnode->set_deep(this->m_deep+1);
        this->m_subnodes[_key] = *this->m_newnode;

        if ( this->m_newnode )
            delete this->m_newnode;
        this->m_newnode = NULL;

        return this->m_subnodes[_key];
    }

// *---------------------------------------------------------------*
    long get_deep(void)
// *---------------------------------------------------------------*
    {
        return this->m_deep;
    }

// *---------------------------------------------------------------*
    long get_id(void)
// *---------------------------------------------------------------*
    {
        return this->m_id;
    }

// *---------------------------------------------------------------*
    bool operator==( node_ref _obj)
// *---------------------------------------------------------------*
    {
        return this->m_id == _obj.get_id();
    }

// *---------------------------------------------------------------*
    bool operator!=( node_ref _obj)
// *---------------------------------------------------------------*
    {
        return this->m_id != _obj.get_id();
    }

// *---------------------------------------------------------------*
    template<class _RECV>
    void accept( _RECV _receiver)
// *---------------------------------------------------------------*
    {
        _receiver(this->m_data, this->m_key, this->m_deep);
        if ( !this->has_subnodes() )
            return;

        subnodes_type &children = this->m_subnodes;
        for ( subnodes_iterator iter = children.begin(); iter != children.end(); iter++ )
            iter->second.accept(_receiver);
    }

// *---------------------------------------------------------------*
    tree_node &operator=( const tree_node &_obj)
// *---------------------------------------------------------------*
    {
        this->init();
        this->m_id        = const_cast<tree_node &>(_obj).get_id();
        this->m_key       = const_cast<tree_node &>(_obj).get_key();
        this->m_data      = const_cast<tree_node &>(_obj).get_data();
        this->m_subnodes  = const_cast<tree_node &>(_obj).get_subnodes();
        this->m_deep      = const_cast<tree_node &>(_obj).get_deep();
        this->m_parent    = &(const_cast<tree_node &>(_obj).get_parent());
        this->m_shortcuts = const_cast<tree_node &>(_obj).get_shortcuts();

        std::string name = "";
        _GenericCall sortfunc = NULL;
        bool recursive = false;
        bool is_valid = false;
        std::deque<node_ptr> refs = const_cast<tree_node &>(_obj).get_next_data_sorter_info(true, name, recursive, sortfunc, is_valid);

        while ( is_valid ) {
            this->add_data_sorter(name, sortfunc, recursive);
            refs = const_cast<tree_node &>(_obj).get_next_data_sorter_info(false, name, recursive, sortfunc, is_valid);
        }

        return *this;
    }

// *---------------------------------------------------------------*
    bool has_subnode( key_type _key)
// *---------------------------------------------------------------*
    {
        if ( !this->has_subnodes() )
            return false;

        if ( this->m_subnodes.find(_key) != this->m_subnodes.end() )
            return true;

        subnodes_type &children = this->m_subnodes;
        for ( subnodes_iterator iter = children.begin(); iter != children.end(); iter++ )
            if ( iter->second.has_subnode(_key) )
                return true;

        return false;
    }

// *---------------------------------------------------------------*
    node_ptr get_node_by_key( key_type _key)
// *---------------------------------------------------------------*
    {
        subnodes_iterator it = this->m_subnodes.find(_key);
        if ( it != this->m_subnodes.end() )
            return &this->m_subnodes[_key];
        return node_ptr(NULL);
    }

// *---------------------------------------------------------------*
    node_ptr get_first_subnode_by_key( key_type _key)
// *---------------------------------------------------------------*
    {
        subnodes_iterator it = this->m_subnodes.find(_key);
        if ( it != this->m_subnodes.end() )
            return &this->m_subnodes[_key];

        subnodes_type &children = this->m_subnodes;
        for ( subnodes_iterator iter = children.begin(); iter != children.end(); iter++ ) {
            node_ptr _ptr = iter->second.get_first_subnode_by_key(_key);
            if ( _ptr )
                return _ptr;
        }

        return node_ptr(NULL);
    }

// *---------------------------------------------------------------*
    node_ptr get_node_by_full_address( std::deque<key_type> &_parm)
// *---------------------------------------------------------------*
    {
        node_ptr retnode = NULL;

        if ( _parm.size() == 0 )
            return node_ptr(NULL);

        if ( this->m_parent == NULL )
            retnode = this;
        else
            retnode = &this->get_root();

        if ( retnode->get_key() != _parm[0] )
            return node_ptr(NULL);

        for ( size_t i = 1; i < _parm.size(); i++ ) {
            if ( !retnode->has_subnodes() )
                return node_ptr(NULL);

            node_ptr anode = retnode->get_node_by_key(_parm[i]);
            if ( anode == NULL )
                return node_ptr(NULL);

            retnode = anode;
        }

        return retnode;
    }

// *---------------------------------------------------------------*
    std::deque<key_type> &get_node_full_address( std::deque<key_type> &_parm)
// *---------------------------------------------------------------*
    {
        _parm.clear();

        _parm.push_back(this->m_key);

        if ( this->m_parent == NULL )
            return _parm;

        _parm.push_back(this->m_parent->get_key());

        node_ptr parent = this->m_parent->get_root_ref();
        while ( parent ) {
            _parm.push_back(parent->get_key());
            parent = parent->get_root_ref();
        }

        std::reverse(_parm.begin(), _parm.end());

        return _parm;
    }

// *---------------------------------------------------------------*
    std::deque<node_ptr> &get_all_subnodes_by_key( std::deque<node_ptr> &_parm,  key_type _key)
// *---------------------------------------------------------------*
    {
        if ( !this->has_subnodes() )
            return _parm;

        subnodes_iterator it = this->m_subnodes.find(_key);
        if ( it != this->m_subnodes.end() )
            _parm.push_back(&this->m_subnodes[_key]);

        subnodes_type &children = this->m_subnodes;
        for ( subnodes_iterator iter = children.begin(); iter != children.end(); iter++ )
            iter->second.get_all_subnodes_by_key(_parm, _key);

        return _parm;
    }

// *---------------------------------------------------------------*
    template<class _SORTER>
    void add_data_sorter( std::string _name,  _SORTER _receiver,  bool _recursive = false)
// *---------------------------------------------------------------*
    {
        struct node_data_sorter<_SORTER> fsort;
        fsort.sortpred = _receiver;
        fsort.recursive = _recursive;
        fsort.refs.clear();
        this->m_sorters[_name] = fsort;
    }

// *---------------------------------------------------------------*
    void refresh_data_sorters(void)
// *---------------------------------------------------------------*
    {
        if ( !this->has_subnodes() )
            return;

        for ( typename std::map<std::string, node_data_sorter<_GenericCall> >::iterator iter = this->m_sorters.begin(); iter != this->m_sorters.end(); iter++ ) {
            iter->second.refs.clear();
            this->load_data_refs(iter);
            std::sort(iter->second.refs.begin(), iter->second.refs.end(), iter->second.sortpred);
        }
    }

// *---------------------------------------------------------------*
    std::deque<node_ptr> &get_data_sorter_by_name( std::string _name,  bool &_is_valid)
// *---------------------------------------------------------------*
    {
        typename std::map<std::string, node_data_sorter<_GenericCall> >::iterator iter = this->m_sorters.find(_name);
        if ( iter == this->m_sorters.end() ) {
            _is_valid = false;
            return this->invalidate_data_sorter();
        }
        _is_valid = true;
        return (this->m_sorters[_name].refs);
    }

// *---------------------------------------------------------------*
    template<class _SORTER>
    std::deque<node_ptr> &get_next_data_sorter_info( bool _begin,  std::string &_name,  bool &_recursive,  _SORTER &_sortfunc,  bool &_is_valid)
// *---------------------------------------------------------------*
    {
        this->m_ds_iter = _begin ? this->m_sorters.begin() : ++this->m_ds_iter;

        if ( this->m_ds_iter == this->m_sorters.end() ) {
            _is_valid = false;
            _name  = "";
            _recursive = false;
            _sortfunc = NULL;
            return this->invalidate_data_sorter();
        }
        _is_valid = true;
        _name = this->m_ds_iter->first;
        _recursive = this->m_ds_iter->second.recursive;
        _sortfunc = this->m_ds_iter->second.sortpred;
        return (this->m_sorters[_name].refs);
    }

// *---------------------------------------------------------------*
    std::deque<node_ptr> &select_data_equalsto( std::string _name,  std::deque<node_ptr> &_parm,  const data_type _value)
// *---------------------------------------------------------------*
    {
        _parm.clear();

        typename std::map<std::string, node_data_sorter<_GenericCall> >::iterator iter = this->m_sorters.find(_name);
        if ( iter == this->m_sorters.end() )
            return this->invalidate_data_sorter();

        typename std::deque<node_ptr>::iterator low, up;

        tree_node<_DATA, _KEY, _COMP> val;
        val.setData(_value);

        low = std::lower_bound(iter->second.refs.begin(), iter->second.refs.end(), node_ptr(&val), iter->second.sortpred);
        up  = std::upper_bound(iter->second.refs.begin(), iter->second.refs.end(), node_ptr(&val), iter->second.sortpred);

        if ( !(low == up) ) {
            long sz = up - low;
            _parm.resize(sz);
            std::copy(low, up, _parm.begin());
        }

        return _parm;
    }

// *---------------------------------------------------------------*
    std::deque<node_ptr> &select_data_equalsto( std::string _name,  std::deque<node_ptr> &_parm,  const std::deque<data_type> &_vals)
// *---------------------------------------------------------------*
    {
        _parm.clear();

        typename std::map<std::string, node_data_sorter<_GenericCall> >::iterator iter = this->m_sorters.find(_name);
        if ( iter == this->m_sorters.end() )
            return this->invalidate_data_sorter();

        if ( _vals.size() == 0 )
            return _parm;

        for ( size_t ii = 0; ii < _vals.size(); ii++ ) {
            typename std::deque<node_ptr>::iterator low, up;

            tree_node<_DATA, _KEY, _COMP> val;
            val.setData(_vals[ii]);

            low = std::lower_bound(iter->second.refs.begin(), iter->second.refs.end(), node_ptr(&val), iter->second.sortpred);
            up  = std::upper_bound(iter->second.refs.begin(), iter->second.refs.end(), node_ptr(&val), iter->second.sortpred);

            if ( !(low == up) ) {
                size_t sz = up - low;
                long prevsize = (long)_parm.size();
                _parm.resize(prevsize ? prevsize+sz : sz);
                std::copy(low, up, prevsize ? _parm.begin()+prevsize : _parm.begin());
            }
        }

        std::sort(_parm.begin(), _parm.end(), iter->second.sortpred);

        return _parm;
    }

// *---------------------------------------------------------------*
    long size(void)
// *---------------------------------------------------------------*
    {
        return this->get_count(1);
    }

// *---------------------------------------------------------------*
    bool remove_subnode_by_key( key_type _key)
// *---------------------------------------------------------------*
    {
        subnodes_iterator it = this->m_subnodes.find(_key);
        if ( it != this->m_subnodes.end() ) {
            it->second.remove_subnodes();
            this->m_subnodes.erase(it);
            return true;
        }

        return false;
    }

// *---------------------------------------------------------------*
    node_ref remove_subnodes(void)
// *---------------------------------------------------------------*
    {
        if ( !this->has_subnodes() )
            return *this;

        subnodes_type &children = this->m_subnodes;
        for ( subnodes_iterator iter = children.begin(); iter != children.end(); iter++ )
            iter->second.remove_subnodes();

        this->m_subnodes.clear();
        // developer must call refresh_data_sorters

        return *this;
    }

// *---------------------------------------------------------------*
    shortcuts_type &get_shortcuts(void)
// *---------------------------------------------------------------*
    {
        return this->is_root() ? this->m_shortcuts : this->get_root().get_shortcuts();
    }

// *---------------------------------------------------------------*
    node_ref set_shortcut( std::string _label)
// *---------------------------------------------------------------*
    {
        deque<key_type> keys;
        this->get_node_full_address(keys);
        this->get_root().add_shortcut(_label, keys);
        return *this;
    }

// *---------------------------------------------------------------*
    node_ref add_shortcut( std::string _label,  std::deque<key_type> &_parm)
// *---------------------------------------------------------------*
    {
        this->get_root().get_shortcuts()[_label] = _parm;
        return *this;
    }

// *---------------------------------------------------------------*
    node_ptr get_node_by_shortcut( std::string _parm)
// *---------------------------------------------------------------*
    {
        node_ptr retnode = NULL;

        if ( _parm.size() == 0 || this->get_root().get_shortcuts().size() == 0 )
            return node_ptr(NULL);

        shortcuts_iterator iter = this->get_root().get_shortcuts().find(_parm);
        if ( iter == this->get_root().get_shortcuts().end() )
            return node_ptr(NULL);

        return this->get_node_by_full_address(iter->second);
    }

};

#define TREE_NODE_SET_TYPE(_name, _tdata, _tkey)					\
   typedef tree_node<_tdata, _tkey, std::less<_tkey> >	_name;		\
   typedef _name&									_name##_ref;	\
   typedef _name*									_name##_ptr;	\
   typedef _name::subnodes_type&					_name##_nodes;	\
   typedef _name::subnodes_type::iterator			_name##_nodes_iterator;

#define TREE_NODE_PTR_TO_REF(_ptr)					*_ptr

#endif
