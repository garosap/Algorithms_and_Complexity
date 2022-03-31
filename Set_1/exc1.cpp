#include <iostream>
#include <iterator>
#include <set>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

int ARRAY_SIZE = 10000000;
int N;
int M;
int lastTunnelIndex = 0;

class Tunnel
{

public:
    set<int> indexes;
    set<int> values;
    bool deleted = false;

    Tunnel()
    {
        // cout << "TUNNEL CREATED "<< deleted << endl;
    }

    bool isValid()
    {
        return indexes == values;
    }

    void addPosition(int arr[], int index)
    {
        indexes.insert(index);
        values.insert(arr[index - 1]); // VERY IMPORTANT: DO -1 BECAUSE WE ARE GIVEN VALUES [1, 2, ..., N]
    }

    int getSize()
    {
        return indexes.size();
    }

    bool includesPosition(int index)
    {
        return indexes.find(index) != indexes.end();
    }

    void printIndexes()
    {
        set<int>::iterator itr;
        if (deleted)
            cout << "THIS TUNNEL IS DELETED!!!\n";
        else
        {
            cout << "INDEX set is : \n";
            for (itr = indexes.begin(); itr != indexes.end(); itr++)
            {
                cout << *itr << " ";
            }
            cout << endl;
        }
    }

    void print()
    {
        set<int>::iterator itr;
        if (deleted)
            cout << "THIS TUNNEL IS DELETED!!!\n";
        else
        {
            cout << "INDEX set is : \n";
            for (itr = indexes.begin(); itr != indexes.end(); itr++)
            {
                cout << *itr << " ";
            }
            cout << endl;

            cout << "VALUES set is : \n";
            for (itr = values.begin(); itr != values.end(); itr++)
            {
                cout << *itr << " ";
            }
            cout << endl;
        }
    }
};

void printSet(set<int> s1)
{
    set<int>::iterator itr;
    cout << "[";
    for (itr = s1.begin(); itr != s1.end(); itr++)
    {
        cout << *itr << ", ";
    }
    cout << "]\n";
}

// Tunnel mergeTunnels(Tunnel t1, Tunnel t2, int arr[]){
//     Tunnel result, other;
//     if(t1.getSize() < t2.getSize()) {
//         result = t2;
//         other = t1;
//     }
//     else{
//         result = t1;
//         other = t2;
//     }

//     set<int>::iterator itr;
//     for (itr = other.indexes.begin(); itr != other.indexes.end(); itr++){
//         result.addPosition(arr, *itr);
//     }

//     return result;
// }

Tunnel mergeTunnels(Tunnel t1, Tunnel t2, int arr[])
{
    set<int>::iterator itr;
    // cout << "PRE MERGING t1: ";
    // t1.printIndexes();
    // t2.printIndexes();
    for (itr = t2.indexes.begin(); itr != t2.indexes.end(); itr++)
    {
        t1.addPosition(arr, *itr);
    }
    // cout << "AFTER MERGING t1: ";
    // t1.printIndexes();
    return t1;
}

struct Portal
{
    int start, end, width;
};

bool comparePortal(Portal p1, Portal p2)
{
    return (p1.width > p2.width);
}

bool includedInTunnel(Tunnel tunnels[], int number)
{
    for (int i = 0; i < lastTunnelIndex; i++)
    {
        if (tunnels[i].deleted)
            continue;

        if (tunnels[i].includesPosition(number))
            return true;
    }
    return false;
}

void printTunnelIndexes(Tunnel tunnels[])
{
    cout << "TUNNELS ARE: [ ";
    for (int i = 0; i < N; i++)
    {
        if (!tunnels[i].deleted)
            printSet(tunnels[i].indexes);
    }
    cout << " ]";
}

