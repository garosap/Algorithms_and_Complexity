#include <stdio.h>
#include <stdlib.h>
#include <vector>
// #include <climits>
#include <iostream>
using namespace std;

#define maxValue 16000000000000

int N, M;
int A;
unsigned long long P;

unsigned long long helperArray[1501][5001];

vector <pair<int, unsigned long long> > offersArray[4][4];

vector <unsigned long long> optimal[4][4];

void helperFunc(int seller, int product) {

    for(int j = 1; j <= N; j++)
        helperArray[0][j] = maxValue;   
    for(int i = 0; i <= offersArray[seller][product].size(); i++)
        helperArray[i][0] = 0;
    for(int i = 1; i <= offersArray[seller][product].size(); i++) {
        int quantity = offersArray[seller][product][i-1].first;
        int price = offersArray[seller][product][i-1].second;
        for(int j = 1; j <= N; j++)
            helperArray[i][j] = min(helperArray[i-1][max(0,j-quantity)] + price, helperArray[i-1][j]);
    }
    for(int j = 0; j <= N; j++)
        optimal[seller][product].push_back(helperArray[offersArray[seller][product].size()][j]);
}

int main() {
    cin >> N;
    cin >> M;

    for(int i = 0; i < M; i++) {
        int seller;
        // char productCode;

        cin >> seller;
        // cin >> productCode;

        int productNumber;
        char productCode = getchar();

        switch(productCode) {
            case 'A': productNumber = 1; break;
            case 'B': productNumber = 2; break;
            case 'C': productNumber = 3; break;
        }

        cin >> A;
        cin >> P;
        offersArray[seller][productNumber].push_back(make_pair(A,P));
    }
    for(int i = 1; i <= 3; i++)
        for(int j = 1; j <= 3; j++)
            helperFunc(i,j);
    for(int i = 1; i <= 3; i++)
        for(int j = 0; j <= N; j++) 
            optimal[i][1][j] += optimal[i][2][j] + optimal[i][3][j];
    unsigned long long res = maxValue;
    for(int i = 0; i <= N; i++)
        for(int j = 0; j <= N; j++)
            if(N-i-j >= 0)
                res = min(res, optimal[1][1][i] + optimal[2][1][j] + optimal[3][1][N-i-j]);
            else
                res = min(res, optimal[1][1][i] + optimal[2][1][j]);

    // cout << res << "  " << maxValue << "  " << (    ) << endl;
    if (res == maxValue) cout << -1 << endl;
    else cout << res << endl;

}