#pragma once
class HexaGonNew;

class HexaGonPopulation
{
private:
	int MAX_POINT = 3;
	int* m_pStart;
	int* m_pLength;
	int* m_pLookup1;
	int* m_pLookup2;

	HexaGonNew** m_pPopulation;
	int m_numPopulation;
	int m_numElement;
	int m_next;
	bool m_bSorted;
public:
	HexaGonPopulation();
	HexaGonPopulation(int numPopulation, int row);
	~HexaGonPopulation();
	void Crossover(int p1, int p2, int new1, int new2);
	
	void Sorting();
	void FullCrossover(int topK);
	float GetFitness();
	void PrintFittest();
};

