#include "Graph.h"

Graph_Ford_Falk::Graph_Ford_Falk()
{
	oriented = 1;
}
Graph_Ford_Falk::~Graph_Ford_Falk()
{
	if (vertex_num)
	{
		for (int i = 0; i < vertex_num + 1; i++)
		{
			delete[] flow_matr[i];
		}
		delete[] flow_matr;
		
		delete[] edges;
		delete[] firstEdge; delete[] visited;
	}
}
void Graph_Ford_Falk::make_solve()
{
	count_edges();
	alloc_flow_matrix();
	find_source_dest();
	add_edge();
	int iterationResult = 0;
	while ((iterationResult = find_flow(sourcev, INT_MAX)) > 0)
	{
		memset(visited, false, vertex_num * sizeof(bool));
		maxFlow += iterationResult;
	}
}
void Graph_Ford_Falk::print_answer() const
{
	cout << "Source vertex № " << sourcev << ". " << "Drain vertex № " << destinationv << endl;
	cout << "Max flow: ";
	cout << maxFlow << endl;
	print_flow_matrix();
}

void Graph_Ford_Falk::print_flow_matrix() const
{
	cout << "Flow: " << endl;
	for (int i = 0; i < vertex_num + 1; i++)
	{
		for (int j = 0; j < vertex_num + 1; j++)
		{
			cout << flow_matr[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "Flow/edge capacity:" << endl;
	for (int i = 1; i < vertex_num + 1; i++)
	{
		for (int j = 1; j < vertex_num + 1; j++)
		{
			cout << flow_matr[i][j] << "/" << adjacency_matrix[i][j] << " ";
		}
		cout << endl;
	}
}
void Graph_Ford_Falk::alloc_flow_matrix(void)
{
	flow_matr = new int* [vertex_num + 1];
	for (int i = 0; i < vertex_num + 1; i++)
	{
		flow_matr[i] = new int[vertex_num + 1]();
	}

	flow_matr[0][0] = 0;
	for (int i = 1; i < vertex_num + 1; i++)
	{
		flow_matr[i][0] = i;
		flow_matr[0][i] = i;
	}
}
void Graph_Ford_Falk::add_edge(void)
{
	int edgeCount = 0;
	edges = new FF_edge[double_edges_num]();

	firstEdge = new int[vertex_num];
	visited = new bool[vertex_num];

	memset(firstEdge, -1, vertex_num * sizeof(int));
	memset(visited, false, vertex_num * sizeof(bool));

	for (int i = 1; i < vertex_num + 1; i++)
	{
		for (int j = 1; j < vertex_num + 1; j++)
		{
			if (adjacency_matrix[i][j])
			{
				// Прямое ребро
				edges[edgeCount].onEnd = j;                   // на конце прямого v
				edges[edgeCount].nextEdge = firstEdge[i - 1];     // добавляем в начало списка для u
				firstEdge[i - 1] = edgeCount;               // теперь начало списка - новое ребро
				edges[edgeCount++].capacity = adjacency_matrix[i][j];
				// Обратное ребро
				edges[edgeCount].onEnd = i;                   // на конце обратного u
				edges[edgeCount].nextEdge = firstEdge[j - 1];     // добавляем в начало списка для v
				firstEdge[j - 1] = edgeCount;               // теперь начало списка - новое ребро
				edges[edgeCount++].capacity = 0;
			}
		}
	}
}
void Graph_Ford_Falk::find_source_dest(void)
{
	int flag = 1;
	for (int i = 1; i < vertex_num + 1; i++)
	{
		flag = 1;
		for (int j = 1; j < vertex_num + 1; j++)
		{
			if (adjacency_matrix[i][j] != 0)
			{
				flag = 0; break;
			}
		}
		if (flag)
			destinationv = i;
	}
	for (int j = 1; j < vertex_num + 1; j++)
	{
		flag = 1;
		for (int i = 1; i < vertex_num + 1; i++)
		{
			if (adjacency_matrix[i][j] != 0)
			{
				flag = 0; break;
			}
		}
		if (flag)
			sourcev = j;
	}
}
void Graph_Ford_Falk::count_edges(void)
{
	for (int i = 1; i < vertex_num + 1; i++) {
		for (int j = 1; j < vertex_num + 1; j++) {
			if (adjacency_matrix[i][j]) {
				double_edges_num += 2; //прямое и обратное
			}
		}
	}
}

int Graph_Ford_Falk::find_flow(int u, int flow)
{
	if (u == destinationv) return flow; // возвращяем полученный минимум на пути
	visited[u - 1] = true;
	for (int idx_edge = firstEdge[u - 1]; idx_edge != -1; idx_edge = edges[idx_edge].nextEdge)
	{
		int to = edges[idx_edge].onEnd;
		if (!visited[to - 1] && edges[idx_edge].capacity > 0)
		{
			int minResult = find_flow(to, std::min(flow, edges[idx_edge].capacity)); // ищем поток в поддереве
			if (minResult > 0) {// если нашли
				if (idx_edge % 2 == 0)
				{
					int idx_x = edges[idx_edge ^ 1].onEnd, idx_y = edges[idx_edge].onEnd;
					flow_matr[idx_x][idx_y] += minResult;
				}
				edges[idx_edge].capacity -= minResult;   // у прямых ребер вычитаем поток
				edges[idx_edge ^ 1].capacity += minResult;   // к обратным прибавляем //only ^ иначе там где-то расчет неверный на обратном ребре
				return minResult;
			}
		}
	}
	return 0; // если не нашли поток из этой вершины - 0

}