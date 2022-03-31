#include <iostream>
// #include <iterator>
#include <algorithm>
// #include <set>
// #include <fstream>
#include <bits/stdc++.h>

using namespace std;

// Subsequence struct with all the needed values
struct Subsequence
{
    int length, last, lastIndex;
};

int N;
int K;

int *initialList;
int *incrementedList;

Subsequence *maxSubseqLis;
Subsequence *changesSubseqLis;
Subsequence *maxSubseqLds;
Subsequence *changesSubseqLds;
Subsequence *changesSubseqLdsPlus;

Subsequence createSubseq(int length, int last, int lastIndex)
{
    Subsequence temp;
    temp.length = length;
    temp.last = last;
    temp.lastIndex = lastIndex;

    return temp;
}
void printSubSeq(Subsequence sub)
{
    cout << "{ length: " << sub.length << ", last: " << sub.last << ", lastIndex: " << sub.lastIndex << " }" << endl;
}

// Binary search helper function for LIS
int CeilIndex(int *v, int l, int r, int key)
{
    while (r - l > 1)
    {
        int m = l + (r - l) / 2;
        if (v[m] >= key)
            r = m;
        else
            l = m;
    }

    return r;
}

int HamiltonianBinary(Subsequence* v, int l, int r, int length, int lastIndex)
{
    while (r - l > 1)
    {
        int m = l + (r - l) / 2;
        if (v[m].length == length && v[m].lastIndex == lastIndex){
            return m;
        }
        if (v[m].length > length || (v[m].length == length && v[m].lastIndex > lastIndex))
            r = m;
        else
            l = m;
    }
    return r;

}

// Funtion that fills the LIS data arrays
void LongestIncreasingSubsequenceLength(int *v)
{
    // if (v.size() == 0)
    //     return 0;

    int tail[N];

    // std::vector<int> tail(v.size(), 0);
    int length = 1; // max Length

    tail[0] = v[0];
    maxSubseqLis[0] = createSubseq(1, v[0], 0);
    changesSubseqLis[0] = createSubseq(1, v[0], 0);

    for (int i = 1; i < N; i++)
    {

        // new smallest value
        if (v[i] < tail[0])
        {
            tail[0] = v[i];
            // If maxLength is not 1 just keep the old maxSubsequence
            if (length > 1)
                maxSubseqLis[i] = maxSubseqLis[i - 1];
            else
                maxSubseqLis[i] = createSubseq(1, v[i], i);

            changesSubseqLis[i] = createSubseq(1, v[i], i);
        }

        // v[i] extends largest subsequence
        else if (v[i] > tail[length - 1])
        {
            tail[length++] = v[i];
            maxSubseqLis[i] = createSubseq(length, v[i], i);
            changesSubseqLis[i] = createSubseq(length, v[i], i);
        }

        // v[i] will become end candidate of an existing
        // subsequence or Throw away larger elements in all
        // LIS, to make room for upcoming greater elements
        // than v[i] (and also, v[i] would have already
        // appeared in one of LIS, identify the location
        // and replace it)
        else
        {
            int index = CeilIndex(tail, -1, length - 1, v[i]);
            tail[index] = v[i];

            // If we changed the last chain, we need to update the max Subsequence
            if (index + 1 == length)
                maxSubseqLis[i] = createSubseq(length, v[i], i);
            else
                maxSubseqLis[i] = maxSubseqLis[i - 1];

            changesSubseqLis[i] = createSubseq(index + 1, v[i], i);

        }
    }
    // return length;
}

// Binary search helper function for LDS
int FloorIndex(int *v, int l, int r, int key)
{
    while (r - l > 1)
    {
        int m = l + (r - l) / 2;
        if (v[m] <= key)
            r = m;
        else
            l = m;
    }

    return r;
}

