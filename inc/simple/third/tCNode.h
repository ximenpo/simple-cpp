//   TCNode.h 
//
//   Implements a indexed multi-node data tree using STL containers 
//
//   Copyright (C) 2013,  Ciro Sisman Pereira
// *-----------------------------------------------------------------------------------*
//
#ifndef _TCNODE_INCLUDED_

#define _TCNODE_INCLUDED_

#include <iostream>
#include <exception>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#ifndef _WIN32
typedef unsigned long BOOL;
#define FALSE 0
#define TRUE  1
#else
#pragma warning( disable : 4311 )
#endif

#define _IN
#define _OUT

using namespace std;
 
template <class _DATA, class _KEY, class _COMP = less<_KEY> > class tCNode 
{

// ########################################################################
   public:
// ########################################################################

      typedef _DATA tcnode_data;
      typedef _KEY  tcnode_key;
      typedef tCNode<_DATA, _KEY, _COMP> *tcnode_ptr;
      typedef tCNode<_DATA, _KEY, _COMP> &tcnode_ref;
      typedef std::map<_KEY, tCNode<_DATA, _KEY, _COMP>, _COMP> tcnode_subnodes;
      typedef typename std::map<_KEY, tCNode<_DATA, _KEY, _COMP>, _COMP>::iterator tcnode_iter;
      typedef std::map<std::string, std::vector<tcnode_key> > tcnode_shortcuts;
      typedef typename std::map<std::string, std::vector<tcnode_key> >::iterator tcnode_shortcuts_iter;     

// ########################################################################
   private:
// ########################################################################

      typedef bool (*_GenericCall)(tcnode_ptr, tcnode_ptr);

// ########################################################################
   public:
// ########################################################################

      typedef _GenericCall SortPredCall;

// ########################################################################
   private:
// ########################################################################

      template<class _SORTER>
      struct TNodeDataSorter
      {
            _SORTER sortpred;
            BOOL recursive;
            std::vector<tcnode_ptr> refs;

            TNodeDataSorter() : recursive(FALSE) { refs.clear(); } 
           ~TNodeDataSorter() { refs.clear(); }

           TNodeDataSorter &operator=(TNodeDataSorter &_obj) // just in case
           {
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
      tcnode_key      m_key;
      tcnode_data     m_data;
      tcnode_subnodes m_subnodes;
      tcnode_ptr      m_parent;
      tcnode_ptr      m_newnode;
      long            m_deep;

      std::map<std::string, TNodeDataSorter<_GenericCall> > m_sorters;
      typename std::map<std::string, TNodeDataSorter<_GenericCall> >::iterator m_ds_iter;
      std::vector<tcnode_ptr> m_invalid_ds;
      tcnode_shortcuts m_shortcuts;

// ########################################################################
   private:
// ########################################################################

// *---------------------------------------------------------------*
      void init(void)
// *---------------------------------------------------------------*
      {
         this->m_parent = (tcnode_ptr)NULL;      
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
      void load_data_refs(typename std::map<std::string, TNodeDataSorter<_GenericCall> >::iterator _iter)
// *---------------------------------------------------------------*
      {
         tcnode_subnodes &children1 = this->m_subnodes;
         for ( tcnode_iter iter1 = children1.begin(); iter1 != children1.end(); iter1++ )
            _iter->second.refs.push_back(&iter1->second);

         if ( _iter->second.recursive )
         {
            tcnode_subnodes &children2 = this->m_subnodes;
            for ( tcnode_iter iter2 = children2.begin(); iter2 != children2.end(); iter2++ )
               iter2->second.load_data_refs(_iter);
         }
      }

// *---------------------------------------------------------------*
      long get_count(long _ref)
// *---------------------------------------------------------------*
      {
         long count = _ref;

         tcnode_subnodes &children = this->m_subnodes;
         for ( tcnode_iter iter = children.begin(); iter != children.end(); iter++ )
            count += iter->second.get_count(1);

         return count;
      }

// *---------------------------------------------------------------*
      std::vector<tcnode_ptr> &invalidate_data_sorter(void)
// *---------------------------------------------------------------*
      {
         this->m_invalid_ds.clear();
         return this->m_invalid_ds;
      }


// ########################################################################
   protected:
// ########################################################################

// *---------------------------------------------------------------*
      void set_parent(tcnode_ptr _obj)
// *---------------------------------------------------------------*
      { this->m_parent = _obj; }

// *---------------------------------------------------------------*
      void set_deep(long _deep)
// *---------------------------------------------------------------*
      { this->m_deep = _deep; }

// *---------------------------------------------------------------*
      tcnode_ptr get_root_ref(void)
// *---------------------------------------------------------------*
      { return this->m_parent; }

// ########################################################################
   public:  
// ########################################################################

// *---------------------------------------------------------------*
      tCNode() 
// *---------------------------------------------------------------*
      { this->init(); }

// *---------------------------------------------------------------*
      tCNode(const tCNode &_obj)
// *---------------------------------------------------------------*
      { this->operator=(_obj); }

// *---------------------------------------------------------------*
     ~tCNode() 
// *---------------------------------------------------------------*
      { this->release(); }

// *---------------------------------------------------------------*
      bool isRoot(void)
// *---------------------------------------------------------------*
      { return (this->m_parent == NULL); }

// *---------------------------------------------------------------*
      tcnode_subnodes &getSubNodes(void)
// *---------------------------------------------------------------*
      { return this->m_subnodes; }

// *---------------------------------------------------------------*
      tcnode_ref getParent(void)
// *---------------------------------------------------------------*
      { 
         if ( this->m_parent == NULL )
            return *this;
         return *this->m_parent; 
      }

// *---------------------------------------------------------------*
      tcnode_ref getRoot(void)
// *---------------------------------------------------------------*
      {
         if ( this->m_parent == NULL )
            return *this;

         tcnode_ptr parent = this->m_parent->get_root_ref();
         tcnode_ptr last   = this->m_parent;

         while ( parent ) 
         {
            last   = parent;
            parent = parent->get_root_ref();
         }

         return *last;
      }

// *---------------------------------------------------------------*
      BOOL hasSubNodes(void)
// *---------------------------------------------------------------*
      { return this->m_subnodes.size() ? TRUE : FALSE; }

// *---------------------------------------------------------------*
      tcnode_data &getData(void)
// *---------------------------------------------------------------*
      { return this->m_data; }

// *---------------------------------------------------------------*
      tcnode_key &getKey(void)
// *---------------------------------------------------------------*
      { return this->m_key; }

// *---------------------------------------------------------------*
      tcnode_ref setData(_IN tcnode_data _data)
// *---------------------------------------------------------------*
      {
         this->m_data = _data;
         return *this;
      }

// *---------------------------------------------------------------*
      tcnode_ref setKey(_IN tcnode_key _key)
// *---------------------------------------------------------------*
      {
         this->m_key = _key;
         return *this;
      }

// *---------------------------------------------------------------*
      tcnode_ref setDataAndKey(_IN tcnode_data _data, _IN tcnode_key _key)
// *---------------------------------------------------------------*
      {
         this->m_data = _data;
         this->m_key  = _key;
         return *this;
      }

// *---------------------------------------------------------------*
      tcnode_ref createNode(_IN tcnode_data _data, _IN tcnode_key _key)
// *---------------------------------------------------------------*
      {
         tcnode_iter it = this->m_subnodes.find(_key);
         if ( it != this->m_subnodes.end() )
         {
            it->second.setData(_data);
            return this->m_subnodes[_key];
         }

         this->m_newnode = new tCNode<tcnode_data, tcnode_key>();
         this->m_newnode->setDataAndKey(_data, _key);
         
         this->m_newnode->set_parent(this);
         this->m_newnode->set_deep(this->m_deep+1);
         this->m_subnodes[_key] = *this->m_newnode;

         if ( this->m_newnode )
            delete this->m_newnode;
         this->m_newnode = NULL;

         return this->m_subnodes[_key];
      }

// *---------------------------------------------------------------*
      long getDeep(void)
// *---------------------------------------------------------------*
      { return this->m_deep; }

// *---------------------------------------------------------------*
      long getId(void)
// *---------------------------------------------------------------*
      { return this->m_id; }

// *---------------------------------------------------------------*
      bool operator==(_IN tcnode_ref _obj)
// *---------------------------------------------------------------*
      { return this->m_id == _obj.getId(); }

// *---------------------------------------------------------------*
      bool operator!=(_IN tcnode_ref _obj)
// *---------------------------------------------------------------*
      { return this->m_id != _obj.getId(); }

// *---------------------------------------------------------------*
      template<class _RECV>
      void transverse(_IN _RECV _receiver)
// *---------------------------------------------------------------*
      {
         _receiver(this->m_data, this->m_key, this->m_deep);
         if ( !this->hasSubNodes() ) 
            return;

         tcnode_subnodes &children = this->m_subnodes;
         for ( tcnode_iter iter = children.begin(); iter != children.end(); iter++ )
               iter->second.transverse(_receiver);
      }

// *---------------------------------------------------------------*
      tCNode &operator=(_IN const tCNode &_obj)
// *---------------------------------------------------------------*
      {
         this->init();
         this->m_id        = const_cast<tCNode &>(_obj).getId();
         this->m_key       = const_cast<tCNode &>(_obj).getKey();
         this->m_data      = const_cast<tCNode &>(_obj).getData();
         this->m_subnodes  = const_cast<tCNode &>(_obj).getSubNodes();
         this->m_deep      = const_cast<tCNode &>(_obj).getDeep();
         this->m_parent    = &(const_cast<tCNode &>(_obj).getParent());
         this->m_shortcuts = const_cast<tCNode &>(_obj).getShortcuts();

         std::string name = "";
         _GenericCall sortfunc = NULL;
         BOOL recursive = FALSE;
         BOOL is_valid = FALSE;
         std::vector<tcnode_ptr> refs = const_cast<tCNode &>(_obj).getNextDataSorterInfo(TRUE, name, recursive, sortfunc, is_valid);

         while ( is_valid )
         {
            this->addDataSorter(name, sortfunc, recursive);
            refs = const_cast<tCNode &>(_obj).getNextDataSorterInfo(FALSE, name, recursive, sortfunc, is_valid);
         }

         return *this;
      }

// *---------------------------------------------------------------*
      BOOL subNodeExists(_IN tcnode_key _key)
// *---------------------------------------------------------------*
      {
         if ( !this->hasSubNodes() ) 
            return FALSE;

         if ( this->m_subnodes.find(_key) != this->m_subnodes.end() )
            return TRUE;

         tcnode_subnodes &children = this->m_subnodes;
         for ( tcnode_iter iter = children.begin(); iter != children.end(); iter++ )
               if ( iter->second.subNodeExists(_key) )
                  return TRUE;

         return FALSE;
      }

// *---------------------------------------------------------------*
      tcnode_ptr getNodeByKey(_IN tcnode_key _key) 
// *---------------------------------------------------------------*
      {
         tcnode_iter it = this->m_subnodes.find(_key);
         if ( it != this->m_subnodes.end() )
            return &this->m_subnodes[_key];
         return tcnode_ptr(NULL);
      }

// *---------------------------------------------------------------*
      tcnode_ptr getFirstSubNodeByKey(_IN tcnode_key _key) 
// *---------------------------------------------------------------*
      {
         tcnode_iter it = this->m_subnodes.find(_key);
         if ( it != this->m_subnodes.end() )
            return &this->m_subnodes[_key];

         tcnode_subnodes &children = this->m_subnodes;
         for ( tcnode_iter iter = children.begin(); iter != children.end(); iter++ )
         {
               tcnode_ptr _ptr = iter->second.getFirstSubNodeByKey(_key);
               if ( _ptr )
                  return _ptr;
         }

         return tcnode_ptr(NULL);
      }

// *---------------------------------------------------------------*
      tcnode_ptr getNodeByFullAddress(_IN std::vector<tcnode_key> &_parm)
// *---------------------------------------------------------------*
      {
         tcnode_ptr retnode = NULL;

         if ( _parm.size() == 0 )
            return tcnode_ptr(NULL);

         if ( this->m_parent == NULL )  
            retnode = this;
         else
            retnode = &this->getRoot();

         if ( retnode->getKey() != _parm[0] )
            return tcnode_ptr(NULL);

         for ( size_t i = 1; i < _parm.size(); i++ )
         {
            if ( !retnode->hasSubNodes() )
           		return tcnode_ptr(NULL);

            tcnode_ptr anode = retnode->getNodeByKey(_parm[i]);
            if ( anode == NULL ) 
           		return tcnode_ptr(NULL);
 
            retnode = anode;
         }

         return retnode;
      }

// *---------------------------------------------------------------*
      std::vector<tcnode_key> &getNodeFullAddress(_OUT std::vector<tcnode_key> &_parm)
// *---------------------------------------------------------------*
      {
         _parm.clear();

         _parm.push_back(this->m_key);

         if ( this->m_parent == NULL )
            return _parm;
         
         _parm.push_back(this->m_parent->getKey());

         tcnode_ptr parent = this->m_parent->get_root_ref();
         while ( parent ) 
         {
            _parm.push_back(parent->getKey());
            parent = parent->get_root_ref();
         }

         std::reverse(_parm.begin(), _parm.end());

         return _parm;
      }
      
// *---------------------------------------------------------------*
      std::vector<tcnode_ptr> &getAllSubNodesByKey(_OUT std::vector<tcnode_ptr> &_parm, _IN tcnode_key _key)
// *---------------------------------------------------------------*
      {
         if ( !this->hasSubNodes() ) 
            return _parm;

         tcnode_iter it = this->m_subnodes.find(_key);
         if ( it != this->m_subnodes.end() )
            _parm.push_back(&this->m_subnodes[_key]);

         tcnode_subnodes &children = this->m_subnodes;
         for ( tcnode_iter iter = children.begin(); iter != children.end(); iter++ )
                iter->second.getAllSubNodesByKey(_parm, _key);

         return _parm;
      }

// *---------------------------------------------------------------*
      template<class _SORTER>
      void addDataSorter(_IN std::string _name, _IN _SORTER _receiver, _IN BOOL _recursive = FALSE)
// *---------------------------------------------------------------*
      {
         struct TNodeDataSorter<_SORTER> fsort;
         fsort.sortpred = _receiver;
         fsort.recursive = _recursive;
         fsort.refs.clear();
         this->m_sorters[_name] = fsort;
      }
      
// *---------------------------------------------------------------*
      void refreshDataSorters(void)
// *---------------------------------------------------------------*
      {
         if ( !this->hasSubNodes() )
            return;

         for ( typename std::map<std::string, TNodeDataSorter<_GenericCall> >::iterator iter = this->m_sorters.begin(); iter != this->m_sorters.end(); iter++ )
         {
            iter->second.refs.clear();
            this->load_data_refs(iter);
            std::sort(iter->second.refs.begin(), iter->second.refs.end(), iter->second.sortpred); 
         }
      }

// *---------------------------------------------------------------*
      std::vector<tcnode_ptr> &getDataSorterByName(_IN std::string _name, _OUT BOOL &_is_valid)
// *---------------------------------------------------------------*
      {
         typename std::map<std::string, TNodeDataSorter<_GenericCall> >::iterator iter = this->m_sorters.find(_name);
         if ( iter == this->m_sorters.end() )
         {
            _is_valid = FALSE;
            return this->invalidate_data_sorter();
         }
         _is_valid = TRUE;
         return (this->m_sorters[_name].refs);
      }

// *---------------------------------------------------------------*
      template<class _SORTER>
      std::vector<tcnode_ptr> &getNextDataSorterInfo(_IN BOOL _begin, _OUT std::string &_name, _OUT BOOL &_recursive, _OUT _SORTER &_sortfunc, _OUT BOOL &_is_valid)
// *---------------------------------------------------------------*
      {
         this->m_ds_iter = _begin ? this->m_sorters.begin() : ++this->m_ds_iter;

         if ( this->m_ds_iter == this->m_sorters.end() )         
         {
            _is_valid = FALSE;
            _name  = "";
            _recursive = FALSE;
            _sortfunc = NULL;
            return this->invalidate_data_sorter();
         }
         _is_valid = TRUE;
         _name = this->m_ds_iter->first;
         _recursive = this->m_ds_iter->second.recursive;
         _sortfunc = this->m_ds_iter->second.sortpred;
         return (this->m_sorters[_name].refs);
      }

// *---------------------------------------------------------------*
      std::vector<tcnode_ptr> &selectDataEqualsTo(_IN std::string _name, _OUT std::vector<tcnode_ptr> &_parm, _IN const tcnode_data _value)
// *---------------------------------------------------------------*
      {
         _parm.clear();

         typename std::map<std::string, TNodeDataSorter<_GenericCall> >::iterator iter = this->m_sorters.find(_name);
         if ( iter == this->m_sorters.end() )
            return this->invalidate_data_sorter();

         typename std::vector<tcnode_ptr>::iterator low, up;

         tCNode<_DATA, _KEY, _COMP> val;
         val.setData(_value);

         low = std::lower_bound(iter->second.refs.begin(), iter->second.refs.end(), tcnode_ptr(&val), iter->second.sortpred);
         up  = std::upper_bound(iter->second.refs.begin(), iter->second.refs.end(), tcnode_ptr(&val), iter->second.sortpred);

         if ( !(low == up) )
         {
            long sz = up - low;
            _parm.resize(sz);
            std::copy(low, up, _parm.begin());
         }
         
         return _parm;
      }

// *---------------------------------------------------------------*
      std::vector<tcnode_ptr> &selectDataEqualsTo(_IN std::string _name, _OUT std::vector<tcnode_ptr> &_parm, _IN const std::vector<tcnode_data> &_vals)
// *---------------------------------------------------------------*
      {
         _parm.clear();

         typename std::map<std::string, TNodeDataSorter<_GenericCall> >::iterator iter = this->m_sorters.find(_name);
         if ( iter == this->m_sorters.end() )
            return this->invalidate_data_sorter();

         if ( _vals.size() == 0 )
            return _parm;

         for ( size_t ii = 0; ii < _vals.size(); ii++ )
         {
            typename std::vector<tcnode_ptr>::iterator low, up;

            tCNode<_DATA, _KEY, _COMP> val;
            val.setData(_vals[ii]);

            low = std::lower_bound(iter->second.refs.begin(), iter->second.refs.end(), tcnode_ptr(&val), iter->second.sortpred);
            up  = std::upper_bound(iter->second.refs.begin(), iter->second.refs.end(), tcnode_ptr(&val), iter->second.sortpred);

            if ( !(low == up) )
            {         
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
      long getCount(void)
// *---------------------------------------------------------------*
      { return this->get_count(1); }

// *---------------------------------------------------------------*
      bool removeSubNodeByKey(_IN tcnode_key _key) 
// *---------------------------------------------------------------*
      {
         tcnode_iter it = this->m_subnodes.find(_key);
         if ( it != this->m_subnodes.end() )
         {
            it->second.removeSubNodes();
            this->m_subnodes.erase(it);   
            return true;
         }

         return false;
      }

// *---------------------------------------------------------------*
      tcnode_ref removeSubNodes(void)
// *---------------------------------------------------------------*
      {
         if ( !this->hasSubNodes() ) 
            return *this;

         tcnode_subnodes &children = this->m_subnodes;
         for ( tcnode_iter iter = children.begin(); iter != children.end(); iter++ )
               iter->second.removeSubNodes();

         this->m_subnodes.clear();
         // developer must call refreshDataSorters

         return *this;
      }

// *---------------------------------------------------------------*
      tcnode_shortcuts &getShortcuts(void)
// *---------------------------------------------------------------*
      { return this->isRoot() ? this->m_shortcuts : this->getRoot().getShortcuts(); }

// *---------------------------------------------------------------*
      tcnode_ref setShortcut(_IN std::string _label)
// *---------------------------------------------------------------*
      {
         vector<tcnode_key> keys;
         this->getNodeFullAddress(keys);
         this->getRoot().addShortcut(_label, keys);
         return *this;
      }

// *---------------------------------------------------------------*
      tcnode_ref addShortcut(_IN std::string _label, _IN std::vector<tcnode_key> &_parm)
// *---------------------------------------------------------------*
      {
         this->getRoot().getShortcuts()[_label] = _parm;
         return *this;
      }

// *---------------------------------------------------------------*
      tcnode_ptr getNodeByShortcut(_IN std::string _parm)
// *---------------------------------------------------------------*
      {
         tcnode_ptr retnode = NULL;

         if ( _parm.size() == 0 || this->getRoot().getShortcuts().size() == 0 )
            return tcnode_ptr(NULL);

         tcnode_shortcuts_iter iter = this->getRoot().getShortcuts().find(_parm);
         if ( iter == this->getRoot().getShortcuts().end() )
            return tcnode_ptr(NULL);

         return this->getNodeByFullAddress(iter->second);
      }

};

#define TNODE_SET_TYPE(_name, _tdata, _tkey)\
   typedef tCNode<_tdata, _tkey, less<_tkey> > T##_name;\
   typedef T##_name& T##_name##Ref;\
   typedef T##_name* T##_name##Ptr;\
   typedef T##_name::tcnode_subnodes& T##_name##Nodes;\
   typedef T##_name::tcnode_subnodes::iterator T##_name##NodesIter;

#define TNODE_PTR_TO_REF(_ptr) *_ptr

#endif


