#include "datatype.h"

bool Config::load() {
	ifstream fi;
	fi.open("data/config.gulu");
	if (!fi.is_open()) {
		cout << "Error: Missing config.gulu file\n" << endl;
		return false;
	}

	fi >> login_status; fi.ignore(100, '\n');
	getline(fi, curID);
	fi >> curPosition;
	fi >> keep_login;
	fi >> max_enterpw_turn;

	fi.close();
	return true;
}

void Config::update() {
	ofstream fo;
	fo.open("data/config.gulu");

	fo << login_status << '\n';
	fo << curID << '\n';
	fo << curPosition << '\n';
	fo << keep_login << '\n';
	fo << max_enterpw_turn;

	fo.close();
}

int AccountList::size() {
	int ret = 0;
	for (nodeAccount* iter = head; iter != nullptr; iter = iter->next, ++ret);
	return ret;
}

void AccountList::append(Account account) {
	if (head == nullptr)
		head = tail = new nodeAccount,
		head->prev = nullptr;
	else
		tail->next = new nodeAccount,
		tail->next->prev = tail,
		tail = tail->next;
	tail->account = account;
	tail->next = nullptr;
}

bool AccountList::load() {
	ifstream fi;
	fi.open("data/account.gulu");
	if (!fi.is_open()) {
		cout << "Error: Missing account.gulu file\n" << endl;
		return false;
	}

	head = tail = nullptr;
	while (!fi.eof()) {
		Account tmp;
		getline(fi, tmp.userID);
		getline(fi, tmp.password);
		fi >> tmp.position;
		fi.ignore(100, '\n');

		fi.ignore(100, '\n');

		append(tmp);
	}
	_delete(tail);

	fi.close();
	return true;
}

void AccountList::update() {
	ofstream fo;
	fo.open("data/account.gulu");
	for (nodeAccount* iter = head; iter != nullptr; iter = iter->next) {
		fo << iter->account.userID << '\n';
		fo << iter->account.password << '\n';
		fo << iter->account.position << '\n';

		fo << '\n';
	}
	fo.close();
}

void AccountList::_delete() {
	while (head != nullptr) {
		nodeAccount* tmp = head;
		head = head->next;
		delete tmp;
	}
	tail = nullptr;
}

void AccountList::_delete(nodeAccount* p) {
	if (head == p) head = head->next;
	if (tail == p) tail = tail->prev;
	if (p->next != nullptr)
		p->next->prev = p->prev;
	if (p->prev != nullptr)
		p->prev->next = p->next;
	delete p;
}

bool Date::input() {
	cout << "YYYY: ";
	cin >> year;
	if (year > 2020 || year < 1) return false;
	cout << "MM: ";
	cin >> month;
	if (month > 12 || month < 1) return false;
	cout << "DD: ";
	cin >> day;
	if (day > 31 || day < 1) return false;
	return true;
}

string Date::password() {
	string ret;
	int x = day;
	for (int i = 0; i < 2; ++i)
		ret = char(x % 10 + 48) + ret,
		x /= 10;
	x = month;
	for (int i = 0; i < 2; ++i)
		ret = char(x % 10 + 48) + ret,
		x /= 10;
	x = year;
	for (int i = 0; i < 4; ++i)
		ret = char(x % 10 + 48) + ret,
		x /= 10;
	
	return ret;
}

