//
//  States.h
//
//  Created by game over on 25/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_STATES
#define _H_STATES

typedef int StateEnum;
typedef string StateStr;

class State{
    
public:
    
    State(){active = false;};
    State(StateEnum _stateEnum, StateStr _stateStr):
    stateEnum(_stateEnum), stateStr(_stateStr), active(false){};
    ~State(){};
    
    StateEnum stateEnum;
    StateStr stateStr;
    bool active;
    
    bool operator==(State & s){
        if(stateEnum == s.stateEnum &&
           stateStr == s.stateStr){
            return true;
        }else{
            return false;
        }
    };
    
    friend ostream& operator<< (ostream &os, State &s);
    
};

inline ostream& operator<<(ostream& os, State &s){
	os << s.stateEnum << " " << s.stateStr << " == " << (string)(s.active ? "TRUE" : "FALSE");
	return os;
};

class StateGroup{
    
public:
    
    StateGroup(){
        name = "NONE";
        exclusive = true;
    }
    
    StateGroup(string _name, bool _exclusive = true):
    name(_name), exclusive(_exclusive){};
    
    ~StateGroup(){
        name.clear();
        group.clear();
    };
    
    void addState(State state){
        assert(!getStateExists(state));
        group.push_back(state);
    };
    
    void removeState(StateEnum stateEnum){
        int index = getStateIndex(stateEnum);
        assert(index != -1);
        group.erase(group.begin() + index);
    };
    
    void setStatesExclusive(bool b){
        exclusive = b;
    };
    
    inline void toggleState(StateEnum stateEnum){
        setState(stateEnum, !getState(stateEnum));
    }
    
    inline void setState(StateEnum stateEnum){
        bool exists = false;
        for(int i = 0; i < group.size(); i++){
            if(group[i].stateEnum == stateEnum){
                group[i].active = true;
                exists = true;
            }else{
                if(exclusive) group[i].active = false;
            }
        }
        assert(exists);
    };
    
    inline void setState(StateEnum stateEnum, bool active){
        assert(!exclusive);
        bool exists = false;
        for(int i = 0; i < group.size(); i++){
            if(group[i].stateEnum == stateEnum){
                group[i].active = active;
                exists = true;
                break;
            }
        }
        assert(exists);
    };
    
    inline int getState(){
        assert(exclusive);
        for(int i = 0; i < group.size(); i++){
            if(group[i].active == true){
                return group[i].stateEnum;
            }
        }
        return -1;
    };
    
    inline bool getState(StateEnum stateEnum){
        int index = getStateIndex(stateEnum);
        assert(index != -1);
        return group[index].active;
    };
    
    inline bool getStateExists(State & state){
        if(getStateIndex(state) == -1){
            return false;
        }else{
            return true;
        }
    }
    
    inline int getStateIndex(State & state){
        return getStateIndex(state.stateEnum);
    }
    
    inline int getStateIndex(StateEnum stateEnum){
        for(int i = 0; i < group.size(); i++){
            if(group[i].stateEnum == stateEnum){
                return i;
            }
        }
        return -1;
    }
    
    friend ostream& operator<< (ostream &os, StateGroup &sg);
    
    string name;
    bool exclusive;
    vector<State> group;
    
};

inline ostream& operator<<(ostream& os, StateGroup &sg){
    os << "---------------------------" << endl;
	os << sg.name << endl;
    for(int i = 0; i < sg.group.size(); i++){
        os << sg.group[i] << endl;
    }
    os << "---------------------------" << endl;
	return os;
};

#endif
