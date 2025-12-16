#pragma once
class HexaGonNew
{
private:
	int m_num;
	int* m_pValue;
	int*** m_pHexa;
	int m_row;
	float m_fitness;
	int* m_sum;
	bool* m_pValid;
	void MakeHexa(int row);
	void ComputeFitness();
	void ComputeSum();
	int ComputeSumOneHexa(int row, int col);

public:
	HexaGonNew();
	HexaGonNew(int row);
	~HexaGonNew();
	
	void Generate();
	void Generate(int* value);
	void Update();

	void PrintOneHexa(int row, int col);
	void PrintFullHexa();
	void Rotate(int row, int col);

	float GetFitness();
	int* GetValue();
	int GetNumValue();
	bool CheckValid();
};

