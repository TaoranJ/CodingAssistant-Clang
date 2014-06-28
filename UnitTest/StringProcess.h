#include <iostream>
#define M 100
 
using namespace std;
 
const bool debug = false; // Whether to print debug info
const bool memoize = true; // Whether to use memoization
unsigned int cnt = 0; // Line number for the debug info
int memoizedDistance[M][M]; // Matrix for memoiztion


int minValue(int a, int b, int c);
int calculateStringDistance(string strA, int pABegin, int pAEnd, string strB, int pBBegin, int pBEnd);
double calculateSimilarity(string sourceStr, string targertStr);
 
int minValue(int a, int b, int c)
{
	if(a < b && a < c) return a;
	else if(b < a && b < c) return b;
	else return c;
}
 
/*
 * A recursive method which can be decorated by memoization.
 * Calculate from top to bottom.
 */
int calculateStringDistance(string strA, int pABegin, int pAEnd, string strB, int pBBegin, int pBEnd)
{
	if(memoize && memoizedDistance[pABegin][pBBegin] >= 0)
		return memoizedDistance[pABegin][pBBegin];
 
	if(pABegin > pAEnd)
	{
		if(pBBegin > pBEnd)
		{
			if(memoize)
				memoizedDistance[pABegin][pBBegin] = 0;
			if(debug)
				cout << cnt++ << ": m(" << pABegin << "," << pBBegin << ")=0" << endl;
			return 0;
		}
		else
		{
			int temp = pBEnd - pBBegin + 1;
			if(memoize)
				memoizedDistance[pABegin][pBBegin] = temp;
			if(debug)
				cout << cnt++ << ": m(" << pABegin << "," << pBBegin << ")=" << temp << endl;
			return temp;
		}
	}
	if(pBBegin > pBEnd)
	{
		if(pABegin > pAEnd)
		{
			if(memoize)
				memoizedDistance[pABegin][pBBegin] = 0;
			if(debug)
				cout << cnt++ << ": m(" << pABegin << "," << pBBegin << ")=0" << endl;
			return 0;
		}
		else
		{
			int temp = pAEnd - pABegin + 1;
			if(memoize)
				memoizedDistance[pABegin][pBBegin] = temp;
			if(debug)
				cout << cnt++ << ": m(" << pABegin << "," << pBBegin << ")=" << temp << endl;
			return temp;
		}
	}
 
	if(strA[pABegin] == strB[pBBegin])
	{
		int temp = calculateStringDistance(strA, pABegin+1, pAEnd, strB, pBBegin+1, pBEnd);
		if(memoize)
			memoizedDistance[pABegin][pBBegin] = temp;
		if(debug)
			cout << cnt++ << ": m(" << pABegin << "," << pBBegin << ")=" << temp << endl;
		return temp;
	}
	else
	{
		int t1 = calculateStringDistance(strA, pABegin, pAEnd, strB, pBBegin+1, pBEnd);
		int t2 = calculateStringDistance(strA, pABegin+1, pAEnd, strB, pBBegin, pBEnd);
		int t3 = calculateStringDistance(strA, pABegin+1, pAEnd, strB, pBBegin+1, pBEnd);
		int temp = minValue(t1, t2, t3) + 1;
		if(memoize)
			memoizedDistance[pABegin][pBBegin] = temp;
		if(debug)
			cout << cnt++ << ": m(" << pABegin << "," << pBBegin << ")=" << temp << endl;
		return temp;
	}
}

double calculateSimilarity(string sourceStr, string targertStr){
	if(memoize)
	{
		// initialize the matrix : memoizedDistance[][]
		for(int i = 0; i < M; i++)
			for(int j = 0; j < M; j++)
				memoizedDistance[i][j] = -1; // -1 means unfilled cell yet
	}
	int sourceStrLen = sourceStr.length() - 1;
	int targertStrLen = targertStr.length() - 1;
	int strDistance = calculateStringDistance(sourceStr,0,sourceStrLen,targertStr,0,targertStrLen);
	return 1.0 / (1 + strDistance);
}

/*
int main()
{
	if(memoize)
	{
		// initialize the matrix : memoizedDistance[][]
		for(int i = 0; i < M; i++)
			for(int j = 0; j < M; j++)
				memoizedDistance[i][j] = -1; // -1 means unfilled cell yet
	}

	string strA = "abcdfef";
	string strB = "abcdfefgf";
 
	cout << calculateStringDistance(strA,0,(int)strA.length() - 1, strB, 0, (int)strB.length() - 1) << endl;
	cout << endl << "Similarity = " << 1.0 / (1 + calculateStringDistance(strA, 0, (int)strA.length()-1, strB, 0, (int)strB.length()-1)) << endl;
	return 0;
}
*/