bool User::get_info() {
	ifstream fi;
	switch (position) {
	case 0: {
		fi.open("data/academic_staff.gulu");
		if (!fi.is_open()) {
			cout << "Error: Missing academic_staff.gulu file\n" << endl;
			return false;
		}

		string tmpID = ID;

		while (!fi.eof()) {
			getline(fi, ID);
			getline(fi, fullname);

			fi >> DoB.year;
			fi >> DoB.month;
			fi >> DoB.day;

			fi >> sex;
			fi.ignore(100, '\n');

			fi.ignore(100, '\n');

			if (tmpID == ID) {
				fi.close(); return true;
			}
		}

		fi.close();
		cout << "Error: Missing << " << ID << " in academic_staff.gulu\n" << endl;
		return false;
	}

	case 1: {
		fi.open("data/lecturer.gulu");
		if (!fi.is_open()) {
			cout << "Error: Missing lecturer.gulu file\n" << endl;
			return false;
		}

		while (!fi.eof()) {
			string tmpID;
			getline(fi, tmpID);
			getline(fi, fullname);
			fi >> DoB.year >> DoB.month >> DoB.day;
			fi >> sex;
			fi.ignore(100, '\n');
			fi.ignore(100, '\n');
			fi.ignore(100, '\n');

			if (tmpID == ID) {
				fi.close(); return true;
			}
		}

		fi.close();
		cout << "Error: Missing " << ID << " in lecturer.gulu\n" << endl;
		return false;
	}

	case 2: {
		fi.open("data/student.gulu");
		if (!fi.is_open()) {
			cout << "Error: Missing student.gulu file\n" << endl;
			return false;
		}

		string tmpID, tmpClassCode;
		bool found = false;

		while (!fi.eof()) {
			getline(fi, tmpID);
			getline(fi, tmpClassCode);
			fi.ignore(100, '\n');
			if (tmpID == ID) {
				found = true;
			}
		}

		fi.close();
		if (!found) {
			cout << "Error: Missing this user in student.gulu\n" << endl;
			return false;
		}

		fi.open("data/class/" + tmpClassCode + "-student.gulu");
		if (!fi.is_open()) {
			cout << "Error: Missing " << tmpClassCode << "-student.gulu file\n" << endl;
			return false;
		}

		tmpID = ID;

		while (!fi.eof()) {
			getline(fi, ID);
			getline(fi, fullname);

			fi >> DoB.year;
			fi >> DoB.month;
			fi >> DoB.day;

			fi >> sex;
			fi.ignore(100, '\n');
			fi.ignore(100, '\n');
		
			fi.ignore(100, '\n');

			if (tmpID == ID) {
				fi.close(); return true;
			}
		}

		fi.close();
		cout << "Error: Missing this user in " << tmpClassCode << "/student.gulu\n" << endl;
		return false;
	}
	}

	cout << "Error: User infomation not found\n" << endl;

	fi.close();
	return false;
}

void User::view_profile() {
	cout << "=========================================================\n";

	if (position < 2) {
		if (sex) cout << "Mr. ";
		else cout << "Ms. ";
	}
	cout << fullname << " (ID: " << ID << ")\n";

	cout << endl;

	cout << "D.o.B: ";
	month_in_text(DoB.month);
	cout << " " << DoB.day << ", " << DoB.year;
	cout << "\n";

	cout << "Sex: ";
	if (sex) cout << "Male\n"; else cout << "Female\n";

	cout << endl;

	cout << "Job title: ";
	if (position == 0) profileStaff();
	else if (position == 1) profileLecturer();
	else if (position == 2) profileStudent();

	cout << endl;

	cout << "=========================================================\n";

	system("pause");// Pause the console for reading purpose
}

void User::profileStaff() {
	cout << "Academic Staff\n";
}

void User::profileLecturer() {
	cout << "Lecturer\n";

	Lecturer lecturer;
	lecturer.general = *this;
	lecturer.load();

	cout << "Degree: " << lecturer.degree << '\n';
}

void User::profileStudent() {
	Student student;
	student.general = *this;
	student.load();

	cout << "Student (";
	switch (student.status) {
	case 0: {
		cout << "graduated";
		break;
	}
	case 1: {
		cout << "studying";
		break;
	}
	case 2: {
		cout << "reserved";
		break;
	}
	}
	cout << ")\n";

	cout << "Class: " << student.classID << '\n';

	int graduation_year = (student.classID[0] - 48) * 10 + (student.classID[1] - 48) + 4;
	cout << "Graduation year: " << graduation_year << "\n";
}

