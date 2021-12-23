#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <sstream>

using namespace std;

typedef string vertex;
struct edge
{
	vertex start;
	vertex end;
	string diff;
};
typedef unordered_map<vertex, vector<edge>> adjacency_list;
struct attributes
{
	int distance;
	vertex path;
};

void read_words(adjacency_list &graph, ifstream& file);
void construct_edges(adjacency_list& graph);
void compute_insertions(adjacency_list& graph, vertex key);
void compute_deletions(adjacency_list& graph, vertex key);
void compute_swaps(adjacency_list& graph, vertex key);
string build_swap_diff_string(char c, int pos, char to);
string build_insert_diff_string(char c, int pos);
string build_deletion_diff_string(char c, int pos);
void find_paths(unordered_map<vertex, attributes>& bst_tree, adjacency_list& graph, vertex word);
void trace_path(adjacency_list& graph, unordered_map<vertex, attributes>& bfs_tree, vertex v);
string find_edge(adjacency_list& graph, vertex from, vertex to);

int main()
{
	ifstream db_file("words_medium.txt");
	if (db_file.fail()) return -1;

	adjacency_list graph;

	read_words(graph, db_file);
	construct_edges(graph);
	db_file.close();

	string input, a, b;
	cout << ">> ";
	while (getline(cin, input))
	{
		istringstream lineStream(input);
		lineStream >> a;
		lineStream >> b;

		unordered_map<vertex, attributes> bfs_tree;
		find_paths(bfs_tree, graph, a);
		trace_path(graph, bfs_tree, b);
		cout << endl << ">> ";
	}

	return 0;
}

void read_words(adjacency_list& graph, ifstream& file)
{
	string word;
	while (file >> word)
		graph[word] = {};
}

void construct_edges(adjacency_list& graph)
{
	for (const auto &item : graph)
	{
		vertex key = item.first;
		compute_insertions(graph, key);
		compute_deletions(graph, key);
		compute_swaps(graph, key);
	}
}

void compute_insertions(adjacency_list& graph, vertex key)
{
	string derivation;
	for (unsigned char c = 'a'; c <= 122; c++)
	{
		for (int i = 0; i <= key.length(); i++)
		{
			derivation = key;
			string str = " ";
			str[0] = c;
			derivation.insert(i, str);
			if (graph.find(derivation) != graph.end())
			{
				vertex new_vertex = derivation;
				graph[key].push_back({ key, new_vertex, build_insert_diff_string(c, i) });
			}
		}
	}
}


void compute_deletions(adjacency_list& graph, vertex key)
{
	string derivation;
	for (int i = 0; i < key.length(); i++)
	{
		derivation = key;
		derivation.erase(i, 1);
		if (graph.find(derivation) != graph.end())
		{
			vertex new_vertex = derivation;
			graph[key].push_back({ key, new_vertex, build_deletion_diff_string(key[i], i)});
		}
	}
}


void compute_swaps(adjacency_list& graph, vertex key)
{
	string derivation;
	for (unsigned char c = 'a'; c <= 122; c++)
	{
		for (int i = 0; i < key.length(); i++)
		{
			derivation = key;
			derivation[i] = c;
			if (key != derivation && graph.find(derivation) != graph.end())
			{
				vertex new_vertex = derivation;
				graph[key].push_back({ key, new_vertex, build_swap_diff_string(key[i], i, c)});
			}
		}
	}
}

string build_swap_diff_string(char c, int pos, char to)
{
	ostringstream s;
	s << "(change " << c << " at" << " position " << pos + 1 << " to " << to << ")";
	return s.str();
}

string build_insert_diff_string(char c, int pos)
{
	ostringstream s;
	s << "(insert " << c << " after" << " position " << pos << ")";
	return s.str();
}

string build_deletion_diff_string(char c, int pos)
{
	ostringstream s;
	s << "(delete " << c << " at" << " position " << pos + 1 << ")";
	return s.str();
}

void find_paths(unordered_map<vertex, attributes>& bst_tree, adjacency_list& graph, vertex word)
{
	queue<vertex> q;
	vertex v, w;
	q.push(word);
	bst_tree[word] = { 0, "" };

	while (!q.empty())
	{
		v = q.front();
		q.pop();

		for (auto e : graph[v])
		{
			w = e.end;
			if (bst_tree.find(w) == bst_tree.end())
			{
				bst_tree[w] = {};
				bst_tree[w].distance = bst_tree[v].distance + 1;
				bst_tree[w].path = v;
				q.push(w);
			}
		}
	}
}

void trace_path(adjacency_list &graph, unordered_map<vertex, attributes> &bfs_tree, vertex v)
{
	stack<vertex> path;

	if (bfs_tree[v].path == "")
	{
		cout << "There is no path!" << endl;
	}
	else
	{
		while (bfs_tree[v].path != "")
		{
			path.push(v);
			v = bfs_tree[v].path;
		}
		path.push(v);
	}

	vertex current, previous = "";
	while (!path.empty())
	{
		current = path.top();
		path.pop();
		cout << current << " ";
		if (previous != "")
		{
			cout << find_edge(graph, previous, current) << endl;
		}
		else
		{
			cout << endl;
		};

		previous = current;
	}
}

string find_edge(adjacency_list& graph, vertex from, vertex to)
{
	for (auto edge : graph[from])
	{
		if (edge.end == to)
			return edge.diff;
	}
	return "";
}
