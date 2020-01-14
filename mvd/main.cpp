#include<iostream>
using std::cin; using std::cout; using std::endl;
#include<vector>
using std::vector;
#include<sstream>
using std::istringstream;
#include<string>
using std::stoi;
#include "pairwise_distances.h"


int main(int argc, char** argv){

	string filename;
	string feature_vectors_filename;
	int dist_type;
	//string pairs_filename;
	 
        //vector<string> sequences;
        vector<string> pairs_strings;

	if (argc>=4){
		filename = argv[1];
		feature_vectors_filename = argv[2];
		dist_type = stoi(argv[3]);
	}
      
	vector<string> sequences;
        get_dataset_sequences(sequences, filename); 
        cout<<sequences.size()<<endl;

	vector<vector<unsigned char> > fvectors;
        get_fvectors_of_sequences(fvectors, feature_vectors_filename);
        cout<<fvectors.size()<<endl;
	
	//1 =edit distance
        if(dist_type==1){
		pairwise_edit_distances(sequences);
	}
	else if(dist_type==2){ //2 = mvd
		int thold = 8;
		pairwise_mvd_distances(fvectors,thold);	
	}	
	/*
	
        vector<string> sequences;
	get_dataset_sequences(sequences, filename); 
        cout<<sequences.size()<<endl;
	
	get_pairs(pairs_strings, pairs_filename);
	cout<<pairs_strings.size()<<endl;
	
	int pdist;
	//istringstream iss;
	int a,b,dist;
	const int thold = 6; //distance threshold 
	const int max_diff = 6; //maximum number of insertions/delettions at any block (assumption) max_diff<=thold

	for(const string &pair : pairs_strings){
		istringstream iss(pair);
		iss >> a >> b >> dist;
		//cout<<"::"<<pair<<endl;
		cout<<a<<"\t"<<b<<"\t"<<dist<<endl;
		//cout<<sequences[a]<<endl;
	        //cout<<sequences[b]<<endl;	
                pdist = compute_edist(sequences[a],sequences[b],thold,max_diff);
		cout<<endl;	
		if(pdist!=dist){
			cout<<a<<"\t"<<b<<"\t"<<dist<<endl;
			exit(1);
		}

	} 
        */

	return 0;

}
