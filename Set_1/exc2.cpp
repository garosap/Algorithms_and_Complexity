#include <iostream>
#include <iterator>
#include <set>
#include <fstream>
#include <bits/stdc++.h>
#include <unordered_map>
#include <iterator>

using namespace std;

struct SubArray
{
    int startIndex, endIndex, sum;
};

int ARRAY_SIZE = 10000000;
SubArray *subarrays = new SubArray[ARRAY_SIZE];

int N;
int K;
int lastSubArrayIndex = 0;

// Returns SubArray
SubArray subArrayConstructor(int si, int ei, int s)
{
    SubArray temp;
    temp.startIndex = si;
    temp.endIndex = ei;
    temp.sum = s;

    return temp;
}

void printSubArray(SubArray sa)
{
    cout << "Subarray: { startIndex: " << sa.startIndex << ", endIndex: " << sa.endIndex << ", sum: " << sa.sum << " }" << endl;
}

// Returns the funtion of a subarray
int length(SubArray sa)
{
    return sa.endIndex - sa.startIndex + 1;
}

// Returns true if subarrays overlap
bool overlapping(SubArray s1, SubArray s2)
{
    if ((s1.endIndex < s2.startIndex) || (s1.startIndex > s2.endIndex))
        return false;
    else
        return true;
}

// Calculates prefix sum array
void calcPrefixSums(int *arr)
{
    for (int i = 1; i < N; i++)
        arr[i] = arr[i - 1] + arr[i];
}

bool compareSubarrays(SubArray s1, SubArray s2)
{
    return (s1.sum < s2.sum);
}

// Returns true if conditions are matched and we have found a better minLength
bool checkConditions(SubArray s1, SubArray s2, int minLength)
{
    if (!overlapping(s1, s2) && length(s1) + length(s2) < minLength)
        return true;
    else
        return false;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // ------------------------------------------------------
    cin >> N;
    cin >> K;

    int buildings[N];

    for (int i = 0; i < N; i++)
        cin >> buildings[i];


    int i = 0, j = 0;
    int prefixSums[N];

    for (int ii = 0; ii < N; ii++)
        prefixSums[ii] = buildings[ii];

    calcPrefixSums(prefixSums);
    // Buildings is now prefixSum array

    // Becomes true if an optimal building is found
    bool foundOptimal = false;

    // Create array of subarrays
    for (i = 0; i < N; i++)
    {
        if (foundOptimal)
            break;
        for (j = i; j < N; j++)
        {
            if (i == j)
            {
                if (buildings[i] >= K)
                    break;
                else if (buildings[i] < K)
                    subarrays[lastSubArrayIndex++] = subArrayConstructor(i, j, buildings[i]);
                else
                {
                    minLength = 1;
                    foundOptimal = true;
                    break;
                }
            }
            else
            {
                int sum = (i == 0 ? prefixSums[j] : prefixSums[j] - prefixSums[i - 1]);

                if (sum >= K)
                    break;
                else
                    subarrays[lastSubArrayIndex++] = subArrayConstructor(i, j, sum);
            }
        }
    }

    if (foundOptimal)
        cout << minLength << endl;
    else
    {

        sort(subarrays, subarrays + lastSubArrayIndex, compareSubarrays);

        // for(int ii=0; ii<lastSubArrayIndex; ii++) printSubArray(subarrays[ii]);

        int low = 0;
        int high = lastSubArrayIndex - 1;
        int minLength = INT_MAX;

        while (low < high)
        {
            if (subarrays[low].sum + subarrays[high].sum == K)
            {
                // If conditions are met update value, else dont
                if (checkConditions(subarrays[low], subarrays[high], minLength))
                {
                    minLength = length(subarrays[low]) + length(subarrays[high]);
                }

                int high_backup = high, low_backup = low;

                while (subarrays[low].sum + subarrays[high - 1].sum == K && low < high - 1)
                {
                    high--;
                    // If conditions are met update value, else dont
                    if (checkConditions(subarrays[low], subarrays[high], minLength))
                    {
                        minLength = length(subarrays[low]) + length(subarrays[high]);
                    }
                }
                high = high_backup;

                while (subarrays[low + 1].sum + subarrays[high].sum == K && low + 1 < high)
                {
                    low++;
                    // If conditions are met update value, else dont
                    if (checkConditions(subarrays[low], subarrays[high], minLength))
                    {
                        minLength = length(subarrays[low]) + length(subarrays[high]);
                    }
                }
                low = low_backup;

                high--;
                low++;
            }
            else if (subarrays[low].sum + subarrays[high].sum > K)
            {
                high--;
            }
            else
            {
                low++;
            }
        }

        if (minLength == INT_MAX)
            cout << -1 << endl;
        else
            cout << minLength << endl;
    }
}