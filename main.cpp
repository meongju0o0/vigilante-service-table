#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <random>
using namespace std;

const int ammunition = 8;  //ź��� �ٹ��ð�
const int guardroom = 3;  //�Ĺ� �ٹ��ð�
const int vigil = 2;  //��ħ�� �ٹ��ð�
const int onDuty = 1;  //������� �ٹ��ð�

/*
	1. �� �ٹ�ǥ�� �ߴ� ������ 2�� ���� �ۼ��ȴ�.
	2. ź���->�Ĺ�->ź���->�Ĺ�...(5��� �Ҵ�� �˾Ƽ� ���Ѵ�.)
	3. ź��� �ٹ��� �Ϸ翡 12���� ������ 3�����̴�.
	4. ź��� �ٹ��� �Ѱ� Ÿ�ӿ� 4���̸� 30�� �� �����̴�.
	5. ���������� ������ ���Ұ�.
	6. �ּ� �ߴ� �ο��� 12(ź���) + 5(��ħ��) + 1(������) = 18�� �̴�.
	7. �������� �����̴�.
	8. ���� ���� ������ �Ҽ� �Ҵ뵵 �߰��� ����
*/

class Serviceman {
private:
	string rank = "???";  //���
	string name = "???";  //�̸�
	bool chkUsable = true;  //���� ���� (false => ����)
	string inferiority_reason = "???"; //���� ����
	int workingHr = 0;
public:
	Serviceman(string _rank = "???", string _name = "???")
		:rank(_rank), name(_name) {

	}

	virtual string getRank() const {
		return rank;
	}

	virtual string getName() const {
		return name;
	}

	virtual int getHr() const {
		return workingHr;
	}

	virtual bool chk_inf() const {
		return chkUsable;
	}

	virtual void inferiority() {
		chkUsable = false;
	}

	virtual void doWork() {
		chkUsable = true;
	}

	virtual void plusHr(int num) {
		workingHr += num;
	}

	virtual void printInfo() const {
		cout << this->getRank() << '\t' << this->getName() << '\t' << this->getID() << '\t';
		if (this->chk_inf())
			cout << "����" << endl;
		else
			cout << "����" << endl;
	}

	virtual string getID() const = 0;
};

class Cadre : public Serviceman {
	//���� Ŭ����
	//�������(�ߴ� ���� ���ۼ��̹Ƿ� ��������� ����)
private:
	string ID = "??-??????";  //����
public:
	Cadre(string _rank = "???", string _name = "???", string _ID = "??-??????")
		:Serviceman(_rank, _name), ID(_ID) {

	}

	virtual string getID() const override {
		return ID;
	}
};

class Soldier : public Serviceman {
	//���� Ŭ����
	//ź���, 5���, ������, ��ħ��
private:
	string ID = "??-????????";  //����
public:
	Soldier(string _rank = "???", string _name = "???", string _ID = "??-????????")
		:Serviceman(_rank, _name), ID(_ID) {

	}

	virtual string getID() const override {
		return ID;
	}
};

vector<Serviceman*> getMinList(vector<Serviceman*>& vec) {
	int smallest = vec.at(0)->getHr();
	vector<Serviceman*> minList;

	for (int i = 1; i < vec.size(); ++i)
		smallest = min(smallest, vec.at(i)->getHr());

	for (int i = 0; i < vec.size(); i++)
		if (smallest == vec.at(i)->getHr())
			minList.push_back(vec.at(i));

	return minList;
}

class SoldierHandler {
private:
	vector<Serviceman*> cadreList, soldierList;
	string** workSch = NULL;
public:
	SoldierHandler() { }

	void readFile() {
		string rank, name, ID, inferiority;

		ifstream fin1("cadreList.txt");
		if (!fin1) {
			cout << "textFile Error!!" << endl;
			cout << "Program Exit!!" << endl;
			exit();
		}

		ifstream fin2("soldierList.txt");
		if (!fin2) {
			cout << "textFile Error!!" << endl;
			cout << "Program Exit!!" << endl;
			exit();
		}

		while (fin1 >> rank >> name >> ID >> inferiority) {
			cadreList.push_back(new Cadre(rank, name, ID));
			if (inferiority == "����")
				cadreList.back()->inferiority();
			else if (inferiority == "����")
				cadreList.back()->doWork();
			else {
				cout << "textFile Error!!" << endl;
				cout << "Program Exit!!" << endl;
				exit();
			}
		}

		while (fin2 >> rank >> name >> ID >> inferiority) {
			soldierList.push_back(new Soldier(rank, name, ID));
			if (inferiority == "����")
				soldierList.back()->inferiority();
			else if (inferiority == "����")
				soldierList.back()->doWork();
			else {
				cout << "textFile Error!!" << endl;
				cout << "Program Exit!!" << endl;
				exit();
			}
		}

		fin1.close();
		fin2.close();
	}

