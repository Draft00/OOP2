#include "Graph.h"
#include <iostream>
#include <fstream>

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

void Graph::malloc_matrix(void)
{
	adjacency_matrix = new int* [vertex_num + 1];
	for (int i = 0; i < vertex_num + 1; i++)
	{
		adjacency_matrix[i] = new int[vertex_num + 1]();
	}
	/*for (int i = 1; i < vertex_num + 1; i++)
		for (int j = 1; j < vertex_num + 1; j++)
			adjacency_matrix[i][j] = 0;*/
}

void Graph::get_adjacency(const char file_name[MAX_PATH])
{
	ifstream fin(file_name);
	if (!fin.is_open()) // если файл не открыт
		cout << "Error open file!" << endl;

	string str; 
	char buf[MAX_SYMBOLS + 1] = { 0 };
	//fin.getline(str, MAX_SYMBOLS, '\n');
	fin >> str;
	vertex_num = count(str.begin(), str.end(), ' ');
	/*for (int i = 0; ; i++)
	{
		if (buf[i] == ' ')
			vertex_num++;
		else if (buf[i] == '\0')
			break;
	}
	*/
	vertex_num++;

	fin.seekg(0);

	malloc_matrix();

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

	//cout << "Граф №" << count_G + 1 << " успешно прочитан!" << endl;
}

Graph::Graph()
{

}
Graph::~Graph()
{
	if (vertex_num) //если эти матрицы вообще были созданы
	{
		for (int i = 0; i < vertex_num + 1; i++)
		{
			delete[] adjacency_matrix[i];
		}
		delete[] adjacency_matrix;
	}
}

/*
	Graph_Routing
*/
Graph_Routing::Graph_Routing()
{
	oriented = 1;
}