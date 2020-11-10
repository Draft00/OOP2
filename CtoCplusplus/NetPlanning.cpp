#include "Graph.h"

Graph_NetPlanning::Graph_NetPlanning(int s_time)
{
	m_time = s_time;
}
Graph_NetPlanning::~Graph_NetPlanning()
{
	if (vertex_num) {
		delete[] m_vertex;
		m_veh.clear();
		m_adjacency_list.clear();
		m_way.clear();
	}
}
void Graph_NetPlanning::adj_to_list(void)
{
	for (int i = 1; i < vertex_num + 1; i++)
	{
		m_adjacency_list.push_back(std::vector<int>());
		m_adjacency_list[i - 1].push_back(adjacency_matrix[i][0]);
		for (int j = 1; j < vertex_num + 1; j++)
		{
			if (adjacency_matrix[i][j] != -1 && i != j)
				m_adjacency_list[i - 1].push_back(adjacency_matrix[j][0]);
		}
	}
}

void Graph_NetPlanning::make_solve()
{
	adj_to_list();
	get_sources();

	m_vertex = new NetPlanning_vertex[vertex_num + 1]();

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
	cout << "Optimal plan = " << m_vertex[vertex_num].weigh << endl; //<< weigh[parent[vertex_num] - 1] << " " 
	cout << "Critical rote. New num(old num): " << endl;
	for (int i = m_way.size() - 1; i >= 0; i--)
	{
		cout << m_way[i] << "(" << m_vertex[m_way[i] - 1].new_ind_old_val << ")" << " ";
	}
	cout << endl;
	cout << "new num - old num - ebeg - efin - lbeg - lfin - r :" << endl;
	for (int i = 0; i < vertex_num + 1; i++)
	{
		cout << i + 1 << " " << m_vertex[i].new_ind_old_val << " " << m_vertex[i].ebeg << " " << m_vertex[i].efin << " " << m_vertex[i].lbeg << " " << m_vertex[i].lfin << " " << m_vertex[i].r << endl;
	}
}

void Graph_NetPlanning::get_sources(void)
{
	for (int i = 0; i < m_adjacency_list.size(); i++) { //добавляю стартовые стоки, чтобы из них выбрать предка для t и ответ
		if (m_adjacency_list[i].size() == 1) {
			m_veh.push_back(m_adjacency_list[i][0]);
		}
	}
}

void Graph_NetPlanning::get_new_num_vertex(void)
{
	m_vertex[0].new_num = 1;
	m_vertex[vertex_num].new_num = vertex_num + 1;

	int i = 0;
	int number = vertex_num;
	int rem_ver = 0;
	while (m_adjacency_list.size() > 1)
	{
		if (m_adjacency_list[i].size() == 1)
		{
			rem_ver = m_adjacency_list[i][0];
			m_vertex[rem_ver - 1].new_num = number;
			number--;
			m_adjacency_list[i].clear();
			m_adjacency_list.erase(m_adjacency_list.begin() + i);
			for (int j = 0; j < m_adjacency_list.size(); j++)
			{
				for (int k = 1; k < m_adjacency_list[j].size(); k++)
				{
					if (m_adjacency_list[j][k] == rem_ver)
					{
						m_adjacency_list[j].erase(m_adjacency_list[j].begin() + k);
						k--;
					}
				}
			}
			i--;
			//print_adj_list();
		}
		if (i == m_adjacency_list.size() - 1)
			i = 0;
		else i++;
	}

	for (int i = 0; i < vertex_num + 1; i++) //теперь индекс новой веришны - ее старый номер
	{
		int index = m_vertex[i].new_num - 1;
		m_vertex[index].new_ind_old_val = i + 1;
	}
}

