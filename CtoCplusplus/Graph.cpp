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

void Graph::malloc_matrix(void)
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

	cout << "Success!\n";
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

/*
	Graph_Routing
*/
Graph_Routing::Graph_Routing()
{
	oriented = 1;
}

void Graph_Routing::Dijkstra(int start, int finish)
{
	int* distance = new int[vertex_num];
	int* parent = new int[vertex_num];
	bool* visited = new bool[vertex_num];

	int index;

	for (int i = 0; i < vertex_num; i++)
	{
		distance[i] = INT_MAX;
		visited[i] = false;
		parent[i] = 0;
	}

	distance[start - 1] = 0;
	parent[start - 1] = start;

	for (int count = 0; count < vertex_num - 1; count++)
	{
		int min = INT_MAX;
		for (int i = 0; i < vertex_num; i++)
		{
			if (!visited[i] && distance[i] <= min)
			{
				min = distance[i]; index = i;
			}
		}
		visited[index] = true;
		for (int i = 0; i < vertex_num; i++)
		{
			if (!visited[i] && adjacency_matrix[index + 1][i + 1] != -1 && distance[index] != INT_MAX)
			{
				if (distance[index] + adjacency_matrix[index + 1][i + 1] < distance[i])
				{
					distance[i] = distance[index] + adjacency_matrix[index + 1][i + 1];
					parent[i] = index + 1;
				}
			}
		}
	}

	vector <int> way;
	way.push_back(finish);
	int temp = finish;
	while (temp != start)
	{
		temp = parent[temp - 1];
		way.push_back(temp);
	}

	cout << "Route from: " << start << " to " << finish << " = " << distance[finish - 1] << endl;
	cout << "Route: ";

	for (vector<int>::iterator iter = way.end(); iter != way.begin(); iter++) {
		cout << *iter << " ";
	}
	/*for (int i = way.size() - 1; i >= 0; i--)
	{
		cout << way[i] << " ";
	}*/
	cout << endl;

	cout << "Array D[istance]: ";
	for (int i = 0; i < vertex_num; i++)
	{
		cout << distance[i] << " ";
	}
	cout << endl;

	cout << "Array P[arent]: ";
	for (int i = 0; i < vertex_num; i++)
	{
		cout << parent[i] << " ";
	}
	cout << endl;


	delete[] distance;
	delete[] parent;
	delete[] visited;
	way.clear();
}

