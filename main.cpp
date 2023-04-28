#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <functional>

#include "student.cpp"

using namespace std;

string keyToStr(int x) {
	return to_string(x);
}

string keyToStr(Fullname x) {
	return x.toString();
}

string valToStr(Student* st) {
	return to_string(st->id);
}

template<class K, class V>
class Node {
public:
	K key;
	V value;
	Node<K,V>* left;
	Node<K,V>* right;
	int leftLen, rightLen;
	function<bool(K,K)> isLess;
	
	Node(K k, V v, function<bool(K,K)> cmp) {
		key = k;
		value = v;
		left = nullptr;
		right = nullptr;
		leftLen = 0;
		rightLen = 0;
		isLess = cmp;
	}

	void push(Node<K,V>* nd) {
		if (isLess(nd->key, this->key)) {
			leftLen++;

			if (left == nullptr) {
				left = nd;
				return;
			}

			left->push(nd);
			balance();
			return;
		}

		rightLen++;

		if (right == nullptr) {
			right = nd;
			return;
		}

		right->push(nd);
		balance();
		return;
	}

	vector<V> find(K k) {
		vector<V> ans;
		if (isLess(k, this->key)) {
			if (left != nullptr) {
				return left->find(k);
			}
			return ans;
		}

		if (isLess(this->key, k)) {
			if (right != nullptr) {
				return right->find(k);
			}
			return ans;
		}

		if (left != nullptr) {
			ans = left->find(k);
		}

		ans.push_back(this->value);

		if (right != nullptr) {
			auto rightAns = right->find(k);

			for (auto x : rightAns) {
				ans.push_back(x);
			}
		}

		return ans;
	}

	void print() {
		cout << keyToStr(key) << ' ' << value->toString() << '\n'; 
	}

private:
	void balance() {
		if (leftLen-rightLen > 1) {
			if (left->rightLen <= left->leftLen) {
				rotateRight();
				return;
			}

			left->rotateLeft();
			rotateRight();
			return;
		}

		if (rightLen-leftLen > 1) {
			if (right->leftLen <= right->rightLen) {
				rotateLeft();
				return;
			}

			left->rotateRight();
			rotateLeft();
			return;
		}
	}

	void rotateLeft() {
		swap(key, right->key);
		swap(value, right->value);
		Node<K,V>* l = left;
		left = right;
		right = left->right;
		left->right = left->left;
		left->left = l;
		setLen();
		left->setLen();
	}

	void rotateRight() {
		swap(key, left->key);
		swap(value, left->value);
		auto r = right;
		right = left;
		left = right->left;
		right->left = right->right;
		right->right = r;
		setLen();
		right->setLen();
	}

	void setLen() {
		if (left == nullptr) {
			leftLen = 0;
		} else {
			leftLen = max(left->leftLen, left->rightLen)+1;
		}
		
		if (right == nullptr) {
			rightLen = 0;
		} else {
			rightLen = max(right->leftLen, right->rightLen)+1;
		}
	}
};

template<class K, class V>
class AVL {
public:
	Node<K,V>* root;
	function<bool(K,K)> isLess;

	AVL(function<bool(K,K)> cmp) {
		root = nullptr;
		isLess = cmp;
	}

	void push(K key, V val) {
		auto nd = new Node<K,V>(key, val, isLess);

		if (root == nullptr) {
			root = nd;
			return;
		}
		root->push(nd);
	}

	vector<V> find(K key) {
		vector<V> ans;
		if (root == nullptr) {
			return ans;
		}

		return root->find(key);
	}

	void print() {
		if (root == nullptr) {
			return;
		}

		queue<Node<K,V>*> q;
		q.push(root);

		while (!q.empty()) {
			Node<K,V>* cur = q.front();
			q.pop();
			cur->print();

			if (cur->left != nullptr) {
				q.push(cur->left);
			}
			if (cur->right != nullptr) {
				q.push(cur->right);
			}
		}

		cout << '\n';
	}
};

int main() {
	Generator gen("db/names.txt", "db/lastnames.txt", "db/universities.txt", "db/faculties.txt");

	int n;
	cout << "Enter db size:\n";
	cin >> n;

	AVL<int, Student*> idIndex([](int a, int b) -> bool {
		return a < b;
	});
	AVL<Fullname, Student*> fullnameIndex([](Fullname a, Fullname b) -> bool {
		if (a.lastname == b.lastname) {
			return a.name < b.name;
		}
		return a.lastname < b.lastname;
	});

	cout << "Full db:\n";
	Student stdb[n];
	for (int i = 0; i < n; i++) {
		Student st = gen.getStudent();
		stdb[i] = (st);
		idIndex.push(st.id, &(stdb[i]));
		fullnameIndex.push(st.fullname, &(stdb[i]));
		cout << st.toString() << '\n';
	}
	cout << '\n';

	int k;
	cout << "Enter queries count:\n";
	cin >> k;
	cout << "Write queries in one of these formats (without brackets):\n";
	cout << "\"id {studentID}\"\n";
	cout << "\"name {name} {lastname}\"\n";

	for (int i = 0; i < k; i++) {
		string type;
		vector<Student*> ans;
		cin >> type;

		if (type == "id") {
			int id;
			cin >> id;
			ans = idIndex.find(id);
		} else {
			string name, lastname;
			cin >> name >> lastname;
			Fullname key;
			key.name = name;
			key.lastname = lastname;
			ans = fullnameIndex.find(key);
		}

		for (auto st : ans) {
			cout << st->toString() << '\n';
		}
	}

	return 0;
}
