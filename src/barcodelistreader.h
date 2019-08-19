#ifndef  BARCODE_LIST_READER_H
#define BARCODE_LIST_READER_H

//includes
#include<cctype>
#include<string>
#include<map>
#include<iostream>
#include<fstream>

using namespace std;

class BarcodeListReader
{
public:
	BarcodeListReader(string barcodeListFile, bool forceUpperCase = true);
	~BarcodeListReader();
	void readAll();

public:
	string mCurrentSequence;
	string mCurrentID;
	map<string, int> barcode_map;

private:
	string mBarcodeListFile;
	ifstream mBarcodeListFileStream;
	bool mForceUpperCase;
};

#endif