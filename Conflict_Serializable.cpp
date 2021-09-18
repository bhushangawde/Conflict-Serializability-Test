#include <iostream>
#include <algorithm>
#include <string.h>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
using namespace std;

#define MAX_TRANSACTIONS 9 // Max transactions is fixed to 9

// Some sample input schedules

//w1xr2xw2yr3yw3zr1z
//r1xr1yw2xw1xr2y 
//r1xr3yw1xw2yr3xw2x
//r1xw1xr2xw2xr1yw1yr2yw2y
//r2xw2xr1xw1xr1yw1yr2yw2y
//r1xr1yr2xr2yw2yw1x 
//r1xr2xr2yw2yr1yw1x 

class Serialize {
    
    unordered_map<int,vector<string>> un_mp;
    int scheduleSize;
    int totalTransactions;
    bool visited[MAX_TRANSACTIONS];
    bool recStack[MAX_TRANSACTIONS];
    int graph[MAX_TRANSACTIONS][MAX_TRANSACTIONS];
    
    public:

    void init(){
        for(int i = 0 ; i < MAX_TRANSACTIONS; i++){
            for(int j = 0 ; j < MAX_TRANSACTIONS; j++){
                graph[i][j] = 0;
            }
            visited[i] = false;
            recStack[i] = false;
        }
    }

    void preprocess(string schedule){
        scheduleSize = schedule.size();
        vector<int> vmax;
        for(int i = 0; i < scheduleSize; i++){
            if(isdigit(schedule[i])){
                vmax.push_back(int(schedule[i] - '0'));
                // cout<<int(schedule[i] - '0')<<endl;
            }
        }
        totalTransactions = *max_element(vmax.begin(), vmax.end());
        cout<<"Total number of transactions : "<< totalTransactions<<endl<<endl;
        for(int i = 0; i< scheduleSize; i++){
            if(isdigit(schedule[i])){
                un_mp[int(schedule[i] - '0')].push_back(schedule.substr(i-1,3));
            }
        }
        for(int i = 0 ; i < totalTransactions; i++){
            cout<<"T"<<(i+1)<<":";
            for(int v = 0; v < un_mp[i+1].size(); v++){
                cout<<" "<<un_mp[i+1][v]<< " ";
            }
            cout<<endl;
        }
        cout<<endl;
        return;
    }  

    void createGrid(string schedule){
        for(int i = 0; i < scheduleSize; i++){
            if(isdigit(schedule[i])){
                for(int j = i + 1; j < scheduleSize; j++){
                    if(isdigit(schedule[j]) && (schedule[j] != schedule[i])){
                        bool condition1 = (schedule[i-1] == 'w' && schedule[j-1] == 'r' && (schedule[i+1] == schedule[j+1]));
                        bool condition2 = (schedule[i-1] == 'r' && schedule[j-1] == 'w' && (schedule[i+1] == schedule[j+1]));
                        bool condition3 = (schedule[i-1] == 'w' && schedule[j-1] == 'w' && (schedule[i+1] == schedule[j+1]));

                        if(condition1 || condition2 || condition3){
                            int transaction1 = int(schedule[i] - '0');
                            int transaction2 = int(schedule[j] - '0');
                            graph[transaction1][transaction2] = 1;
                        }
                    }
                }
            }
        }

        cout<< "Printing the Grid"<< endl<<endl;

        for(int i = 0; i < totalTransactions; i++){
            for(int j = 0 ; j < totalTransactions; j++){
                cout<<graph[i+1][j+1]<<" ";
            }
            cout<<endl;
        } 
        cout<<endl;
    }

    bool detectCycleUtil(int v){
        visited[v] = true;
        recStack[v] = true;
        
        for(int i = 1 ; i <= totalTransactions; i++){
            if(graph[v][i] == 1 && (i != v)){
                if(visited[i] == false && detectCycleUtil(i)){
                    return true;
                }
                else if(recStack[i]){
                    return true;
                }
            }
        }
        
        recStack[v] = false;
        return false;
    }

    bool detectCycle(){
        for(int i = 1 ; i <= totalTransactions; i++){
            if(!visited[i]){
                if(detectCycleUtil(i)){
                    return true;
                }
            }
        }
        return false;
    }

    void process(string schedule){
        init();
        preprocess(schedule);
        createGrid(schedule);
        if(detectCycle()){
            cout<< "Cycle is detected! The schedule is NOT conflict serializable."<<endl<<endl;
        }
        else{
            cout<< "Cycle is NOT detected! The schedule is conflict serializable."<<endl<<endl;
        }
        return;
    }
};


int main(void){
    cout<<"Enter the transaction schedule"<<endl;
    string schedule;
    cin>>schedule;

    if(schedule.size() == 0){ 
        cout<<"Invalid transaction!";
        return -1;
    }
    Serialize obj;
    cout<<"The input schedule is "<<schedule<<endl<<endl;
    obj.process(schedule);
    
    return 0;
}