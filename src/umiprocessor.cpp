#include "umiprocessor.h"

UmiProcessor::UmiProcessor(Options* opt){
    mOptions = opt;
}


UmiProcessor::~UmiProcessor(){
}

void UmiProcessor::process(Read* r1, Read* r2,  FilterResult* fr) {
	//cout << "umi process start..." << endl;
	if (!mOptions->umi.enabled || r1->mBarcode == "0000_0000_0000")
		return;
	string umi1 = umiFind(r1);
	string umi2 = umiFind(r2);
	if (umi1 != "") {
		if (! mOptions->umi.umiout)
			addUmiToName(r1, r2, umi1);
		fr->addUMI(r1->mBarcode, umi1);
	}
	else if (umi2 != "") {
		if(! mOptions->umi.umiout)
			addUmiToName(r1, r2, umi2);
		fr->addUMI(r2->mBarcode, umi2);
	}
}

string  UmiProcessor::umiFind(Read* r) {
	string umi="";
	string umiPrefix = mOptions->umi.prefix;
	Sequence umiPrefixSeq = Sequence(umiPrefix);
	string umiPreixRC = umiPrefixSeq.reverseComplement().mStr;
	size_t umiLen = mOptions->umi.length;
	string polyT = POLYT;
	string polyA = POLYA;
	string findDT;
	string findDA;
	int readLen = r->length();

	string::size_type position = r->mSeq.mStr.find(umiPrefix);
	string::size_type positionRC = r->mSeq.mStr.find(umiPreixRC);
	if (position != string::npos && position + umiPrefix.size()+ umiLen + polyT.size()<= readLen) {
		findDT = r->mSeq.mStr.substr(position + umiPrefix.size() + umiLen, polyT.size());
		if (findDT == polyT) {
			umi = r->mSeq.mStr.substr(position + umiPrefix.size(), umiLen);
		}
	}
	else if (positionRC != string::npos && positionRC  >= umiLen + polyA.size()) {
		findDA = r->mSeq.mStr.substr(positionRC - umiLen - polyA.size(),  polyA.size());
		if (findDA == polyA) {
			umi = r->mSeq.mStr.substr(positionRC - umiLen, umiLen);
			Sequence umiSeq = Sequence(umi);
			umi = umiSeq.reverseComplement().mStr;
		}
	}
	return umi;
}

void UmiProcessor::addUmiToName(Read* r1,  Read* r2, string umi){
    string tag;
	tag = "/" + umi;
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
}


bool UmiProcessor::test() {
    return true;
}