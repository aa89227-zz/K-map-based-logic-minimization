#ifndef BINARYCODE_H
#define BINARYCODE_H

#include <iostream>
#include <vector>
using namespace std;

class binarycode
{
public:
	using binarycodeType = vector< bool >;
	binarycode(size_t);
	vector< binarycodeType > getCode() { return code; }
	int getSize() { return size; }
	void output()
	{
		cout << "size: " << size << endl;
		for ( int i = 0; i < code.size(); ++i ) {
			for ( int j = size - 1; j >= 0; --j )
				cout << code[i][j];
			cout << " " << i << endl;
		}
	}
private:
	size_t size;
	vector< binarycodeType > code;
};

binarycode::binarycode(size_t Size)
	:size(Size)
{
	int loopNum = pow(2, size);
	binarycodeType temp(size, false);
	code.push_back(temp);
	for ( int i = 1; i < loopNum; ++i ) {
		for ( int j = 0; j < size; ++j ) {
			if ( temp[j] )
				temp[j] = false;
			else {
				temp[j] = true;
				break;
			}
		}
		code.push_back(temp);
	}
}

#endif // !BINARYCODE_H

