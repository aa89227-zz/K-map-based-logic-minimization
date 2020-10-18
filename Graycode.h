#ifndef GRAYCODE_H
#define GRAYCODE_H

#include <iostream>
#include <vector>
#include "BinaryCode.h"
using namespace std;

class graycode
{
public:
	using graycodeList = vector< int >;
	graycode(size_t);
	
	graycodeList getList() { return list; }
	void output();
	
private:
	graycodeList list;
	void doNext();
	binarycode bCode;
};

graycode::graycode(size_t size)
	:bCode(size)
{
	list.push_back(0);
	for ( int i = 1; i < pow(2, size); ++i ) 
		doNext();
}

void graycode::doNext()
{
	vector< binarycode::binarycodeType > temp = bCode.getCode();
	if ( list.size() % 2 != 0 ) {
		if ( temp[list.back()][0] )
			list.push_back(list.back() - 1);
		else 
			list.push_back(list.back() + 1);
	}
	else {
		for ( int i = 0; i < bCode.getSize(); ++i ) {
			if ( temp[list.back()][i] ) {
				++i;
				if ( temp[list.back()][i] )
					list.push_back(list.back() - pow(2, i));
				else
					list.push_back(list.back() + pow(2, i));
				return;
			}
		}
	}

}

inline void graycode::output()
{
	vector< binarycode::binarycodeType > temp = bCode.getCode();
	for ( int i = 0; i < list.size(); ++i ) {
		for ( int j = bCode.getSize() - 1; j >= 0; --j )
			cout << temp[list[i]][j];
		cout << " " << list[i] << endl;
	}
}

#endif // !GRAYCODE_H
