#pragma once
class HexaGonNew;

class HexaGonPopulation
{
private:
	int MAX_POINT = 3;
	int* m_pStart;
    int* m_pCrossoverPoint;
	//int* m_pLength;
	int* m_pLookup1;
	int* m_pLookup2;
	bool* m_pVisited;
	bool* m_pCrossover1;
	bool* m_pCrossover2;

    HexaGonNew** m_pPopulation;
	int m_numPopulation;
	int m_numElement;
	int m_next;
	bool m_bSorted;
    void MakeLookupTable(int numPoint, int* val1, int* val2, int* newVal1, int *newVal2);
public:
	HexaGonPopulation();
	HexaGonPopulation(int numPopulation, int row);
	~HexaGonPopulation();
	void Crossover(int p1, int p2, int new1, int new2);
	void CrossoverRandomPoint(int p1, int p2, int new1, int new2);
	void Sorting();
	void FullCrossover(int topK);
	float GetFitness();
	void PrintFittest();
	bool CheckValid();
};

