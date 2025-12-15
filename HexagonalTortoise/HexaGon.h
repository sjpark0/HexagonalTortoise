#pragma once
class HexaGon
{
private:
	int m_num;
	int* m_pValue;
	
	int m_row;
public:
	HexaGon();
	~HexaGon();
	void Generate(int row);	
	int ComputeOneHexa(int row, int col);
	void PrintOneHexa(int row, int col);
	void PrintFullHexa();
};

