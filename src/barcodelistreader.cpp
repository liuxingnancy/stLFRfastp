#include"barcodelistreader.h"
#include "util.h"
#include <sstream>

BarcodeListReader::BarcodeListReader(string barcodeListFile, bool forceUpperCase)
{
	// Set local and disable stdio synchronization to improve iostream performance
	setlocale(LC_ALL, "C");
	ios_base::sync_with_stdio(false);

	mBarcodeListFile = barcodeListFile;
	mForceUpperCase = forceUpperCase;
	if (is_directory(mBarcodeListFile)) {
		string error_msg = "There is a problem with the provided barcode list file: \'";
		error_msg.append(mBarcodeListFile);
		error_msg.append("\' is a derectory Not a file...\n");
		throw invalid_argument(error_msg);
	}
	mBarcodeListFileStream.open(mBarcodeListFile.c_str(), ios::in);
	//verify that the file can be read
	if (!mBarcodeListFileStream.is_open()) {
		string msg = "There is a problem with the provided barcode list file: could NOT read ";
		msg.append(mBarcodeListFile.c_str());
		msg.append("...\n");
		throw invalid_argument(msg);
	}
}

BarcodeListReader::~BarcodeListReader()
{
	if (mBarcodeListFileStream.is_open())
		mBarcodeListFileStream.close();
	if (barcode_map.size() > 0) {
		barcode_map.clear();
		map<string, int>().swap(barcode_map);
	}
}

void BarcodeListReader::readAll()
{
	string base4[5] = { "A", "T", "C", "G", "N" };
	string barcodename;
	int barcodeid;
	while (mBarcodeListFileStream >> barcodename) {
		mBarcodeListFileStream >> barcodeid;
		barcode_map[barcodename] = barcodeid;
		for (int i = 0; i < barcodename.length(); i++) {
			for (int b = 0; b < base4->length(); b++) {
				string misbarcodename = barcodename;
				misbarcodename.replace(i, 1, base4[b]);
				barcode_map[misbarcodename] = barcodeid;
			}
		}
	}
}

