#ifndef ASTAR_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define ASTAR_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include <map>
#include <algorithm>

//
//	A* 路径搜索算法（二维矩阵地图），使用方法如下：
//
//		SearchContext	context;
//		astar_searcher<SearchContext>	searcher;
//
//		if( searcher(nodesBegin, nodeEnd) )	{
//			// searcher.get_path();
//		}
//
//	其中，SearchContext接口如下：
//
//		struct SearchContext {
//			typedef int					weight_type;	// 权重类型，为两节点间路径估计值
//			typedef unsigned int		node_type;		// 节点类型，为值类型，可赋值，可比较（<）
//			typedef deque<node_type>	node_list;		// 节点列表类型
//
//			// 估价函数,估价 pos 到目的地的距离,估计值必须保证比实际值小
//			weight_type		evaluation(const node_type& nodeFrom, const node_type& nodeEnd)const;
//			// 获取 pos 周围的所有可用点
//			void			fetch_neighbors(const node_type& node, node_list& nodes)const;
//		};
//


template<typename SearchContext>
struct astar_searcher {
public:
    typedef SearchContext	context_type;

    typedef	typename	context_type::weight_type	weight_type;
    typedef	typename	context_type::node_type		node_type;
    typedef typename	context_type::node_list		node_list;

private:
    struct  node_data {
        node_type	pos;
        node_type	parent;
        weight_type	h, w;
    };

public:
    typedef	typename	std::map<node_type, node_data>	node_queue;

public:
    // 获取路径
    const node_list&  get_path() {
        return path_;
    }
    // 搜索是否可以到达
    bool operator()(const node_type& nodeBegin, const node_type& nodeEnd, const context_type& context) {
        path_.clear();
        open_.clear();
        closed_.clear();

        node_data node;
        node.pos    = nodeBegin;
        node.h = 0;
        node.w	= context.evaluation(nodeBegin, nodeEnd);
        node.parent = nodeBegin;

        open_[node.pos] = node;

        while(!open_.empty()) {
            node = get_next_node();

            if(node.pos == nodeEnd) {
                // succeed.
                generate_path(node);
                return true;
            }

            // get_next_node step.
            node_list child_nodes;
            context.fetch_neighbors(node.pos, child_nodes);

            typename	node_list::const_iterator it = child_nodes.begin();
            typename	node_list::const_iterator it_end = child_nodes.end();
            for( ; it != it_end; ++it ) {
                node_data tmpNode;
                tmpNode.pos = *it;
                tmpNode.w	= context.evaluation(*it, nodeEnd);
                tmpNode.h	= node.h + 1;
                tmpNode.parent = node.pos;

                if(open_.find(*it) != open_.end()) {
                    node_data tn = open_[*it];
                    if((tmpNode.h + tmpNode.w) < (tn.h + tn.w)) {
                        open_[*it]  = tmpNode;
                    }
                } else if(closed_.find(*it) == closed_.end()) {
                    // not in closed & open queue.
                    open_[*it] = tmpNode;
                }
            }
            closed_[node.pos] = node;
        }

        return false;
    }

private:
    // 下一个最可能的点（此处可进一步优化）
    node_data	get_next_node() {
        typename	node_queue::const_iterator it		= open_.begin();
        typename	node_queue::const_iterator it_end	= open_.end();

        node_data node = it->second;
        for( ++it; it != it_end; ++it ) {
            if( it->second.w < node.w ) {
                node = it->second;
            }
        }

        open_.erase(node.pos);
        return node;
    }
    // 生成路径
    void	generate_path(const node_data& node) {
        node_data p = node;
        path_.push_back(p.pos);
        while(p.pos != p.parent) {
            p = closed_[p.parent];
            path_.push_back(p.pos);
        }

        std::reverse(path_.begin(),path_.end());
    }

private:
    node_list	path_;
    node_queue	open_, closed_;
};

#endif
