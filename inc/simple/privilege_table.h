#ifndef PRIVILEGE_TABLE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define PRIVILEGE_TABLE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<cassert>
#include	<map>

//
//	权限控制表，权限项值为int类型，用户对应权限值小于该值时为允许
//
template<
    class			ImplClass,		//	具体实现类, 通常为子类
    typename		ItemType,		//	权限项
    typename		CatalogType,	//	权限类别，通常为用户类型
    unsigned int	CatalogTypeSum	//	权限类别数量
    >
class	privilege_table {
public:
    enum {
        CATALOG_SUM	= CatalogTypeSum,
    };

    ///	typedefs.
    typedef	ImplClass		impl_type;
    typedef	CatalogType		catalog_type;
    typedef	ItemType		item_type;
    typedef	int				value_type;

    ///	table item define
    struct	privilege_table_item {
        item_type	item;
        value_type	values[CATALOG_SUM];
    };

    typedef	std::map<item_type, value_type>	list_type;
    typedef	privilege_table_item			table_item_type;

protected:
    ///	constructor
    explicit	privilege_table(const catalog_type& type) {
        size_t	size	= 0;
        const table_item_type* pItems = impl_type::do_get_privilege_table(size);
        for(size_t i = 0; i < size; ++i) {
            values_[pItems[i].item]	= pItems[i].values[impl_type::index_from_catalog_type(type)];
        }
    }

    ///	methods.
public:
    value_type	get(const item_type& item)const {
        typename list_type::const_iterator	it = values_.find(item);
        return (values_.end() == it)	?	0	:	it->second;
    }

    void		set(const item_type& item,	value_type value) {
        values_[item]=value;
    }

    bool		check(const item_type& item, value_type value)const {
        typename list_type::const_iterator	it = values_.find(item);
        return (values_.end() == it )	?	false	:	(value < it->second);
    }

    ///	override
public:
    static	size_t	index_from_catalog_type(size_t type) {
        return	type;
    }

protected:
    catalog_type	type_;
    list_type		values_;
};

//
//	权限检测支持类
//
template<
    class			ImplClass,			//	具体实现类, 通常为子类
    typename		ItemType,			//	权限项
    typename		CatalogType,		//	权限类别，通常为用户类型
    unsigned int	CatalogTypeSum,		//	权限类别数量
    int				DefaultValue	= 0	//	缺省的权限值
    >
class	support_privilege_table
    : protected privilege_table<ImplClass, ItemType, CatalogType, CatalogTypeSum> {
public:
    typedef		privilege_table<ImplClass, ItemType, CatalogType, CatalogTypeSum>	table_type;
    typedef		typename table_type::impl_type			impl_type;
    typedef		typename table_type::catalog_type		catalog_type;
    typedef		typename table_type::item_type			item_type;
    typedef		typename table_type::value_type			value_type;
    typedef		typename table_type::list_type			list_type;
    typedef		typename table_type::table_item_type	table_item_type;
    enum {
        DEFAULT_VALUE	=	DefaultValue
    };

public:
    explicit	support_privilege_table(const catalog_type& type) : table_type(type) {
    }

public:
    value_type	get(const item_type& item)const {
        typename list_type::const_iterator	it = privileges_.find(item);
        return (privileges_.end() == it)	?	DEFAULT_VALUE	:	it->second;
    }

    void		set(const item_type& item,	value_type value) {
        privileges_[item]=value;
    }

    bool		check(const item_type& item)const {
        value_type	val	= get(item);
        return table_type::check(item, val);
    }

    ///	override
public:
    static	size_t	index_from_catalog_type(size_t type) {
        return	type;
    }

protected:
    table_type&			do_get_privilege_table() {
        return static_cast<table_type&>(*this);
    }

    const table_type&	do_get_privilege_table()const {
        return static_cast<const table_type&>(*this);
    }

private:
    list_type	privileges_;
};

#define	PRIVILEGE_TABLE_BEGIN()		static	const table_item_type*	do_get_privilege_table(size_t& size){	static const table_item_type	table[]	= {
#define	PRIVILEGE_TABLE_END()		{item_type(),	{0}	} };	size = sizeof(table)/sizeof(table[0]) - 1;	return table;}

#define	PRIVILEGE_TABLE_ITEM1(item,_1	)																		{item, {_1}	},
#define	PRIVILEGE_TABLE_ITEM2(item,_1,_2 )																		{item, {_1,_2} },
#define	PRIVILEGE_TABLE_ITEM3(item,_1,_2,_3 )																	{item, {_1,_2,_3} },
#define	PRIVILEGE_TABLE_ITEM4(item,_1,_2,_3,_4 )																{item, {_1,_2,_3,_4} },
#define	PRIVILEGE_TABLE_ITEM5(item,_1,_2,_3,_4,_5	)															{item, {_1,_2,_3,_4,_5}	},
#define	PRIVILEGE_TABLE_ITEM6(item,_1,_2,_3,_4,_5,_6 )															{item, {_1,_2,_3,_4,_5,_6} },
#define	PRIVILEGE_TABLE_ITEM7(item,_1,_2,_3,_4,_5,_6,_7 )														{item, {_1,_2,_3,_4,_5,_6,_7} },
#define	PRIVILEGE_TABLE_ITEM8(item,_1,_2,_3,_4,_5,_6,_7,_8 )													{item, {_1,_2,_3,_4,_5,_6,_7,_8} },
#define	PRIVILEGE_TABLE_ITEM9(item,_1,_2,_3,_4,_5,_6,_7,_8,_9	)												{item, {_1,_2,_3,_4,_5,_6,_7,_8,_9}	},
#define	PRIVILEGE_TABLE_ITEM10(item,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10 )											{item, {_1,_2,_3,_4,_5,_6,_7,_8,_9,_10}	},
#define	PRIVILEGE_TABLE_ITEM11(item,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11 )										{item, {_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11}	},
#define	PRIVILEGE_TABLE_ITEM12(item,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12 )									{item, {_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12}	},
#define	PRIVILEGE_TABLE_ITEM13(item,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13 )								{item, {_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13}	},
#define	PRIVILEGE_TABLE_ITEM14(item,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14 )							{item, {_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14}	},
#define	PRIVILEGE_TABLE_ITEM15(item,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15 )						{item, {_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15}	},
#define	PRIVILEGE_TABLE_ITEM16(item,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16 )					{item, {_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16}	},
#define	PRIVILEGE_TABLE_ITEM17(item,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17 )				{item, {_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17}	},
#define	PRIVILEGE_TABLE_ITEM18(item,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18 )			{item, {_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18}	},
#define	PRIVILEGE_TABLE_ITEM19(item,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19 )		{item, {_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19}	},
#define	PRIVILEGE_TABLE_ITEM20(item,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20 )	{item, {_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20}	},

#endif
