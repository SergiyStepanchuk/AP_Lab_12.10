#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

struct Node {
	Node* left = nullptr;
	Node* right = nullptr;
	int data;
};

int count_of_nodes(Node*& start) 
{
	if (start != nullptr)
		return 1 + count_of_nodes(start->left) + count_of_nodes(start->right);
	return 0;
}

void add_nodes_tree(Node*& start, int size) {
	if (start != nullptr) {
		if (count_of_nodes(start->left) > count_of_nodes(start->right))
			add_nodes_tree(start->right, size);
		else add_nodes_tree(start->left, size);
		return;
	}
	if (size < 1)
		return;
	start = new Node();
	cout << "Input node int: ";
	cin >> start->data;
	if (size > 0) {
		add_nodes_tree(start->left, size / 2);
		if (size > 1)
			add_nodes_tree(start->right, size - size / 2 - 1);
	}
}

bool remove_tree_node(Node*& start, const int &val, bool find = false) {
	bool state[2] = {false, false};
	if (start == nullptr)
		return find;

	if (!find && start->data == val)
		find = true;

	if (start->left != nullptr)
		state[0] = remove_tree_node(start->left, val, find);

	if (start->right != nullptr)
		state[1] = remove_tree_node(start->right, val, find);

	if (find) {
		delete start;
		start = nullptr;
	}

	return find || state[0] || state[1];
}

bool search_tree_node(Node* start, const int& val) {
	if (start == nullptr)
		return false;
	if (start->data == val)
		return true;
	return search_tree_node(start->left, val) || search_tree_node(start->right, val);
}

void PrintTree(Node* root, int level)
{
	if (root == nullptr) return;
	
	PrintTree(root->right, level + 1);
	for (int i = 1; i <= level; i++)
		cout << " ";
	cout << root->data << endl;
	PrintTree(root->left, level + 1);
}

void save_tree(fstream& stream, Node* node) {
	if (!node) {
		stream.write("\0\21\1\0\56\21\12\14\52\76", 10);
		return;
	}
	stream.write((char *)&node->data, sizeof(int));
	save_tree(stream, node->left);
	save_tree(stream, node->right);
}

void load_tree(fstream& stream, Node*& node) {
	char tmp[10] = {};
	stream.read(tmp, 10);
	if (!memcmp(tmp, "\0\21\1\0\56\21\12\14\52\76", 10))
		return;
	stream.seekg(-10, ios::cur);
	node = new Node();
	stream.read((char*)&node->data, sizeof(int));
	load_tree(stream, node->left);
	load_tree(stream, node->right);
}

Node* get_predok(Node* cur, const int& some, const int& some2, bool* has, int& findlevel, const int &level = 1) {
	if (cur == nullptr)
		return nullptr;
	if (!has[0] && cur->data == some)
		has[0] = true;
	if (!has[1] && cur->data == some2)
		has[1] = true;
	
	if (has[0] && has[1])
		return nullptr;

	bool hhh[2] = { false, false };
	Node* tmp = get_predok(cur->left, some, some2, hhh, findlevel, level+1);
	if (tmp == nullptr && (!hhh[0] || !hhh[1]))
		tmp = get_predok(cur->right, some, some2, hhh, findlevel, level + 1);
	//cout << cur->data << " " << hhh[0] << " " << hhh[1] << " " << tmp << endl;
	if (tmp == nullptr)
	{
		if (hhh[0] && hhh[1]) {
			findlevel = level;
			return cur;
		}
		else {
			if (hhh[0])
				has[0] = true;
			if (hhh[1])
				has[1] = true;
		}
	}
	
	return tmp;
		
}

void main() {
	Node* nodes = nullptr;
	int state;
	char message[256] = {0};

	do
	{
		if (message[0] != 0) {
			cout << message << endl;
			message[0] = 0;
		}
		else cout << endl;
		PrintTree(nodes, 1);
		cout << "[1] Add nodes" << endl;
		cout << "[2] Remove node" << endl;
		cout << "[3] Search node" << endl;
		cout << "[4] Save to file" << endl;
		cout << "[5] Load from file" << endl;
		cout << "[6] Edit" << endl;
		cout << "[0] Exit" << endl;
		cout << "Select: ";
		do
		{
			if (cin.fail()) {
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}
			cin >> state;
		} while (cin.fail());
		switch (state) {
		case 1: {
			int count;
			cout << "Input nodes count: ";
			do
			{
				if (cin.fail()) {
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}
				cin >> count;
			} while (cin.fail() || count < 1);
			
			add_nodes_tree(nodes, count);
		}break;
		case 2: 
		{
			int val;
			cout << "Input node value: ";
			do
			{
				if (cin.fail()) {
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}
				cin >> val;
			} while (cin.fail());

			if (remove_tree_node(nodes, val))
				sprintf_s(message, 255, "Node with val %d succesfully removed!", val);
			else sprintf_s(message, 255, "Node not found!");
		}
			break;
		case 3:
		{
			int val;
			cout << "Input search node value: ";
			do
			{
				if (cin.fail()) {
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}
				cin >> val;
			} while (cin.fail());

			if (search_tree_node(nodes, val))
				sprintf_s(message, 255, "Node with val %d succesfully find!", val);
			else sprintf_s(message, 255, "Node not found!");
		}
		break;
		case 4:
		{
			string file_name;
			cout << "Input save file name: ";
			cin >> file_name;
			fstream f(file_name, ios::out | ios::binary);
			if (f.is_open()) {
				save_tree(f, nodes);
				f.close();
				sprintf_s(message, 255, "Nodes succesfully saved!");
			}else sprintf_s(message, 255, "Can't open file!");
			
		}
			break;
		case 5:
		{
			if (nodes != nullptr)
				remove_tree_node(nodes, 0, true);
			string file_name;
			cout << "Input load file name: ";
			cin >> file_name;
			fstream f(file_name, ios::in | ios::binary);
			if (f.is_open()) {
				load_tree(f, nodes);
				f.close();
				sprintf_s(message, 255, "Nodes succesfully loaded!");
			}
			else sprintf_s(message, 255, "Can't open file!");

		}
			break;
		case 6: {
			int val[2];
			cout << "Input first node value: ";
			do
			{
				if (cin.fail()) {
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}
				cin >> val[0];
			} while (cin.fail());
			cout << "Input second node value: ";
			do
			{
				if (cin.fail()) {
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}
				cin >> val[1];
			} while (cin.fail());
			bool hhh[2] = {false, false};
			int findlevel;
			Node* m = get_predok(nodes, val[0], val[1], hhh, findlevel);
			if (m)
				sprintf_s(message, 255, "Node find with value %d and has level %d", m->data, findlevel);
			else sprintf_s(message, 255, "Node not find!");
		}
			break;
		case 0:
			break;
		default:
			sprintf_s(message, 255, "Incorrect action!");
		}
		

	} while (state != 0);
}