bool Student::load() {
	ifstream fi;
	fi.open("data/student.gulu");
	if (!fi.is_open()) {
		cout << "Error: Missing student.gulu file\n" << endl;
		return false;
	}

	bool found = false;

	while (!fi.eof()) {
		string tmpID;
		getline(fi, tmpID);
		getline(fi, classID);

		if (tmpID == general.ID) {
			found = true; break;
		}

		fi.ignore(100, '\n');
	}

	fi.close();
	if (!found) {
		cout << "Error: Missing " << general.ID << " in student.gulu\n" << endl;
		return false;
	}

	fi.open("data/class/" + classID + "-student.gulu");
	if (!fi.is_open()) {
		cout << "Error: Missing " << classID << "-student.gulu file\n" << endl;
		return false;
	}

	while (!fi.eof()) {
		string tmpID;
		getline(fi, tmpID);

		fi.ignore(100, '\n');
		fi.ignore(100, '\n');
		fi.ignore(100, '\n');

		fi >> status;
		fi.ignore(100, '\n');

		if (tmpID == general.ID) {
			fi.close(); return true;
		}

		fi.ignore(100, '\n');
	}

	cout << "Error: Missing " << general.ID << " in " << classID << "/student.gulu\n" << endl;
	return false;
}

int StudentList::size() {
	int ret = 0;
	for (nodeStudent* iter = head; iter != nullptr; iter = iter->next, ++ret);
	return ret;
}

int StudentList::count() {
	int ret = 0;
	for (nodeStudent* iter = head; iter != nullptr; iter = iter->next)
		ret += iter->student.status == 1;
	return ret;
}

void StudentList::append(Student student) {
	if (head == nullptr)
		head = tail = new nodeStudent,
		head->prev = nullptr;
	else
		tail->next = new nodeStudent,
		tail->next->prev = tail,
		tail = tail->next;
	tail->student = student;
	tail->next = nullptr;
}

bool StudentList::load(string classID) {
	ifstream fi;
	fi.open("data/class/" + classID + "-student.gulu");
	if (!fi.is_open()) {
		cout << "Error: Missing " << classID << "-student.gulu file\n" << endl;
		return false;
	}

	head = tail = nullptr;
	while (!fi.eof()) {
		Student tmp;

		getline(fi, tmp.general.ID);
		getline(fi, tmp.general.fullname);
		fi >> tmp.general.DoB.year >> tmp.general.DoB.month >> tmp.general.DoB.day;
		fi >> tmp.general.sex;
		fi >> tmp.status;
		fi.ignore();

		tmp.classID = classID;
		tmp.general.position = 2;

		fi.ignore(100, '\n');

		append(tmp);
	}
	_delete(tail);

	fi.close();
	return true;
}

void StudentList::update(string classID) {
	ofstream fo;
	fo.open("data/class/" + classID + "-student.gulu");
	for (nodeStudent* iter = head; iter != nullptr; iter = iter->next) {
		fo << iter->student.general.ID << '\n';
		fo << iter->student.general.fullname << '\n';
		fo << iter->student.general.DoB.year << ' ';
		fo << iter->student.general.DoB.month << ' ';
		fo << iter->student.general.DoB.day << '\n';
		fo << iter->student.general.sex << '\n';
		fo << iter->student.status << '\n';

		fo << '\n';
	}
	fo.close();
}

bool StudentList::loadAll() {
	ifstream fi;
	fi.open("data/student.gulu");
	if (!fi.is_open()) {
		cout << "Error: Missing student.gulu file\n" << endl;
		return false;
	}

	head = tail = nullptr;
	while (!fi.eof()) {
		Student tmp;

		getline(fi, tmp.general.ID);
		getline(fi, tmp.classID);

		fi.ignore(100, '\n');

		append(tmp);
	}
	_delete(tail);

	fi.close();
	return true;
}

void StudentList::updateAll() {
	ofstream fo;
	fo.open("data/student.gulu");
	for (nodeStudent* iter = head; iter != nullptr; iter = iter->next) {
		fo << iter->student.general.ID << '\n';
		fo << iter->student.classID << '\n';

		fo << '\n';
	}
	fo.close();
}

void StudentList::_delete() {
	while (head != nullptr) {
		nodeStudent* tmp = head;
		head = head->next;
		delete tmp;
	}
	tail = nullptr;
}

void StudentList::_delete(nodeStudent* p) {
	if (head == p) head = head->next;
	if (tail == p) tail = tail->prev;
	if (p->next != nullptr)
		p->next->prev = p->prev;
	if (p->prev != nullptr)
		p->prev->next = p->next;
	delete p;
}

