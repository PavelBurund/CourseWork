#include "Graph.h"

int main() 
{
	setlocale(LC_ALL, "RUS");
	Graph flow;

	flow.take_file("C:/Users/LENOVO/source/repos/Ford_Falk/Ford_Falk/text.txt");

	flow.get_flowSize();

	system("pause");
}