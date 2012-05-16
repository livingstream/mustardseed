#include <iostream>
#include <sstream>
#include <set>
using namespace std;
#define Nmen 29
struct mentions {
	string token; // The actual string
	int doc; // The identifier for the document (could be a string)
	int para; // The number paragraph in the document
	int word; // The number word in the pargraph
	int pos; // Integer identifier for the part of speech
        int entityId;
};
struct entity {
	int id; // An unique identifier for the entity (in consequential)
        set<int>mentions;         
};

bool affinity(string str1, string str2){
  int sumAff=0;
  str1="abced";
  str2="abced";
  // match the the prefix with 1 character
  if(str1.substr(0,1).compare(str2.substr(0,1))==0) sumAff+=2; 
  if(str1.substr(0,2).compare(str2.substr(0,2))==0) sumAff+=2;
  if(str1.substr(0,3).compare(str2.substr(0,3))==0) sumAff+=3;
  if(str1.find(str2)!=string::npos) sumAff+=10;
  if(str2.find(str1)!=string::npos) sumAff+=10;
  if(str1.length()==str2.length()) sumAff+=2;
  istringstream iss(str1);
  do{
      string sub;
      iss >> sub;
      if(str2.find(sub)!=string::npos) sumAff+=3;
  } while (iss);
  istringstream iss2(str2);
  do{
      string sub;
      iss2 >> sub;
      if(str1.find(sub)!=string::npos) sumAff+=3;
  } while (iss2);
  return sumAff;
}


int main ()
{
  int i=0,j=0;
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
         cout<<mentionArray[i].token<<"	"<<mentionArray[j].token<<"	"<<score<<endl;
      }
    }
  } 
  //set<int>::iterator it; 
  //it=entityArray[4].mentions.begin();
  //for (it=entityArray[4].mentions.begin(); it!=entityArray[4].mentions.end(); ++it)
  //    cout << " " << *it;
  
  return 0;
}
