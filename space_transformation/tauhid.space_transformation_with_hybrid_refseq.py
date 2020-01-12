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


def is_threshold_satisfied(transf, candidate_ref,minDist):

    for i in range(len(transf)):
        if transf[i][candidate_ref]<minDist:
            return False;
       
    return  True;



def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('seqfile', help='Input Sequence File')
    #parser.add_argument('pairsfile',help='Input Pairs File')
    parser.add_argument('numRefSeq', help='Maximum number of refrence sequences', type=int)
    parser.add_argument('minDist', help='Minimum Distance between the reference sequences', type=int)
    parser.add_argument('stepSize', help='Downgrade of distances step size', type=int)
    parser.add_argument('refSeqFile', help='Input Random Reference Sequence File')
    parser.add_argument('numRandRef', help='Number of dimesnions using random reference sequences', type=int)
    args = parser.parse_args()
    
    d_arr,s_labels = read_seqfile(args.seqfile)
    nseq = d_arr.shape[0];
    
    numRefSeq = int(args.numRefSeq);
    minDist = int(args.minDist);
    downStepSize = int(args.stepSize);
    
    refseqs = [];
    transf = [];

    #compute the edit distance for the default reference sequence (1st sequence in the dataset)
    first_row = [];

    for i in range(nseq):
        first_row.append(editdistance.eval(d_arr[0],d_arr[i]));

    #create map based on the distances from 1st sequence in the dataset
    refseqs.append(0);
    default_dist_map = [];
    for i in range(max(first_row)+1):
        default_dist_map.append([]);
    

    #print(len(default_dist_map));
    for i in range(len(first_row)):
        default_dist_map[first_row[i]].append(i);
    '''   
    for i in range(len(default_dist_map)):
        print(i,len(default_dist_map[i]));
    '''
    transf.append(first_row);

    maxDist = len(default_dist_map)-1;
    #curDist = minDist;
    
    
    m = 0;
    while len(refseqs)<numRefSeq:
        #print('m',m,'===========================================================');
        curDist = minDist;
        while (curDist<=maxDist):
            delete_index = [];
            #print('curDist:',curDist);
            for i in range(len(default_dist_map[curDist])):
            
                #print(default_dist_map[curDist][i],end=' ');
                #examine if the current sequence qualifies as a reference sequence
                candidate_ref = default_dist_map[curDist][i];
              
                #Check distances from the existing reference sequences
                #print(candidate_ref);
                if is_threshold_satisfied(transf,candidate_ref,minDist)==True:
                    #print(candidate_ref,' ');
                    next_row = [];
                    for j in range(nseq):
                        next_row.append(editdistance.eval(d_arr[candidate_ref],d_arr[j]));
                
                    transf.append(next_row);
                    refseqs.append(candidate_ref);
                    delete_index.append(i);
        
                if len(refseqs)==numRefSeq:
                    #print('minDist:',minDist);
                    break;
            
            else:
                #delete the already added refseqs
                #print(len(default_dist_map[curDist]));
                #print(delete_index);
                rev_delete_index = delete_index[::-1];
                 #print(rev_delete_index);
                for indx in rev_delete_index:
                    #print('---',indx);
                    del(default_dist_map[curDist][indx]);
                        
                #print();
                curDist+=1;
                continue;

            break;  

        #reduce minDist to get required number of refernce sequences 
        minDist -=downStepSize;
        m+=1;
        
    #print(refseqs);
    #print(len(refseqs));
    #print('minDist:',minDist);
    
    #starting for here, we'll do transformation using dataset independent reference sequences
    ref_arr,ref_labels = read_seqfile(args.refSeqFile)
    nRefSeq = ref_arr.shape[0];
    
    for i in range(min(nRefSeq,int(args.numRandRef))):
        next_row = [];
        for j in range(nseq):
            next_row.append(editdistance.eval(ref_arr[i],d_arr[j]));
        transf.append(next_row);

    #print the transformed space feature vectors
    transf = np.array(transf);
    #print(transf.shape);

    for i in range(transf.shape[1]):
        fvec = [];
        for j in range(transf.shape[0]):
            fvec.append(transf[j][i]);
        print(" ".join(str(item) for item in fvec));
       

    #print(len(transf));
  
    return (0)


if __name__ == "__main__":
    main()
