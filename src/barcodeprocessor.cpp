#include "barcodeprocessor.h"

BarcodeProcessor::BarcodeProcessor(Options* opt) {
	mOptions = opt;
	barcodeStart = mOptions->barcode.location;
	barcodeLength = mOptions->barcode.length;
	barcodeSkip = mOptions->barcode.skip;
	barcode_map = mOptions->barcode.barcode_map;
}

BarcodeProcessor::~BarcodeProcessor() {
	if (mOptions->barcode.barcode_map.size() > 0) {
		mOptions->barcode.barcode_map.clear();
		map<string, int>().swap(mOptions->barcode.barcode_map);
	}
}

void BarcodeProcessor::process(Read* read1,  Read* read2, FilterResult* fr) {
	if (read2 == NULL) {
		string msg = "The read2 must be given when get the barcode marker...\n";
		throw invalid_argument(msg);
	}
	string  read2Seq = read2->mSeq.mStr;
	string code[3];
	code[0].assign(read2Seq, barcodeStart, barcodeLength);
	code[1].assign(read2Seq, barcodeStart + barcodeLength + barcodeSkip, barcodeLength);
	code[2].assign(read2Seq, barcodeStart + barcodeLength * 2 + barcodeSkip * 2, barcodeLength);

	int barcodeLabel[3];
	for (int i = 0; i < 3; i++) {
		if (barcode_map.count(code[i]) > 0) {
			barcodeLabel[i] = barcode_map[code[i]];
		}
		else {
			barcodeLabel[0] = 0;
			barcodeLabel[1] = 0;
			barcodeLabel[2] = 0;
			break;
		}
	}

	char barcodeTag[16];
	sprintf(barcodeTag, "%04d_%04d_%04d", barcodeLabel[0], barcodeLabel[1], barcodeLabel[2]);

	addBarcodeToName(read1, read2, barcodeTag);
	read2->mSeq.mStr = read2->mSeq.mStr.substr(0,  barcodeStart);
	read2->mQuality = read2->mQuality.substr(0, barcodeStart);
	//string barcode = barcodeTag;
	if(barcodeLabel[0]!=0)
		fr->addBarcode(barcodeTag);
}

void BarcodeProcessor::addBarcodeToName(Read* r1, Read* r2, string barcode) {
	string tag;
	tag = "#" + barcode;
	int nameTailPos1 = r1->mName.rfind("/1");
	if (nameTailPos1 == -1) {
		nameTailPos1 = r1->mName.rfind(' ');
	}
	int nameTailPos2 = r2->mName.rfind("/2");
	if (nameTailPos2 == -1) {
		nameTailPos2 = r2->mName.rfind(' ');
	}
	r1->mName = r1->mName.replace(nameTailPos1, 0, tag);
	r2->mName = r2->mName.replace(nameTailPos2, 0, tag);
	r1->mBarcode = barcode;
	r2->mBarcode = barcode;
}

bool BarcodeProcessor::test() {
	return true;
}