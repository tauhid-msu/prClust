#include<iostream>
using std::cin; using std::cout; using std::endl;
#include<string>
using std::string;
using std::stoi;
#include<vector>
using std::vector;
#include<sstream>
using std::istringstream;
#include<fstream>
using std::ifstream;

int main(int argc, char** argv){

        string filename;
        string pairs_filename;
        vector<string> pairs_strings;
	int thold;

        if (argc>=4){
                filename = argv[1];
                pairs_filename = argv[2];
		thold = stoi(argv[3]);
        }

	ifstream iss(pairs_filename);
        string line;
	int a,b,dist;
	vector<vector<string> > srted_pairs(thold+1,vector<string>() );

        while(getline(iss,line)){
		//pairs_strings.push_back(line);
   		istringstream iss2(line);
                iss2 >> a >> b >> dist; 
		srted_pairs[dist].push_back(line);
       				
        }
	
	for(int i=1; i<=thold; i++){
	
		for(int j=0; j<srted_pairs[i].size(); j++){
			cout<<srted_pairs[i][j]<<endl;

		}
		//cout<<srted_pairs[i].size()<<endl;
	}
	

	return 0;

}