void Graph_Routing::Net_planning(int time)
{
	vector <int> veh;
	for (int i = 0; i < adjacency_list.size(); i++) //добавл€ю стартовые стоки, чтобы из них выбрать предка дл€ t и ответ
	{
		if (adjacency_list[i].size() == 1)
		{
			veh.push_back(adjacency_list[i][0]);
		}
	}

	int* new_ind_old_val = new int[vertex_num + 1]; //индекс - новые вершины, значение по индексу - старые
	int* new_num = new int[vertex_num + 1]; //индекс - старые вершины, значение по индексу - новые
	int* weigh = new int[vertex_num + 1];
	int* parent = new int[vertex_num + 1];
	int* ebeg = new int[vertex_num + 1];
	int* efin = new int[vertex_num + 1];
	int* lfin = new int[vertex_num + 1];
	int* lbeg = new int[vertex_num + 1];
	int* r = new int[vertex_num + 1];

	new_num[0] = 1;
	new_num[vertex_num] = vertex_num + 1;
	for (int i = 0; i < vertex_num + 1; i++)
	{
		//new_ind_old_val[i] = i + 1; 
		weigh[i] = 0;
		parent[i] = 1;
		lfin[i] = INT_MAX;
		efin[i] = -1;
	}

	int i = 0;
	int number = vertex_num;
	int rem_ver = 0;
	while (adjacency_list.size() > 1)
	{
		if (adjacency_list[i].size() == 1)
		{
			rem_ver = adjacency_list[i][0];
			new_num[rem_ver - 1] = number;
			number--;
			adjacency_list[i].clear();
			adjacency_list.erase(adjacency_list.begin() + i);
			for (int j = 0; j < adjacency_list.size(); j++)
			{
				for (int k = 1; k < adjacency_list[j].size(); k++)
				{
					if (adjacency_list[j][k] == rem_ver)
					{
						adjacency_list[j].erase(adjacency_list[j].begin() + k);
						k--;
					}
				}
			}
			i--;
			print_adj_list();
		}
		if (i == adjacency_list.size() - 1)
			i = 0;
		else i++;
	}

	for (int i = 0; i < vertex_num + 1; i++) //теперь индекс новой веришны - ее старый номер
	{
		new_ind_old_val[new_num[i] - 1] = i + 1;
	}

	ebeg[0] = 0; efin[0] = 0;
	int old_num = 0, new_ind = 0; //номер вершины в старых номерах и индекс новой вершины

	int max_weigh = -1;
	int min_time = INT_MAX;
	for (int i = 2; i < vertex_num + 1; i++)
	{
		min_time = INT_MAX;
		old_num = new_ind_old_val[i - 1];

		for (int k = 1; k < vertex_num + 1; k++)
		{
			if (adjacency_matrix[k][old_num] > 0) //просматриваем все вход€щие вершины. в матрице это числа, не равные 0 и -1
			{
				int new_num_for_v = new_num[k - 1] - 1;
				min_time = min(min_time, efin[new_num_for_v]);
			}
			/*if (adjacency_matrix[k][old_num] > max_weigh) //вход€щие вершины
				max_weigh = adjacency_matrix[k][old_num];*/
		}
		ebeg[i - 1] = min_time;

		for (int k = 1; k < vertex_num + 1; k++)
		{
			if (adjacency_matrix[k][old_num] != -1 && k != old_num) //все вход€щие дуги просматриваем
			{
				new_ind = new_num[k - 1] - 1;
				if (efin[new_ind] + adjacency_matrix[k][old_num] > efin[i - 1]) //если путь в вершину + дуга из вершины больше чем у нас сейчас врем€ конца
				{
					efin[i - 1] = efin[new_ind] + adjacency_matrix[k][old_num];
					//ebeg[i - 1] = efin[i - 1] - max_weigh;
				}

				if (weigh[new_ind] + adjacency_matrix[k][old_num] > weigh[i - 1])
				{
					weigh[i - 1] = weigh[new_ind] + adjacency_matrix[k][old_num];
					parent[i - 1] = new_ind + 1;
				}
			}
		}
	}

	max_weigh = -1;
	for (int i = 0; i < veh.size(); i++)
	{
		new_ind = new_num[veh[i] - 1] - 1;
		if (efin[new_ind] > efin[vertex_num])
		{
			efin[vertex_num] = efin[new_ind];
			ebeg[vertex_num] = efin[vertex_num];
		}
		if (weigh[new_ind] > weigh[vertex_num])
		{
			weigh[vertex_num] = weigh[new_ind];
			parent[vertex_num] = new_ind + 1;
		}
	}

	vector <int> way;
	way.push_back(vertex_num);
	int temp = vertex_num;
	while (temp != 1)
	{
		temp = parent[temp - 1];
		way.push_back(temp);
	}

	/*for (int i = 0; i < vertex_num + 1; i++)
		cout << weigh[i] << " ";
	cout << endl;
	for (int i = 0; i < vertex_num + 1; i++)
		cout << parent[i] << " ";
	cout << endl;*/


	lbeg[vertex_num] = time;
	lfin[vertex_num] = time;

	for (int i = 0; i < veh.size(); i++) //дл€ вис€чих вершин, потому что в матрице смежности нигде не указано, что после них идет добавленна€ работа t
	{
		max_weigh = -1;
		new_ind = new_num[veh[i] - 1] - 1; //индекс висчей вершины в массиве
		lfin[new_ind] = lfin[vertex_num];
		for (int k = 1; k < vertex_num + 1; k++)
		{
			//-max всех ребер
			if (adjacency_matrix[k][veh[i]] > max_weigh) //смотрим все вход€щие
			{
				max_weigh = adjacency_matrix[k][veh[i]];
				lbeg[new_ind] = lfin[new_ind] - max_weigh;
			}
			/*if (adjacency_matrix[veh[i]][k] > time_work) //смотрим все вход€щие дуги в сток в изначальной сети ищем ћј —»ћ”ћ ƒЋя LBEG
			{
				lfin[new_ind] = lbeg[vertex_num]; //ее врем€ конца - врем€ началы мнимого стока
				//-max всех ребер
				lbeg[new_ind] = lfin[new_ind] - time_work;
				}*/

		}
	}

	//здесь i уже индекс, а не номер, поэтому единичку не вычитаем

	for (int i = vertex_num - 1 - veh.size(); i >= 0; i--)
	{
		max_weigh = -1;
		old_num = new_ind_old_val[i];
		for (int k = 1; k < vertex_num + 1; k++) //находим врем€ работы. ищем максимум по всем ¬’ќƒяў»ћ дугам
			if (adjacency_matrix[k][old_num] > max_weigh)
				max_weigh = adjacency_matrix[k][old_num];

		//минимум из работ, которые следуют после
		for (int k = 1; k < vertex_num + 1; k++)
		{
			if (adjacency_matrix[old_num][k] != -1 && old_num != k) //если из этой вершины есть работа »—’ќƒяў»≈ ƒ”√» —ћќ“–»ћ
			{
				new_ind = new_num[k - 1] - 1;
				if (lfin[new_ind] - adjacency_matrix[old_num][k] < lfin[i])
				{
					lfin[i] = lfin[new_ind] - adjacency_matrix[old_num][k];
					lbeg[i] = lfin[i] - max_weigh;
				}
				/*if (lbeg[new_ind] < lfin[i]) //она не может закончитьс€ позже, чем начнетс€ работа, выход€ща€ из нее, ищем min
				{
					lfin[i] = lbeg[new_ind];
					par_old = new_ind_old_val[parent[i] - 1];
					lbeg[i] = lfin[i] - adjacency_matrix[par_old][old_num];
				}*/
			}
		}
	}

	for (int i = 0; i < vertex_num + 1; i++)
	{
		r[i] = lfin[i] - efin[i];
	}

	cout << "ќптимальный план = " << weigh[vertex_num] << endl; //<< weigh[parent[vertex_num] - 1] << " " 
	cout << " ритический путь. Ќовый номер(старый номер): " << endl;
	for (int i = way.size() - 1; i >= 0; i--)
	{
		cout << way[i] << "(" << new_ind_old_val[way[i] - 1] << ")" << " ";
	}
	cout << endl;
	cout << "новый номер - старый номер - ebeg - efin - lbeg - lfin - r :" << endl;
	for (int i = 0; i < vertex_num + 1; i++)
	{
		cout << i + 1 << " " << new_ind_old_val[i] << " " << ebeg[i] << " " << efin[i] << " " << lbeg[i] << " " << lfin[i] << " " << r[i] << endl;
	}

	delete[] new_ind_old_val;
	delete[] new_num;
	delete[] weigh;
	delete[] parent;
	delete[] ebeg; delete[] efin; delete[] lbeg; delete[] lfin;
	delete[] r;
	veh.clear();
	adjacency_list.clear();
	way.clear();
}