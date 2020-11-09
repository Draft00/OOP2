#ifndef GRAPH_H
#define GRAPT_H

#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
#define MAX_PATH 260

class IAction
{
public:
	virtual void make_solve(void) = 0;
	virtual void print_answer() const = 0;
};

class Graph : public IAction
{
public:
	Graph();
	void get_adjacency(const char file_name[MAX_PATH]);
	void print_adj_matrix(void) const;
	~Graph();

protected:
	int vertex_num = 0;
	int** adjacency_matrix = nullptr;
	bool oriented = false;

private:
	void alloc_matrix();
};

class Dijkstra_vertex {
public:
	Dijkstra_vertex() {};
	int distance = INT_MAX;
	int parent = 0;
	bool visited = false;
	~Dijkstra_vertex() {};
};

class Graph_Dijkstra : public Graph//, public IAction //for L3. infinity is -1
{
	//std::vector <int> way;
public:
	Graph_Dijkstra(int s_start, int s_finish);
	void make_solve() override;
	void print_answer() const override;
	~Graph_Dijkstra();

private:
	int start = 0;
	int finish = 0;
	Dijkstra_vertex* vertex;
	std::vector <int> way;
};

class NetPlanning_vertex {

public:
	NetPlanning_vertex() {};
	int new_ind_old_val = 0, new_num = 0; //индекс - старые вершины, значение по индексу - новые
	int weigh = 0;
	int parent = 1;
	int efin = -1;
	int lfin = INT_MAX;
 
	int r = 0;
	int ebeg = 0;
	int lbeg = 0;
	~NetPlanning_vertex() {};
};

class Graph_NetPlanning : public Graph//, public IAction
{
	std::vector <int> veh;
	std::vector<std::vector<int>> adjacency_list;
	std::vector <int> way;

public:
	Graph_NetPlanning(int s_time);
	void make_solve() override;
	void print_answer() const override;
	~Graph_NetPlanning();
private:
	int time = 0;
	NetPlanning_vertex* vertex;

	void adj_to_list(void);
	void get_new_num_vertex(void);
	void get_ebeg_efin(void);
	void get_time(void);
	void get_critical_way(void);
	void get_lbeg_lfin(void);
	void get_r(void);
	void get_sources(void);
};

class FF_edge {
public:
	FF_edge() {};
	int capacity = 0;
	int onEnd = 0;
	int nextEdge = 0;
	~FF_edge() {};
};

class Graph_Ford_Falk : public Graph//, public IAction
{
public:
	Graph_Ford_Falk();
	void make_solve() override;
	void print_answer() const override;
	~Graph_Ford_Falk();

private:
	int double_edges_num = 0;
	FF_edge* edges = nullptr; 
	int** flow_matr = nullptr;
	int* firstEdge = nullptr;
	bool* visited = nullptr;
	int sourcev = 0, destinationv = 0; //исток, сток
	int maxFlow = 0;

	void alloc_flow_matrix(void);
	void add_edge(void);
	int find_flow(int u, int flow);
	void find_source_dest(void);
	void count_edges(void);
	void print_flow_matrix() const;
};

class Kr_edge {
public:
	Kr_edge() {};
	int from = 0, to = 0, w = 0;
	~Kr_edge() {};
};

bool cmp_edge(const Kr_edge& a, const Kr_edge& b);

class Graph_Kruskal : public Graph//, public IAction 
{
public:
	Graph_Kruskal(bool s_w_or_adj);
	void make_solve() override;
	void print_answer() const override;
	~Graph_Kruskal();
private:
	std::vector <Kr_edge> edges;
	int edges_count = 0;
	bool w_or_adj = 1;
	int** spanning_tree = nullptr;
	int* connect_comp = nullptr; 
	int w_sp = 0;

	void get_edges(void);
	void alloc_spanning_matrix(void);
	bool not_same_con_com(int from, int to);
};
#endif //!GRAPH_H