int main()
{
    // Data input start
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int inputNumber;

    cin >> inputNumber;
    N = inputNumber;

    cin >> inputNumber;
    M = inputNumber;

    int *mortisPositions = (int *)malloc(N * sizeof(int));

    for (int k = 0; k < N; k++)
    {
        cin >> inputNumber;
        mortisPositions[k] = inputNumber;
    }

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! MALLOC WEIRD BEHAVIOUR
    Portal *portals = (Portal *)malloc(M * 2 * (sizeof(Portal)));

    for (int k = 0; k < M; k++)
    {
        Portal temp;
        cin >> inputNumber;
        temp.start = inputNumber;

        cin >> inputNumber;
        temp.end = inputNumber;

        cin >> inputNumber;
        temp.width = inputNumber;

        portals[k] = temp;
        // cout << k << "/" << M << " { " << portals[k].start << ", " <<  portals[k].end <<  ", " <<  portals[k].width <<  " }" << endl;
    }

    if (N <= 100000 || M <= 100000)
        sort(portals, portals + M, comparePortal);
    // Portal portals[] = {p4, p1, p2, p3};
    // data input end

    Tunnel *tunnels = (Tunnel *)malloc(M * 2 * (sizeof(Tunnel)));

    int i = 0;

    for (i = 0; i < M; i++)
    {

        // Add portal to tunnels
        Portal portal = portals[i];
        // cout << "Checking: " << i << "/" << M << endl;
        // cout << "Adding portal { " << portal.start << ", " <<  portal.end <<  ", " <<  portal.width <<  " }" << endl;

        bool foundStart = false, foundEnd = false;
        int tunnelIndex1 = 0, tunnelIndex2 = 0;
        for (int j = 0; j < lastTunnelIndex; j++)
        {
            Tunnel tunnel = tunnels[j];
            if (tunnel.deleted)
                continue;

            if (tunnel.includesPosition(portal.start))
            {
                foundStart = true;
                tunnelIndex1 = j;
            }

            if (tunnel.includesPosition(portal.end))
            {
                foundEnd = true;
                tunnelIndex2 = j;
            }
        }

        if (foundStart == true && foundEnd == true)
        {
            // Merge && store to tunnels[tunnelIndex1] && tunnels[tunnelIndex2].deleted = true
            if (tunnelIndex1 != tunnelIndex2)
            {
                Tunnel temp = mergeTunnels(tunnels[tunnelIndex1], tunnels[tunnelIndex2], mortisPositions);
                tunnels[tunnelIndex1] = temp;
                tunnels[tunnelIndex2].deleted = true;
            }
        }
        else if (foundStart == true && foundEnd == false)
        {
            // Add portal to tunnelIndex1 (tunnels[tunnelIndex1].addPosition(portal.end))
            tunnels[tunnelIndex1].addPosition(mortisPositions, portal.end);
        }
        else if (foundStart == false && foundEnd == true)
        {
            // Add portal to tunnelIndex2 (tunnels[tunnelIndex2].addPosition(portal.start))
            tunnels[tunnelIndex2].addPosition(mortisPositions, portal.start);
        }
        else
        {
            // Add new tunnel
            Tunnel temp;
            temp.addPosition(mortisPositions, portal.start);
            temp.addPosition(mortisPositions, portal.end);
            tunnels[lastTunnelIndex++] = temp;
        }

        // check if tunnels are valid
        bool allTunnelsValid = true;
        for (int j = 0; j < lastTunnelIndex; j++)
        {
            if (tunnels[j].deleted)
                continue;

            if (!tunnels[j].isValid())
            {
                allTunnelsValid = false;
                break;
            }
        }

        // printTunnelIndexes(tunnels);
        // cout << i << " allTunnelsValid: " << allTunnelsValid << endl ;
        if (!allTunnelsValid)
            continue;

        // All tunnels are valid, check positions out of tunnels
        bool outOfTunnelPositionsValid = true;
        for (int j = 0; j < N; j++)
        {
            // if position is not in a tunnel && position == mortisPostions[position] VALID
            if (!includedInTunnel(tunnels, j + 1) && mortisPositions[j] != j + 1)
            {
                outOfTunnelPositionsValid = false;
                break;
            }
        }
        // cout << i << " outOfTunnelPositionsValid: " << outOfTunnelPositionsValid << endl ;
        if (!outOfTunnelPositionsValid)
            continue;

        cout << portal.width << endl;
        // return portal.width;
        break;
    }

    // printTunnelIndexes(tunnels);

    // cout <<  "The answer is: " << portals[M].width << endl;
    return 0;
}