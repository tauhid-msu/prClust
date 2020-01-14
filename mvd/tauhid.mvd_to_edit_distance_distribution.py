'''
Initiated: 07/03/2018
The goal is to record the number of letters at each step point where step size is user defined. This is an 
althernative of R^l sequences containing same letter. This approach should reduce the problem of predicting
multiple edit opertions by separating them from each other via step size.

'''

import sys
import numpy as np
import argparse
import editdistance
from Bio import SeqIO
#import matplotlib.pyplot as plt 

def read_seqfile(input_file):
    #sys.stderr.write("\tReading Sequence File {0} ... ".format(input_file))
  
    records = list(SeqIO.parse(input_file, "fasta"));
    nseq = len(records);
    #sys.stderr.write(" Number of sequences: %d\n"%nseq)

    d_arr = np.empty(nseq, dtype='object')
    seq_labels = np.empty(nseq, dtype='object')
    i = 0;
    for record in SeqIO.parse(input_file, "fasta"):
        d_arr[i] = record.seq;
        seq_labels[i] = record.id
        i += 1;
    return (d_arr,seq_labels)



def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('seqfile', help='Input Sequence File')
    parser.add_argument('pairsfile',help='Input Pairs File')
    parser.add_argument('mvd', help='Mvd pairs to be checked', type=int)
    #parser.add_argument('stepsize', help='Step size', type=int)
    
    args = parser.parse_args()
    
    d_arr,s_labels = read_seqfile(args.seqfile)
    nseq = d_arr.shape[0];
    mvd = int(args.mvd);
    
    pairs = np.loadtxt(args.pairsfile,delimiter=' ',dtype=int, usecols=range(0,3));
    mvd_histogram = np.zeros(shape=(mvd*10), dtype=int);

    for i in range(pairs.shape[0]):
        if mvd==pairs[i][2]:
            pair_edist = int(editdistance.eval(d_arr[pairs[i][0]],d_arr[pairs[i][1]]))
            mvd_histogram[pair_edist]+=1;

    print(mvd_histogram);
    '''
    max_val = 0;
    ed_histogram = np.zeros(shape=(args.maxedist*10), dtype=int);
    
    count = 0;
    for i in range(1):
        for j in range(i+1,nseq):
            pair_edist = int(editdistance.eval(d_arr[i],d_arr[j]));
            if pair_edist > max_val: max_val = pair_edist;
            ed_histogram[pair_edist] += 1;
            count+=1;
    
    '''
    '''
    for i in range(max_ed+1):
        list_elt = int(len(pair_list[i])/2);
        for j in range(list_elt):
            print("%d,%d,%d"%(pair_list[i][j*2],pair_list[i][j*2+1],i));
    ''' 
    '''
    print('count',count);
    #x_axis = [];      
    for i in range(1,max_val+1):
        #x_axis.append(i);
        print("%d %d"%(i,ed_histogram[i]));
    #print(ed_histogram[:max_val]);    
    #plt.plot(ed_histogram[:100]) 
    #plt.bar(x_axis[:100],ed_histogram[:100]);
    #plt.show() 
    '''
    return (0)


if __name__ == "__main__":
    main()
