#include "Graph.h"

Graph_NetPlanning::Graph_NetPlanning(int s_time)
{
	time = s_time;
}
Graph_NetPlanning::~Graph_NetPlanning()
{
	if (vertex_num) {
		delete[] vertex;
		veh.clear();
		adjacency_list.clear();
		way.clear();
	}
}
void Graph_NetPlanning::adj_to_list(void)
{
	for (int i = 1; i < vertex_num + 1; i++)
	{
		adjacency_list.push_back(std::vector<int>());
		adjacency_list[i - 1].push_back(adjacency_matrix[i][0]);
		for (int j = 1; j < vertex_num + 1; j++)
		{
			if (adjacency_matrix[i][j] != -1 && i != j)
				adjacency_list[i - 1].push_back(adjacency_matrix[j][0]);
		}
	}
}

void Graph_NetPlanning::make_solve()
{
	adj_to_list();
	get_sources();

	vertex = new NetPlanning_vertex[vertex_num + 1]();

	/*for (int i = 0; i < vertex_num + 1; i++)
	{
		//vertex[i].weigh = 0; 
		parent[i] = 1;
		lfin[i] = INT_MAX;
		efin[i] = -1;
	}*/ //now its default 

	get_new_num_vertex();
	get_time();
	get_ebeg_efin();
	get_critical_way();
	get_lbeg_lfin();
	get_r();
}

void Graph_NetPlanning::print_answer() const
{
	cout << "Optimal plan = " << vertex[vertex_num].weigh << endl; //<< weigh[parent[vertex_num] - 1] << " " 
	cout << "Critical rote. New num(old num): " << endl;
	for (int i = way.size() - 1; i >= 0; i--)
	{
		cout << way[i] << "(" << vertex[way[i] - 1].new_ind_old_val << ")" << " ";
	}
	cout << endl;
	cout << "new num - old num - ebeg - efin - lbeg - lfin - r :" << endl;
	for (int i = 0; i < vertex_num + 1; i++)
	{
		cout << i + 1 << " " << vertex[i].new_ind_old_val << " " << vertex[i].ebeg << " " << vertex[i].efin << " " << vertex[i].lbeg << " " << vertex[i].lfin << " " << vertex[i].r << endl;
	}
}

void Graph_NetPlanning::get_sources(void)
{
	for (int i = 0; i < adjacency_list.size(); i++) { //добавляю стартовые стоки, чтобы из них выбрать предка для t и ответ
		if (adjacency_list[i].size() == 1) {
			veh.push_back(adjacency_list[i][0]);
		}
	}
}

void Graph_NetPlanning::get_new_num_vertex(void)
{
	vertex[0].new_num = 1;
	vertex[vertex_num].new_num = vertex_num + 1;

	int i = 0;
	int number = vertex_num;
	int rem_ver = 0;
	while (adjacency_list.size() > 1)
	{
		if (adjacency_list[i].size() == 1)
		{
			rem_ver = adjacency_list[i][0];
			vertex[rem_ver - 1].new_num = number;
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
			//print_adj_list();
		}
		if (i == adjacency_list.size() - 1)
			i = 0;
		else i++;
	}

	for (int i = 0; i < vertex_num + 1; i++) //теперь индекс новой веришны - ее старый номер
	{
		int index = vertex[i].new_num - 1;
		vertex[index].new_ind_old_val = i + 1;
	}
}