bool StudentList::loadCourse(int academic_year, int semester, string classID, string courseID) {
	string ayearCode = AcademicYearCode(academic_year);
	string sCode = SemesterCode(semester);

	ifstream fi;
	fi.open("data/course/" + ayearCode + "-" + sCode + "-" + classID + "-" + courseID + "-enrolled.gulu");
	if (!fi.is_open()) {
		cout << "Error: Missing" << ayearCode << "-" << sCode << "-" << classID << "-" << courseID << "-enrolled.gulu file\n" << endl;
		return false;
	}

	head = tail = nullptr;
	while (!fi.eof()) {
		Student tmp;
		getline(fi, tmp.general.ID);
		getline(fi, tmp.classID);
		fi >> tmp.midtermGrade >> tmp.finalGrade >> tmp.bonusGrade >> tmp.totalGrade;

		tmp.attended = new int[10];
		for (int i = 0; i < 10; ++i)
			fi >> tmp.attended[i];

		fi.ignore(100, '\n');
		fi.ignore(100, '\n');

		append(tmp);
	}
	_delete(tail);

	fi.close();
	return true;
}

void StudentList::updateCourse(int academic_year, int semester, string classID, string courseID) {
	string ayearCode = AcademicYearCode(academic_year);
	string sCode = SemesterCode(semester);

	ofstream fo;
	fo.open("data/course/" + ayearCode + "-" + sCode + "-" + classID + "-" + courseID + "-enrolled.gulu");

	for (nodeStudent* iter = head; iter != nullptr; iter = iter->next) {
		fo << iter->student.general.ID << '\n';
		fo << iter->student.classID << '\n';

		fo << iter->student.midtermGrade << ' ';
		fo << iter->student.finalGrade << ' ';
		fo << iter->student.bonusGrade << ' ';
		fo << iter->student.totalGrade << '\n';

		for (int i = 0; i < 10; ++i)
			if (iter->student.attended == nullptr) fo << "0 ";
			else fo << iter->student.attended[i] << ' ';

		fo << "\n\n";
	}

	fo.close();
}

bool Lecturer::load() {
	ifstream fi;
	fi.open("data/lecturer.gulu");
	if (!fi.is_open()) {
		cout << "Error: Missing lecturer.gulu file\n" << endl;
		return false;
	}

	bool found = false;

	while (!fi.eof()) {
		string tmpID;
		getline(fi, tmpID);
		getline(fi, general.fullname);
		fi >> general.DoB.year >> general.DoB.month >> general.DoB.day;
		fi >> general.sex;
		fi.ignore(100, '\n');
		getline(fi, degree);

		fi.ignore(100, '\n');

		if (tmpID == general.ID) {
			found = true; break;
		}
	}

	fi.close();
	if (!found) {
		cout << "Error: Missing " << general.ID << " in lecturer.gulu\n" << endl;
		return false;
	}

	return true;
}

void Lecturer::view_course_list() {
	//There are still some problems here, when calling cl.load() 
	//it prints out missing erolled.gulu
	//it should have looked for the file schedule.gulu instead
	cout << "Please enter the academic year.\n";
	int academic_year; cin>>academic_year;
	cout <<"Please enter the semester (1,2 or 3)?\n";
	int semester; cin>>semester;
	cout <<"Please enter the code of the class?\n";
	string classID; cin>>classID;

	CourseList cl;
	if(!cl.load(academic_year, semester, classID)) return;

	CourseList::nodeCourse *nc = cl.head;
	cout << "There are " << cl.size() << " courses in this list:\n";
	while(nc!=nullptr)
	{
		cout << nc->course.ID << " - " << nc->course.name << '\n';
		nc = nc->next;
	}
	cl._delete();
}

void Lecturer::view_student_list() {
    cout << "Please enter the academic year.\n";
    int academic_year; cin>>academic_year;
    cout <<"Please enter the semester (1,2 or 3)?\n";
    int semester; cin>>semester;
    cout <<"Please enter the code of the class?\n";
    string classID; cin>>classID;
    cout << "Please enter the code of the course?\n";
    string courseID; cin>>courseID;
    StudentList sl;
    if(!sl.loadCourse(academic_year, semester, classID, courseID)) return;

    StudentList::nodeStudent *ns = sl.head;
    cout << "There are " << sl.size() << " students in this course:\n";
    while(ns!=nullptr)
    {
        User cur_student = ns->student.general;
        cur_student.view_profile();
        ns = ns->next;
    }
    sl._delete();
}

