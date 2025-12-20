#include "HexaGonPopulation.h"
#include "HexaGonNew.h"
#include <stdio.h>
#include <cstdlib>
#include <ctime>

HexaGonPopulation::HexaGonPopulation()
{
	srand((unsigned int)time(NULL));
	m_pPopulation = NULL;
	m_pPopulationTemp = NULL;
	m_pStart = NULL;
    m_pCrossoverPoint = NULL;
	//m_pLength = NULL;
	m_pLookup1 = NULL;
	m_pLookup2 = NULL;
	m_pVisited = NULL;
	m_pCrossover = NULL;
	m_pFitness = NULL;
}

HexaGonPopulation::HexaGonPopulation(int numPopulation, int row)
{
	srand((unsigned int)time(NULL));
	
	m_numPopulation = numPopulation;
	m_pPopulation = new HexaGonNew*[m_numPopulation];
	m_pPopulationTemp = new HexaGonNew * [m_numPopulation];

	for (int i = 0; i < m_numPopulation; i++) {
		m_pPopulation[i] = new HexaGonNew(row);
		m_pPopulationTemp[i] = new HexaGonNew(row);
	}
	for (int i = 0; i < m_numPopulation; i++) {
		m_pPopulation[i]->Generate();
	}

	m_numElement = m_pPopulation[0]->GetNumValue();
	m_pStart = new int[MAX_POINT];
    m_pCrossoverPoint = new int[m_numElement];
	//m_pLength = new int[m_numElement];

	m_pLookup1 = new int[m_numElement + 1];
	m_pLookup2 = new int[m_numElement + 1];		
	m_pVisited = new bool[m_numElement + 1];
	m_pCrossover = new bool[m_numElement];
	m_pFitness = new int[m_numPopulation];
	ComputeFitnessOrder();
	Sorting();
}

HexaGonPopulation::~HexaGonPopulation()
{
	if (m_pPopulation) {
		for (int i = 0; i < m_numPopulation; i++) {
			delete m_pPopulation[i];
		}
		delete[]m_pPopulation;
		m_pPopulation = NULL;
	}
    if(m_pStart){
        delete []m_pStart;
        m_pStart = NULL;
    }
    if(m_pCrossoverPoint){
        delete []m_pCrossoverPoint;
        m_pCrossoverPoint = NULL;
    }
	if (m_pVisited) {
		delete[]m_pVisited;
		m_pVisited = NULL;
	}
	if (m_pLookup1) {
		delete[]m_pLookup1;
		m_pLookup1 = NULL;
	}
	if (m_pLookup2) {
		delete[]m_pLookup2;
		m_pLookup2 = NULL;
	}
	if (m_pCrossover) {
		delete[]m_pCrossover;
		m_pCrossover = NULL;
	}
	if (m_pFitness) {
		delete[]m_pFitness;
	}
}

void HexaGonPopulation::MakeLookupTable(int numPoint, int* val1, int* val2, int *newVal1, int *newVal2)
{
    
	int temp;
	for (int i = 0; i < m_numElement + 1; i++) {
		m_pLookup1[i] = i;
		m_pLookup2[i] = i;
	}
	for (int i = 0; i < numPoint; i++) {
		m_pLookup1[val2[m_pCrossoverPoint[i]]] = val1[m_pCrossoverPoint[i]];
		m_pLookup2[val1[m_pCrossoverPoint[i]]] = val2[m_pCrossoverPoint[i]];
	}
	
}

void HexaGonPopulation::Crossover(HexaGonNew* p1, HexaGonNew* p2, HexaGonNew* new1, HexaGonNew* new2)
{
	int numPoint = (int)(rand() % MAX_POINT) + 1;
	int temp;
	for (int i = 0; i < numPoint; i++) {
		m_pStart[i] = (int)(rand() % m_numElement);		
	}
	for (int i = 0; i < numPoint; i++) {
		for (int j = i + 1; j < numPoint; j++) {
			if (m_pStart[i] > m_pStart[j]) {
				temp = m_pStart[i];
				m_pStart[i] = m_pStart[j];
				m_pStart[j] = temp;
			}
		}
	}
    
    int cnt = 0;
	for (int i = 0; i < numPoint - 1; i++) {
		if (m_pStart[i] == m_pStart[i + 1]) {
            m_pCrossoverPoint[cnt] = m_pStart[i];
            cnt++;
		}
		else {
			temp = (int)(rand() % (m_pStart[i + 1] - m_pStart[i]));
            for (int j = m_pStart[i]; j < m_pStart[i] + temp; j++) {
                m_pCrossoverPoint[cnt] = j;
                cnt++;
            }
		}
        
	}
	if (m_pStart[numPoint - 1] == m_numElement - 1) {
        m_pCrossoverPoint[cnt] = m_numElement - 1;
        cnt++;
	}
	else {
        temp = (int)(rand() % (m_numElement - 1 - m_pStart[numPoint - 1]));
        for (int j = m_pStart[numPoint - 1]; j < m_pStart[numPoint - 1] + temp; j++) {
            m_pCrossoverPoint[cnt] = j;
            cnt++;
        }
	}
    numPoint = cnt;
	int* val1 = p1->GetValue();
	int* val2 = p2->GetValue();

	int* newVal1 = new1->GetValue();
	int* newVal2 = new2->GetValue();

    MakeLookupTable(numPoint, val1, val2, newVal1, newVal2);
    
	for (int i = 0; i < m_numElement; i++) {
		newVal1[i] = m_pLookup1[val1[i]];
		newVal2[i] = m_pLookup2[val2[i]];
		m_pCrossover[i] = false;
	}
	for (int i = 0; i < numPoint; i++) {
		newVal1[m_pCrossoverPoint[i]] = val2[m_pCrossoverPoint[i]];
		newVal2[m_pCrossoverPoint[i]] = val1[m_pCrossoverPoint[i]];
		m_pCrossover[m_pCrossoverPoint[i]] = true;
	}
	
	for (int i = 0; i < m_numElement; i++) {
		if (!m_pCrossover[i]) {
			temp = m_pLookup1[val1[i]];
			while (temp != m_pLookup1[temp]) {
				temp = m_pLookup1[temp];
			}
			newVal1[i] = temp;

			temp = m_pLookup2[val2[i]];
			while (temp != m_pLookup2[temp]) {
				temp = m_pLookup2[temp];
			}
			newVal2[i] = temp;


		}
		//newVal1[i] = m_pLookup1[val1[i]];
		//newVal2[i] = m_pLookup2[val2[i]];
	}
    
    
	new1->Update();
	new2->Update();

}

