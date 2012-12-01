#ifndef COREFERENCE_H_
#define COREFERENCE_H_

/* Meta-information
 *
 *  GIST_DESC
 *      NAME(Coreference)
 *      OUTPUTS(</(num_entity, INT)/>)   // Fill this in!
 *      RESULT_TYPE(single) // Fill this in!
 *
 *      TASK_TYPE(Task)
 *      LOCAL_SCHEDULER_TYPE(LocalScheduler)
 *      GLA_TYPE(CGLA)
 *
 *	REQ_CONST_STATES(</(mGLA, MentionsGLA)/>)
 *  END_DESC
 */
#include <unordered_set>
#include <vector>
#include <math.h>
#include "coreference/GLAs/MentionsGLA.h"
#include <pthread.h>
#include <iostream>
#include <sstream>

#define COREF_USE_SINGLE
#define num_iter 3

#define DEBUG true

class Entity {
public:
    size_t id; // An unique identifier for the entity (in consequential)
    unordered_set<size_t> mentionSet; // all the mentions belong to the entity
    pthread_mutex_t  mutex;
    //c_sameprefix_f prefixf;
    //c_samedoc_f docf;
    //c_samepos_f posf;
    void insert(int mentionId) {
        mentionSet.insert(mentionId);
    }
    /*int pre_add_score() {
        return prefixf.pre_add_score()+docf.pre_add_score()+posf.pre_add_score();
    }
    int pre_remove_score() {
        return prefixf.pre_remove_score()+docf.pre_remove_score()+posf.pre_remove_score();
    }*/
    Entity() {
        id=0;
        pthread_mutex_init(&mutex, NULL);
    }

    ~Entity() {
        pthread_mutex_destroy(&mutex);
    }

    void lock() {
        pthread_mutex_lock(&mutex);
    }

    void unlock() {
        pthread_mutex_unlock(&mutex);
    }
};

void printMention(const Mention& mm, ostream& out, bool debug = DEBUG) {
    if(debug) {
        out << "<" << mm.stringL << "| " << mm.doc << " " << mm.para <<
            " " << mm.entityId <<  ">" << endl;
    }
}

void printEntity(const Entity& e, vector<Mention> &ml, ostream& out, bool debug = DEBUG) {
    if(debug) {
        out << endl<< "printEntity:"<<"entity id=" <<  e.id << endl <<"[" <<endl;
        for(auto it = e.mentionSet.begin(); it != e.mentionSet.end(); it++) {
            //cerr << (*it) << " ";
            printMention( ml[*it], out);
        }
        out << "]" << endl<<endl;
    }
}


/** Task, in this case a pair of mentions or entities that should be merged

    NOTE: Since we have as many entities as we have mentions, the
    numers can be interpreted in any way the state wants.
 */
class Task {
public:

    int ment_id;
    int src_enti_id;
    int des_enti_id;

public:
    // make up an invalid task
    Task():ment_id(-1), des_enti_id(-1) {}
    Task(int mentid, int des_enti_id):ment_id(mentid), des_enti_id(des_enti_id) {}

};

class LocalScheduler {
    int numMentions;
    int numToGenerate; // how many tasks we still need to generate?
    vector<Entity> entities;
    vector<Mention> mentions;
public:
    LocalScheduler(int _numMentions, int numThreads):
        numMentions(_numMentions), numToGenerate(_numMentions/numThreads) {}

    void set(vector<Entity> entities, vector<Mention> mentions) {
        this->entities = entities;
        this->mentions = mentions;
    }

    size_t findNonEmptyEntity(size_t src_entity, ostream& out = cerr) {
        int Nmen=entities.size();
        size_t dest_entity=-1;
        while(true) {
            dest_entity=rand()%Nmen;
            if(entities[dest_entity].mentionSet.size()>0 && dest_entity!=src_entity)
            {
                cerr<<"nonempty:"<<" src_entity="<<src_entity<<" des_entity="<<dest_entity<<endl;
                return dest_entity;
            }
        }
    }

    size_t findEmptyEntity(size_t src_entity, ostream& out = cerr) {
        int Nmen=entities.size();
        size_t dest_entity=-1;
        while(true) {
            dest_entity=rand()%Nmen;
            if(entities[dest_entity].mentionSet.size()==0 && dest_entity!=src_entity)
            {
                cerr<<"emptyentity:"<<" src_entity "<<src_entity<<" des_entity "<<dest_entity<<endl;
                return dest_entity;
            }
        }
    }

    bool GetNextTask(Task& task) {
        cout<<"numtogener="<<numToGenerate<<endl;
        if (numToGenerate<=0)
            return false;
        else {
            ostringstream out;
            numToGenerate--;
            task.ment_id = rand()%numMentions;
            int src_entity = mentions[task.ment_id].entityId;
            task.src_enti_id=src_entity;
            if(entities[src_entity].mentionSet.size()==1||((double)rand()/(double)RAND_MAX)<=0.8) {
                task.src_enti_id=src_entity;
                task.des_enti_id=findNonEmptyEntity(src_entity, out);
            } else { // place it in an empty or create a new entity
                task.des_enti_id=findEmptyEntity(src_entity, out);
            }

            cerr << out.str();
            return true;
        }
    }
};

/* convergence GLA */
class CGLA {
    int numIter;
public:
    CGLA(int _numIter):numIter(_numIter) {}

