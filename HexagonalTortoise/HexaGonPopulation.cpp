#include "HexaGonPopulation.h"
#include "HexaGonNew.h"
#include <stdio.h>
#include <cstdlib>
#include <ctime>

HexaGonPopulation::HexaGonPopulation()
{
	srand((unsigned int)time(NULL));
	m_pPopulation = NULL;
	m_pStart = NULL;
    m_pCrossoverPoint = NULL;
	//m_pLength = NULL;
	m_pLookup1 = NULL;
	m_pLookup2 = NULL;
	m_bSorted = false;
	m_pVisited = NULL;
}

HexaGonPopulation::HexaGonPopulation(int numPopulation, int row)
{
	srand((unsigned int)time(NULL));
	
	m_numPopulation = numPopulation;
	m_pPopulation = new HexaGonNew*[m_numPopulation];

	for (int i = 0; i < m_numPopulation; i++) {
		m_pPopulation[i] = new HexaGonNew(row);
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
	m_bSorted = false;
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
	m_bSorted = false;
}

void HexaGonPopulation::MakeLookupTable(int numPoint, int* val1, int* val2, int *newVal1, int *newVal2)
{
    for (int i = 0; i < m_numElement + 1; i++) {
        m_pLookup1[i] = i;
        m_pLookup2[i] = i;
        m_pVisited[i] = false;
    }
    for (int i = 0; i < numPoint; i++) {
        for (int j = 0; j < numPoint; j++) {
            if (val1[m_pCrossoverPoint[i]] == val2[m_pCrossoverPoint[j]]) {
                m_pVisited[val1[m_pCrossoverPoint[i]]] = true;
                break;
            }
        }
    }
    int pos;
    for (int i = 0; i < numPoint; i++) {
        pos = 0;
        if (!m_pVisited[val1[m_pCrossoverPoint[i]]]) {
            while (pos < numPoint) {
                if (!m_pVisited[val2[m_pCrossoverPoint[pos]]]) {
                    m_pLookup1[val2[m_pCrossoverPoint[pos]]] = val1[m_pCrossoverPoint[i]];
                    m_pLookup2[val1[m_pCrossoverPoint[i]]] = val2[m_pCrossoverPoint[pos]];
                    m_pVisited[val1[m_pCrossoverPoint[i]]] = true;
                    m_pVisited[val2[m_pCrossoverPoint[pos]]] = true;
                    //pos++;
                    break;
                }
                pos++;
            }
        }
    }
}

void HexaGonPopulation::Crossover(int p1, int p2, int new1, int new2)
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
	int* val1 = m_pPopulation[p1]->GetValue();
	int* val2 = m_pPopulation[p2]->GetValue();

	int* newVal1 = m_pPopulation[new1]->GetValue();
	int* newVal2 = m_pPopulation[new2]->GetValue();

    MakeLookupTable(numPoint, val1, val2, newVal1, newVal2);
    
    for (int i = 0; i < m_numElement; i++) {
        newVal1[i] = m_pLookup1[val1[i]];
        newVal2[i] = m_pLookup2[val2[i]];
    }
    for (int i = 0; i < numPoint; i++) {
        newVal1[m_pCrossoverPoint[i]] = val2[m_pCrossoverPoint[i]];
        newVal2[m_pCrossoverPoint[i]] = val1[m_pCrossoverPoint[i]];
    }
    /*if (!m_pPopulation[new1]->CheckValid()) {
        printf("Debug!!\n");
        m_pPopulation[p1]->PrintFullHexa();
        m_pPopulation[p2]->PrintFullHexa();
        printf("%d, %d => %d, %d\n", p1, p2, m_pPopulation[p1]->CheckValid(), m_pPopulation[p2]->CheckValid());
        m_pPopulation[new1]->PrintFullHexa();
        printf("\n");
    }
    if (!m_pPopulation[new2]->CheckValid()) {
        printf("Debug!!\n");
        m_pPopulation[p1]->PrintFullHexa();
        m_pPopulation[p2]->PrintFullHexa();
        printf("%d, %d => %d, %d\n", p1, p2, m_pPopulation[p1]->CheckValid(), m_pPopulation[p2]->CheckValid());
        
        m_pPopulation[new2]->PrintFullHexa();
        printf("\n");
    }*/
    
	m_pPopulation[new1]->Update();
	m_pPopulation[new2]->Update();

}

