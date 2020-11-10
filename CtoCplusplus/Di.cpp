#include "Graph.h"

using namespace std;

Graph_Dijkstra::Graph_Dijkstra(int s_start, int s_finish)
{
	m_start = s_start;
	m_finish = s_finish;
	oriented = 1;
}
Graph_Dijkstra::~Graph_Dijkstra()
{
	delete[] m_vertex;
	m_way.clear();
}

void Graph_Dijkstra::make_solve()
{
	int index;

	m_vertex = new Dijkstra_vertex[vertex_num]();

	m_vertex[m_start - 1].distance = 0;
	m_vertex[m_start - 1].parent = m_start;

	for (int count = 0; count < vertex_num - 1; count++)
	{
		int min = INT_MAX;
		for (int i = 0; i < vertex_num; i++)
		{
			if (!m_vertex[i].visited && m_vertex[i].distance <= min)
			{
				min = m_vertex[i].distance; index = i;
			}
		}
		m_vertex[index].visited = true;
		for (int i = 0; i < vertex_num; i++)
		{
			if (!m_vertex[i].visited && adjacency_matrix[index + 1][i + 1] != -1 && m_vertex[index].distance != INT_MAX)
			{
				if (m_vertex[index].distance + adjacency_matrix[index + 1][i + 1] < m_vertex[i].distance)
				{
					m_vertex[i].distance = m_vertex[index].distance + adjacency_matrix[index + 1][i + 1];
					m_vertex[i].parent = index + 1;
				}
			}
		}
	}

	m_way.push_back(m_finish);
	int temp = m_finish;
	while (temp != m_start)
	{
		temp = m_vertex[temp - 1].parent;
		m_way.push_back(temp);
	}

}

void Graph_Dijkstra::print_answer() const
{
	cout << "Route from: " << m_start << " to " << m_finish << " = " << m_vertex[m_finish - 1].distance << endl;
	cout << "Route: ";

	for (int i = m_way.size() - 1; i >= 0; i--)
	{
		cout << m_way[i] << " ";
	}
	cout << endl;

	cout << "Array D[istance]: ";
	for (int i = 0; i < vertex_num; i++)
	{
		cout << m_vertex[i].distance << " ";
	}
	cout << endl;

	cout << "Array P[arent]: ";
	for (int i = 0; i < vertex_num; i++)
	{
		cout << m_vertex[i].parent << " ";
	}
	cout << endl;

}
