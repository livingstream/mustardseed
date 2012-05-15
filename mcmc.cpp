#include <iostream>
#include <hash_map>
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
         
};


int main ()
{
  int i=0;
  mentions* mentionArray = new mentions[Nmen];
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
  }
  
  return 0;
}
