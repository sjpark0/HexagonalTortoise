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
	m_pLength = NULL;
	m_pLookup1 = NULL;
	m_pLookup2 = NULL;
	m_bSorted = false;
}

HexaGonPopulation::HexaGonPopulation(int numPopulation, int row)
{
	srand((unsigned int)time(NULL));
	m_pStart = new int[MAX_POINT];
	m_pLength = new int[MAX_POINT];

	m_numPopulation = numPopulation;
	m_pPopulation = new HexaGonNew*[m_numPopulation];

	for (int i = 0; i < m_numPopulation; i++) {
		m_pPopulation[i] = new HexaGonNew(row);
	}
	for (int i = 0; i < m_numPopulation; i++) {
		m_pPopulation[i]->Generate();
	}

	m_numElement = m_pPopulation[0]->GetNumValue();

	m_pLookup1 = new int[m_numElement + 1];
	m_pLookup2 = new int[m_numElement + 1];		
	m_bSorted = false;
}

HexaGonPopulation::~HexaGonPopulation()
{
	if (m_pPopulation) {
		for (int i = 0; i < m_numPopulation; i++) {
			delete m_pPopulation[i];
		}
		delete[]m_pPopulation;
	}
	m_bSorted = false;
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

	for (int i = 0; i < numPoint - 1; i++) {
		if (m_pStart[i] == m_pStart[i + 1]) {
			m_pLength[i] = 0;
		}
		else {
			m_pLength[i] = (int)(rand() % (m_pStart[i + 1] - m_pStart[i]));
		}
	}
	if (m_pStart[numPoint - 1] == m_numElement - 1) {
		m_pLength[numPoint - 1] = 0;
	}
	else {
		m_pLength[numPoint - 1] = (int)(rand() % (m_numElement - 1 - m_pStart[numPoint - 1]));
	}

	int* val1 = m_pPopulation[p1]->GetValue();
	int* val2 = m_pPopulation[p2]->GetValue();

	int* newVal1 = m_pPopulation[new1]->GetValue();
	int* newVal2 = m_pPopulation[new2]->GetValue();

	for (int i = 0; i < m_numElement + 1; i++) {
		m_pLookup1[i] = i;
		m_pLookup2[i] = i;
	}
	for (int i = 0; i < numPoint; i++) {
		for (int j = m_pStart[i]; j < m_pStart[i] + m_pLength[i]; j++) {
			m_pLookup1[val2[j]] = val1[j];
			m_pLookup2[val1[j]] = val2[j];
		}
	}
	for (int i = 0; i < m_numElement; i++) {
		newVal1[i] = m_pLookup1[val1[i]];
		newVal2[i] = m_pLookup2[val2[i]];
	}
	for (int i = 0; i < numPoint; i++) {
		for (int j = m_pStart[i]; j < m_pStart[i] + m_pLength[i]; j++) {
			newVal1[j] = val2[j];
			newVal2[j] = val1[j];
		}
	}
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
		while (p1 == p2) {
			p2 = (int)(rand() % m_numPopulation);
		}

		Crossover(p1, p2, next, next + 1);
	}
	Sorting();
}

void HexaGonPopulation::PrintFittest()
{
	Sorting();
	m_pPopulation[0]->PrintFullHexa();
}
