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
	bool* m_pCrossover;
	int* m_pFitness;
	int  m_sumFitness;
    HexaGonNew** m_pPopulation;
	HexaGonNew** m_pPopulationTemp;
	int m_numPopulation;
	int m_numElement;
	int m_next;

	void Crossover(HexaGonNew* p1, HexaGonNew* p2, HexaGonNew* new1, HexaGonNew* new2);
	void CrossoverRandomPoint(HexaGonNew* p1, HexaGonNew* p2, HexaGonNew* new1, HexaGonNew* new2);
	void Mutate(HexaGonNew* c);
	void Replacement(HexaGonNew* c);
	void MakeLookupTable(int numPoint, int* val1, int* val2, int* newVal1, int *newVal2);
	int Similarity(HexaGonNew* c1, HexaGonNew* c2);
	int GetSimilarityIdx(HexaGonNew* c);
	int GetSelect();
	void ComputeFitnessOrder();
	void Sorting();

public:
	HexaGonPopulation();
	HexaGonPopulation(int numPopulation, int row);
	~HexaGonPopulation();
	void MakeChoromosome();
	void FullCrossover(int topK);
	float GetFitness();
	void PrintFitness(int topK);
	void PrintFittest();
	bool CheckValid();
};