void HexaGonPopulation::CrossoverRandomPoint(int p1, int p2, int new1, int new2)
{
    int numPoint = (int)(rand() % m_numElement);
	for (int i = 0; i < numPoint; i++) {
		m_pCrossoverPoint[i] = (int)(rand() % m_numElement);
	}

	int* val1 = m_pPopulation[p1]->GetValue();
	int* val2 = m_pPopulation[p2]->GetValue();

	int* newVal1 = m_pPopulation[new1]->GetValue();
	int* newVal2 = m_pPopulation[new2]->GetValue();
    
    MakeLookupTable(numPoint, val1, val2, newVal1, newVal2);
    
	for (int i = 0; i < m_numElement; i++) {
		newVal1[i] = m_pLookup1[val1[i]];
		newVal2[i] = m_pLookup2[val2[i]];
	}
	for (int i = 0; i < numPoint; i++) {
		newVal1[m_pCrossoverPoint[i]] = val2[m_pCrossoverPoint[i]];
		newVal2[m_pCrossoverPoint[i]] = val1[m_pCrossoverPoint[i]];
	}
	/*if (!m_pPopulation[new1]->CheckValid()) {
        printf("Debug!!\n");
        m_pPopulation[p1]->PrintFullHexa();
		m_pPopulation[p2]->PrintFullHexa();
		printf("%d, %d => %d, %d\n", p1, p2, m_pPopulation[p1]->CheckValid(), m_pPopulation[p2]->CheckValid());
        m_pPopulation[new1]->PrintFullHexa();
        printf("\n");
	}
	if (!m_pPopulation[new2]->CheckValid()) {
        printf("Debug!!\n");
        m_pPopulation[p1]->PrintFullHexa();
		m_pPopulation[p2]->PrintFullHexa();
		printf("%d, %d => %d, %d\n", p1, p2, m_pPopulation[p1]->CheckValid(), m_pPopulation[p2]->CheckValid());
        
		m_pPopulation[new2]->PrintFullHexa();
        printf("\n");
	}*/
	
	
	m_pPopulation[new1]->Update();
	m_pPopulation[new2]->Update();

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
	m_bSorted = true;
}
float HexaGonPopulation::GetFitness()
{
	if (!m_bSorted) {
		Sorting();
	}
	
	//m_pPopulation[0]->PrintFullHexa();
	return m_pPopulation[0]->GetFitness();
}	
void HexaGonPopulation::FullCrossover(int topK)
{
	int p1;
	int p2;
	if (!m_bSorted) {
		Sorting();
	}
	for (int next = topK; next < m_numPopulation; next += 2) {
		p1 = (int)(rand() % m_numPopulation);
		p2 = (int)(rand() % m_numPopulation);
		while (p1 == p2 || p1 == next || p1 == next + 1 || p2 == next || p2 == next + 1) {
            p1 = (int)(rand() % m_numPopulation);
			p2 = (int)(rand() % m_numPopulation);
		}

		//Crossover(p1, p2, next, next + 1);
		CrossoverRandomPoint(p1, p2, next, next + 1);
	}
	Sorting();
	
}

void HexaGonPopulation::PrintFittest()
{
	Sorting();
	m_pPopulation[0]->PrintFullHexa();
}

bool HexaGonPopulation::CheckValid()
{
	bool res = true;
	for (int i = 0; i < m_numPopulation; i++) {
		res &= m_pPopulation[i]->CheckValid();
	}
	return res;
}
