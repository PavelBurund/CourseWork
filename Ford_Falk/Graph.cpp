#include "Graph.h"

Graph::~Graph() 
{
	delete[] flow;

	items.clear();

	file_lines.clear();

	way_to_finished.clear();
};

//Принятие файла
void Graph::take_file(string file) 
{
	read_file(file);
	create_graph();
};
//Возвращает значение потока(максимум)
int Graph::get_flowSize() 
{
	int flow = counting_flow();
	cout << "Максимальный поток в транспортной сети:" << flow << endl << endl;
	return flow;
}
//Чтение файла
void Graph::read_file(string pathToFile) 
{
	ifstream file(pathToFile);

	if (!file.is_open())
		throw ios_base::failure("Файл не открывается!");

	string line;
	string start_point = "", finish_point = "";
	int item_number = 1; // code of city

	while (getline(file, line)) 
	{
		file_lines.enqueue(line);

		string item_one = get_word(line);
		string item_two = get_word(line);

		if (item_one == "S") 
		{
			start_point = item_one;
			continue;
		}

		if (item_two == "T") 
		{
			finish_point = item_two;
			continue;
		}
		//Если конечная точка и начальная равны это петля(ошибка)
		if (item_one == item_two) throw invalid_argument("Начальная и конечная точки равны");
		//Начальной точкой не может быть T
		if (item_one == "T") throw invalid_argument("Неправильный формат ввода");
		//Конечной точкой не может быть S
		if (item_two == "S") throw invalid_argument("Неправильный формат ввода");

		if (!items.key_check(item_one)) items.insert(item_one, item_number++);
		if (!items.key_check(item_two)) items.insert(item_two, item_number++);
	}

	if (!items.get_size())
		throw invalid_argument("Файл пустой!!");

	if (start_point != "S") throw logic_error("Нет начала!");
	else items.insert(start_point, 0);

	if (finish_point != "T") throw logic_error("Нет конца!");
	else items.insert(finish_point, items.get_size());

	file.close();
};
//Создание графа
void Graph::create_graph() 
{
	string point_one, point_two; 
	int channel_width; 

	flow = new flowStruct[items.get_size()];
	bool first = true;
	while (!file_lines.isEmpty()) 
	{
		read_line(file_lines.dequeue(), point_one, point_two, &channel_width);

		int index_one = items.find_value(point_one);
		int index_two = items.find_value(point_two);

        flow[index_one].ways.insert(flow[index_one].ways.get_size(), index_two);

		if (!flow[index_two].channel_width.key_check(index_one))
			flow[index_two].channel_width.insert(index_one, channel_width);
		else
			flow[index_two].channel_width.Value_Add(index_one, channel_width);
	}
};

int Graph::counting_flow() 
{
	int max_index;
	int max_bandwidth;
	int start_index = 0;

	int MAX_FLOW = INT_MIN;
	do {
		find_any_way:
			max_index = INT_MIN;
			max_bandwidth = INT_MIN;

			for (int i = 0; i < flow[start_index].ways.get_size(); i++)
			{
				int index = flow[start_index].ways.find_value(i);
				int channel_width = flow[index].channel_width.find_value(start_index);

				if (channel_width > max_bandwidth && !flow[index].checked)
				{
					max_index = index;
					max_bandwidth = channel_width;
				}
			}

			if (max_index != INT_MIN)
			{
				if (!create_way(max_index)) goto find_any_way;

				if (update_flow(&MAX_FLOW)) break; // В случае если нет пути в сток
			}

		
	} while (max_index != INT_MIN);
	if (MAX_FLOW == INT_MIN)
		throw invalid_argument("Нет связи между истоком и стоком");
	return MAX_FLOW;
}

bool Graph::create_way(int start_index)
{
	if (start_index == items.get_size() - 1)
	{
		way_to_finished.push_back(start_index);
		return true;
	}
	bool find_any_way = true;
	while (find_any_way)
	{
		int max_index = INT_MIN;
		int max_bandwidth = INT_MIN;

		for (int i = 0; i < flow[start_index].ways.get_size(); i++)
		{

			int index = flow[start_index].ways.find_value(i);
			int channel_width = flow[index].channel_width.find_value(start_index);

			if (channel_width > max_bandwidth && !flow[index].checked)
			{
				max_index = index;
				max_bandwidth = channel_width;
			}
		}

		flow[start_index].checked = true;

		if (max_index != INT_MIN)
		{
			if (!create_way(max_index)) find_any_way = true;
			else
			{
				find_any_way = false;
				way_to_finished.push_back(start_index);
				return true;
			}
		}
		else return false;
	}
}

bool Graph::update_flow(int* MAX_FLOW) 
{
	bool channel_is_empty = false;
	int min_bandwidth = INT_MAX;

	//Создание массива пройденных путей
	int prev_index = 0;
	int* indexes = new int[way_to_finished.get_size()];

	for (int i = way_to_finished.get_size() - 1; i >= 0; i--) 
	{
		indexes[i] = way_to_finished.at(i);

		int channel_width = flow[indexes[i]].channel_width.find_value(prev_index);
		if (channel_width < min_bandwidth && channel_width)
			min_bandwidth = channel_width;

		prev_index = indexes[i];
	}

	prev_index = 0;

	for (int i = way_to_finished.get_size() - 1; i >= 0; i--) 
	{
		flow[indexes[i]].channel_width.Value_Sub(prev_index, min_bandwidth);

		if ((indexes[i] == items.get_size() - 1) && !flow[indexes[i]].channel_width.find_value(prev_index))
			channel_is_empty = true;

		if (flow[indexes[i]].checked && flow[indexes[i]].channel_width.find_value(prev_index))
			flow[indexes[i]].checked = false;

		prev_index = indexes[i];
	}

	if (min_bandwidth > 0 && *MAX_FLOW == INT_MIN)
		*MAX_FLOW = 0;

	*MAX_FLOW = *MAX_FLOW + min_bandwidth;

	way_to_finished.clear();

	return channel_is_empty;
}
//Получение слова(до пробела или до конца строки)
string Graph::get_word(string& route) 
{
	int i = 0;
	while (route[i] != ' ' && route[i] != '\0') i++;

	string word = route.substr(0, i);
	route.replace(0, word.length() + 1, "");

	return word;
};
//Чтение строки
void Graph::read_line(string line, string& item_one, string& item_two, int* channel_width) 
{
	item_one = get_word(line); 
	item_two = get_word(line); 

	string bw = get_word(line); 

	*channel_width = stoi(bw); 

	if (*channel_width <= 0) throw invalid_argument("Пропускная способность не может быть меньше 0");
};