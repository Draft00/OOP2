#ifndef GRAPH_H
#define GRAPT_H

#include <vector>

#define MAX_PATH 260

class IPrintInfo
{
public:
	virtual void print_answer(const char file_name[MAX_PATH]) const;
};

class Graph
{
public:
	Graph();
	void get_adjacency(const char file_name[MAX_PATH]);
	void print_adj_matrix(void) const;

	~Graph();

protected:
	int vertex_num = 0;
	int** adjacency_matrix = nullptr;
	bool oriented = 0;

	
private:
	void malloc_matrix();
};

class Graph_Routing : public Graph, public IPrintInfo //for L3. infinity is -1
{
public:
	Graph_Routing();
	void Dijkstra(int start, int finish);
	void Net_planning(int time);
	~Graph_Routing() {};
private:
	vector<vector<int>> adjacency_list;

	void adj_to_list(void);
};

#endif //!GRAPH_H
