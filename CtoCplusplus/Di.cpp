#include "Graph.h"

using namespace std;

Graph_Dijkstra::Graph_Dijkstra(int s_start, int s_finish)
{
	start = s_start;
	finish = s_finish;
	oriented = 1;
}
Graph_Dijkstra::~Graph_Dijkstra()
{
	delete[] vertex;
	way.clear();
}

void Graph_Dijkstra::make_solve()
{
	int index;

	vertex = new Dijkstra_vertex[vertex_num]();

	vertex[start - 1].distance = 0;
	vertex[start - 1].parent = start;

	for (int count = 0; count < vertex_num - 1; count++)
	{
		int min = INT_MAX;
		for (int i = 0; i < vertex_num; i++)
		{
			if (!vertex[i].visited && vertex[i].distance <= min)
			{
				min = vertex[i].distance; index = i;
			}
		}
		vertex[index].visited = true;
		for (int i = 0; i < vertex_num; i++)
		{
			if (!vertex[i].visited && adjacency_matrix[index + 1][i + 1] != -1 && vertex[index].distance != INT_MAX)
			{
				if (vertex[index].distance + adjacency_matrix[index + 1][i + 1] < vertex[i].distance)
				{
					vertex[i].distance = vertex[index].distance + adjacency_matrix[index + 1][i + 1];
					vertex[i].parent = index + 1;
				}
			}
		}
	}

	way.push_back(finish);
	int temp = finish;
	while (temp != start)
	{
		temp = vertex[temp - 1].parent;
		way.push_back(temp);
	}

}

void Graph_Dijkstra::print_answer() const
{
	cout << "Route from: " << start << " to " << finish << " = " << vertex[finish - 1].distance << endl;
	cout << "Route: ";

	for (int i = way.size() - 1; i >= 0; i--)
	{
		cout << way[i] << " ";
	}
	cout << endl;

	cout << "Array D[istance]: ";
	for (int i = 0; i < vertex_num; i++)
	{
		cout << vertex[i].distance << " ";
	}
	cout << endl;

	cout << "Array P[arent]: ";
	for (int i = 0; i < vertex_num; i++)
	{
		cout << vertex[i].parent << " ";
	}
	cout << endl;

}
