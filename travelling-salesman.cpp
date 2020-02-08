#include<bits/stdc++.h>

using namespace std;

struct Path{
    vector<int> path;
};

class comparision{
    //for cost comparision
    public:
        bool operator()(Path p1, Path p2) const
        {
            int size_p1 = p1.path.size();
            int size_p2 = p2.path.size();

            return (
                    p1.path[size_p1 - 1] + p1.path[size_p1 -2]
                    >
                    p2.path[size_p2 - 1] + p2.path[size_p2 -2]
            );
        }
};

class TSP{
    private:
        int cities, edges;
        int **edgeWeights;
        int minCost = INT_MAX;
        int start;

        Path States;
        priority_queue<Path, vector<Path>, comparision> pq;
        vector<int> solutions;
    public:
        TSP(int cities){
            this->cities = cities;
            edges = (cities * (cities-1))/2;
            edgeWeights = new int*[cities];
            for(int i=0; i<cities; ++i){
                edgeWeights[i] = new int[cities];
            }
            input();
        }

        void input(){
            cout<<"city1 city2 edgeWeight ? \n";
            // cout<<edges<<endl;
            for(int i=0; i<edges; ++i){
                int u, v, cost;
                cin>>u>>v>>cost;
                edgeWeights[u][v] = cost;
                edgeWeights[v][u] = cost;
                if(minCost > cost){
                    minCost = cost;
                }
            }

            cout<<"Starting city? ";
            cin>>start;
        }

        void A_star(){
            for(int i=0; i<cities; ++i){
                if(i == start)
                    States.path.push_back(1);
                else if(i != cities)
                    States.path.push_back(-1);
            }
            States.path.push_back(0);
            States.path.push_back(0);

            int t = 0;

            pq.push(States);
            for(int i=0; i<=cities+1; ++i){
                if(i==cities || i==cities+1)
                    solutions.push_back(100000);
                else
                    solutions.push_back(-1);
            }

            while (!pq.empty())
            {
                Path tpath;
                tpath = pq.top();
                pq.pop();

                if(isGoal(tpath.path)){
                    ++t;

                    if( solutions[solutions.size() - 1] + solutions[solutions.size() - 2] > tpath.path[tpath.path.size() -1] + tpath.path[tpath.path.size() -2])
                        solutions = tpath.path;
                }

                int max=0, maxi=0;
                for(int i=0; i<tpath.path.size()-2; ++i){
                    if(tpath.path[i] > max){
                        max = tpath.path[i];
                        maxi = i;
                    }
                }

                int visits = 0;
                if( max < tpath.path.size()-1){
                    for(int i=0; i<cities; ++i){
                        int gn = tpath.path[tpath.path.size() -1 ] + edgeWeights[maxi][i];
                        int hn = minCost * (cities - max - 1) + edgeWeights[maxi][i];

                        if(edgeWeights[i][maxi] > 0){
                            Path tmpPath;
                            // vector<int> tmp;
                            for(int x=0; x<tpath.path.size(); ++x){
                                tmpPath.path.push_back(tpath.path[x]);
                            }
                            if(tmpPath.path[i]==-1){
                                tmpPath.path[i] = max + 1;
                                tmpPath.path[tmpPath.path.size() - 1] = gn;
                                tmpPath.path[tmpPath.path.size() - 2] = hn;
                                pq.push(tmpPath);
                                visits++;
                            }
                        }
                    }
                }

                if (visits == 0 && max < tpath.path.size() - 1) {
                    int gn = tpath.path[tpath.path.size() - 1] + edgeWeights[maxi][start];
                    int hn = minCost * (cities - max - 1) + edgeWeights[maxi][start];
                    Path tmpPath;
                    tmpPath.path = tpath.path;
                    tmpPath.path[start] = max + 1;
                    tmpPath.path[tmpPath.path.size() - 1] = gn;
                    tmpPath.path[tmpPath.path.size() - 2] = hn;

                    pq.push(tmpPath);
                }
            }
            
        }

        bool isGoal(vector<int> path){
            return path[start] == path.size() -1 ;
        }

        void printResult(){
            cout << "Path: " << start << " -> ";
            for (int i = 2; i <= solutions.size() - 1; i++) {
                for (int j = 0; j < solutions.size() - 2; j++) {
                    if (solutions[j] == i){
                        if(solutions.size() - 1 != i){
                            cout << j << " -> ";
                        }else{
                            cout<<j;
                        }
                    }
                }
            }
            cout << "\nTravelling cost: " << solutions[solutions.size() - 1] << "\n";
        }
};

int main(){
    int cities;
    cout<<"Total number of cities ? ";
    cin>>cities;
    
    TSP *tsp = new TSP(cities);
    // tsp->input();
    tsp->A_star();
    tsp->printResult();
    return 0;
}