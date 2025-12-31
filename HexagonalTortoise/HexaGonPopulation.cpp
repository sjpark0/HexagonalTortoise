#include "HexaGonPopulation.h"
#include "HexaGon.h"
#include <ctime>
#include <random>
//#include <stdio.h>
//#include <cstdlib>

std::random_device rd;
std::mt19937 gen(rd());

HexaGonPopulation::HexaGonPopulation()
{
	//srand((unsigned int)time(NULL));

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
#ifdef Sharing
	m_pSharing = NULL;
	m_pFitnessSharing = NULL;
#endif
}

HexaGonPopulation::HexaGonPopulation(int numPopulation, int row)
{
	//srand((unsigned int)time(NULL));
	m_numPopulation = numPopulation;
	m_pPopulation = new HexaGon *[m_numPopulation];
	m_pPopulationTemp = new HexaGon * [m_numPopulation];

	for (int i = 0; i < m_numPopulation; i++) {
		m_pPopulation[i] = new HexaGon(row);
		m_pPopulationTemp[i] = new HexaGon(row);
	}
#ifdef Sharing
	m_pSharing = new float* [m_numPopulation];
	m_pFitnessSharing = new float[m_numPopulation];
	for (int i = 0; i < m_numPopulation; i++) {
		m_pSharing[i] = new float[m_numPopulation];
	}
#endif
	for (int i = 0; i < m_numPopulation; i++) {
		m_pPopulation[i]->Generate();
		m_pPopulation[i]->Update();
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
#ifdef Sharing
	if (m_pSharing) {
		for (int i = 0; i < m_numPopulation; i++) {
			delete[]m_pSharing[i];
		}
		delete[]m_pSharing;
		m_pSharing = NULL;
	}
	if (m_pFitnessSharing) {
		delete[]m_pFitnessSharing;
		m_pFitnessSharing = NULL;
	}
#endif

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
		m_pFitness = NULL;
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

void HexaGonPopulation::Crossover(HexaGon* p1, HexaGon* p2, HexaGon* new1, HexaGon* new2)
{
	//int numPoint = (int)(rand() % MAX_POINT) + 1;
	std::uniform_int_distribution<int> dist(0, MAX_POINT - 1);
	std::uniform_int_distribution<int> dist1(0, m_numElement - 1);

	int numPoint = dist(gen) + 1;
	int temp;
	for (int i = 0; i < numPoint; i++) {
		//m_pStart[i] = (int)(rand() % m_numElement);
		m_pStart[i] = dist1(gen);
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
			std::uniform_int_distribution<int> dist2(0, m_pStart[i + 1] - m_pStart[i] - 1);
			temp = dist2(gen);
			//temp = (int)(rand() % (m_pStart[i + 1] - m_pStart[i]));
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
		std::uniform_int_distribution<int> dist3(0, m_numElement - 1 - m_pStart[numPoint - 1] - 1);
		temp = dist3(gen);
        //temp = (int)(rand() % (m_numElement - 1 - m_pStart[numPoint - 1]));
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

void HexaGonPopulation::CrossoverRandomPoint(HexaGon* p1, HexaGon* p2, HexaGon* new1, HexaGon* new2)
{
	int temp;
	std::uniform_int_distribution<int> dist(0, m_numElement - 1);
    //int numPoint = (int)(rand() % m_numElement);
	int numPoint = dist(gen);
	for (int i = 0; i < numPoint; i++) {
		//m_pCrossoverPoint[i] = (int)(rand() % m_numElement);
		m_pCrossoverPoint[i] = dist(gen);
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
	HexaGon* temp;
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
	int minFitness = 100;
	int k = 2;
	m_sumFitness = 0;
	for (int i = 0; i < m_numPopulation; i++) {
		m_pFitness[i] = minFitness + (m_numPopulation - i) * k;
		m_sumFitness += m_pFitness[i];
	}
}
#ifdef Sharing

void HexaGonPopulation::SharingFitness()
{
	m_sumFitness = 0;
	for (int i = 0; i < m_numPopulation; i++) {
		m_pFitnessSharing[i] = 0.0;
		for (int j = 0; j < m_numPopulation; j++) {
			m_pFitnessSharing[i] += m_pSharing[i][j];
		}
		m_pFitnessSharing[i] = (float)m_pFitness[i] / m_pFitnessSharing[i];
		m_sumFitness += (int)m_pFitnessSharing[i];
	}
}
#endif

float HexaGonPopulation::GetFitness()
{
	return m_pPopulation[0]->GetFitness();
}	
int HexaGonPopulation::GetSelect()
{
	//int val = (int)rand();
	//int point = (int)(val % m_sumFitness);
	std::uniform_int_distribution<int> dist(0, m_sumFitness - 1);
	int point = dist(gen);
	int sum = 0;
	for (int i = 0; i < m_numPopulation; i++) {
#ifdef Sharing
		sum += (int)m_pFitnessSharing[i];
#else
		sum += m_pFitness[i];
#endif
		if (point < sum) return i;
	}
	return m_numPopulation - 1;
}
int HexaGonPopulation::Similarity(HexaGon* c1, HexaGon* c2)
{
	int sum = 0;
	int* val1 = c1->GetValue();
	int* val2 = c2->GetValue();
	//int* val1 = c1->GetPhenoValue();
	//int* val2 = c2->GetPhenoValue();

	for (int i = 0; i < m_numElement; i++) {
		sum += abs(val1[i] - val2[i]);
	}
	return sum;
}

int HexaGonPopulation::GetSimilarityIdx(HexaGon* c)
{
	int minSimilairty = Similarity(m_pPopulation[0], c);
	int minIdx = 0;
	int sim;
	for (int i = 1; i < m_numPopulation; i++) {
		sim = Similarity(m_pPopulation[i], c);
		if (minSimilairty > sim) {
			minSimilairty = sim;
			minIdx = i;
		}
	}
	return minIdx;
}
void HexaGonPopulation::Replacement(HexaGon* c)
{
	int id = GetSimilarityIdx(c);
	/*if (c->GetFitness() > m_pPopulation[id]->GetFitness()) {
		for (int i = id + 1; i < m_numPopulation; i++) {
			if (c->GetFitness() < m_pPopulation[i]->GetFitness()) {
				m_pPopulation[i - 1]->Generate(c->GetValue());
				m_pPopulation[i - 1]->Update();

				break;
			}
			else {
				m_pPopulation[i - 1]->Generate(m_pPopulation[i]->GetValue());
				m_pPopulation[i - 1]->Update();
			}
		}
		if (c->GetFitness() > m_pPopulation[m_numPopulation - 1]->GetFitness()) {
			m_pPopulation[m_numPopulation - 1]->Generate(c->GetValue());
			m_pPopulation[m_numPopulation - 1]->Update();
		}
		
	}
	else {
		for (int i = id - 1; i >= 0; i--) {
			if (c->GetFitness() > m_pPopulation[i]->GetFitness()) {
				m_pPopulation[i + 1]->Generate(c->GetValue());
				m_pPopulation[i + 1]->Update();
				break;
			}
			else {
				m_pPopulation[i + 1]->Generate(m_pPopulation[i]->GetValue());
				m_pPopulation[i + 1]->Update();
			}
		}
		if (c->GetFitness() < m_pPopulation[0]->GetFitness()) {
			m_pPopulation[0]->Generate(c->GetValue());
			m_pPopulation[0]->Update();
		}
	}*/
	if (c->GetFitness() < m_pPopulation[id]->GetFitness()) {
		for (int i = id - 1; i >= 0; i--) {
			if (c->GetFitness() > m_pPopulation[i]->GetFitness()) {
				m_pPopulation[i + 1]->Generate(c->GetValue());
				m_pPopulation[i + 1]->Update();
				break;
			}
			else {
				m_pPopulation[i + 1]->Generate(m_pPopulation[i]->GetValue());
				m_pPopulation[i + 1]->Update();
			}
		}
		if (c->GetFitness() < m_pPopulation[0]->GetFitness()) {
			m_pPopulation[0]->Generate(c->GetValue());
			m_pPopulation[0]->Update();
		}
	}

}
void HexaGonPopulation::Mutate(HexaGon* c)
{
	int *val = c->GetValue();
	int id1;
	int id2;
	int temp;
	std::uniform_int_distribution<int> dist(0, 99);
	std::uniform_int_distribution<int> dist1(0, m_numElement - 1);
	//if (rand() % 100 == 0) {
		//id1 = rand() % m_numElement;
		//id2 = rand() % m_numElement;
	if(dist(gen) == 0){
		id1 = dist1(gen);
		id2 = dist1(gen);
		temp = val[id1];
		val[id1] = val[id2];
		val[id2] = temp;
		c->Update();
	}
}

void HexaGonPopulation::MakeChoromosome()
{
#ifdef Sharing
	m_maxSimilarity = 0;
	for (int i = 0; i < m_numPopulation; i++) {
		for (int j = 0; j < m_numPopulation; j++) {
			m_pSharing[i][j] = (float)Similarity(m_pPopulation[i], m_pPopulation[j]);
			if (m_pSharing[i][j] > m_maxSimilarity) {
				m_maxSimilarity = m_pSharing[i][j];
			}
		}
	}
	for (int i = 0; i < m_numPopulation; i++) {
		for (int j = 0; j < m_numPopulation; j++) {
			m_pSharing[i][j] = 1.0 - (m_pSharing[i][j] / m_maxSimilarity) * (m_pSharing[i][j] / m_maxSimilarity);
		}
	}
	SharingFitness();
#endif

	int p1 = GetSelect();
	int p2 = GetSelect();
	//CrossoverRandomPoint(m_pPopulation[p1], m_pPopulation[p2], m_pPopulationTemp[0], m_pPopulationTemp[1]);
	Crossover(m_pPopulation[p1], m_pPopulation[p2], m_pPopulationTemp[0], m_pPopulationTemp[1]);

	Mutate(m_pPopulationTemp[0]);
	Mutate(m_pPopulationTemp[1]);

	Replacement(m_pPopulationTemp[0]);
	Replacement(m_pPopulationTemp[1]);
}


void HexaGonPopulation::PrintFittest()
{
	m_pPopulation[0]->PrintFullHexa();
	for (int i = 0; i < m_pPopulation[0]->GetRow(); i++) {
		for (int j = 0; j < m_pPopulation[0]->GetRow(); j++) {
			m_pPopulation[0]->PrintOneHexa(i, j);
		}
	}
}
void HexaGonPopulation::PrintFitness(int topK)
{
	for (int i = 0; i < topK - 1; i++) {
		printf("%f, ", m_pPopulation[i]->GetFitness());
	}
	printf("%f\n", m_pPopulation[topK - 1]->GetFitness());
	m_pPopulation[0]->PrintFullHexa();
	m_pPopulation[m_numPopulation - 1]->PrintFullHexa();
}
bool HexaGonPopulation::CheckValid()
{
	bool res = true;
	for (int i = 0; i < m_numPopulation; i++) {
		res &= m_pPopulation[i]->CheckValid();
	}
	return res;
}