void Graph_NetPlanning::get_ebeg_efin(void)
{
	int max_weigh = -1;
	int old_num = 0, new_ind = 0; //номер вершины в старых номерах и индекс новой вершины
	for (int i = 0; i < veh.size(); i++)
	{
		new_ind = vertex[veh[i] - 1].new_num - 1;
		if (vertex[new_ind].efin > vertex[vertex_num].efin)
		{
			vertex[vertex_num].efin = vertex[new_ind].efin;
			vertex[vertex_num].ebeg = vertex[vertex_num].efin;
		}
		if (vertex[new_ind].weigh > vertex[vertex_num].weigh)
		{
			vertex[vertex_num].weigh = vertex[new_ind].weigh;
			vertex[vertex_num].parent = new_ind + 1;
		}
	}
}
void Graph_NetPlanning::get_time(void)
{
	vertex[0].ebeg = 0; vertex[0].efin = 0;

	int old_num = 0, new_ind = 0; //номер вершины в старых номерах и индекс новой вершины
	int min_time = INT_MAX;
	for (int i = 2; i < vertex_num + 1; i++)
	{
		min_time = INT_MAX;
		old_num = vertex[i - 1].new_ind_old_val;

		for (int k = 1; k < vertex_num + 1; k++)
		{
			if (adjacency_matrix[k][old_num] > 0) //просматриваем все входящие вершины. в матрице это числа, не равные 0 и -1
			{
				int new_num_for_v = vertex[k - 1].new_num - 1;
				min_time = min(min_time, vertex[new_num_for_v].efin);
			}
		}
		vertex[i - 1].ebeg = min_time;

		for (int k = 1; k < vertex_num + 1; k++)
		{
			if (adjacency_matrix[k][old_num] != -1 && k != old_num) //все входящие дуги просматриваем
			{
				new_ind = vertex[k - 1].new_num - 1;
				if (vertex[new_ind].efin + adjacency_matrix[k][old_num] > vertex[i - 1].efin) //если путь в вершину + дуга из вершины больше чем у нас сейчас время конца
				{
					vertex[i - 1].efin = vertex[new_ind].efin + adjacency_matrix[k][old_num];
				}

				if (vertex[new_ind].weigh + adjacency_matrix[k][old_num] > vertex[i - 1].weigh)
				{
					vertex[i - 1].weigh = vertex[new_ind].weigh + adjacency_matrix[k][old_num];
					vertex[i - 1].parent = new_ind + 1;
				}
			}
		}
	}
}

void Graph_NetPlanning::get_critical_way(void)
{
	way.push_back(vertex_num);
	int temp = vertex_num;
	while (temp != 1)
	{
		temp = vertex[temp - 1].parent;
		way.push_back(temp);
	}
}
void Graph_NetPlanning::get_lbeg_lfin(void)
{
	int max_weigh = -1;
	int old_num = 0, new_ind = 0; //номер вершины в старых номерах и индекс новой вершины

	vertex[vertex_num].lbeg = time;
	vertex[vertex_num].lfin = time;

	for (int i = 0; i < veh.size(); i++) //для висячих вершин, потому что в матрице смежности нигде не указано, что после них идет добавленная работа t
	{
		max_weigh = -1;
		new_ind = vertex[veh[i] - 1].new_num - 1; //индекс висчей вершины в массиве
		vertex[new_ind].lfin = vertex[vertex_num].lfin;
		for (int k = 1; k < vertex_num + 1; k++)
		{
			//-max всех ребер
			if (adjacency_matrix[k][veh[i]] > max_weigh) //смотрим все входящие
			{
				max_weigh = adjacency_matrix[k][veh[i]];
				vertex[new_ind].lbeg = vertex[new_ind].lfin - max_weigh;
			}
		}
	}

	//здесь i уже индекс, а не номер, поэтому единичку не вычитаем
	for (int i = vertex_num - 1 - veh.size(); i >= 0; i--)
	{
		max_weigh = -1;
		old_num = vertex[i].new_ind_old_val;
		for (int k = 1; k < vertex_num + 1; k++) //находим время работы. ищем максимум по всем ВХОДЯЩИМ дугам
			if (adjacency_matrix[k][old_num] > max_weigh)
				max_weigh = adjacency_matrix[k][old_num];

		//минимум из работ, которые следуют после
		for (int k = 1; k < vertex_num + 1; k++)
		{
			if (adjacency_matrix[old_num][k] != -1 && old_num != k) //если из этой вершины есть работа ИСХОДЯЩИЕ ДУГИ СМОТРИМ
			{
				new_ind = vertex[k - 1].new_num - 1;
				if (vertex[new_ind].lfin - adjacency_matrix[old_num][k] < vertex[i].lfin)
				{
					vertex[i].lfin = vertex[new_ind].lfin - adjacency_matrix[old_num][k];
					vertex[i].lbeg = vertex[i].lfin - max_weigh;
				}
			}
		}
	}
}
void Graph_NetPlanning::get_r(void)
{
	for (int i = 0; i < vertex_num + 1; i++)
	{
		vertex[i].r = vertex[i].lfin - vertex[i].efin;
	}
}