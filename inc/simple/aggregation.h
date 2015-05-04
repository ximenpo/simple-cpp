#ifndef AGGREGATION_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define AGGREGATION_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include <map>
#include "creator.h"

/**
 *	Usage:	class YourDriveredClass
 *			:	public YourBaseClass1 ,\
 *				public YourBaseClassn ,\
 *				public aggregation<YourIDType, YourDriveredClass>
 *			{
 *			...
 *			};
 *
 *	Note:
 *		1.	KeyType must support = and < operators and a public 0-parameter constructor.
 *		2.	You	can	call instance( id ) to get the singleton	ValType	instance ptr.
 *		3.	You	can	override InitCurrentInstance() to init the current instance;
 *		4.	You	can	call instance_id()	to get the id of current instance;
 *		5.	You	can	call has_instance(id)	to check if the id	existed	or not;
 *		6.	You	can	use	the	UniqueType as the KeyType and the UniqueObject as the id
 *			to sepecify	a unique object	(Singleton type);
 *		7*.	This class would not clean the instaces	automatically.
 */

template<typename KeyType, typename ValType, typename Container = std::map<KeyType, ValType*> >
struct aggregation {
    ///	types defines.
public:
    typedef	KeyType			key_type;
    typedef	ValType			value_type;
    typedef	Container		container_type;

public:
    ///	check if the id	existed	or not.
    static bool					has( const key_type& id ) {
        container_type&	instances_ = static_aggregation_map();
        return instances_.end()!=instances_.find(id);
    }

    static value_type*			find( const key_type& id ) {
        container_type&	instances_ = static_aggregation_map();
        typename container_type::const_iterator	it	= instances_.find(id);
        return (instances_.end() != it) ? it->second : 0;
    }

    ///	get	instances' count.
    static size_t				size() {
        return static_aggregation_map().size();
    }

    ///	visitor-liked operations.
    template<typename UnaryPredicate>
    static value_type*			accept(UnaryPredicate& func) {
        container_type&	tmpIds = static_aggregation_map();
        typename container_type::iterator it		= tmpIds.begin();
        typename container_type::iterator it_end	= tmpIds.end();
        for( ; it != it_end; ++it ) {
            if(func(static_cast<value_type*>(it->second))) {
                return it->second;
            }
        }
        return 0;
    }

    ///	attributes.
protected:
    static container_type&			static_aggregation_map() {
        static container_type instances_;
        return instances_;
    }
};

template<typename KeyType, typename ValType, typename Creator = creator_new<ValType>, typename Container = std::map<KeyType, ValType*> >
class support_aggregation : private aggregation<KeyType, ValType, Container> {
protected:
    ///	constructor/destructors.
    support_aggregation() {
    }

    ~support_aggregation() {
        arregation_type::static_aggregation_map().erase(instance_id_);
    }

    ///	types defines.
public:
    typedef	KeyType			key_type;
    typedef	ValType			value_type;
    typedef	Container		container_type;

    typedef	aggregation<KeyType, ValType, Container>	arregation_type;
    typedef	support_aggregation<KeyType, ValType, Creator, Container>	this_type;

public:
    ///	get	current	id name.
    const key_type&				instance_id()const {
        return	instance_id_;
    }

    ///	overridable
protected:
    ///	init current instance.
    virtual	void				do_init_instance() {
        // dummy.
    }

public:
    ///	check if the id	existed	or not.
    static bool					has_instance( const key_type& id ) {
        return arregation_type::has(id);
    }
    static value_type*			find_instance( const key_type& id ) {
        return arregation_type::find(id);
    }

    ///	get	instances' count.
    static size_t				instance_sum() {
        return arregation_type::size();
    }

    ///	get	the	instance of	type.
    static value_type*			instance() {
        return instance(key_type());
    }
    static value_type*			instance( const key_type& id ) {
        container_type&	instances_ = arregation_type::static_aggregation_map();
        if(	instances_.end() == instances_.find(id) ) {
            instances_[id] = Creator::create();
            value_type* pObj = static_cast<value_type*>(instances_[id]);
            pObj->instance_id_ =	id;
            pObj->do_init_instance();
        }
        return instances_[id];
    }
    static	void				destroy_instance( const key_type& id ) {
        destroy_instance(arregation_type::find(id));
    }
    static	void				destroy_instance( value_type* instance ) {
        if(0 != instance) {
            Creator::destory(instance);
        }
    }


    ///	visitor-liked operations.
    template<typename UnaryPredicate>
    static value_type*			accept(UnaryPredicate& func) {
        return arregation_type::accept(func);
    }

    ///	attributes.
private:
    key_type	instance_id_;
};

#endif
