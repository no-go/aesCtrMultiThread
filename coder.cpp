#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <thread>

/* compilie with:
 *  g++ -std=c++11 -pthread -o ctr -march=native -O3 -pipe -Wall ctr.cpp
 * check threads:
 *  ps -fL -u <username> | grep ctr
 */
#define BLOCKS_PER_THREAD 10000
#define THREAD_MAX 4
#define FILE_PREFIX ".coded"

using namespace std;

// this is a thread
void ctrCore(vector<char> org, char * tmp, string key, uint8_t blk, unsigned limit) {
	unsigned j = (blk-1) % key.size();
	for(unsigned i=0; i<limit; i++,j++) {
		if(j == key.size()) j=0;
		tmp[i] = org[i];
		tmp[i] ^= key[j];
		tmp[i] ^= blk;
		blk++;
	}
}

// encode and decode a file
void aesCTR(ifstream & in, ofstream & out, string & key, char iv) {
	vector<char> org(BLOCKS_PER_THREAD);
	char tmp;

	// 0. Block
	in.read((char *) &(org[0]), 1);
	tmp = org[0] ^ iv;
	out.write(&tmp, 1);
	
	in.read((char *) &(org[0]), BLOCKS_PER_THREAD);
	unsigned limit = in.gcount();
	//cout<<limit<<" "; cout.flush();
	uint8_t blk = 1;
	
	thread * t[THREAD_MAX];
	char results[THREAD_MAX][BLOCKS_PER_THREAD];
	unsigned readed[THREAD_MAX];
	unsigned i;
	
	while(limit > 0) {
		for(i=0; (i < THREAD_MAX) && (limit > 0); i++) {
			t[i] = new thread(
				ctrCore, org, results[i],
				key, blk, limit
			);
			blk += limit;
			readed[i] = limit;
			in.read((char *) &(org[0]), BLOCKS_PER_THREAD);
			limit = in.gcount();
		}
		//cout<<i<<" "; cout.flush();
		for(unsigned j=0; j<i; j++) {
			//cout<<"j";cout.flush();
			t[j]->join();
			out.write((char *) &(results[j][0]), readed[j]);
			delete t[j];
		}
	}
}

// encode and decode a file
void aesECB(ifstream & in, ofstream & out, string & key) {
	unsigned siz = 0;
	char org, tmp;
	
	while(in.read(&org, 1)) {
		if(siz >= key.size()) siz = 0;
		tmp = key[siz];
		tmp ^= org;
		out.write(&tmp, 1);
		siz++;
	}
}

// only decode a file
void aesCBC_decode(ifstream & in, ofstream & out, string & key, char iv) {
	char org, tmp, last;

	in.read(&org, 1);
	tmp = key[0];
	tmp ^= org;
	tmp ^= iv;
	out.write(&tmp, 1);
	last = org;
	
	unsigned siz = 1;
	while(in.read(&org, 1)) {
		if(siz >= key.size()) siz = 0;
		tmp = key[siz];
		tmp ^= org;
		tmp ^= last;
		out.write(&tmp, 1);
		last = org;
		siz++;
	}
}

// only encode a file
void aesCBC_code(ifstream & in, ofstream & out, string & key, char iv) {
	char org, tmp, last;
	
	in.read(&org, 1);
	tmp = key[0];
	tmp ^= org;
	tmp ^= iv;
	out.write(&tmp, 1);
	last = tmp;
	
	unsigned siz = 1;
	while(in.read(&org, 1)) {
		if(siz >= key.size()) siz = 0;
		tmp = key[siz];
		tmp ^= org;
		tmp ^= last;
		out.write(&tmp, 1);
		last = tmp;
		siz++;
	}
}

int main(int argc, char ** argv, char ** envp) {
	ifstream in; ofstream out;
	string key; char iv;
	
	if (argc<3) {
		cout<<"usage: "<<argv[0]<<" [ecb|cbc|ctr] <file>"<<endl;
		return 1;
	}
	string mode = argv[1];
	string infileName = argv[2];
	in.open(infileName.c_str(), std::ifstream::binary);
	if(in.is_open() != true) {
		cout<<"<infile> missing or other error"<<endl;
		return 2;
	}

	string pre = FILE_PREFIX;
	string outfileName = infileName;
	size_t found = infileName.rfind(pre);
	bool isCoded = false;

	if(found != string::npos) isCoded = true;
	
	if(isCoded == true)
		outfileName.replace(found, pre.length(), "");
	else
		outfileName += pre;

	out.open(outfileName.c_str(), std::ifstream::binary);
	
	if(mode == "ecb") {
		cout<<"ECB Key: "; cin>>key;
		aesECB(in, out, key);
		
	} else if(mode == "cbc") {
		cout<<"CBC Key.......: "; cin>>key;
		cout<<"Initial Char..: "; cin>>iv;
		if(isCoded == true) {
			aesCBC_decode(in, out, key, iv);
		} else {
			aesCBC_code(in, out, key, iv);
		}
	} else {
		cout<<"CTR Key.......: "; cin>>key;
		cout<<"Initial Char..: "; cin>>iv;
		aesCTR(in, out, key, iv);
	}
	out.close(); in.close();
	return 0;
}
