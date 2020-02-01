/**
 *  NOTICE:- Working fine on https://www.onlinegdb.com/ , but may face some error on compiling locally.  
 *  Please try it out on https://www.onlinegdb.com/ , if not working locally.
 **/

#include<bits/stdc++.h>

using namespace std;

struct Path{
    vector<int> path;
};

struct Result{
    vector< vector<int> > results;
};

class comparision{
    public:
        bool operator()( Path p1,  Path p2) const
        {
           return p1.path.size() < p2.path.size();
        }
};
class Queen{
    public:
        priority_queue< Path, vector< Path>, comparision> pq;
        Result solutions;

        Queen(){

        }

        void UCS(){
            for(int i=0; i<8; ++i){
                Path tpath;
                tpath.path.push_back(i);
                pq.push(tpath);       
            }

            while(!pq.empty()){
                Path tpath = pq.top();
                pq.pop();
                

                //current tpath is a goal
                if(isGoal(tpath.path)){
                    solutions.results.push_back(tpath.path);
                };

                for(int i=0; i<8; ++i){
                     Path tpath1 = tpath;
                    tpath1.path.push_back(i);
                    cout<<endl;
                    if(isValid(tpath1.path)){
                        pq.push(tpath1);
                    }
                }
            }
        }

        bool isGoal(vector<int> path){
            return path.size()==8;
        }

        bool isValid(vector<int> path){
            for(int i=0; i<path.size()-1; ++i){
                //check column
                if(path[path.size()-1] == path[i]){
                    return false;
                }
                
                // check diagonal
                if( abs( path[path.size()-1]-path[i] )== abs( path.size()-1 - i ) ){
                    return false;
                }
            }
            return true;
        }

        void printResult(){
            for(int i=0; i<solutions.results.size(); ++i){
                for(int j=0; j<8; ++j){
                    for(int k=0; k<8; ++k){
                        if(solutions.results[i][j] == k){
                            cout<<" Q ";
                        }else{
                            cout<<" - ";
                        }
                    }
                    cout<<endl;
                }
                cout<<endl;
            }
            cout<<"\nTotal solutions: "<<solutions.results.size()<<endl;
        }
};

int main(){
    Queen queen;
    queen.UCS();
    queen.printResult();
    return 0;
}