void Lecturer::view_attendance_list() {
    cout << "Please enter the academic year.\n";
    int academic_year; cin>>academic_year;
    cout <<"Please enter the semester (1,2 or 3)?\n";
    int semester; cin>>semester;
    cout <<"Please enter the code of the class?\n";
    string classID; cin>>classID;
    cout << "Please enter the code of the course?\n";
    string courseID; cin>>courseID;
    StudentList sl;
    if(!sl.loadCourse(academic_year, semester, classID, courseID)) return;
    StudentList::nodeStudent *ns = sl.head;
    cout << "          This is the attendance list of the course " << courseID << '\n';
    cout << "(Note that * means that student attends that class while - otherwise)"<<'\n';
    cout << "              There are 10 weeks in this course" << '\n';
    cout << "Student's ID  ";
    for(int i=1; i<=10; i++)
        if(i<10) cout << "W0" << i << " ";
        else cout << "W" << 10 << '\n';

    while(ns!=nullptr)
    {
        Student sd = ns->student;
        cout << "   "<<sd.general.ID << " ";
        for(int i=0; i<10; i++)
        {
            if(sd.attended[i]) cout <<"   "<< "*";
            else cout <<"   "<< "-";
        }
        cout << "\n\n";
        ns = ns->next;
    }

    sl._delete();
}

void Lecturer::edit_attendance() {
    cout << "Please enter the academic year.\n";
    int academic_year; cin>>academic_year;
    cout <<"Please enter the semester (1,2 or 3)?\n";
    int semester; cin>>semester;
    cout <<"Please enter the code of the class?\n";
    string classID; cin>>classID;
    cout << "Please enter the code of the course?\n";
    string courseID; cin>>courseID;
    StudentList sl;
    if(!sl.loadCourse(academic_year, semester, classID, courseID)) return;
    cout << "Please enter the student ID of the student you want to edit?\n";
    string SID; cin>>SID;
    StudentList::nodeStudent* cur = sl.head;
    bool found = false;
    while(cur!=nullptr) {
        if(cur->student.general.ID==SID) {
            found = true; break;
        }
        cur = cur->next;
    }
    if(!found) {
        sl._delete();
        cout << SID << " not found.\n";
        return;
    }
    //now cur points to that student
    cout << "This is the attended list of this student for ten weeks\n";
    cout << "* means attended while - means otherwise.\n";
    for(int i=0; i<10; i++)
        if(cur->student.attended[i]) cout <<"   "<< "*";
        else cout <<"   "<< "-";

    cout << "\nWhich week to you want to change? (Enter a number from 1 to 10)?\n";
    int change; cin>>change; change--;
    if(change<0 or change>9){
        cout << "You entered the wrong number\n";
        sl._delete();
        return;
    }
    cur->student.attended[change] = 1-cur->student.attended[change];
    sl.updateCourse(academic_year, semester, classID, courseID);
    cout << "Change made sucessfully.\n";
    sl._delete();
    return;
}

void Lecturer::edit_student_grade() {
    cout << "Please enter the academic year.\n";
    int academic_year; cin>>academic_year;
    cout <<"Please enter the semester (1,2 or 3)?\n";
    int semester; cin>>semester;
    cout <<"Please enter the code of the class?\n";
    string classID; cin>>classID;
    cout << "Please enter the code of the course?\n";
    string courseID; cin>>courseID;
    StudentList sl;
    if(!sl.loadCourse(academic_year, semester, classID, courseID)) return;
    cout << "Please enter the student ID of the student whose grades you want to edit?\n";
    string SID; cin>>SID;
    StudentList::nodeStudent* cur = sl.head;
    bool found = false;
    while(cur!=nullptr) {
        if(cur->student.general.ID==SID) {
            found = true; break;
        }
        cur = cur->next;
    }
    if(!found) {
        sl._delete();
        cout << SID << " not found.\n";
        return;
    }
    //now cur points to that student
    cout << "The current grades of that student:\n";
    cout << "Midterm: " << cur->student.midtermGrade << '\n';
    cout << "Final: " << cur->student.finalGrade << '\n';
    cout << "Bonus: " << cur->student.bonusGrade << '\n';
    cout << "Total: " << cur->student.totalGrade << '\n';

    cout << "Now enter the new grades of that student following the above order, sepearted by a space.\n";
    cin>>cur->student.midtermGrade>>cur->student.finalGrade>>cur->student.bonusGrade>>cur->student.totalGrade;

    sl.updateCourse(academic_year, semester, classID, courseID);
    cout << "Change made sucessfully.\n";
    sl._delete();
    return;
}

