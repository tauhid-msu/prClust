#include<string>
using std::string;
#include<vector>
using std::vector;
#include<sstream>
using std::istringstream;
#include<fstream>
using std::ifstream;
#include<iostream>
using std::cout; using std::endl;
#include<utility>
using std::pair; using std::make_pair;
#include<set>
using std::set;
#include<algorithm>
using std::replace;

void get_dataset_sequences(vector<string>& sequences, string filename){

	ifstream iss(filename);
        string line;

	while(getline(iss,line)){
	        //cout<<line<<endl;
		if(line[0]!='>'){
			sequences.push_back(line);
			
		}
	
	
	}

}

void get_fvectors_of_sequences(vector<vector<unsigned char> > &fvectors, string fvectors_filename){

	ifstream iss(fvectors_filename);
	string line;
	vector<unsigned char> fvector;
	int feature; 

	while(getline(iss,line)){
		if(line.length()>0){
			//cout<<line<<endl;
			replace(line.begin(), line.end(), ',',' ');
			fvector.clear();

			istringstream iss2(line);
			while(iss2>>feature){
				fvector.push_back(feature);
			
			}
			fvectors.push_back(fvector);
			
		}
	
	}

}


void get_pairs(vector<string>& pairs_strings, string filename){

        ifstream iss(filename);
        string line;

        while(getline(iss,line)){
               pairs_strings.push_back(line);

 	}

}

int min(int x, int y, int z){ 

    int m_val = x;
    if (y < m_val){ m_val = y;}
    if (z < m_val){ m_val = z;}
    return m_val;
     
} 

int editDistDP(string &str1, string &str2){

    int m = str1.length();
    int n = str2.length();   
    // Create a table to store results of subproblems 
    int dp[m+1][n+1]; 
  
    // Fill d[][] in bottom up manner 
    for (int i=0; i<=m; i++){ 
	for (int j=0; j<=n; j++) {

            // If first string is empty, only option is to insert all characters of second string 
            if (i==0){ 
                dp[i][j] = j;  // Min. operations = j 
  	    }
            // If second string is empty, only option is to remove all characters of second string 
            else if (j==0){ 
                dp[i][j] = i; // Min. operations = i 
            }
            // If last characters are same, ignore last char  and recur for remaining string 
            else if (str1[i-1] == str2[j-1]){ 
                dp[i][j] = dp[i-1][j-1]; 
            }
            // If the last character is different, consider all possibilities and find the minimum 
            else{
                dp[i][j] = 1 + min(dp[i][j-1], dp[i-1][j], dp[i-1][j-1]); // Insert, Remove and Replace 
	    }

        } 
    } 
  
    return dp[m][n]; 	
	
}	    


void update_dp_row(string &str1, int &i, string &str2, vector<unsigned char> &prev_row, vector<unsigned char> & cur_row){
	
	int n = str2.length();
	
	for(int j=1; j<=n; j++){
		if(str1[i-1]==str2[j-1]){
			cur_row[j] = prev_row[j-1];
		}
		else{
			cur_row[j] = cur_row[j-1];
			if(prev_row[j-1]<cur_row[j]){
				cur_row[j] = prev_row[j-1]; 
			}
			if (prev_row[j]<cur_row[j]){
				cur_row[j] = prev_row[j];
			}
			cur_row[j] += 1;
		        //if(cur_row[j-1]<)	+ min(cur_row[j-1], prev_row[j], prev_row[j-1]);

		}
	}

}

int editDistDpMemoization(string &str1, string &str2){

	int m = str1.length();
	int n = str2.length();
	int ed = 0;
	//initalizing two rows
	vector<unsigned char> prev_row;
	for (int i=0; i<=n; i++){
		prev_row.push_back(i);
	}
	vector<unsigned char> cur_row(n+1,0);
	

	for(int i=1; i<=m; i++){
		if(i%2!=0){
			cur_row[0]= prev_row[0]+1;
			update_dp_row(str1, i, str2, prev_row, cur_row);
			ed = cur_row[n];		
		}
		else{
			prev_row[0]= cur_row[0]+1;
                        update_dp_row(str1, i, str2, cur_row, prev_row);
			ed = prev_row[n];
		}
	
	}
	return ed;
    		
}

