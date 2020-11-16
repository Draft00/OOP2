#include "Graph.h"
#include <iostream>

using namespace std;

void get_answer(IAction& task)
{
	task.make_solve();
	task.print_answer();
}

char file[MAX_PATH] = { 0 };

void enter_filename()
{
	cout << "Enetr file name: ";
	cin >> file;
	cout << endl;
}
int main()
{
	int choice = 0;

	cout << "Welcome to programm \"Graph algorithms\"\n";
	cout << "0 - exit.\n1 - Dijkstra.\n2 - NetPlanning.\n3 - Ford Falkerson.\n4 - Kruskal.\n";
	cout << "Your choice: ";
	cin >> choice;

	switch (choice)
	{
	case 0:
		return 0;
	case 1:
	{ //w/out { switch “transfer of control bypasses initialization of:” when calling a function
		int first = 1, last = 1;
		cout << "Enter start vertex and end vertex via key ENTER" << endl;
		cin >> first >> last;

		enter_filename();
		Graph_Dijkstra g_di(first, last);
		g_di.get_adjacency(file);
		g_di.make_solve();
		g_di.print_answer();
		break;
	}
	case 2:
	{
		int time = 0;
		cout << "Enter time: ";
		cin >> time;

		enter_filename();
		Graph_NetPlanning g_net(time);
		g_net.get_adjacency(file);
		g_net.make_solve();
		g_net.print_answer();
		break;
	}
	case 3:
	{
		enter_filename();
		Graph_Ford_Falk g_ff;
		g_ff.get_adjacency(file);
		g_ff.make_solve();
		g_ff.print_answer();
		break;
	}
	case 4:
	{
		bool choice;
		cout << "Enter 1 if your matrix is weight matrix. Enter 0 if adjacency matrix\n";
		cout << "Your choice: ";
		cin >> choice;
		//if (choice != 1 && choice != 0)
			//exit(0);

		enter_filename();
		Graph_Kruskal g_kr(choice);
		g_kr.get_adjacency(file);
		g_kr.make_solve();
		g_kr.print_answer();
		break;
	}
	default:
		break;
	}
	
	return 0;
}