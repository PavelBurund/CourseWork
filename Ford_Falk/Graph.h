#pragma once
#include <string>
#include <fstream>
#include <stdio.h>
#include <iterator>
#include <limits.h>
#include <exception>

#include "Map.h"
#include "List.h"
#include "Queue.h"

using namespace std;

class Graph
{
public:
	Graph() {};
	~Graph();

	void take_file(string);

	int get_flowSize();

private:
	class flowStruct
	{
	public:
		flowStruct() : checked(false) {}

		bool checked;

		Map <int, int> channel_width;

		Map <int, int> ways;
	};

	flowStruct *flow;

	List<int> way_to_finished;

	Map <string, int> items;

	Queue <string> file_lines;

	void read_file(string);

	void create_graph();

	int counting_flow();

	bool create_way(int);

	bool update_flow(int*);

	string get_word(string&);

	void read_line(string, string&, string&, int*);
};