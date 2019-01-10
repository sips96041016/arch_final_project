#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
using namespace std;

void simulate(vector<unsigned int> A,bool ans[],int indice[],int n,int addr_bit,int num_set,int num_way,int num_offset){
    int i,j,k,tag,index, num_index = log2(num_set);
    int id,minimum;
    int cache[num_set][num_way][2];
    for(i=0;i<num_set;i++) for(j=0;j<num_way;j++) { cache[i][j][0] = cache[i][j][1] = -1; }
    for(k=0;k<n;k++){
        tag = A[k]>>(num_index+num_offset);
        index = ( A[k]>>num_offset )%num_set;
        /*
        for(j=0;j<32;j++) cout<<((unsigned)(A[k]<<j)>>31);
        cout<<" ";
        for(j=num_index+num_offset;j<32;j++) cout<<((unsigned)(tag<<j)>>31);
        cout<<" ";
        for(j=32-num_index;j<32;j++) cout<<((unsigned)(index<<j)>>31);
        cout<<" "<<endl;
        */
        for(i=0;i<num_way;i++) { if(cache[index][i][0]==tag) break; }
        if( i<num_way && cache[index][i][0]==tag && cache[index][i][1]!=-1 ) { cache[index][i][1] = k; ans[k] = true; continue; }

        ans[k] = false;
        minimum = cache[index][0][1]; id = 0;
        for(i=1;i<num_way;i++)
            if( minimum > cache[index][i][1] ) { minimum = cache[index][i][1]; id = i; }
        cache[index][id][0] = tag; cache[index][id][1] = k;
    }
}

int main(int argc,char *argv[]){
    fstream fsin,fout;
    string str,bench;
    vector<unsigned int> A;
    int addr_bit, num_set, num_way, block_size, num_index, num_offset;
    int i,j,n;

    fsin.open(argv[1],ios::in);
    fsin>>str>>str; addr_bit = stoi(str);
    fsin>>str>>str; num_set = stoi(str);
    fsin>>str>>str; num_way = stoi(str);
    fsin>>str>>str; block_size = stoi(str);
    num_index = log2(num_set);
    num_offset = log2(block_size);
    fsin.close();

    fsin.open(argv[2],ios::in);
    fsin>>str>>bench;
    while(fsin>>str){ A.push_back( strtol(str.c_str(),NULL,2) ); }
    fsin.close();
    n = A.size();

    //index
    int indice[num_index];
    for(i=0;i<num_index;i++) indice[i] = num_offset+i;
    //process
    bool ans[n];
    simulate(A,ans,indice,n,addr_bit,num_set,num_way,num_offset);

    fout.open(argv[3],ios::out);
    fout<<"Address bits: "<<addr_bit<<endl;
    fout<<"Number of sets: "<<num_set<<endl;
    fout<<"Associativity: "<<num_way<<endl;
    fout<<"Block size: "<<block_size<<endl<<endl;
    fout<<"Indexing bit count: "<<num_index<<endl;
    fout<<"Indexing bits:";
    for(i=num_index-1;i>=0;i--) fout<<" "<<indice[i];
    fout<<endl;
    fout<<"Offset bit count: "<<num_offset<<endl<<endl;

    fout<<".benchmark "<<bench<<endl;
    for(i=0;i<n;i++) {
        for(j=0;j<32;j++) fout<<((unsigned)(A[i]<<j)>>31); fout<<" ";
        if(ans[i]) fout<<"hit"; else fout<<"miss";
        fout<<endl;
    }
    fout.close();
    return 0;
}