void HexaGonPopulation::CrossoverRandomPoint(HexaGonNew* p1, HexaGonNew* p2, HexaGonNew* new1, HexaGonNew* new2)
{
	int temp;
    int numPoint = (int)(rand() % m_numElement);
	for (int i = 0; i < numPoint; i++) {
		m_pCrossoverPoint[i] = (int)(rand() % m_numElement);
	}

	int* val1 = p1->GetValue();
	int* val2 = p2->GetValue();

	int* newVal1 = new1->GetValue();
	int* newVal2 = new2->GetValue();
    
    MakeLookupTable(numPoint, val1, val2, newVal1, newVal2);
    
	for (int i = 0; i < m_numElement; i++) {
		newVal1[i] = m_pLookup1[val1[i]];
		newVal2[i] = m_pLookup2[val2[i]];
		m_pCrossover[i] = false;
	}
	for (int i = 0; i < numPoint; i++) {
		newVal1[m_pCrossoverPoint[i]] = val2[m_pCrossoverPoint[i]];
		newVal2[m_pCrossoverPoint[i]] = val1[m_pCrossoverPoint[i]];
		m_pCrossover[m_pCrossoverPoint[i]] = true;
	}

	for (int i = 0; i < m_numElement; i++) {
		if (!m_pCrossover[i]) {
			temp = m_pLookup1[val1[i]];
			while (temp != m_pLookup1[temp]) {
				temp = m_pLookup1[temp];
			}
			newVal1[i] = temp;

			temp = m_pLookup2[val2[i]];
			while (temp != m_pLookup2[temp]) {
				temp = m_pLookup2[temp];
			}
			newVal2[i] = temp;


		}
		//newVal1[i] = m_pLookup1[val1[i]];
		//newVal2[i] = m_pLookup2[val2[i]];
	}
	
	
	new1->Update();
	new2->Update();

}
void HexaGonPopulation::Sorting()
{
	HexaGonNew* temp;
	for (int i = 0; i < m_numPopulation; i++) {
		for (int j = i + 1; j < m_numPopulation; j++) {
			if (m_pPopulation[i]->GetFitness() > m_pPopulation[j]->GetFitness()) {
				temp = m_pPopulation[i];
				m_pPopulation[i] = m_pPopulation[j];
				m_pPopulation[j] = temp;
			}
		}
	}
}
void HexaGonPopulation::ComputeFitnessOrder()
{
	int minFitness = 10000;
	int maxFitness = 40000;
	m_sumFitness = 0;
	for (int i = 0; i < m_numPopulation; i++) {
		m_pFitness[i] = maxFitness + i * (float)(minFitness - maxFitness) / (float)(m_numPopulation - 1);
		m_sumFitness += m_pFitness[i];
	}
}
float HexaGonPopulation::GetFitness()
{
	return m_pPopulation[0]->GetFitness();
}	
int HexaGonPopulation::GetSelect()
{
	int val = (int)rand() * (int)rand();
	int point = (int)(val % m_sumFitness);
	int sum = 0;
	for (int i = 0; i < m_numPopulation; i++) {
		sum += m_pFitness[i];
		if (point < sum) return i;
	}
	return m_numPopulation - 1;
	//return (int)(rand() % m_numPopulation);
}
void HexaGonPopulation::FullCrossover(int topK)
{
	int p1;
	int p2;
	
	for (int i = 0; i < m_numPopulation; i += 2) {
		p1 = GetSelect();
		p2 = GetSelect();
		CrossoverRandomPoint(m_pPopulation[p1], m_pPopulation[p2], m_pPopulationTemp[i], m_pPopulationTemp[i + 1]);
	}
	for (int i = 0; i < m_numPopulation; i++) {
		m_pPopulation[i]->Generate(m_pPopulationTemp[i]->GetValue());
		m_pPopulation[i]->Update();
	}
	
	Sorting();
	
}

void HexaGonPopulation::PrintFittest()
{
	Sorting();
	m_pPopulation[0]->PrintFullHexa();
}
void HexaGonPopulation::PrintFitness(int topK)
{
	for (int i = 0; i < topK - 1; i++) {
		printf("%f, ", m_pPopulation[i]->GetFitness());
	}
	printf("%f\n", m_pPopulation[topK - 1]->GetFitness());
}
bool HexaGonPopulation::CheckValid()
{
	bool res = true;
	for (int i = 0; i < m_numPopulation; i++) {
		res &= m_pPopulation[i]->CheckValid();
	}
	return res;
}
