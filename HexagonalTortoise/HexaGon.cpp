#include "HexaGon.h"
#include <stdio.h>
#include <math.h>

HexaGon::HexaGon()
{
	m_pValue = NULL;
}

HexaGon::~HexaGon() 
{
	if (m_pValue) {
		delete[]m_pValue;
		m_pValue = NULL;
	}
}

void HexaGon::Generate(int row)
{	
	m_row = row;
	m_num = 2 * row * (row + 2);
	m_pValue = new int[m_num];
	
	m_pValue[0] = 7;
	m_pValue[1] = 11;
	m_pValue[2] = 28;
	m_pValue[3] = 2;
	m_pValue[4] = 22;
	m_pValue[5] = 9;
	m_pValue[6] = 12;

	m_pValue[7] = 8;
	m_pValue[8] = 25;
	m_pValue[9] = 30;
	m_pValue[10] = 6;
	m_pValue[11] = 21;
	m_pValue[12] = 19;
	m_pValue[13] = 26;
	m_pValue[14] = 4;

	m_pValue[15] = 1;
	m_pValue[16] = 24;
	m_pValue[17] = 23;
	m_pValue[18] = 27;
	m_pValue[19] = 13;
	m_pValue[20] = 29;
	m_pValue[21] = 18;
	m_pValue[22] = 16;

	m_pValue[23] = 5;
	m_pValue[24] = 10;
	m_pValue[25] = 20;
	m_pValue[26] = 3;
	m_pValue[27] = 17;
	m_pValue[28] = 14;
	m_pValue[29] = 15;		
}
int HexaGon::ComputeOneHexa(int row, int col)
{
	int start1 = 2 * (m_row + 1) * row + 2 * col;
	int start2 = 2 * (m_row + 1) * (row + 1) - 1 + 2 * col;
	int res = 0;
	for (int i = 0; i < 3; i++) {
		res += m_pValue[start1 + i] + m_pValue[start2 + i];
	}
	return res;
}
void HexaGon::PrintOneHexa(int row, int col)
{
	int start1 = 2 * (m_row + 1) * row + 2 * col;
	int start2 = 2 * (m_row + 1) * (row + 1) - 1 + 2 * col;
	printf("%d, %d\n", row, col);
	printf("\t%d\n", m_pValue[start1]);
	printf("%d\t\t%d\n", m_pValue[start2], m_pValue[start1 + 1]);
	printf("\t%d\n", ComputeOneHexa(row, col));
	printf("%d\t\t%d\n", m_pValue[start2 + 1], m_pValue[start1 + 2]);
	printf("\t%d\n", m_pValue[start2 + 2]);
}
void HexaGon::PrintFullHexa()
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
}