// Funtion that fills the LDS data arrays
void LongestDecreasingSubsequenceLength(int *v)
{
    // if (v.size() == 0)
    //     return 0;

    int tail[N];

    // std::vector<int> tail(v.size(), 0);
    int length = 1; // max Length

    tail[0] = v[N - 1];
    maxSubseqLds[0] = createSubseq(1, v[N - 1], N - 1);
    changesSubseqLds[0] = createSubseq(1, v[N - 1], N - 1);
    changesSubseqLdsPlus[0] = createSubseq(1, v[N - 1], N - 1);

    for (int i = N - 2; i >= 0; i--)
    {

        // new largest value
        if (v[i] > tail[0])
        {
            tail[0] = v[i];
            // If maxLength is not 1 just keep the old maxSubsequence
            if (length > 1)
                maxSubseqLds[N - 1 - i] = maxSubseqLds[N - 1 - i - 1];
            else
                maxSubseqLds[N - 1 - i] = createSubseq(1, v[i], i);

            changesSubseqLds[N - 1 - i] = createSubseq(1, v[i], i);
            changesSubseqLdsPlus[N - 1 - i] = createSubseq(1, v[i], i);

        }

        // v[i] extends largest subsequence
        else if (v[i] < tail[length - 1])
        {
            tail[length++] = v[i];
            maxSubseqLds[N - 1 - i] = createSubseq(length, v[i], i);
            changesSubseqLds[N - 1 - i] = createSubseq(length, v[i], i);
            changesSubseqLdsPlus[N - 1 - i] = createSubseq(length, v[i], i);
        }

        // v[i] will become end candidate of an existing
        // subsequence or Throw away larger elements in all
        // LIS, to make room for upcoming greater elements
        // than v[i] (and also, v[i] would have already
        // appeared in one of LIS, identify the location
        // and replace it)
        else
        {
            int index = FloorIndex(tail, -1, length - 1, v[i]);
            tail[index] = v[i];

            // If we changed the last chain, we need to update the max Subsequence
            if (index + 1 == length)
                maxSubseqLds[N - 1 - i] = createSubseq(length, v[i], i);
            else
                maxSubseqLds[N - 1 - i] = maxSubseqLds[N - 1 - i - 1];

            changesSubseqLds[N - 1 - i] = createSubseq(index + 1, v[i], i);
            changesSubseqLdsPlus[N - 1 - i] = createSubseq(index + 1, v[i], i);
        }
    }
    // return length;
}

// Returns the length of 2 subsequences combined
int subseqComboLength(Subsequence first, Subsequence second)
{
    if (first.lastIndex == second.lastIndex)
        return first.length + second.length - 1;
    else
        return first.length + second.length;
}

// Takes 4 subsequences and returns the length of the largest valid combination
int bestCombinationLength(Subsequence *maxLisArray, Subsequence *changeLisArray, Subsequence *maxLdsArray, Subsequence *changeLdsArray, int i)
{
    int maxLength = 0;
    Subsequence maxLis = maxLisArray[i];
    Subsequence changeLis = changeLisArray[i];
    Subsequence maxLds = maxLdsArray[N - 1 - i];
    Subsequence changeLds = changeLdsArray[N - 1 - i];

    // #1
    if (maxLis.lastIndex == maxLds.lastIndex)
    {
        maxLength = max(maxLength, subseqComboLength(maxLis, maxLds));

        int tempIndex = N - 1 - i;
        while (tempIndex >= 0 && maxLdsArray[tempIndex].lastIndex == maxLis.lastIndex)
            tempIndex--;
        if (tempIndex != -1 && maxLdsArray[tempIndex].lastIndex > maxLis.lastIndex && maxLdsArray[tempIndex].last > maxLis.last)
            maxLength = max(maxLength, subseqComboLength(maxLis, maxLdsArray[tempIndex]));
    }
    else if (maxLis.last < maxLds.last)
        maxLength = max(maxLength, subseqComboLength(maxLis, maxLds));

    // #2
    // if(maxLis.last < changeLds.last || maxLis.lastIndex == changeLds.lastIndex) maxLength = max(maxLength, subseqComboLength(maxLis, changeLds));
    if (maxLis.lastIndex == changeLds.lastIndex)
    {
        maxLength = max(maxLength, subseqComboLength(maxLis, changeLds));

        int tempIndex = N - 1 - i;
        int index = HamiltonianBinary(changesSubseqLdsPlus, -1, N-1, changeLds.length, changeLds.lastIndex);

        // cout << changeLds.length << " " << changeLds.lastIndex << " " << (changesSubseqLdsPlus[index].length == changeLds.length) << " && " << (changesSubseqLdsPlus[index].lastIndex == changeLds.lastIndex) << endl;

        if(index != N-1 && changesSubseqLdsPlus[index + 1].length == changeLds.length && changesSubseqLdsPlus[index + 1].last > changeLds.last) {
            maxLength = max(maxLength, subseqComboLength(maxLis, changesSubseqLdsPlus[index + 1]));
        }

        // while (tempIndex >= 0)
        // {
        //     if (changeLdsArray[tempIndex].lastIndex > maxLis.lastIndex && changeLdsArray[tempIndex].last > maxLis.last)
        //     {
        //         maxLength = max(maxLength, subseqComboLength(maxLis, changeLdsArray[tempIndex]));
        //     }
        //     tempIndex--;
        // }
    }
    else if (maxLis.last < changeLds.last)
        maxLength = max(maxLength, subseqComboLength(maxLis, changeLds));

    // #3
    // if(changeLis.last < maxLds.last || changeLis.lastIndex == maxLds.lastIndex) maxLength = max(maxLength, subseqComboLength(changeLis, maxLds));
    if (changeLis.lastIndex == maxLds.lastIndex)
    {
        maxLength = max(maxLength, subseqComboLength(changeLis, maxLds));

        int tempIndex = N - 1 - i;
        while (tempIndex >= 0 && maxLdsArray[tempIndex].lastIndex == changeLis.lastIndex)
            tempIndex--;
        if (tempIndex != -1 && maxLdsArray[tempIndex].lastIndex > changeLis.lastIndex && maxLdsArray[tempIndex].last > changeLis.last)
            maxLength = max(maxLength, subseqComboLength(changeLis, maxLdsArray[tempIndex]));
    }
    else if (changeLis.last < maxLds.last)
        maxLength = max(maxLength, subseqComboLength(changeLis, maxLds));

    // #4
    // if(changeLis.last < changeLds.last || changeLis.lastIndex == changeLds.lastIndex) maxLength = max(maxLength, subseqComboLength(changeLis, changeLds));
    if (changeLis.lastIndex == changeLds.lastIndex)
    {
        maxLength = max(maxLength, subseqComboLength(changeLis, changeLds));

        int tempIndex = N - 1 - i;
        int index = HamiltonianBinary(changesSubseqLdsPlus, -1, N-1, changeLds.length, changeLds.lastIndex);

        // cout << changeLds.length << " " << changeLds.lastIndex << " " << (changesSubseqLdsPlus[index].length == changeLds.length) << " && " << (changesSubseqLdsPlus[index].lastIndex == changeLds.lastIndex) << endl;


        if(index != N-1 && changesSubseqLdsPlus[index + 1].length == changeLds.length && changesSubseqLdsPlus[index + 1].last > changeLds.last) {
            maxLength = max(maxLength, subseqComboLength(changeLis, changesSubseqLdsPlus[index + 1]));
        }

        // while (tempIndex >= 0 && maxLdsArray[tempIndex].lastIndex == changeLis.lastIndex)
        // {
        //     if (changeLdsArray[tempIndex].lastIndex > changeLis.lastIndex && changeLdsArray[tempIndex].last > changeLis.last)
        //     {
        //         maxLength = max(maxLength, subseqComboLength(changeLis, changeLdsArray[tempIndex]));
        //     }
        //     tempIndex--;
        // }
    }
    else if (changeLis.last < changeLds.last)
        maxLength = max(maxLength, subseqComboLength(changeLis, changeLds));

    return maxLength;
}

