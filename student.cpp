#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <fstream>
#include <ctime>

using namespace std;

string join(const vector<string> &v, const string &del) {
	string ans = "";

	for (int i = 0; i < v.size()-1; i++) ans += v[i] + del;

	return ans + v[v.size()-1];
}

vector<string> readFile(string filepath) {
	vector<string> ans;
	ifstream in(filepath);
	string val;

	while (!in.eof()) {
		in >> val;
		ans.push_back(val);
	}

	in.close();

	return ans;
}

class Fullname {
public:
	string name, lastname;
	string toString() {
		return name + " " + lastname;
	}
};

class Date {
public:
	int dd,mm,yyyy;
	string toString() {
		vector<string> ans = {
			(dd+1 < 10 ? "0" : "") + to_string(dd+1),
			(mm+1 < 10 ? "0" : "") + to_string(mm+1),
			to_string(yyyy+1)
		};

		return join(ans, ".");
	}
};

class Student {
public:
	int id;
	Fullname fullname;
	Date dateOfBirth;
	string university;
	string faculty;
	int course;
	int room;

	string toString() {
		vector<string> ans = {
			to_string(id) + ":",
			fullname.toString(),
			dateOfBirth.toString(),
			join({university, faculty, to_string(course)}, "/"),
			to_string(room)
		};

		return join(ans, " ");
	}
};

class Generator {
public:
	Generator(string namesFilePath, string lnamesFilePath, string uniFilePath, string facFilePath) {
		names = readFile(namesFilePath);
		lastnames = readFile(lnamesFilePath);
		universities = readFile(uniFilePath);
		faculties = readFile(facFilePath);

		lastId = 1e6;
		room3Id = 3;

		srand(time(nullptr));
		rnd = [](int a, int b) -> int {
			return (((rand() & 0x7fff) << 15) | (rand() & 0x7fff) )% (b-a) + a;
		};
	}

	Student getStudent() {
		Student st;
		st.id = lastId++;
		st.fullname = getFullname();
		st.dateOfBirth = getDate();
		st.course = rnd(1,5);
		st.university = universities[rnd(0, universities.size())];
		st.faculty = faculties[rnd(0, faculties.size())];
		st.room = room3Id/3;
		room3Id++;
		return st;
	}

	Fullname getFullname() {
		Fullname fn;
		fn.name = names[rnd(0, names.size())];
		fn.lastname = lastnames[rnd(0, lastnames.size())];
		return fn;
	}

	Date getDate() {
		Date dt;
		dt.dd = rnd(0, 28);
		dt.mm = rnd(0, 12);
		dt.yyyy = rnd(1970, 2000);
		return dt;
	}

private:
	vector<string> names, lastnames, universities, faculties;
	function<int(int,int)> rnd;
	int lastId, room3Id;
};
