#ifndef MCCLUSKEY_H
#define MCCLUSKEY_H
#include <vector>
#include <iostream>
#include <set>
#include "BinaryCode.h"
#include <iomanip>
#include <fstream>
using namespace std;

class McCluskey
{
public:
	McCluskey(vector<int>);

	struct implicant
	{
		bool isPrime = true; //prime: true
		int oneNum = 0;
		set< int > n; //implicant with any size; m(1,5,7,10)
		vector< int > s; //example: 2001 -> -001
	};

	using implicantlist = vector< implicant >;
	
	void outputMin(int);

	void output();
private:
	int bits;

	vector< int > m; //min term
	vector< int > d; //don't care

	//push with boolean code
	void push(implicantlist&, int, vector< bool >);


	vector< implicantlist > fullImplicantList; //full implicant list ;
	vector< binarycode::binarycodeType > bCode;

	implicantlist prime; //prime implicant
	int min = 99999;
	//recursive find prime implicant
	void findPrime(int);

	//compare two implicant and combine them (push to implicantList)
	bool compare(int, int, int);

	vector< set< int > > minPossibleList; //list all possible combanition


	void doPetrick();
	vector<vector<bool>> Index;
	void findPossible(vector<bool>, vector<vector<int>>&, int, int, set<int>);

	void outputProcess(vector< vector< int > >&);
};

McCluskey::McCluskey(vector<int> sop)
{
	bits = 0;
	for ( size_t s = sop.size(); s != 1; s /= 2 )++bits;
	binarycode tempB(bits);
	bCode = tempB.getCode();
	fullImplicantList.resize(1);
	for ( int i = 0; i < sop.size(); ++i ) //set m
		if ( sop[i] == 1 ) {
			m.push_back(i);
			push(fullImplicantList[0], i, bCode[i]);
		}
	for ( int i = 0; i < sop.size(); ++i ) //set d
		if ( sop[i] == 2 ) {
			d.push_back(i);
			push(fullImplicantList[0], i, bCode[i]);
		}

	findPrime(1);
	doPetrick();
}

void McCluskey::findPrime(int index)
{
	if ( index + 1 >= bits || fullImplicantList[index - 1].size() == 0 ) { //all finished, push prime
		for ( int i = 0; i < fullImplicantList.size(); ++i ) {
			for ( int j = 0; j < fullImplicantList[i].size(); ++j ) {
				if ( fullImplicantList[i][j].isPrime )
					prime.push_back(fullImplicantList[i][j]);
			}
		}
		return;
	}
	fullImplicantList.resize(fullImplicantList.size() + 1);
	for ( int i = 0; i < fullImplicantList[index - 1].size(); ++i ) {
		for ( int j = 0; j < fullImplicantList[index - 1].size(); ++j ) {
			if ( fullImplicantList[index - 1][i].oneNum < fullImplicantList[index - 1][j].oneNum )
				if ( compare(index, i, j) )
					fullImplicantList[index - 1][i].isPrime = fullImplicantList[index - 1][j].isPrime = false;
		}
	}
	findPrime(index + 1);
}

bool McCluskey::compare(int index, int first, int second)
{
	int bit = -1; //bit of different

	for ( int i = 0; i < bits; ++i ) {
		if ( fullImplicantList[index - 1][first].s[i] != fullImplicantList[index - 1][second].s[i] ) {
			if ( bit == -1 )
				bit = i;
			else
				return false;
		}
	}
	fullImplicantList[index].push_back(fullImplicantList[index - 1][first]);
	fullImplicantList[index].back().isPrime = true;
	fullImplicantList[index].back().oneNum -= fullImplicantList[index - 1][first].s[bit] ? 1 : 0;
	fullImplicantList[index].back().s[bit] = 2;

	fullImplicantList[index].back().n.insert(fullImplicantList[index - 1][second].n.begin(),
											 fullImplicantList[index - 1][second].n.end());
	for ( int i = 0; i < fullImplicantList[index].size() - 1; ++i )
		if ( fullImplicantList[index][i].n == fullImplicantList[index].back().n ) {
			fullImplicantList[index].pop_back();
			break;
		}
		
	return true;
}

void McCluskey::doPetrick()
{
	vector<vector<int>> table;
	
	table.resize(m.size());
	for ( int i = 0; i < prime.size(); ++i ) { //create table https://en.wikipedia.org/wiki/Petrick%27s_method
		for ( set<int>::iterator it = prime[i].n.begin(); it != prime[i].n.end(); ++it ) {
			for ( int j = 0; j < m.size(); ++j )
				if ( m[j] == *it ) {
					table[j].push_back(i);
					break;
				}
		}
	}
	
	Index.resize(prime.size());
	set<int> temp; 
	for ( int i = 0; i < table.size(); ++i ) { //把所有一定會用到的row放入temp
		if ( table[i].size() == 1 ) {
			temp.insert(table[i][0]);
		}
	}
	bool isDelete;
	for ( vector<vector<int>>::iterator it = table.begin(); it != table.end(); ) { //將擁有一定會用到row的選項刪掉
		isDelete = false;
		for ( vector<int>::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2) {
			if ( temp.find(*it2) != temp.end() ) {
				it = table.erase(it);
				isDelete = true;
				break;
			}
		}
		if ( !isDelete )
			++it;
	}
	vector<bool> enableIndex(table.size(), false);
	for ( int i = 0; i < Index.size(); ++i )
		Index[i].resize(table.size());
	for ( int i = 0; i < table.size(); ++i ) {
		for ( int j = 0; j < table[i].size(); ++j ) {
			Index[table[i][j]][i] = true;
		}
	}
	if ( table.size() > 0 )
		for ( int j = 0; j < table[0].size(); ++j )
			findPossible(enableIndex, table, 0, j, temp);
	else
		minPossibleList.push_back(temp);
	outputProcess(table);
}