void Graph_NetPlanning::get_ebeg_efin(void)
{
	int max_weigh = -1;
	int old_num = 0, new_ind = 0; //номер вершины в старых номерах и индекс новой вершины
	for (int i = 0; i < m_veh.size(); i++)
	{
		new_ind = m_vertex[m_veh[i] - 1].new_num - 1;
		if (m_vertex[new_ind].efin > m_vertex[vertex_num].efin)
		{
			m_vertex[vertex_num].efin = m_vertex[new_ind].efin;
			m_vertex[vertex_num].ebeg = m_vertex[vertex_num].efin;
		}
		if (m_vertex[new_ind].weigh > m_vertex[vertex_num].weigh)
		{
			m_vertex[vertex_num].weigh = m_vertex[new_ind].weigh;
			m_vertex[vertex_num].parent = new_ind + 1;
		}
	}
}
void Graph_NetPlanning::get_time(void)
{
	m_vertex[0].ebeg = 0; m_vertex[0].efin = 0;

	int old_num = 0, new_ind = 0; //номер вершины в старых номерах и индекс новой вершины
	int min_time = INT_MAX;
	for (int i = 2; i < vertex_num + 1; i++)
	{
		min_time = INT_MAX;
		old_num = m_vertex[i - 1].new_ind_old_val;

		for (int k = 1; k < vertex_num + 1; k++)
		{
			if (adjacency_matrix[k][old_num] > 0) //просматриваем все входящие вершины. в матрице это числа, не равные 0 и -1
			{
				int new_num_for_v = m_vertex[k - 1].new_num - 1;
				min_time = min(min_time, m_vertex[new_num_for_v].efin);
			}
		}
		m_vertex[i - 1].ebeg = min_time;

		for (int k = 1; k < vertex_num + 1; k++)
		{
			if (adjacency_matrix[k][old_num] != -1 && k != old_num) //все входящие дуги просматриваем
			{
				new_ind = m_vertex[k - 1].new_num - 1;
				if (m_vertex[new_ind].efin + adjacency_matrix[k][old_num] > m_vertex[i - 1].efin) //если путь в вершину + дуга из вершины больше чем у нас сейчас время конца
				{
					m_vertex[i - 1].efin = m_vertex[new_ind].efin + adjacency_matrix[k][old_num];
				}

				if (m_vertex[new_ind].weigh + adjacency_matrix[k][old_num] > m_vertex[i - 1].weigh)
				{
					m_vertex[i - 1].weigh = m_vertex[new_ind].weigh + adjacency_matrix[k][old_num];
					m_vertex[i - 1].parent = new_ind + 1;
				}
			}
		}
	}
}

void Graph_NetPlanning::get_critical_way(void)
{
	m_way.push_back(vertex_num);
	int temp = vertex_num;
	while (temp != 1)
	{
		temp = m_vertex[temp - 1].parent;
		m_way.push_back(temp);
	}
}
void Graph_NetPlanning::get_lbeg_lfin(void)
{
	int max_weigh = -1;
	int old_num = 0, new_ind = 0; //номер вершины в старых номерах и индекс новой вершины

	m_vertex[vertex_num].lbeg = m_time;
	m_vertex[vertex_num].lfin = m_time;

	for (int i = 0; i < m_veh.size(); i++) //для висячих вершин, потому что в матрице смежности нигде не указано, что после них идет добавленная работа t
	{
		max_weigh = -1;
		new_ind = m_vertex[m_veh[i] - 1].new_num - 1; //индекс висчей вершины в массиве
		m_vertex[new_ind].lfin = m_vertex[vertex_num].lfin;
		for (int k = 1; k < vertex_num + 1; k++)
		{
			//-max всех ребер
			if (adjacency_matrix[k][m_veh[i]] > max_weigh) //смотрим все входящие
			{
				max_weigh = adjacency_matrix[k][m_veh[i]];
				m_vertex[new_ind].lbeg = m_vertex[new_ind].lfin - max_weigh;
			}
		}
	}

	//здесь i уже индекс, а не номер, поэтому единичку не вычитаем
	for (int i = vertex_num - 1 - m_veh.size(); i >= 0; i--)
	{
		max_weigh = -1;
		old_num = m_vertex[i].new_ind_old_val;
		for (int k = 1; k < vertex_num + 1; k++) //находим время работы. ищем максимум по всем ВХОДЯЩИМ дугам
			if (adjacency_matrix[k][old_num] > max_weigh)
				max_weigh = adjacency_matrix[k][old_num];

		//минимум из работ, которые следуют после
		for (int k = 1; k < vertex_num + 1; k++)
		{
			if (adjacency_matrix[old_num][k] != -1 && old_num != k) //если из этой вершины есть работа ИСХОДЯЩИЕ ДУГИ СМОТРИМ
			{
				new_ind = m_vertex[k - 1].new_num - 1;
				if (m_vertex[new_ind].lfin - adjacency_matrix[old_num][k] < m_vertex[i].lfin)
				{
					m_vertex[i].lfin = m_vertex[new_ind].lfin - adjacency_matrix[old_num][k];
					m_vertex[i].lbeg = m_vertex[i].lfin - max_weigh;
				}
			}
		}
	}
}
void Graph_NetPlanning::get_r(void)
{
	for (int i = 0; i < vertex_num + 1; i++)
	{
		m_vertex[i].r = m_vertex[i].lfin - m_vertex[i].efin;
	}
}