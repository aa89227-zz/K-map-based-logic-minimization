#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "BinaryCode.h"
#include "Graycode.h"
#include "McCluskey.h"

using namespace std;

vector< int > kmapToSop(vector< int >); //把 k-map format 轉成 sop format

int main()
{
	int testCaseNum = 0;
	int bits = 0;
	string filename;
	cout << "FileName : ";
	cin >> filename;
	ifstream inFile(filename, ios::in);
	stringstream buffer;
	string str;

	vector< int > kmap;
	vector< int > sop;
	vector< McCluskey* > mccluskey;
	buffer << inFile.rdbuf();
	inFile.close();
	getline(buffer, str, '\n');
	testCaseNum = std::atoi(str.c_str());

	mccluskey.resize(testCaseNum);
	ofstream outFile("output", ios::out);

	for ( int i = 0; i < testCaseNum; ++i ) {
		kmap.clear();
		getline(buffer, str, '\n');
		for ( int j = 0; j < str.size(); j++ ) {
			if ( str[j] != ' ' )
				kmap.push_back(str[j] - '0');
		}
		sop = kmapToSop(kmap);
		mccluskey[i] = new McCluskey(sop);
		mccluskey[i]->outputMin(i);
	}

	
	
	for ( int i = 0; i < mccluskey.size(); ++i ) {
		cout << "#" << i + 1 << endl;
		mccluskey[i]->output();
		delete mccluskey[i];
	}
	
}

vector< int > kmapToSop(vector< int > kmap)
{
	size_t size = 0; // bits count
	for ( size_t s = kmap.size(); s != 1; s /= 2 ) ++size;
	vector< int > result(kmap.size());

	graycode gCode(size);
	int height = pow(2, size / 2); //k-map height
	int width = kmap.size() / height; //k-map width
	/*
	kmap :
	→→→→
	→→→→
	→→→→
	→→→→
	graycode:
	→→→↓
	↓←←←
	→→→↓
	←←←←
	按照 graycode 順序(i) 讀取kmap的index
	將 result[graycode[i]] 設為 kmap[index]
	*/
	for ( int i = 0, index = 0; i < height; ++i ) {
		for ( int j = 0; j < width; ++j ) {
			result[gCode.getList()[i * width + j]] = kmap[index];
			index += ((i + 1) % 2) * 1 + (i % 2) * (-1);
		}
		index = (i + 1) * width + ((i + 1) % 2) * (width - 1);
	}
	return result;
}
