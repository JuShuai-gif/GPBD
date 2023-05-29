#pragma once
#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <queue>
#include <iostream>

#include "Common.h"

#include <array>
#include <list>

namespace PBD
{
    template <typename HullType>
    class kdTree
    {
    public:
        using TraversalPredicate = std::function<bool(unsigned int node_index,unsigned int depth)>;
        using TraversalCallback = std::function<void(unsigned int node_inpth,unsigned int depth)>;
        using TraversalPriorityLess = std::function<bool(std::array<int,2> const& nodes)>;

        struct Node
        {
            Node(unsigned int b_,unsigned int n_)
                :children({-1,-1}),
                begin(n_),n(n_){}
            Node() = default;
            bool is_leaf()const {
                return children[0] < 0 && children[1] < 0;
            }
            std::array<int,2> children;
            unsigned int begin;
            unsigned int n;
        };
        
        struct QueueItem{
            unsigned int n,d;
        };
        using TraversalQueue = std::queue<QueueItem>;

        kdTree(std::size_t n,unsigned int maxPrimitivesPerLeaf = 1)
            :m_lst(n),m_maxPrimitivesPerleaf(maxPrimitivesPerLeaf){}
        
        virtual ~kdTree(){}

        Node const& node(unsigned int i)const{
            return m_nodes[i];
        }
		// 返回第i个包围盒
        HullType const& hull(unsigned int i) const {
            return m_hulls[i];
        }
		// 获取实体
        unsigned int entity(unsigned int i) const { return m_lst[i]; }

		void construct(){
			m_nodes.clear();
			m_hulls.clear();
			if (m_lst.empty()) return;
    		// 会将 m_lst 中从 begin 到 end 的所有元素依次赋值为 0, 1, 2, ..., 最后一个元素值等于容器长度 - 1
			std::iota(m_lst.begin(), m_lst.end(), 0);

			// 确定所有域的边界框
			/*
			AlignedBox3r通过其两个角点（最小和最大）的坐标来定义一个三维轴对齐盒子
			*/
			auto box = AlignedBox3r{};
			// 通过循环遍历m_lst中的实体，扩展box的边界，并更新box的最小和最大角点以覆盖所有实体
			for (auto i = 0u; i < m_lst.size(); ++i)
				box.extend(entity_position(i));// 扩展边界
			
			auto ni = add_node(0, static_cast<unsigned int>(m_lst.size()));
			construct(ni, box, 0, static_cast<unsigned int>(m_lst.size()));
		}

		void traverse_depth_first(TraversalPredicate pred, TraversalCallback cb,
			TraversalPriorityLess const& pless = nullptr) const
		{
			if (m_nodes.empty())
				return;

			if (pred(0, 0)) 
				traverse_depth_first(0, 0, pred, cb, pless);
		}
		
		void traverse_breadth_first(TraversalPredicate const& pred, TraversalCallback const& cb, unsigned int start_node = 0, TraversalPriorityLess const& pless = nullptr, TraversalQueue& pending = TraversalQueue()) const
		{
			cb(start_node, 0);
			if (pred(start_node, 0)) pending.push({ start_node, 0 });
				traverse_breadth_first(pending, pred, cb, pless);
		}

