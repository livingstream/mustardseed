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
#include "mention.h"
#include "entity.h"
using namespace std;
#define Nmen 300000 // number of mentions
//#define Nmen 3000 // number of mentions
#define Niter 200000 // number of iterations
#define bias 0 // affinity score and replusion score bais
//#define nytdatapath "/home/cgrant/data/NYT/dbdump/nytmentionsfull.csv"
#define nytdatapath "/home/kun/Desktop/nytmentionspy.csv"

entity entityArray[Nmen];
mention mentionArray[Nmen];

void printMention(const mention& mm) {
	cerr << "<" << mm.stringL << "| " << mm.doc << " " << mm.para << 
		" " << mm.entityId <<  ">";
}

void printEntity(const entity& e, const mention* ml) {
	cerr << "[" <<  e.id << "|";
	for(auto it = e.mentionSet.begin(); it != e.mentionSet.end(); it++) {
		//cerr << (*it) << " ";
		printMention( ml[*it] );
	}
	cerr << "]";
}

int findNonEmptyEntity(int sourceId){
	int randomEntity=-1;
	while(true){
		randomEntity=rand()%Nmen;
		if(entityArray[randomEntity].mentionSet.size()>0 && randomEntity!=sourceId)
			return randomEntity;
	}
}

int findEmptyEntity(int sourceId){
	int randomEntity=-1;
	while(true){
		randomEntity=rand()%Nmen;
		if(entityArray[randomEntity].mentionSet.size()==0 && randomEntity!=sourceId)
			return randomEntity;
	}
}

int main ()
{
	int k=0;
	for(k=0;k<Nmen;k++)
		entityArray[k].id=k;

	int currentEntropy=1;
	unordered_map<string,int>literalMap;
	// read data from nyt dataset 
	ifstream namefile(nytdatapath);
	string input;
	if(!namefile.is_open()){
		cerr << "Error opening file";
		exit(EXIT_FAILURE);
	}
	getline(namefile,input,'\n'); //Ignore the header row

	int mentionInter=0;
	while(!namefile.eof() && mentionInter<Nmen){
		getline(namefile,input,'\n');
		string word;
		stringstream stream(input);
		int i=0, doc_id, para_id, word_num, str_len, dest_entity;
		char tmpS[maxtokenlen+1];
		memset(tmpS,'\0',maxtokenlen+1);
		while( getline(stream, word, ',') ){
			if(i==0)//extract the integer docid from string
				doc_id=(atoi(word.substr(8,16).c_str())-19920000)*10000+atoi(word.substr(17,21).c_str());
			else if(i==1)
				para_id=atoi(word.c_str());
			else if(i==2)
				word_num=atoi(word.c_str());
			else if(i==3){
				transform(word.begin(),word.end(),word.begin(),::tolower);
				str_len=word.size();
				memcpy(tmpS,word.c_str(),min(maxtokenlen,(int)strlen(word.c_str())+1));
				if(literalMap.count(word)==0){
					dest_entity=literalMap.size();
					literalMap.insert(pair<string,int>(word,dest_entity)); 
				} else {
					dest_entity=literalMap.find(word)->second;
				}
			}
			i++;
		}
		mentionArray[mentionInter].set(tmpS,str_len,doc_id,para_id,word_num,0,dest_entity);
		entityArray[dest_entity].mentionSet.insert(mentionInter);
		mentionInter++;
	}

	namefile.close();
	//propose a change 
	int iter=0;
	int randomMention=0;
	int randomEntity=0;
	int accepted=0;
	srand((unsigned)time(0));
	time_t beginTime, endTime;
	beginTime = time (NULL);
	while(iter<Niter){
		iter=iter+1;
		randomMention=(rand()%Nmen);//random mention range from 0 to Nmen-1
		randomEntity=-1;
		if(entityArray[mentionArray[randomMention].entityId].mentionSet.size()==1||
				((double)rand()/(double)RAND_MAX)<=0.8){
			randomEntity=findNonEmptyEntity(mentionArray[randomMention].entityId);
		} else{ // place it in an empty or create a new entity
			randomEntity=findEmptyEntity(mentionArray[randomMention].entityId);
		}
		unordered_set<int>::iterator it;
		int loss=0;
		for(it=entityArray[mentionArray[randomMention].entityId].mentionSet.begin();it!=
				entityArray[mentionArray[randomMention].entityId].mentionSet.end();++it){
			loss+=mentionArray[randomMention].pairwiseScore(mentionArray[*it]);
		}
		int gain=0;
		for(it=entityArray[randomEntity].mentionSet.begin();it!=
				entityArray[randomEntity].mentionSet.end();++it){
			gain+=mentionArray[randomMention].pairwiseScore(mentionArray[*it]);
		}
		bool accept=false;
		//accept or not
		if(gain>loss){// we should accept it
			accept=true;
		} else {// accept it with a probablity
			double ratio=exp(gain-loss);
			double p=((double)rand()/(double)RAND_MAX);
			if(ratio>p){// accept it
				accept=true;
			}
		}
		if(accept){
			accepted+=1;
			//remove the mention from old entity and place it into the new entity
			cerr << "Mention to move: "; printMention(mentionArray[randomMention]); cerr << "\n";
			cerr << "From Entity: "; printEntity(entityArray[mentionArray[randomMention].entityId], mentionArray); cerr << "\n";
			cerr << "To entity: "; printEntity(entityArray[randomEntity], mentionArray); cerr << "\n";
			cerr << "------------------------------------\n";
			entityArray[mentionArray[randomMention].entityId].mentionSet.erase(randomMention);
			entityArray[randomEntity].mentionSet.insert(randomMention);
			mentionArray[randomMention].entityId=randomEntity;
			currentEntropy=currentEntropy+gain-loss;
		}
	}
	endTime=time (NULL);
	cout<<"number of accepted proposals="<<"	"<<accepted<<endl;
	cout<<"number of rejected proposals="<<"	"<<Niter-accepted<<endl;
	cout<<"speed="<<iter/(endTime-beginTime)<<endl;
	return 0;
}