    // not based on data
    // not needed void AddItem(int){}
    void AddState(CGLA& other) {}

    bool ShouldIterate(void) {
        return numIter < num_iter;
    }
};

/* State object for coreference */
class Coreference {

    int curIter;

public:
    typedef vector<Mention> Mentions;
    typedef vector<Entity> Entities;
    // Work unit typedef, used for preparing new rounds
    typedef std::pair<LocalScheduler*, CGLA*> WorkUnit;
    typedef std::vector<WorkUnit> WUVector;
    Mentions mentions;
    Entities entities;
public:

    // Constructor
    Coreference( const MentionsGLA& mentionsGLA) {
        mentions=mentionsGLA.GetMentions();
        curIter = 0;
        for(int i=0; i<mentions.size(); i++) {
            mentions[i].entityId = i;
            Entity entity;
            entity.id=i;
            entity.insert(i);
            entities.push_back(entity);
        }
    }
    // Destructor
    ~Coreference() {}

    void PrepareRound( WUVector& workUnits, size_t paraHint ) {
        ++curIter;
        cout<<"curInter="<<curIter<<endl;
        for(int i=0; i<paraHint; i++) {
            CGLA* cgla = new CGLA(curIter);
            LocalScheduler* ls = new LocalScheduler(mentions.size(),2);
            ls->set(entities,mentions);
            workUnits.push_back(make_pair(ls,cgla));
        }
        // Generate pairs of local schedulers and GLAs,
        // using paraHint as a hint about the level of parallelization desired
        // by the system.
    }

    void DoStep(Task& task, CGLA& cGla) {
        ostringstream out;
        int src_mention = task.ment_id;
        int src_entity = task.src_enti_id;
        int des_entity = task.des_enti_id;
        out << "\nbegin:------------------------------------\n";
        out<<"DoStep: " << "src_entity:"<<src_entity<<" des_entity:"<<des_entity<<endl;
        int loss=0;
        int gain=0;
        bool accept = false;
        unordered_set<size_t> src_mentionSet = entities[src_entity].mentionSet;
        for(auto it=src_mentionSet.begin(); it!=src_mentionSet.end(); ++it)
            if(src_mention!=*it)
                loss+=mentions[src_mention].pairwiseScore(mentions[*it]);
        //end calculate loss
        //calcuate gain
        unordered_set<size_t> dest_mentionSet = entities[des_entity].mentionSet;
        for(auto it=dest_mentionSet.begin(); it!=dest_mentionSet.end(); ++it)
            gain+=mentions[src_mention].pairwiseScore(mentions[*it]);
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
        if(accept && (src_entity!=des_entity)) {
            //accepted+=1;
            //remove the mention from old entity and place it into the new entity
            if(DEBUG) {
                out << "\nthis jump is accepted\n";
                out << "Mention to move: ";
                printMention(mentions[src_mention], out);
                out << "\n";
                out << "From entity: ";// + mentions[src_mention].entityId;
                printEntity(entities[mentions[src_mention].entityId], mentions, out);
                out << "To entity: ";
                printEntity(entities[des_entity], mentions, out);
            }
            int large=-1;
            int small=-1;
            if(src_entity>des_entity) {
                large = src_entity;
                small = des_entity;
            } else {
                large = des_entity;
                small = src_entity;
            }
            out<<"begin lock"<< " small="<<small<< " large="<<large<<endl;
            cerr << out.str();
            cerr.flush();
            out.str("");
            out.clear();
            entities[small].lock();
            entities[large].lock();
            entities[src_entity].mentionSet.erase(src_mention);
            entities[des_entity].insert(src_mention);
            mentions[src_mention].entityId=des_entity;
            entities[large].unlock();
            entities[small].unlock();
            out<<"end lock"<<endl;
            //currentEntropy=currentEntropy+gain-loss;
        }
        out << "------------------------------------end\n\n";
        // flip a coin and move mention into entity if heads

        cerr << out.str();
    }


    // Use one of the output methods below:

#ifdef COREF_USE_SINGLE
    ///// single /////
    void GetResult(INT& num_entity) {
        num_entity = entities.size();
    }
#endif

#ifdef COREF_USE_MULTI
    ///// multi /////
private:
    // used for internal iteration
    // this is just an example, you can have any kind of state needed here to
    // determine what to produce next.
    int curResult;

public:
    void Finalize() {
        // Do any necessary finalization here.
        // This will likely set the internal iterator (in this case curResult)
    }

    bool GetNextResult( /* outputs go here */ ) {
        // Return false is there are no more results to be produced.
        // Otherwise, set the outputs to the next values and return true.
    }
#endif

#ifdef COREF_USE_FRAGMENT
    ///// fragment /////

    // First we need an iterator type
    struct Iterator {
        // Any state needed to determine iteration goes here.
    }

    int GetNumFragments() {
        // Do anything here you need to figure out how many fragments you will produce.
        // 0 is a valid answer.
    }

    Iterator* Finalize( int fragment ) {
        // Create the iterator for this fragment and return a pointer to it.
    }

    bool GetNextResult( Iterator*, /* outputs go here */ ) {
        // use the iterator to determine the next result to produce.
        // if there is no result to be produced, return false,
        // otherwise return true;
    }
#endif
};

#ifdef COREF_USE_FRAGMENT
// typedef for fragment interface
typedef CoreferenceState::Iterator Coreference_Iterator;
#endif


#endif //  COREFERENCE_H_
