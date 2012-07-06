#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <ctime>
#include <time.h>
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <exception>
#include <assert.h>
#include "entity.h"
using namespace std;
#define bias 0 // affinity score and replusion score bais
#define nytdatapath "/home/cgrant/data/NYT/dbdump/nytmentionsfull_collapsed.csv"
//#define nytdatapath "/home/kun/Desktop/nytmentionspy.csv"
#define DEBUG false

entity entityArray[Nmen];

void printMention(const mention& mm, bool debug = DEBUG) {
    if(debug) {
        cerr << "<" << mm.stringL << "| " << mm.doc << " " << mm.para <<
             " " << mm.entityId <<  ">";
    }
}

void printEntity(const entity& e, const mention* ml, bool debug = DEBUG) {
    if(debug) {
        cerr << "[" <<  e.id << "|";
        for(auto it = e.mentionSet.begin(); it != e.mentionSet.end(); it++) {
            //cerr << (*it) << " ";
            printMention( ml[*it] );
        }
        cerr << "]";
    }
}

size_t findNonEmptyEntity(size_t sourceId) {
    size_t dest_entity=-1;
    while(true) {
        dest_entity=rand()%Nmen;
        if(entityArray[dest_entity].mentionSet.size()>0 && dest_entity!=sourceId)
            return dest_entity;
    }
}

size_t findEmptyEntity(size_t sourceId) {
    size_t dest_entity=-1;
    while(true) {
        dest_entity=rand()%Nmen;
        if(entityArray[dest_entity].mentionSet.size()==0 && dest_entity!=sourceId)
            return dest_entity;
    }
}

