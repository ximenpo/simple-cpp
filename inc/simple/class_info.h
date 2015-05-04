#ifndef CLASS_INFO_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define CLASS_INFO_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<cassert>
#include	<queue>

#include	"creator.h"
#include	"delegate.h"

//	class info
template<typename RootType>
struct	class_info {
    //	typedefs.
    typedef	RootType				root_class_type;
    typedef	class_info<RootType>	class_info_type;

    typedef	delegate<RootType*()>		create_func_type;
    typedef	delegate<void(RootType*)>	destroy_func_type;

    //	class info tree.
    class_info_type*			base_class_info;
    class_info_type*			sub_class_info;
    class_info_type*			prev_class_info;
    class_info_type*			next_class_info;

    //	members.
    const char*					class_name;			// !!! shared class name string
    size_t						instance_size;
    create_func_type			create_instance;
    destroy_func_type			destroy_instance;

private:
    struct	pred_same_str {
        bool	operator()(const class_info* pInfo) {
            return	(str_ == pInfo->class_name) || (0 == strcmp(str_, pInfo->class_name));
        }
        const char	*str_;
    };

public:
    //	constructor.
    class_info(const char*				name,
               size_t					size		= 0,
               const class_info_type*	base_info	= 0,
               create_func_type			create		= 0,
               destroy_func_type		destroy		= 0
              )
        :	base_class_info(const_cast<class_info*>(base_info)), sub_class_info(0), prev_class_info(0), next_class_info(0)
        , class_name(name), instance_size(size)
        , create_instance(create), destroy_instance(destroy) {
        assert(0 != class_name);
        if(0 != base_class_info) {
            class_info_type*	pinfo	= base_class_info->sub_class_info;
            base_class_info->sub_class_info	= this;
            if(0 != pinfo) {
                this->next_class_info	= pinfo;
                pinfo->prev_class_info	= this;
            }
        }
    }
    //	find class_info by name/class_name
    const class_info_type*	find(const char* class_name)const {
        pred_same_str	pred	= {class_name};
        return	accept(pred);
    }
    //	find class_info by functor
    template<typename UnaryPredicate>
    const class_info_type*	accept(UnaryPredicate pred)const {
        if(0 == class_name) {
            return	0;
        }
        //	find root class info.
        const class_info_type*	proot	= this;
        while(0 != proot->base_class_info) {
            proot	= proot->base_class_info;
        }
        //	search for the class.
        std::queue<const class_info_type*>	nodes;  //使用C++的STL标准模板库
        if(pred(proot))	return	proot;
        else			nodes.push(proot);
        while(!nodes.empty()) {
            const class_info_type *node = nodes.front();
            nodes.pop();
            //	brothers.
            const class_info_type *nb	= node->next_class_info;
            if(0 != nb) {
                if(pred(nb))	return	nb;
                else			nodes.push(nb);
            }
            //	sub classes.
            const class_info_type *ns	= node->sub_class_info;
            if(0 != ns) {
                if(pred(ns))	return	ns;
                else			nodes.push(ns);
            }
        }

        return	0;
    }
};

template<typename RootType, typename ClassInfo = class_info<RootType>, typename Creator = creator_null<RootType> >
class support_root_class_info {
public:
    typedef	ClassInfo	class_info_type;
    typedef	RootType	root_class_type;

public:
    const ClassInfo*	class_info() {
        return	this->do_get_class_info();
    }

public:
    //	Note: you can override this static func for custom class name .
    static	const char*			static_class_name() {
        return typeid(RootType).name();
    }

    static	const ClassInfo*	static_class_info() {
        static	ClassInfo	s_class_info_(
            RootType::static_class_name(),
            sizeof(RootType),
            0,
            &Creator::create,
            &Creator::destroy
        );
        return &s_class_info_;
    }

    //	override
protected:
    virtual	const ClassInfo*	do_get_class_info() {
        return	RootType::static_class_info();
    }
};

//
//	运行时类信息支持
//
template<typename Type, typename BaseType, typename ClassInfo = typename BaseType::class_info_type, typename Creator = creator_new<typename BaseType::root_class_type,Type> >
class support_class_info : public BaseType {
public:
    typedef	ClassInfo							class_info_type;
    typedef	typename BaseType::root_class_type	root_class_type;

public:
    const ClassInfo*	class_info() {
        return	this->do_get_class_info();
    }

public:
    //	Note: you can override this static func for custom class name .
    static	const char*			static_class_name() {
        return typeid(Type).name();
    }

    static	const ClassInfo*	static_class_info() {
        static	ClassInfo	s_class_info_(
            Type::static_class_name(),
            sizeof(Type),
            BaseType::static_class_info(),
            &Creator::create,
            &Creator::destroy
        );
        return &s_class_info_;
    }

protected:
    virtual	const ClassInfo*	do_get_class_info() {
        return	Type::static_class_info();
    }
};

#endif
