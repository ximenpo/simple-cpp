#include <igloo/igloo.h>
using namespace igloo;

#include <cmath>
#include <deque>
#include "simple/astar.h"

static	const	int		R=6, C=6;
static	const	int		maze[R][C]	= {
    {8,	0,	0,	0,	0,	0},
    {8,	8,	8,	8,	8,	8},
    {1,	1,	0,	0,	0,	8},
    {1,	0,	1,	1,	1,	8},
    {1,	0,	1,	1,	1,	8},
    {1,	1,	1,	0,	1,	8},
};

static	unsigned int	make_node(int r, int c) {
    return	(r << 8) | c;
}

static	void			get_rc(unsigned int n, int& r, int& c) {
    r	= (n >> 8) & 0xFF;
    c	= (n >> 0) & 0xFF;
}

///
/// define the astar search context[implements the two functions].
///
struct testSearchContext {
    typedef float					weight_type;	// 权重类型，为两节点间路径估计值
    typedef unsigned int			node_type;		// 节点类型，为值类型，可赋值，可比较（<）
    typedef std::deque<node_type>	node_list;		// 节点列表类型

    // 估价函数,估价 pos 到目的地的距离,估计值必须保证比实际值小
    weight_type		evaluation(const node_type& nodeFrom, const node_type& nodeEnd)const {
        int	r1, c1, r2, c2;
        get_rc(nodeFrom, r1, c1);
        get_rc(nodeEnd, r2, c2);
        return	sqrt(float(r1-r2)*float(r1-r2) + float(c1-c2)*float(c1-c2));
    }
    // 获取 pos 周围的所有可用点
    void			fetch_neighbors(const node_type& node, node_list& nodes)const {
        int	r1, c1;
        get_rc(node, r1, c1);
        if(r1 - 1 >= 0 && maze[r1 - 1][c1] > 0) { //左
            nodes.push_back(make_node(r1 - 1, c1));
        }
        if(c1 - 1 >= 0 && maze[r1][c1 - 1] > 0) { //上
            nodes.push_back(make_node(r1, c1 - 1));
        }
        if(r1 + 1 < R && maze[r1 + 1][c1] > 0) { //右
            nodes.push_back(make_node(r1 + 1, c1));
        }
        if(c1 + 1 < C && maze[r1][c1 + 1] > 0) { //下
            nodes.push_back(make_node(r1, c1 + 1));
        }
    }
};

Context(astar_context) {
    Spec(basic_usage) {
        testSearchContext	context;
        testSearchContext::node_type	n1	= make_node(0, 0);
        testSearchContext::node_type	n2	= make_node(5, 5);

        astar_searcher<testSearchContext>	searcher;
        AssertThat(searcher(n1, n2, context),	IsTrue());
        AssertThat(searcher.get_path(),	HasLength(11));

        unsigned int path_data[]	= {0x0000,	0x0100, 0x0101, 0x0102, 0x0103, 0x0104, 0x0105, 0x0205, 0x0305, 0x0405, 0x0505};
        testSearchContext::node_list	path(path_data, path_data + sizeof(path_data)/sizeof(path_data[0]));
        AssertThat(searcher.get_path(),	EqualsContainer(path));
    }
};