int main ()
{
    for(size_t k=0; k<Nmen; k++) entityArray[k].id=k;

    size_t currentEntropy=1;
    unordered_map<string,int> literalMap( {{string("NULL"),0}, {string("NULL"),0}});
    ifstream namefile(nytdatapath);
    string input;
    if(!namefile.is_open()) {
        cerr << "Error opening file";
        exit(EXIT_FAILURE);
    }
    getline(namefile,input,'\n'); //Ignore the header row

    int mentionInter=0;
    while(!namefile.eof() && mentionInter<Nmen) {
        getline(namefile,input,'\n');
        string word;
        stringstream stream(input);
        size_t i=0, doc_id=-1, para_id=-1, word_num=-1, str_len=-1, dest_entity=-1;
        char tmpS[maxtokenlen];
        memset(tmpS,'\0',maxtokenlen);
        while( getline(stream, word, ',') ) {
            if(i==0) {
                const char* cword = word.c_str();
                char s816[9] = {'\0'};
                strncpy(s816, cword+8, 8);
                char s1721[5] = {'\0'};
                strncpy(s1721, cword+17, 4);
                doc_id=(atoi(s816)-19920000)*10000+atoi(s1721);
            }
            else if(i==1)
                para_id=atoi(word.c_str());
            else if(i==2)
                word_num=atoi(word.c_str());
            else if(i==3) {
                transform(word.begin(),word.end(),word.begin(),::tolower);
                str_len=min(maxtokenlen-1,(int)strlen(word.c_str()));
                memcpy(tmpS,word.c_str(),str_len);
                if(literalMap.count(word)==0) {
                    dest_entity=literalMap.size();
                    literalMap.insert(pair<string,int>(word,dest_entity));
                } else {
                    dest_entity=literalMap.find(word)->second;
                }
            }
            i++;
        }
        mentionArray[mentionInter].set(tmpS,str_len,doc_id,para_id,word_num,0,dest_entity);
        entityArray[dest_entity].insert(mentionInter);
        mentionInter++;
    }

    namefile.close();
    size_t iter=0;
    size_t accepted=0;
    srand((unsigned)time(0));
    time_t beginTime, endTime;
		int tic, toc;
    beginTime = time (NULL);
		tic = clock(); // Fast timing
    while(iter<Niter) {
        iter=iter+1;
        size_t source_mention;
        size_t source_entity=-1;
        size_t dest_entity=-1;
        bool accept=false;
        source_mention=(rand()%Nmen);//random mention range from 0 to Nmen-1
        source_entity=mentionArray[source_mention].entityId;
        if(entityArray[source_entity].mentionSet.size()==1||((double)rand()/(double)RAND_MAX)<=0.8) {
            dest_entity=findNonEmptyEntity(source_entity);
        } else { // place it in an empty or create a new entity
            dest_entity=findEmptyEntity(source_entity);
        }
        int loss=0;
        int gain=0;
        //calculate loss
        unordered_set<size_t> source_othersmentionSet = entityArray[source_entity].othersmentionSet;
        for(auto it=source_othersmentionSet.begin(); it!=source_othersmentionSet.end(); ++it)
            if(source_mention!=*it)
                loss+=mentionArray[source_mention].pairwiseScore(mentionArray[*it]);
        for(int i=0; i<group_size; i++) {
            int count=entityArray[source_entity].token_freq[i].count;
            if(count>0) {
                auto it =entityArray[source_entity].group_set[i].begin();
                int single_score=mentionArray[source_mention].pairwiseScore(mentionArray[*it]);
                if(entityArray[source_entity].group_set[i].count(source_mention)==0)
                    loss+=single_score*count;
                else loss+=single_score*(count-1);
            }
        }
        //end calculate loss
        //calcuate gain
        unordered_set<size_t> dest_othersmentionSet = entityArray[dest_entity].othersmentionSet;
        for(auto it=dest_othersmentionSet.begin(); it!=dest_othersmentionSet.end(); ++it)
            gain+=mentionArray[source_mention].pairwiseScore(mentionArray[*it]);
        for(int i=0; i<group_size; i++) {
            int count=entityArray[dest_entity].token_freq[i].count;
            if(count>0) {
                auto it =entityArray[dest_entity].group_set[i].begin();
                int single_score=mentionArray[source_mention].pairwiseScore(mentionArray[*it]);
                if(entityArray[dest_entity].group_set[i].count(source_mention)==0)
                    gain+=single_score*count;
                else gain+=single_score*(count-1);
            }
        }
        //end calculate gain
        //accept or not
        if(gain>loss) { // we should accept it
            accept=true;
        }
        else {// accept it with a probablity
            double ratio=exp(gain-loss);
            double p=((double)rand()/(double)RAND_MAX);
            if(ratio>p) accept=true;
        }
        if(accept) {
            accepted+=1;
            //remove the mention from old entity and place it into the new entity
            if(DEBUG) {
                cerr << "Mention to move: ";
                printMention(mentionArray[source_mention]);
                cerr << "\n";
                cerr << "From Entity: ";
                printEntity(entityArray[mentionArray[source_mention].entityId], mentionArray);
                cerr << "\n";
                cerr << "To entity: ";
                printEntity(entityArray[dest_entity], mentionArray);
                cerr << "\n";
                cerr << "------------------------------------\n";
            }
            entityArray[mentionArray[source_mention].entityId].mentionSet.erase(source_mention);
            entityArray[mentionArray[source_mention].entityId].othersmentionSet.erase(source_mention);
            for(int i=0; i<group_size; i++) {
                if(entityArray[source_entity].group_set[i].count(source_mention)>0) {
                    entityArray[source_entity].token_freq[i].count--;
                    entityArray[source_entity].group_set[i].erase(source_mention);
                }
            }
            entityArray[dest_entity].insert(source_mention);
            mentionArray[source_mention].entityId=dest_entity;
            currentEntropy=currentEntropy+gain-loss;
        }
    }
		toc = clock();
    endTime=time (NULL);
		for(int i =0; i != Nmen; ++i) {
			printMention(mentionArray[i]);
		}
    cout<<"number of accepted proposals="<<"	"<<accepted<<endl;
    cout<<"number of rejected proposals="<<"	"<<Niter-accepted<<endl;
    cout << "iter: " << iter << ", endtime: " << toc*1.0/CLOCKS_PER_SEC << ", beginTime: " << tic*1.0/CLOCKS_PER_SEC << endl;
    //cout << "iter: " << iter << ", endtime: " << endTime << ", beginTime: " << beginTime << endl;
    //cout<<"speed="<<iter/(endTime-beginTime)<<endl;
		cout <<"time= " << (toc-tic*1.0)/CLOCKS_PER_SEC << endl;
		cout <<"speed= " << iter/((toc-tic*1.0)/CLOCKS_PER_SEC) << endl;
		cout << Nmen << " " << Niter << " " << (toc-tic*1.0)/CLOCKS_PER_SEC << " " << iter/((toc-tic*1.0)/CLOCKS_PER_SEC) << endl;
    return 0;
}
