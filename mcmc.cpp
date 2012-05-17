#include <iostream>
#include <sstream>
#include <set>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <vector>
using namespace std;
#define Nmen 29 // number of mentions
#define Niter 10000 // number of iterations
struct mentions {
	string token; // The actual string
	int doc; // The identifier for the document (could be a string)
	int para; // The number paragraph in the document
	int word; // The number word in the pargraph
	int pos; // Integer identifier for the part of speech
        int entityId; // entity the mention belongs to
};
struct entity {
	int id; // An unique identifier for the entity (in consequential)
        set<int>mentions; // all the mentions belong to the entity   
};

// affinity factor or features
int affinity(string str1, string str2){
  int sumAff=0;
  // match the the prefix with 1 character
  if(str1.substr(0,1).compare(str2.substr(0,1))==0) sumAff+=1; 
  if(str1.substr(0,2).compare(str2.substr(0,2))==0) sumAff+=2;
  if(str1.substr(0,3).compare(str2.substr(0,3))==0) sumAff+=3;
  if(str1.find(str2)!=string::npos) sumAff+=10;
  if(str2.find(str1)!=string::npos) sumAff+=10;
  if(str1.length()==str2.length()) sumAff+=3;
  istringstream iss(str1);
  do{
      string sub;
      iss >> sub;
      if(sub!="" && str2.find(sub)!=string::npos) sumAff+=4;
  } while (iss);
  istringstream iss2(str2);
  do{
      string sub;
      iss2 >> sub;
      if(sub!="" && str1.find(sub)!=string::npos) sumAff+=4;
  } while (iss2);
  return sumAff;
}


int main ()
{
  int i=0,j=0; int currentEntropy=1;
  int affinityArray[Nmen][Nmen];
  mentions* mentionArray = new mentions[Nmen];
  entity* entityArray = new entity[Nmen];
  // Andrew MaCallum
  mentionArray[0].token="Andrew McCallum";
  mentionArray[1].token="Andrew MacCallum";
  mentionArray[2].token="Angrew McCallum";
  mentionArray[3].token="McCallum";
  mentionArray[4].token="A. McCallum";
  // Michael Wick
  mentionArray[5].token="Michael Wick";
  mentionArray[6].token="Mike Wick";
  mentionArray[7].token="Michael Andrew Wick";
  mentionArray[8].token="Wick";
  mentionArray[9].token="Wick";
  // Khashayar Rohanemanesh
  mentionArray[10].token="Khashayar Rohanemanesh";
  mentionArray[11].token="Khash R.";
  mentionArray[12].token="Kesh Rohanemanesh";
  // Aron Culotta
  mentionArray[13].token="Aron Culotta";
  mentionArray[14].token="Andrew Culotta";
  mentionArray[15].token="A. Culotta";
  mentionArray[16].token="Culotta McCallum";
  mentionArray[17].token="Culotta";
  mentionArray[18].token="Culotta";
  // Charles Sutton
  mentionArray[19].token="Charles Sutton";
  mentionArray[20].token="Charles A. Sutton";
  mentionArray[21].token="Sutton";
  mentionArray[22].token="Sutton";
  // Nicola Cancceda
  mentionArray[23].token="Nicola Cancceda";
  mentionArray[24].token="Nicola Canceda";
  mentionArray[25].token="Nicolla Cancceda";
  mentionArray[26].token="Nicol Cancheta";
  mentionArray[27].token="Canceda";
  mentionArray[28].token="Cancceda";

  for(i=0;i<Nmen;i++){
      mentionArray[i].entityId=i;
      entityArray[i].mentions.insert(i);
  }
  // calcualte the affinity score 
  for(i=0;i<Nmen;i++){
    for(j=0;j<Nmen;j++){
      if(j>i) {
         int score=affinity(mentionArray[i].token,mentionArray[j].token);
         affinityArray[i][j]=score;
         affinityArray[j][i]=score;
         //cout<<mentionArray[i].token<<" "<<mentionArray[j].token<<" "<<score<<endl;
      } else if (j==i){
          affinityArray[i][j]=0;
      }
    }
  }
  
  //propose a change 
  int iter=0;
  int randomMention=0;
  int randomEntity=0;
  int accepted=0;
  int rejected=0;
  srand((unsigned)time(0));
  while(iter<Niter){
    iter=iter+1;
    randomMention=(rand()%Nmen);//random mention range from 0 to Nmen-1
    randomEntity=-1;
    if(entityArray[mentionArray[randomMention].entityId].mentions.size()==1||
       ((double)rand()/(double)RAND_MAX)<=0.8){
       randomEntity=rand()%Nmen;
    } else{ // place it in an empty or create a new entity TODO create a new entity
      vector<int>emptyEntityVector;
      for(i=0;i<Nmen;i++){
          if(entityArray[i].mentions.size()==0){
             emptyEntityVector.push_back(i);
          }
      }
      if(emptyEntityVector.size()>0){ 
         int pos=(rand())%(emptyEntityVector.size());
         randomEntity=emptyEntityVector.at(pos);
      }
    }
    if(randomEntity!=-1 && randomEntity!=mentionArray[randomMention].entityId){
       set<int>::iterator it;
       int loss=0;
       for(it=entityArray[mentionArray[randomMention].entityId].mentions.begin();it!=
           entityArray[mentionArray[randomMention].entityId].mentions.end();++it){
           loss+=affinityArray[randomMention][*it];
       }
       int gain=0;
       for(it=entityArray[randomEntity].mentions.begin();it!=
           entityArray[randomEntity].mentions.end();++it){
           gain+=affinityArray[randomMention][*it];
       }
       //accept or not
       if(gain>loss){// we should accept it
          accepted+=1;
          //remove the mention from old entity and place it into the new entity
          entityArray[mentionArray[randomMention].entityId].mentions.erase(randomMention);
          entityArray[randomEntity].mentions.insert(randomMention);
          currentEntropy=currentEntropy+gain-loss;
       } else {// accept it with a probablity
               if(currentEntropy==0){cout<<"error! devided by 0"; return -1;}
               double ratio=exp((double)(currentEntropy+gain-loss)/(double)currentEntropy);
               double p=((double)rand()/(double)RAND_MAX);
               if(ratio>p){// accept it
                  accepted+=1;
                  //remove the mention from old entity and place it into the new entity
                  entityArray[mentionArray[randomMention].entityId].mentions.erase(randomMention);
                  entityArray[randomEntity].mentions.insert(randomMention);
                  currentEntropy=currentEntropy+gain-loss;
               }
       }
    }
    cout<<"iteration "<<iter<<" score="<<currentEntropy<<endl;
  }
  cout<<"number of accepted proposals="<<"	"<<accepted<<endl;
  cout<<"number of rejected proposals="<<"	"<<Niter-accepted<<endl;
  
  //set<int>::iterator it; 
  //it=entityArray[4].mentions.begin();
  //for (it=entityArray[4].mentions.begin(); it!=entityArray[4].mentions.end(); ++it)
  //    cout << " " << *it;
  
  return 0;
}
