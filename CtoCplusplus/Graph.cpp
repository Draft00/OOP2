#include "Graph.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std; 

/*
	Graph
*/
void Graph::print_adj_matrix(void) const
{
	cout << "Matrix: " << endl;
	for (int i = 0; i < vertex_num + 1; i++)
	{
		for (int j = 0; j < vertex_num + 1; j++)
		{
			cout << adjacency_matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Graph::alloc_matrix(void)
{
	adjacency_matrix = new int* [vertex_num + 1];
	for (int i = 0; i < vertex_num + 1; i++)
	{
		adjacency_matrix[i] = new int[vertex_num + 1]();
	}
}

void Graph::get_adjacency(const char file_name[MAX_PATH])
{
	ifstream fin(file_name);
	if (!fin.is_open()) 
		cout << "Error open file!" << endl;

	string str; 
	getline(fin, str, '\n');
	vertex_num = count(str.begin(), str.end(), ' ');
	vertex_num++;

	fin.seekg(0);

	alloc_matrix();

	for (int i = 1; i < vertex_num + 1; i++)
	{
		for (int j = 1; j < vertex_num + 1; j++)
		{
			fin >> adjacency_matrix[i][j];
		}
	}

	fin.close();

	adjacency_matrix[0][0] = 0;
	for (int i = 1; i < vertex_num + 1; i++)
	{
		adjacency_matrix[i][0] = i;
		adjacency_matrix[0][i] = i;
	}

	cout << "Successfilly read!\n";
}

Graph::Graph()
{

}

Graph::~Graph()
{
	if (vertex_num)
	{
		for (int i = 0; i < vertex_num + 1; i++)
		{
			delete[] adjacency_matrix[i];
		}
		delete[] adjacency_matrix;
	}
}

