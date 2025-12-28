#include "HexaGonNew.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <cstdlib>
#include <ctime>

HexaGonNew::HexaGonNew()
{
	srand((unsigned int)time(NULL));
	m_pValue = NULL;
	m_pValuePheno = NULL;
	m_pHexa = NULL;
	m_sum = NULL;
	m_pValid = NULL;
}
HexaGonNew::HexaGonNew(int row)
{
	srand((unsigned int)time(NULL));
	MakeHexa(row);
}
HexaGonNew::~HexaGonNew()
{
	if (m_pValue) {
		delete[]m_pValue;
		m_pValue = NULL;
	}
	if (m_pValuePheno) {
		delete[]m_pValuePheno;
		m_pValuePheno = NULL;
	}
	if (m_pHexa) {
		for (int i = 0; i < m_row * m_row; i++) {
			delete[]m_pHexa[i];
		}
		delete[]m_pHexa;
	}
	if (m_sum) {
		delete[]m_sum;
	}
	if (m_pValid) {
		delete[]m_pValid;
		m_pValid = NULL;
	}
}
void HexaGonNew::MakeHexa(int row)
{
	int start1;
	int start2;;

	m_row = row;
	m_num = 2 * row * (row + 2);
	m_pValue = new int[m_num];
	m_pValuePheno = new int[m_num];
	m_pValid = new bool[m_num + 1];
	m_pHexa = new int** [m_row * m_row];
	m_sum = new int[m_row * m_row];
	for (int i = 0; i < m_row; i++) {
		for (int j = 0; j < m_row; j++) {
			m_pHexa[j + i * m_row] = new int* [6];
			start1 = 2 * (m_row + 1) * i + 2 * j;
			start2 = 2 * (m_row + 1) * (i + 1) - 1 + 2 * j;
			for (int k = 0; k < 3; k++) {
				m_pHexa[j + i * m_row][k] = &m_pValue[start1 + k];
				m_pHexa[j + i * m_row][5 - k] = &m_pValue[start2 + k];
			}
		}
	}
}

void HexaGonNew::Generate()
{
	int numVisited = 0;
	int val;
	int pos;
	int cnt;
	memset(m_pValue, 0, m_num * sizeof(int));
	for (int i = 0; i < m_num; i++) {		
		pos = (int)(rand() % (m_num - i));
		cnt = 0;
		for (int j = 0; j < m_num; j++) {
			if (m_pValue[j] > 0) continue;
			if (pos == cnt) {
				m_pValue[j] = i + 1;
				break;
			}
			cnt++;
		}

	}
}
void HexaGonNew::Generate(int* value)
{
	memcpy(m_pValue, value, m_num * sizeof(int));	
}
void HexaGonNew::Update()
{
	memcpy(m_pValuePheno, m_pValue, m_num * sizeof(int));
	ComputeSum();
	ComputeFitness();
	//Optimize();
}
void HexaGonNew::ComputeSum()
{
	for (int i = 0; i < m_row; i++) {
		for (int j = 0; j < m_row; j++) {
			m_sum[j + i * m_row] = ComputeSumOneHexa(i, j);
		}
	}
}

int HexaGonNew::ComputeSumOneHexa(int row, int col)
{
	int res = 0;
	for (int i = 0; i < 6; i++) {
		res += *m_pHexa[col + row * m_row][i];
	}
	return res;
}

void HexaGonNew::ComputeFitness()
{
	float avg = 0.0;
	for (int i = 0; i < m_row * m_row; i++) {
		avg += m_sum[i];
	}
	avg /= (m_row * m_row);

	float std = 0.0;
	for (int i = 0; i < m_row * m_row; i++) {
		std += (m_sum[i] - avg) * (m_sum[i] - avg);
	}
	m_fitness = sqrt(std / (m_row * m_row)) + 10000.0 / avg;
	//m_fitness = sqrt(std / (m_row * m_row));

	/*float std = 1.0;
	for (int i = 0; i < m_row * m_row; i++) {
		std *= (m_sum[i] / avg);
	}
	m_fitness = 1.0 / std;*/
	
}
void HexaGonNew::Optimize()
{
	float minTemp;
	int rotate;
	float minVal = m_fitness;
	while (true) {
		for (int i = 0; i < m_row; i++) {
			for (int j = 0; j < m_row; j++) {
				minTemp = m_fitness;
				rotate = 0;
				for (int k = 1; k < 6; k++) {
					Rotate(i, j);
					if (m_fitness < minTemp) {
						minTemp = m_fitness;
						rotate = k;
					}
				}
				Rotate(i, j);

				for (int k = 0; k < rotate; k++) {
					Rotate(i, j);
				}
			}
		}
		if (minVal == m_fitness) break;

		minVal = m_fitness;

	}
}