	int chkID(string& ID) {
		if (ID[2] == '-' && (ID.length() == 9 || ID.length() == 11)) {
			if (ID.length() == 9) {
				for (int i = 0; i < cadreList.size(); i++)
					if (ID == cadreList.at(i)->getID())
						return i;
			}
			
			else if (ID.length() == 11) {
				for (int i = 0; i < soldierList.size(); i++)
					if (ID == soldierList.at(i)->getID())
						return i;
			}
					
			return -1;  //-1��ȯ -> �������� ���� ������ �Էµ� ���
		}

		else {
			cout << "������ �ùٸ��� �ʽ��ϴ�." << endl;
			return -2;  // -2��ȯ -> �ùٸ��� ���� ������ �Էµ� ���
		}
	}

	void showMenu() const {
		cout << "---------Menu---------" << endl;
		cout << "1. ����" << endl;
		cout << "2. ����" << endl;
		cout << "3. ����" << endl;
		cout << "4. �ٹ� ����" << endl;
		cout << "5. ������ ���" << endl;
		cout << "6. ���������ɼ� ����" << endl;
		cout << "7. ���α׷� ����" << endl;
	}

	void enlistment() {
		int sel;
		string rank, name, ID;

		do {
			cout << "   [����]   " << endl;
			cout << "1. ���� ����" << endl;
			cout << "2. ���� ����" << endl;
			cout << "3. ���" << endl;
			cout << "����: ";
			cin >> sel;
			cout << endl;
		} while (sel != 1 && sel != 2 && sel != 3);

		if (sel == 1) {
			cout << "[���� ����]" << endl;
			cout << "���: ";
			cin >> rank;
			cout << "�̸�: ";
			cin >> name;
			cout << "����: ";
			cin >> ID;
			cout << endl;
			if (ID.length() == 9) {
				if (chkID(ID) == -1)
					cadreList.push_back(new Cadre(rank, name, ID));
				else if (chkID(ID) >= 0)
					cout << "�̹� �����ϴ� �����Դϴ�." << endl << endl;
			}

			else {
				cout << "������ �ùٸ��� �ʽ��ϴ�." << endl << endl;
				return;
			}
		}

		else if (sel == 2) {
			cout << "[���� ����]" << endl;
			cout << "���: ";
			cin >> rank;
			cout << "�̸�: ";
			cin >> name;
			cout << "����: ";
			cin >> ID;
			cout << endl;
			if (ID.length() == 11) {
				if (chkID(ID) == -1)
					soldierList.push_back(new Soldier(rank, name, ID));
				else if (chkID(ID) >= 0)
					cout << "�̹� �����ϴ� �����Դϴ�." << endl << endl;
			}

			else {
				cout << "������ �ùٸ��� �ʽ��ϴ�." << endl << endl;
				return;
			}
		}

		else if (sel == 3) {
			return;
		}
	}

	void discharge() {
		string ID;
		cout << "[����]" << endl;
		cout << "��� �ÿ� ������ ������ X�� �Է��Ͻÿ�." << endl;
		cout << "������ ����: ";
		cin >> ID;
		cout << endl;

		if (ID.length() == 9) {  //����
			if (chkID(ID) >= 0)
				cadreList.erase(cadreList.begin() + chkID(ID));
		}

		else if (ID.length() == 11) {  //����
			if (chkID(ID) >= 0)
				soldierList.erase(soldierList.begin() + chkID(ID));
		}

		else
			return;
	}

	void inferiority() {
		string ID;
		cout << "[����]" << endl;
		cout << "��� �ÿ� ������ ������ X�� �Է��Ͻÿ�." << endl;
		cout << "������ ����: ";
		cin >> ID;
		cout << endl;

		if (ID.length() == 9) {  //����
			if (chkID(ID) >= 0)
				cadreList.at(chkID(ID))->inferiority();
		}

		else if (ID.length() == 11) {  //����
			if (chkID(ID) >= 0)
				soldierList.at(chkID(ID))->inferiority();
		}

		else
			return;
	}

	void doWork() {
		string ID;
		cout << "[�ٹ� ����]" << endl;
		cout << "��� �ÿ� ������ ������ X�� �Է��Ͻÿ�." << endl;
		cout << "�ٹ� ������ ����: ";
		cin >> ID;
		cout << endl;

		if (ID.length() == 9) {  //����
			if (chkID(ID) >= 0)
				cadreList.at(chkID(ID))->doWork();
		}

		else if (ID.length() == 11) {  //����
			if (chkID(ID) >= 0)
				soldierList.at(chkID(ID))->doWork();
		}

		else
			return;
	}

	void printSoldiers() const {
		cout << "[���� ���]" << endl;
		cout << "���\t����\t����\t\t��������" << endl;
		for (int i = 0; i < cadreList.size(); i++)
			cadreList.at(i)->printInfo();
		cout << endl;

		cout << "[���� ���]" << endl;
		cout << "���\t����\t����\t\t��������" << endl;
		for (int i = 0; i < soldierList.size(); i++)
			soldierList.at(i)->printInfo();
		cout << endl;
	}