void McCluskey::findPossible(vector<bool> enableIndex, vector<vector<int>>& table, int ti, int tj, set<int> temp)
{
	
	temp.insert(table[ti][tj]);
	if ( min <= temp.size() )
		return;
	if ( ti + 1 != table.size() ) {
		for ( int i = ti; i < enableIndex.size(); ++i ) {
			if ( !enableIndex[i] )
				enableIndex[i] = Index[table[ti][tj]][i];
		}
		while ( ti + 1 < enableIndex.size() )
			if ( enableIndex[ti + 1] )
				++ti;
			else
				break;
		if ( ti + 1 != table.size() ) {
			for ( int j = 0; j < table[ti + 1].size(); ++j )
				findPossible(enableIndex, table, ti + 1, j, temp);
		}
	}

	if ( ti + 1 == table.size() ) {
		if ( min <= temp.size() )
			return;
		else if ( min > temp.size() ) {
			min = temp.size();
			minPossibleList.clear();
		}
		bool isIn = false;
		for ( int i = 0; i < minPossibleList.size(); ++i )
			if (minPossibleList[i] == temp ) {
				isIn = true;
				break;
			}
		if ( !isIn ) {
			
			minPossibleList.push_back(temp);

			/*
			cout << temp.size() << " : ";
			for ( set<int>::iterator it = temp.begin(); it != temp.end(); ++it )
				cout << *it << " ";
			cout << endl;
			*/
		}
		return;
	}

}

void McCluskey::output()
{
	/*
	for ( int i = 0; i < minPossibleList.size(); ++i ) {
		cout << endl << "(" << i + 1 << ")" << endl;
		for ( set< int >::iterator it = minPossibleList[i].begin(); it != minPossibleList[i].end(); ++it ) {
			for ( int j = bits - 1; j >= 0; --j )
				cout << prime[*it].s[j];
			cout << endl;
		}
	}
	*/
	for ( set< int >::iterator it = minPossibleList[0].begin(); it != minPossibleList[0].end(); ++it ) {
		for ( int j = bits - 1; j >= 0; --j )
			cout << prime[*it].s[j];
		cout << endl;
	}
}

void McCluskey::outputMin(int i)
{
	ofstream outFile("output", ios::app);
	vector< vector<int> > result;
	vector<int> buffer;
	buffer.resize(bits);
	outFile << '#' << i + 1 << endl;
	for ( set< int >::iterator it = minPossibleList[0].begin(); it != minPossibleList[0].end(); ++it ) {
		for ( int j = bits - 1; j >= 0; --j ) {
			outFile << prime[*it].s[j];
		}
		outFile << endl;
	}
}

void McCluskey::push(implicantlist& list, int N, vector<bool> S)
{
	int back = list.size();
	list.resize(back + 1);
	list[back].n.insert(N);
	for ( int i = 0; i < S.size(); ++i ) {
		if ( S[i] ) {
			list[back].s.push_back(1);
			++(list[back].oneNum);
		}
		else
			list[back].s.push_back(0);
	}
}

void McCluskey::outputProcess(vector< vector< int > >& table)
{
	//https://en.wikipedia.org/wiki/Quine%E2%80%93McCluskey_algorithm 
	//step 1
	cout << "step 1 :";
	int i = 0;
	for ( vector<implicantlist>::iterator it1 = fullImplicantList.begin(); it1 != fullImplicantList.end(); ++it1 ) {
		cout << endl << setw(4) << "<1>" << setw(3 + pow(2, i) * 3) << "m" << setw(10) << "implicant";
		for ( implicantlist::iterator it2 = (*it1).begin(); it2 != (*it1).end(); ++it2 ) {
			cout << endl << setw(4) << (*it2).oneNum << "   ";
			for ( set<int>::iterator it3 = (*it2).n.begin(); it3 != (*it2).n.end(); ++it3 )
				cout << setw(3) << *it3 ;
			cout << setw(6);
			for ( vector<int >::iterator it4 = (*it2).s.begin(); it4 != (*it2).s.end(); ++it4 ) {
				if ( *it4 == 2 )
					cout << "-";
				else
					cout << *it4;
			}
			if ( (*it2).isPrime )
				cout << "*";
		}
		++i;
		cout << endl;
	}
	// step 2
	cout << endl << "step 2 :" << endl;
	for ( int i = 0; i < m.size(); ++i )
		cout << setw(3) << m[i];
	for ( int i = 0; i < prime.size(); ++i ) {
		cout << endl;
		for ( int j = 0; j < m.size(); ++j ) {
			if ( prime[i].n.find(m[j]) != prime[i].n.end() )
				cout << "  *";
			else
				cout << "   ";
		}
	}
	cout << endl;

	cout << endl << "table : ";
	for ( int i = 0; i < table.size(); ++i ) {
		cout << endl << "(" << i << ")";
		for ( int j = 0; j < table[i].size(); ++j ) {
			cout << table[i][j] << " ";
		}
	}
	cout << endl;

	cout << endl << "find possible combinations :" << endl;
	for ( int i = 0; i < minPossibleList.size(); ++i ) {
		for ( set<int>::iterator it = minPossibleList[i].begin(); it != minPossibleList[i].end(); ++it )
			cout << *it << " ";
		cout << endl;
	}
	cout << endl;

}

#endif // !MCCLUSKEY_H