bool compareSubsequences(Subsequence s1, Subsequence s2)
{
    if(s1.length == s2.length) return s1.lastIndex < s2.lastIndex;
    else return (s1.length < s2.length );
}


int main()
{
    // Data input start
    // ios_base::sync_with_stdio(false);
    // cin.tie(NULL);

    cin >> N;
    cin >> K;

    initialList = (int *)malloc(N * sizeof(int));
    incrementedList = (int *)malloc(N * sizeof(int));

    int temp;
    // Inputing data
    for (int ii = 0; ii < N; ii++)
    {
        cin >> temp;
        initialList[ii] = temp;
        incrementedList[ii] = temp + K;
    }

    maxSubseqLis = (Subsequence *)malloc(N * sizeof(Subsequence));
    changesSubseqLis = (Subsequence *)malloc(N * sizeof(Subsequence));
    maxSubseqLds = (Subsequence *)malloc(N * sizeof(Subsequence));
    changesSubseqLds = (Subsequence *)malloc(N * sizeof(Subsequence));
    changesSubseqLdsPlus = (Subsequence *)malloc(N * sizeof(Subsequence));

    LongestIncreasingSubsequenceLength(initialList);

    LongestDecreasingSubsequenceLength(incrementedList);

    sort(changesSubseqLdsPlus, changesSubseqLdsPlus + N, compareSubsequences);

    // for(int ii = 0; ii < N; ii++) printSubSeq(maxSubseqLis[ii]);
    //     cout << endl;

    // for(int ii = 0; ii < N; ii++) printSubSeq(changesSubseqLis[ii]);
    //     cout << endl;

    // for(int ii = 0; ii < N; ii++) printSubSeq(maxSubseqLds[ii]);
    //     cout << endl;

    // for(int ii = 0; ii < N; ii++) printSubSeq(changesSubseqLds[ii]);
    //     cout << endl;

    // for(int ii = 0; ii < N; ii++) printSubSeq(changesSubseqLdsPlus[ii]);
    //     cout << endl;


    int maxLength = 0;
    for (int ii = 0; ii < N; ii++)
    {
        maxLength = max(maxLength, bestCombinationLength(maxSubseqLis, changesSubseqLis, maxSubseqLds, changesSubseqLds, ii));
        // cout << maxLength << endl;
    }
    
    cout << maxLength << endl;
}