void HexaGonNew::Rotate(int row, int col)
{
	int temp = *m_pHexa[col + row * m_row][0];
	for (int i = 1; i < 6; i++) {
		*m_pHexa[col + row * m_row][i - 1] = *m_pHexa[col + row * m_row][i];
	}
	*m_pHexa[col + row * m_row][5] = temp;
	ComputeSum();
	ComputeFitness();
}

void HexaGonNew::PrintOneHexa(int row, int col)
{
	printf("%d, %d\n", row, col);
	printf("\t%d\n", *m_pHexa[col + row * m_row][0]);
	printf("%d\t\t%d\n", *m_pHexa[col + row * m_row][5], *m_pHexa[col + row * m_row][1]);
	printf("\t%d\n", m_sum[col + row * m_row]);
	printf("%d\t\t%d\n", *m_pHexa[col + row * m_row][4], *m_pHexa[col + row * m_row][2]);
	printf("\t%d\n", *m_pHexa[col + row * m_row][3]);
}

void HexaGonNew::PrintFullHexa()
{
	int start;
	for (int i = 0; i < m_row + 1; i++) {
		printf("\t");
	}
	printf("%d\n", m_pValue[0]);

	for (int i = 1; i < m_row + 1; i++) {
		start = 2 * (m_row + 1) * i - 1;
		for (int j = i; j < m_row + 1; j++) {
			printf("\t");
		}

		for (int j = 0; j < i + 1; j++) {
			printf("%d\t\t", m_pValue[start - 2 * m_row * j]);
		}
		printf("\n");

		start = 2 * (m_row + 1) * i;
		for (int j = i; j < m_row + 1; j++) {
			printf("\t");
		}

		for (int j = 0; j < i + 1; j++) {
			printf("%d\t\t", m_pValue[start - 2 * m_row * j]);
		}
		printf("\n");

	}
	for (int i = m_row - 1; i >= 1; i--) {
		start = 2 * (m_row + 1) * m_row + (m_row - i) * 2 - 1;
		for (int j = i; j < m_row + 1; j++) {
			printf("\t");
		}

		for (int j = 0; j < i + 1; j++) {
			printf("%d\t\t", m_pValue[start - 2 * m_row * j]);
		}
		printf("\n");

		start = 2 * (m_row + 1) * m_row + (m_row - i) * 2;
		for (int j = i; j < m_row + 1; j++) {
			printf("\t");
		}

		for (int j = 0; j < i + 1; j++) {
			printf("%d\t\t", m_pValue[start - 2 * m_row * j]);
		}
		printf("\n");
	}
	for (int i = 0; i < m_row + 1; i++) {
		printf("\t");
	}
	printf("%d\n", m_pValue[m_num - 1]);
	printf("Fitness : %f\n", m_fitness);
}

float HexaGonNew::GetFitness() 
{
	return m_fitness;
}
int* HexaGonNew::GetValue()
{
	return m_pValue;
}
int* HexaGonNew::GetPhenoValue()
{
	return m_pValuePheno;
}

int HexaGonNew::GetNumValue()
{
	return m_num;
}
int HexaGonNew::GetRow()
{
	return m_row;
}
bool HexaGonNew::CheckValid()
{
	for (int i = 0; i < m_num + 1; i++) {
		m_pValid[i] = false;
	}
	for (int i = 0; i < m_num; i++) {
		if (m_pValid[m_pValue[i]]) {
			return false;
		}
		m_pValid[m_pValue[i]] = true;
	}
	return true;
}