		void traverse_breadth_first_parallel(TraversalPredicate pred, TraversalCallback cb) const
		{
			auto start_nodes = std::vector<QueueItem>{};
			#ifdef _DEBUG
				const unsigned int maxThreads = 1;
			#else
				const unsigned int maxThreads = omp_get_max_threads();
			#endif

				// compute ceiling of Log2
				// assuming double and long long have the same size.
				double d = maxThreads - 1;
				long long ll;  memcpy( &ll, &d, sizeof(d));
				const unsigned int targetDepth = (ll >> 52) - 1022ll;

				traverse_breadth_first(
					[&start_nodes, &maxThreads, &targetDepth](unsigned int node_index, unsigned int depth)
					{			
						return (depth < targetDepth) && (start_nodes.size() < maxThreads);
					},[&](unsigned int node_index, unsigned int depth){			
						if ((depth == targetDepth) || (node(node_index).is_leaf()))
							start_nodes.push_back({ node_index, depth });});

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static) 
				for (int i = 0; i < start_nodes.size(); i++)
				{
					QueueItem const& qi = start_nodes[i];
					cb(qi.n, qi.d);
					traverse_depth_first(qi.n, qi.d, pred, cb, nullptr);
				}
			}
		}
		
		void update(){
			traverse_depth_first(
				[&](unsigned int, unsigned int) { return true; },
				[&](unsigned int node_index, unsigned int)
			{
				auto const& nd = node(node_index);
				compute_hull_approx(nd.begin, nd.n, m_hulls[node_index]);
			}
			);
		}

    protected:
        void construct(unsigned int node, AlignedBox3r const& box,
			unsigned int b, unsigned int n){
				// If only one element is left end recursion.
			//if (n == 1) return;
			if (n <= m_maxPrimitivesPerLeaf) return;
		
			// Determine longest side of bounding box.
			auto max_dir = 0;
			auto d = box.diagonal().eval();
			if (d(1) >= d(0) && d(1) >= d(2))
				max_dir = 1;
			else if (d(2) >= d(0) && d(2) >= d(1))
				max_dir = 2;
		
		#ifdef _DEBUG
			for (auto i = 0u; i < n; ++i)
			{
				if (!box.contains(entity_position(m_lst[b + i])))
					std::cerr << "ERROR: Bounding box wrong!" << std::endl;
			}
		#endif
		
			// Sort range according to center of the longest side.
			std::sort(m_lst.begin() + b, m_lst.begin() + b + n, 
				[&](unsigned int a, unsigned int b)
				{
					return entity_position(a)(max_dir) < entity_position(b)(max_dir);
				}
			);
		
			auto hal = n / 2;
			auto n0 = add_node(b      , hal    );
			auto n1 = add_node(b + hal, n - hal);
			m_nodes[node].children[0] = n0;
			m_nodes[node].children[1] = n1;
		
			auto c = static_cast<Real>(0.5) * (
			entity_position(m_lst[b + hal -1])(max_dir) + 
			entity_position(m_lst[b + hal   ])(max_dir));
			auto l_box = box; l_box.max()(max_dir) = c;
			auto r_box = box; r_box.min()(max_dir) = c;

			construct(m_nodes[node].children[0], l_box, b, hal);
			construct(m_nodes[node].children[1], r_box, b + hal, n - hal);
		}


		void traverse_depth_first(unsigned int node, unsigned int depth,
			TraversalPredicate pred, TraversalCallback cb, TraversalPriorityLess const& pless) const
		{
			Node const& node = m_nodes[node_index];

			cb(node_index, depth);
			auto is_pred = pred(node_index, depth);
			if (!node.is_leaf() && is_pred)
			{
				if (pless && !pless(node.children))
				{
					traverse_depth_first(m_nodes[node_index].children[1], depth + 1, pred, cb, pless);
					traverse_depth_first(m_nodes[node_index].children[0], depth + 1, pred, cb, pless);
				}
				else
				{
					traverse_depth_first(m_nodes[node_index].children[0], depth + 1, pred, cb, pless);
					traverse_depth_first(m_nodes[node_index].children[1], depth + 1, pred, cb, pless);
				}
			}
		}



		void traverse_breadth_first(TraversalQueue& pending,
			TraversalPredicate const& pred, TraversalCallback const& cb, TraversalPriorityLess const& pless = nullptr) const
			{
				while (!pending.empty())
				{
					auto n = pending.front().n;
					auto d = pending.front().d;
					auto const& node = m_nodes[n];
					pending.pop();

					cb(n, d);
					auto is_pred = pred(n, d);
					if (!node.is_leaf() && is_pred)
					{
						if (pless && !pless(node.children))
						{
							pending.push({ static_cast<unsigned int>(node.children[1]), d + 1 });
							pending.push({ static_cast<unsigned int>(node.children[0]), d + 1 });
						}
						else
						{
							pending.push({ static_cast<unsigned int>(node.children[0]), d + 1 });
							pending.push({ static_cast<unsigned int>(node.children[1]), d + 1 });
						}
					}
				}
			}
		// b 是开始的节点序号，n 是当前节点下总共的节点数
		unsigned int add_node(unsigned int b, unsigned int n)
		{
			HullType hull;
			compute_hull(b, n, hull);
			m_hulls.push_back(hull);
			m_nodes.push_back({ b, n });
			return static_cast<unsigned int>(m_nodes.size() - 1);
		}

		virtual Vector3r const& entity_position(unsigned int i) const = 0;
		virtual void compute_hull(unsigned int b, unsigned int n, HullType& hull) const = 0;
		virtual void compute_hull_approx(unsigned int b, unsigned int n, HullType& hull) const
		{
			compute_hull(b, n, hull);
		}

    protected:
        std::vector<unsigned int> m_lst;
        std::vector<Node> m_nodes;
        std::vector<HullType> m_hulls;
        unsigned int m_maxPrimitivesPerleaf;
    };
}




