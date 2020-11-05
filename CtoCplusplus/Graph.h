#ifndef GRAPH_H
#define GRAPT_H

#define MAX_PATH 260
#define MAX_SYMBOLS 200

class Graph
{
public:
	Graph();
	void get_adjacency(const char file_name[MAX_PATH]);

	~Graph();

protected:
	int vertex_num = 0;
	int** adjacency_matrix = nullptr;
	bool oriented = 0;

	void print_adj_matrix(void) const;
	
private:
	void malloc_matrix();
};

class Graph_Routing : public Graph//for L3. infinity is -1
{
	Graph_Routing();

	~Graph_Routing() {};
};

#endif //!GRAPH_H