void Lecturer::view_scoreboard() {
    cout << "Please enter the academic year.\n";
    int academic_year; cin>>academic_year;
    cout <<"Please enter the semester (1,2 or 3)?\n";
    int semester; cin>>semester;
    cout <<"Please enter the code of the class?\n";
    string classID; cin>>classID;
    cout << "Please enter the code of the course?\n";
    string courseID; cin>>courseID;
    StudentList sl;
    if(!sl.loadCourse(academic_year, semester, classID, courseID)) return;

    StudentList::nodeStudent *ns = sl.head;
    cout << "          This is the scoreboard of the course " << courseID << "\n\n";

    cout << "Student's ID     Midterm      Final      Bonus       Total\n";

    while(ns!=nullptr)
    {
        Student sd = ns->student;
        cout << "   "<<sd.general.ID << " ";
        cout << "        "<<sd.midtermGrade << "           " << sd.finalGrade << "           " << sd.bonusGrade << "           " << sd.totalGrade;
        cout << "\n\n";
        ns = ns->next;
    }

    sl._delete();
}

void Lecturer::import_scoreboard() {
    cout << "Please enter the academic year.\n";
    int academic_year; cin>>academic_year;
    cout <<"Please enter the semester (1,2 or 3)?\n";
    int semester; cin>>semester;
    cout <<"Please enter the code of the class?\n";
    string classID; cin>>classID;
    cout << "Please enter the code of the course?\n";
    string courseID; cin>>courseID;
    StudentList sl;
    if(!sl.loadCourse(academic_year, semester, classID, courseID)) return;
    cout << "Please enter the name of the file (including its extension .csv).\n";
    string filename;
    cin.ignore();
    getline(cin, filename, '\n');
    //Assuming that we already have the name of the file
    ifstream myfile("csv files/"+filename);
    if(!myfile.is_open()) {
        cout << "Your path name is wrong!\n";
        sl._delete();
        return;
    }
    //skip the first line
    StudentList::nodeStudent *cur = sl.head;
    string dummy;
    getline(myfile, dummy, '\n');

    string sid, score;
    int midterm, final, bonus, total;
    while(!myfile.eof() and cur!=nullptr) {
        getline(myfile, sid, ',');
        getline(myfile, score, ',');
        cur->student.midtermGrade = stoi(score);
        getline(myfile, score, ',');
        cur->student.finalGrade = stoi(score);
        getline(myfile, score, ',');
        cur->student.bonusGrade = stoi(score);
        getline(myfile, score, '\n');
        cur->student.totalGrade = stoi(score);
        cur = cur->next;
    }
    sl.updateCourse(academic_year, semester, classID, courseID);
    sl._delete();
    cout << "Update successfully.\n";
}

int LecturerList::size() {
	int ret = 0;
	for (nodeLecturer* iter = head; iter != nullptr; iter = iter->next, ++ret);
	return ret;
}

void LecturerList::append(Lecturer lecturer) {
	if (head == nullptr)
		head = tail = new nodeLecturer,
		head->prev = nullptr;
	else
		tail->next = new nodeLecturer,
		tail->next->prev = tail,
		tail = tail->next;
	tail->lecturer = lecturer;
	tail->next = nullptr;
}

