#include "simple/igloo.h"
using namespace igloo;

#include "simple/privilege_table.h"

//	类型定义
enum	TEST_USER_TYPE {
    USER_0,
    USER_1,
    USER_2,
    USERUSER_END
};

//	权限定义
enum TEST_PRIVILEGE_TYPE {
    PRIV_0,
    PRIV_1,
    PRIV_2,
    PRIV_3,
    USERPRIV_END
};

//	权限控制项
class testPrivilege
    :	public privilege_table<testPrivilege, TEST_PRIVILEGE_TYPE, TEST_USER_TYPE, USERUSER_END> {
    typedef		privilege_table<testPrivilege, TEST_PRIVILEGE_TYPE, TEST_USER_TYPE, USERUSER_END>	base_type;
public:
    explicit	testPrivilege(TEST_USER_TYPE type) :base_type(type) {
    }

public:
    PRIVILEGE_TABLE_BEGIN()
    ///					USER_TYPE	USER_0	USER_1	USER_2
    PRIVILEGE_TABLE_ITEM3(	PRIV_1,		2,		0,		0 )
    PRIVILEGE_TABLE_ITEM3(	PRIV_2,		0,		1,		0 )
    PRIVILEGE_TABLE_ITEM3(	PRIV_3,		2,		2,		0 )
    PRIVILEGE_TABLE_END();
};

//	权限控制项(权限项为字符串)
class testPrivilege_SP
    :	public privilege_table<testPrivilege_SP, std::string, TEST_USER_TYPE, USERUSER_END> {
public:
    explicit	testPrivilege_SP(const TEST_USER_TYPE& type)
        :privilege_table<testPrivilege_SP, std::string, TEST_USER_TYPE, USERUSER_END>(type) {
    }

public:
    PRIVILEGE_TABLE_BEGIN()
    ///					USER_TYPE	USER_0	USER_1	USER_2
    PRIVILEGE_TABLE_ITEM3(	"PRIV_1",		2,		0,		0 )
    PRIVILEGE_TABLE_ITEM3(	"PRIV_2",		0,		1,		0 )
    PRIVILEGE_TABLE_ITEM3(	"PRIV_3",		2,		2,		0 )
    PRIVILEGE_TABLE_END();
};

class testUserPrivilege_2
    :	public support_privilege_table<testUserPrivilege_2, TEST_PRIVILEGE_TYPE, TEST_USER_TYPE, USERUSER_END> {
    typedef	support_privilege_table<testUserPrivilege_2, TEST_PRIVILEGE_TYPE, TEST_USER_TYPE, USERUSER_END>	base_type;
public:
    testUserPrivilege_2(TEST_USER_TYPE type):base_type(type) {}

public:
    PRIVILEGE_TABLE_BEGIN()
    ///					PRIV_TYPE	USER_0	USER_1	USER_2
    PRIVILEGE_TABLE_ITEM3(	PRIV_1,		1,		0,		0	)
    PRIVILEGE_TABLE_ITEM3(	PRIV_2,		1,		1,		0	)
    PRIVILEGE_TABLE_ITEM3(	PRIV_3,		1,		2,		0	)
    PRIVILEGE_TABLE_END()
};
///////////////////////////////////////////
struct testPrivilegeTestCase {
    TEST_PRIVILEGE_TYPE	priv;
    int					input;
    bool				result;
};
struct testPrivilegeTestCase_SP {
    std::string			priv;
    int					input;
    bool				result;
};
///////////////////////////////////////////

Context(privilege_table_context) {
    Spec(enum_user_and_privilege_1_usage) {
        testPrivilegeTestCase	testItems[] = {
            /// always false
            {PRIV_0,	-1,	false	},
            {PRIV_0,	0,	false	},
            {PRIV_0,	1,	false	},

            /// 0.
            {PRIV_1,	-1,	true	},
            {PRIV_1,	0,	false	},
            {PRIV_1,	1,	false	},

            /// 1.
            {PRIV_2,	0,	true	},
            {PRIV_2,	1,	false	},
            {PRIV_2,	2,	false	},

            /// 2.
            {PRIV_3,	1,	true	},
            {PRIV_3,	2,	false	},
            {PRIV_3,	3,	false	}
        };

        testPrivilege	priv(USER_1);

        for(size_t i=0; i<sizeof(testItems)/sizeof(testItems[0]); i++) {
            AssertThat(priv.check(testItems[i].priv, testItems[i].input),	Equals(testItems[i].result));
        }
    }

    Spec(enum_user_and_privilege_2_usage) {
        testPrivilegeTestCase	testItems[] = {
            /// always false
            {PRIV_0,	-1,	false	},
            {PRIV_0,	0,	false	},
            {PRIV_0,	1,	false	},

            /// always false/home/zhihuang/Desktop/CPP_Ware/inc/zhihuang
            {PRIV_1,	-1,	true	},
            {PRIV_1,	0,	false	},
            {PRIV_1,	1,	false	},

            /// always false
            {PRIV_2,	0,	false	},
            {PRIV_2,	1,	false	},
            {PRIV_2,	2,	false	},

            /// always false
            {PRIV_3,	1,	false	},
            {PRIV_3,	2,	false	},
            {PRIV_3,	3,	false	}
        };

        testPrivilege	priv(USER_2);

        for(size_t i=0; i<sizeof(testItems)/sizeof(testItems[0]); i++) {
            AssertThat(priv.check(testItems[i].priv, testItems[i].input),	Equals(testItems[i].result));
        }
    }

    Spec(string_user_usage) {
        testPrivilegeTestCase_SP	testItems[] = {
            /// always false
            {"PRIV_0",	-1,	false	},
            {"PRIV_0",	0,	false	},
            {"PRIV_0",	1,	false	},

            /// 0.
            {"PRIV_1",	-1,	true	},
            {"PRIV_1",	0,	false	},
            {"PRIV_1",	1,	false	},

            /// 1.
            {"PRIV_2",	0,	true	},
            {"PRIV_2",	1,	false	},
            {"PRIV_2",	2,	false	},

            /// 2.
            {"PRIV_3",	1,	true	},
            {"PRIV_3",	2,	false	},
            {"PRIV_3",	3,	false	}
        };

        testPrivilege_SP	priv(USER_1);

        for(size_t i=0; i<sizeof(testItems)/sizeof(testItems[0]); i++) {
            AssertThat(priv.check(testItems[i].priv, testItems[i].input),	Equals(testItems[i].result));
        }
    }

    Spec(support_privilege_table_usage) {
        testPrivilegeTestCase	testItems[] = {
            /// always false
            {PRIV_0,	-1,	false	},
            {PRIV_0,	0,	false	},
            {PRIV_0,	1,	false	},

            /// 0.
            {PRIV_1,	-1,	true	},
            {PRIV_1,	0,	false	},
            {PRIV_1,	1,	false	},

            /// 1.
            {PRIV_2,	0,	true	},
            {PRIV_2,	1,	false	},
            {PRIV_2,	2,	false	},

            /// 2.
            {PRIV_3,	1,	true	},
            {PRIV_3,	2,	false	},
            {PRIV_3,	3,	false	}
        };

        testUserPrivilege_2	priv(USER_1);

        for(size_t i=0; i<sizeof(testItems)/sizeof(testItems[0]); i++) {
            priv.set(testItems[i].priv, testItems[i].input);
            AssertThat(priv.check(testItems[i].priv),	Equals(testItems[i].result));
        }
    }

};
