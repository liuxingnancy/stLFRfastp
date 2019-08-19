#ifndef BARCODE_PROCESSOR_H
#define BARCODE_PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include "options.h"
#include "read.h"
#include "filterresult.h"
#include <map>

using namespace std;

class BarcodeProcessor {
public:
	BarcodeProcessor(Options* opt);
	~BarcodeProcessor();
	void process(Read* read1, Read* read2, FilterResult* fr);
	void addBarcodeToName(Read* r1, Read* r2, string barcode);
	static bool test();

public:
	Options* mOptions;
	int barcodeStart;
	int barcodeLength;
	int barcodeSkip;
	map<string, int> barcode_map;
};

#endif