bool LecturerList::load() {
	ifstream fi;
	fi.open("data/lecturer.gulu");
	if (!fi.is_open()) {
		cout << "Error: Missing lecturer.gulu file\n" << endl;
		return false;
	}

	head = tail = nullptr;
	while (!fi.eof()) {
		Lecturer tmp;
		getline(fi, tmp.general.ID);
		getline(fi, tmp.general.fullname);
		
		fi >> tmp.general.DoB.year >> tmp.general.DoB.month >> tmp.general.DoB.day;
		fi >> tmp.general.sex;
		fi.ignore(100, '\n');

		getline(fi, tmp.degree);

		fi.ignore(100, '\n');

		append(tmp);
	}
	_delete(tail);

	fi.close();
	return true;
}

void LecturerList::update() {
	ofstream fo;
	fo.open("data/lecturer.gulu");
	for (nodeLecturer* iter = head; iter != nullptr; iter = iter->next) {
		fo << iter->lecturer.general.ID << '\n';
		fo << iter->lecturer.general.fullname << '\n';

		fo << iter->lecturer.general.DoB.year << ' ';
		fo << iter->lecturer.general.DoB.month << ' ';
		fo << iter->lecturer.general.DoB.day << '\n';

		fo << iter->lecturer.general.sex << '\n';
		fo << iter->lecturer.degree << '\n';

		fo << '\n';
	}
	fo.close();
}

void LecturerList::_delete() {
	while (head != nullptr) {
		nodeLecturer* tmp = head;
		head = head->next;
		delete tmp;
	}
	tail = nullptr;
}

void LecturerList::_delete(nodeLecturer* p) {
	if (head == p) head = head->next;
	if (tail == p) tail = tail->prev;
	if (p->next != nullptr)
		p->next->prev = p->prev;
	if (p->prev != nullptr)
		p->prev->next = p->next;
	delete p;
}

int ClassList::size() {
	int ret = 0;
	for (nodeClass* iter = head; iter != nullptr; iter = iter->next, ++ret);
	return ret;
}

void ClassList::append(string ID) {
	if (head == nullptr)
		head = tail = new nodeClass,
		head->prev = nullptr;
	else
		tail->next = new nodeClass,
		tail->next->prev = tail,
		tail = tail->next;
	tail->ID = ID;
	tail->next = nullptr;
}

bool ClassList::load() {
	ifstream fi;
	fi.open("data/class/class.gulu");
	if (!fi.is_open()) {
		cout << "Error: Missing class.gulu file\n" << endl;
		return false;
	}

	head = tail = nullptr;
	while (!fi.eof()) {
		string tmp;  getline(fi, tmp);
		append(tmp);
	}
	_delete(tail);

	fi.close();
	return true;
}

void ClassList::update() {
	ofstream fo;
	fo.open("data/class/class.gulu");
	for (nodeClass* iter = head; iter != nullptr; iter = iter->next)
		fo << iter->ID << '\n';
	fo.close();
}

void ClassList::_delete() {
	while (head != nullptr) {
		nodeClass* tmp = head;
		head = head->next;
		delete tmp;
	}
	tail = nullptr;
}

void ClassList::_delete(nodeClass* p) {
	if (head == p) head = head->next;
	if (tail == p) tail = tail->prev;
	if (p->next != nullptr)
		p->next->prev = p->prev;
	if (p->prev != nullptr)
		p->prev->next = p->next;
	delete p;
}

int CourseList::size() {
	int ret = 0;
	for (nodeCourse* iter = head; iter != nullptr; iter = iter->next, ++ret);
	return ret;
}

void CourseList::append(Course course) {
	if (head == nullptr)
		head = tail = new nodeCourse,
		head->prev = nullptr;
	else
		tail->next = new nodeCourse,
		tail->next->prev = tail,
		tail = tail->next;
	tail->course = course;
	tail->next = nullptr;
}

