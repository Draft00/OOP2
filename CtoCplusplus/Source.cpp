#include <iostream>
#include "Graph.h"

using namespace std;

int main()
{
	Graph g;
	g.get_adjacency("g_test.txt");
	cout << "Welcome to programm \"Graph algorithms\"\n";
	return 0;
}