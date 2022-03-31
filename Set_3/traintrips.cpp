#include <stdio.h>
#include <vector>
#include <set>
#include <climits>
using namespace std;

int dk = INT_MAX;


vector<unsigned long> distanceList; 
vector<unsigned long> questionList(10001, -1);
set<pair<unsigned long,int>> helperSet;

void distanceCalc() {
    while(!helperSet.empty()) {

        pair<unsigned long,int> temp = *helperSet.begin();
        int d_sum = temp.first;
        unsigned long cur = temp.second;
        helperSet.erase(temp);

        // Loop for every di
        for(auto di : distanceList) {
            int calc = (cur+di) % dk;
            int dist = d_sum + di;

            // if new dist is better, update
            if(questionList[calc] == -1 || dist < questionList[calc]) {
                helperSet.erase(make_pair( questionList[calc], calc ));
                questionList[calc] = dist;
                helperSet.insert( make_pair( questionList[calc], calc ) );
            }
        }
    }
}


int main() {

    int N;
    unsigned long Q; // Q can take really large values

    scanf("%d %lu", &N, &Q);

    for(int i = 1; i <= N; i++) { 
        unsigned long di;
        scanf("%lu", &di);

        distanceList.push_back(di);

        // dk should have the minimum value
        dk = di < dk ? di : dk;

    }

    questionList[0] = 0;
    helperSet.insert(make_pair(questionList[0],0));
    
    // Calculate everything using custom Dijkstra implementation
    distanceCalc();
    
    for(int i = 0; i < Q; i++) {
        unsigned long q;
        scanf("%lu", &q);
        if(questionList[q % dk] > q) printf("NO\n");
        else printf("YES\n");
    }
}