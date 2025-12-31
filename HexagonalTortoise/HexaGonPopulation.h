#pragma once

//#define Sharing

class HexaGon;

class HexaGonPopulation
{
private:
	
	//std::mt19937* m_gen;
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
	HexaGon** m_pPopulation;
	HexaGon** m_pPopulationTemp;
#ifdef Sharing
	float* m_pFitnessSharing;
	float** m_pSharing;
	float   m_maxSimilarity;
#endif
	int m_numPopulation;
	int m_numElement;
	int m_next;

	void Crossover(HexaGon* p1, HexaGon* p2, HexaGon* new1, HexaGon* new2);
	void CrossoverRandomPoint(HexaGon* p1, HexaGon* p2, HexaGon* new1, HexaGon* new2);
	void Mutate(HexaGon* c);
	void Replacement(HexaGon* c);
	void MakeLookupTable(int numPoint, int* val1, int* val2, int* newVal1, int *newVal2);
	int Similarity(HexaGon* c1, HexaGon* c2);
	int GetSimilarityIdx(HexaGon* c);
	int GetSelect();
	void ComputeFitnessOrder();
	void Sorting();
#ifdef Sharing
	void SharingFitness();
#endif
public:
	HexaGonPopulation();
	HexaGonPopulation(int numPopulation, int row);
	~HexaGonPopulation();
	void MakeChoromosome();
	float GetFitness();
	void PrintFitness(int topK);
	void PrintFittest();
	bool CheckValid();
};