void pairwise_edit_distances(vector<string> &sequences){

	for(int i=1; i<sequences.size(); i++){
		editDistDP(sequences[0],sequences[i]);
		//editDistDpMemoization(sequences[0],sequences[i]);
	}
}

int mvdDist(vector<unsigned char> &vecA, vector<unsigned char> &vecB,int &threshold){

	int mvd = 0;
	int vec_size = vecA.size();
	
	for(int i=0; i<vec_size; i++){
		if(abs(vecA[i]-vecB[i])>mvd){ 
			mvd = abs(vecA[i]-vecB[i]);
			if(mvd>threshold){ return 10000; }
		}

	}
	//cout<<"vsize:"<<vec_size<<"\t"<<"mvd:"<<mvd<<endl;
	return mvd;

}

void pairwise_mvd_distances(vector<vector<unsigned char> > &fvectors,int &thold){

        int num_of_vectors = fvectors.size();
	int num_rows = num_of_vectors-1;
	int pair_mvd = 0;
	cout<<"num_rows:"<<num_rows<<endl;

	for(int i=0; i<num_rows; i++){
		for(int j=i+1; j<num_of_vectors; j++){
			pair_mvd = mvdDist(fvectors[i],fvectors[j],thold);
			if(pair_mvd<=thold){
				cout<<i<<" "<<j<<" "<<pair_mvd<<endl;
			}
		}	
	
	}

}

void print_vector(vector<int> &trace){
	for(auto &telt: trace){
		cout<<telt<<"\t";
	}
	cout<<endl;
}

void adjust_the_trace(vector<int> &trace, string &a, string &b, int &a0, int &b0){
	
	int count=0;  
	
	for(int i=trace.size()-1; i>0; i--){
		if(trace[i]>0 && trace[i-1]>0){
			if(trace[i-1]<trace[i]){
				count++;	
			}
			else if(trace[i-1]>=trace[i] && count>0){
				trace[i-1]=-1;	
				
			}
		}
	}

}

void trace_for_d_2d(string &a, string &b, int &a0, int &b0, int &dist, const int &mfar){

        int tsize = (a0+mfar)<=a.length() ? mfar : a.length()-a0;
        int bspread = (b0+2*mfar) <= b.length()? (b0+2*mfar) : b.length(); 	
        
	vector<int> trace(tsize,-1);

	for(int i=a0; i<(a0+tsize); i++){
		for(int j=b0; j<bspread; j++){
			if(a[i]==b[j]){
				//cout<<i<<"\t"<<j<<endl;
				if(trace[i-a0]==-1 || ( abs(i-trace[i-a0])>abs(i-j) ) ){
					trace[i-a0]=j;
				}
				else if(abs(i-trace[i-a0])==abs(i-j) && trace[i-a0-1]>=trace[i-a0]){
					trace[i-a0] = j;
				}
				else{ break;}
			}
			else if( abs(i-j)>abs(i-trace[i-a0]) ){
				break;
			}
		

		}
	
	}
        print_vector(trace);
	//adjust trace elements if needed
	adjust_the_trace(trace,a,b,a0,b0);        
	print_vector(trace);
	int new_b0=b0;
	int neg_count = 0;
	for(int i=0; i<trace.size();  i++){
		cout<<(a0+i)<<"\t";
		if(trace[i]==-1){neg_count++;}
		else{ new_b0 = trace[i];}
	}
	cout<<endl;
	dist+= neg_count;
	dist+= (new_b0-b0)>tsize? (new_b0 - b0) : 0;

	b0 = new_b0;
	a0 += tsize-1;
	cout<<"dist:"<<dist<<"\t"<<"a0:"<<a0<<"\t"<<"b0:"<<b0<<endl;

}

int compute_edist(string &a, string &b, const int  &thold, const int max_diff){
	
	if(a.length()>b.length()){
		return compute_edist(b,a,thold,max_diff);
	}
	int dist = 0,i=0,j=0,a_len = a.length();
        cout<<a<<endl;
        cout<<b<<endl;	
	while(i<a_len && dist<=thold){
		if(a[i]!=b[j]){
			cout<<">>"<<i<<","<<a[i]<<","<<b[j]<<endl;
			trace_for_d_2d(a,b,i,j,dist,max_diff);
			if (dist>thold) {
				return -1; //edist greater than thold, we don't guarantee anymore
			}
			i++; j++;
			//break;
		}
		else{
			i++;j++;
		}	
	}

	return dist;

}