	void makeTable() {
		random_device rd;
		mt19937 gen(rd());

		int day;
		int work;
		int selected_soldier;
		cout << "�� �ٹ� �ϼ��� �Է��Ͻÿ�." << endl;
		cout << "�Է�: ";
		cin >> day;
		cout << "�ٹ� ���¸� �Է��Ͻÿ�." << endl;
		cout << "1. ź���" << endl;
		cout << "2. �Ĺ�" << endl;
		cin >> work;
		cout << endl;

		workSch = new string* [day];
		if (work == 1) {
			for (int i = 0; i < day; i++)
				workSch[i] = new string[17];  //1Ÿ 4��, 2Ÿ 4��, 3Ÿ 4��, ��ħ�� 5�� -> 17��

			for (int i = 0; i < day; i++) {
				for (int j = 0; j < 17; j++) {
					vector<Serviceman*> workerList = getMinList(soldierList);  //�ٹ� ���� �ð��� ���� ���� ���縦 ��� ����Ʈ ����
					uniform_int_distribution<int> dis(0, workerList.size() - 1);  //������ ����Ʈ ������ŭ ���� ���� ����
					selected_soldier = dis(gen);
					workSch[i][j] = workerList.at(selected_soldier)->getName();  //�߻��� ������ ������� ���� �ð��� ���� ���� ���� �� 1���� �����Ͽ� �����ٿ� �ۼ�
					workerList.at(selected_soldier)->plusHr(ammunition);
				}  //�̸� 17�� �ݺ� (1�� �з� ���ۼ� �ϼ�)
			}  //�̸� day ��ŭ �ݺ� (day �з� ���ۼ� �ϼ�)
			for (int i = 0; i < 17; i++) {
				for (int j = 0; j < day; j++) {
					cout << workSch[j][i] << '\t';
				}
				cout << endl;
			}
		}

		else if (work == 2) {
			for (int i = 0; i < day; i++)
				workSch[i] = new string[13];  //1Ÿ 2��, 2Ÿ 2��, 3Ÿ 2��, 4Ÿ 2��, ��ħ�� 5�� -> 13��

			for (int i = 0; i < day; i++) {
				for (int j = 0; j < 13; j++) {
					vector<Serviceman*> workerList = getMinList(soldierList);  //�ٹ� ���� �ð��� ���� ���� ���縦 ��� ����Ʈ ����
					uniform_int_distribution<int> dis(0, workerList.size() - 1);  //������ ����Ʈ ������ŭ ���� ���� ����
					selected_soldier = dis(gen);
					workSch[i][j] = workerList.at(selected_soldier)->getName();  //�߻��� ������ ������� ���� �ð��� ���� ���� ���� �� 1���� �����Ͽ� �����ٿ� �ۼ�
					workerList.at(selected_soldier)->plusHr(ammunition);
				}  //�̸� 17�� �ݺ� (1�� �з� ���ۼ� �ϼ�)
			}  //�̸� day ��ŭ �ݺ� (day �з� ���ۼ� �ϼ�)
			for (int i = 0; i < 17; i++) {
				for (int j = 0; j < day; j++) {
					cout << workSch[j][i] << '\t';
				}
				cout << endl;
			}
		}
	}

	void exit() const {
		ofstream fout1("cadreList.txt");
		ofstream fout2("soldierList.txt");

		for (int i = 0; i < cadreList.size(); i++) {
			fout1 << cadreList.at(i)->getRank() << '\t' << cadreList.at(i)->getName() << '\t' << cadreList.at(i)->getID() << '\t';
			if (cadreList.at(i)->chk_inf())
				fout1 << "����" << endl;
			else
				fout1 << "����" << endl;
		}

		for (int i = 0; i < soldierList.size(); i++) {
			fout2 << soldierList.at(i)->getRank() << '\t' << soldierList.at(i)->getName() << '\t' << soldierList.at(i)->getID() << '\t';
			if (soldierList.at(i)->chk_inf())
				fout2 << "����" << endl;
			else
				fout2 << "����" << endl;
		}

		fout1.close();
		fout2.close();
	}
};

int main() {
	SoldierHandler handle;
	int sel;
	handle.readFile();

	while (true) {
		handle.showMenu();
		cout << "����: ";
		cin >> sel;
		cout << endl;

		if (sel == 1) {
			handle.enlistment();
		}
		else if (sel == 2) {
			handle.discharge();
		}
		else if (sel == 3) {
			handle.inferiority();
		}
		else if (sel == 4) {
			handle.doWork();
		}
		else if (sel == 5) {
			handle.printSoldiers();
		}
		else if (sel == 6) {
			handle.makeTable();
		}
		else if (sel == 7) {
			handle.exit();
			return 0;
		}
	}

	return 0;
}