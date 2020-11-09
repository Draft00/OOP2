#include "Graph.h"

Graph_Kruskal::Graph_Kruskal(bool s_w_or_adj)
{
	w_or_adj = s_w_or_adj;
}
Graph_Kruskal::~Graph_Kruskal()
{
	if (vertex_num)
	{
		for (int i = 0; i < vertex_num + 1; i++)
		{
			delete[] spanning_tree[i];
		}
		delete[] spanning_tree;
		edges.clear(); 
		delete[] connect_comp;
	}
}

void Graph_Kruskal::print_answer() const
{
	cout << "Total weight of the Spanning tree: " << w_sp << endl;
	cout << "Spanning tree: " << endl;
	for (int i = 0; i < vertex_num + 1; i++)
	{
		for (int j = 0; j < vertex_num + 1; j++)
		{
			cout << spanning_tree[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Graph_Kruskal::make_solve()
{
	alloc_spanning_matrix();
	get_edges();

	int N = edges.size();
	int from = 0, to = 0;
	int count = 0;

	connect_comp = new int[vertex_num]();
	for (int i = 0; i < vertex_num; i++)
		connect_comp[i] = i;

	for (int i = 0; i < N; i++)
	{
		from = edges[i].from; to = edges[i].to;
		if (not_same_con_com(from - 1, to - 1))
		{
			spanning_tree[from][to] = 1;
			spanning_tree[to][from] = 1;
			w_sp += edges[i].w;
		}
	}
}
bool cmp_edge(const Kr_edge& a, const Kr_edge& b)
{
	return a.w < b.w;
}

void Graph_Kruskal::get_edges(void)
{
	int i, j;
	Kr_edge temp_e;
	for (i = 1; i < vertex_num + 1; i++)
	{
		for (!oriented ? j = i + 1 : j = 1; j < vertex_num + 1; j++)
		{
			if (adjacency_matrix[i][j] && i != j)
			{
				temp_e.from = i; temp_e.to = j;
				if (w_or_adj) temp_e.w = adjacency_matrix[i][j];
				else  temp_e.w = 1; //какая разница, сколько ребер между вершинами, они все веса 1, бери любое
				edges.push_back(temp_e);
			}
		}
	}
	sort(edges.begin(), edges.end(), cmp_edge);
}
void Graph_Kruskal::alloc_spanning_matrix(void)
{
	spanning_tree = new int* [vertex_num + 1];
	for (int i = 0; i < vertex_num + 1; i++)
	{
		spanning_tree[i] = new int[vertex_num + 1]();
	}

	for (int i = 1; i < vertex_num + 1; i++)
	{
		spanning_tree[i][0] = adjacency_matrix[i][0];
		spanning_tree[0][i] = adjacency_matrix[0][i];
	}
	spanning_tree[0][0] = 0;

}

bool Graph_Kruskal::not_same_con_com(int from, int to)
{
	if (connect_comp[from] == connect_comp[to])
		return false;

	int old_ks = connect_comp[to];
	connect_comp[to] = connect_comp[from];
	for (int i = 0; i < vertex_num; i++) //все, которые были в одной компоненте с у, присвоим такой же номер кс как у ч
	{
		if (connect_comp[i] == old_ks)
			connect_comp[i] = connect_comp[from];
	}
	return true;
}
