#pragma once
#include<vector>
using std::vector;
#include<string>
using std::string;

void get_dataset_sequences(vector<string>& , string filename);
void get_fvectors_of_sequences(vector<vector<unsigned char> > &fvectors, string fvectors_filename);
void get_pairs(vector<string>&, string filename);

void trace_for_d_2d(string &a, string &b, int a0, int b0, int &dist, const int &mfar);
int compute_edist(string &a, string &b, const int &thold, const int max_diff);

void pairwise_edit_distances(vector<string> &);
void pairwise_mvd_distances(vector<vector<unsigned char> > &fvectors,int &thold);