bool CourseList::load(int academic_year, int semester, string classID) {
	string ayearCode = AcademicYearCode(academic_year);
	string sCode = SemesterCode(semester);

	ifstream fi;
	fi.open("data/course/" + ayearCode + "-" + sCode + "-" + classID + "-schedule.gulu");
	if (!fi.is_open()) {
		cout << "Error: Missing " << ayearCode << "-" << sCode << "-" << classID << "-schedule.gulu file\n" << endl;
		return false;
	}

	head = tail = nullptr;
	while (!fi.eof()) {
		Course tmp;
		getline(fi, tmp.ID);
		getline(fi, tmp.name);
		getline(fi, tmp.lectureID);

		fi >> tmp.startDate.year >> tmp.startDate.month >> tmp.startDate.day;
		fi >> tmp.endDate.year >> tmp.endDate.month >> tmp.endDate.day;

		fi >> tmp.sessionDay;
		fi >> tmp.startTime.hour >> tmp.startTime.minute;
		fi >> tmp.endTime.hour >> tmp.endTime.minute;
		
		fi.ignore(100, '\n');

		getline(fi, tmp.room);

		fi.ignore(100, '\n');

		tmp.academic_year = academic_year;
		tmp.semester = semester;
		tmp.classID = classID;

		if (!tmp.studentList.loadCourse(academic_year, semester, classID, tmp.ID)) {
			fi.close(); return false;
		}

		append(tmp);
	}
	_delete(tail);

	fi.close();
	return true;
}

void CourseList::update(int academic_year, int semester, string classID) {
	string ayearCode = AcademicYearCode(academic_year);
	string sCode = SemesterCode(semester);

	ofstream fo;
	fo.open("data/course/" + ayearCode + "-" + sCode + "-" + classID + "-schedule.gulu");

	LecturerList lecturerList;
	lecturerList.load();

	AccountList accountList;
	accountList.load();

	for (nodeCourse* iter = head; iter != nullptr; iter = iter->next) {
		fo << iter->course.ID << '\n';
		fo << iter->course.name << '\n';
		fo << iter->course.lectureID << '\n';

		fo << iter->course.startDate.year << ' ';
		fo << iter->course.startDate.month << ' ';
		fo << iter->course.startDate.day << '\n';

		fo << iter->course.endDate.year << ' ';
		fo << iter->course.endDate.month << ' ';
		fo << iter->course.endDate.day << '\n';

		fo << iter->course.sessionDay << '\n';
		
		fo << iter->course.startTime.hour << ' ';
		fo << iter->course.startTime.minute << '\n';
		
		fo << iter->course.endTime.hour << ' ';
		fo << iter->course.endTime.minute << '\n';

		fo << iter->course.room << '\n';

		fo << '\n';

		iter->course.studentList.updateCourse(academic_year, semester, classID, iter->course.ID);

		bool found = false;
		for (auto p = lecturerList.head; p != nullptr; p = p->next)
			if (p->lecturer.general.ID == iter->course.lectureID) { found = true; break; }
		if (!found) {
			cout << "We found that " << iter->course.lectureID << " hasn't been added to the system, please help us fulfill the information\n" << endl;

			Lecturer newLecturer;

			cout << "Lecturer ID: " << iter->course.lectureID << '\n';
			newLecturer.general.ID = iter->course.lectureID;

			cout << "Lecturer's full name: ";
			getline(cin, newLecturer.general.fullname);

			cout << "Lecturer's degree: ";
			getline(cin, newLecturer.degree);

			cout << "Date of Birth:\n";
			newLecturer.general.DoB.input();

			cout << "Sex (0 Female / 1 Male): ";
			cin >> newLecturer.general.sex;
			cin.ignore();

			newLecturer.general.position = 1;

			lecturerList.append(newLecturer);

			Account newAccount;

			newAccount.userID = newLecturer.general.ID;
			newAccount.password = newLecturer.general.ID;
			newAccount.position = 1;

			accountList.append(newAccount);

			cout << '\n';
		}
	}

	lecturerList.update();
	lecturerList._delete();

	accountList.update();
	accountList._delete();

	fo.close();
}

void CourseList::_delete() {
	while (head != nullptr) {
		nodeCourse* tmp = head;
		head = head->next;
		delete tmp;
	}
	tail = nullptr;
}

void CourseList::_delete(nodeCourse* p) {
	if (head == p) head = head->next;
	if (tail == p) tail = tail->prev;
	if (p->next != nullptr)
		p->next->prev = p->prev;
	if (p->prev != nullptr)
		p->prev->next = p->next;
	delete p;
}
