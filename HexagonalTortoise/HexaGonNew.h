#pragma once
class HexaGonNew
{
private:
	int m_num;
	int* m_pValue;
	int* m_pValuePheno;
	int*** m_pHexa;
	int m_row;
	float m_fitness;
	int* m_sum;
	bool* m_pValid;
	void MakeHexa(int row);
	void ComputeFitness();
	void ComputeSum();
	int ComputeSumOneHexa(int row, int col);
	void Rotate(int row, int col);
	void Optimize();
public:
	HexaGonNew();
	HexaGonNew(int row);
	~HexaGonNew();
	
	void Generate();
	void Generate(int* value);
	void Update();

	void PrintOneHexa(int row, int col);
	void PrintFullHexa();
	
	float GetFitness();
	int* GetValue();
	int* GetPhenoValue();
	int GetNumValue();
	int GetRow();
	bool CheckValid();
};