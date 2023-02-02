#pragma once
#include <vector>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <iostream>


class UFloat {
private:

	static UFloat posPosSum(UFloat& pNum1, UFloat& pNum2);
	static UFloat posNegSum(UFloat& pNum, UFloat& nNum);

	static int compareUFloats(UFloat num1, UFloat num2, bool absolute = false); //return 1 if num1 larger, -1 if num2, 0 if equal

public:
	bool positive = true;
	std::vector<int> integral; //maybe replace with char?
	std::vector<int> decimal;

	static UFloat sum(UFloat& num1, UFloat& num2);
	static UFloat mult(UFloat& num1, UFloat& num2);
	static UFloat floatMult(UFloat& uNum, float fNum);

	static UFloat floatToUFloat(float num);
	static UFloat intToUFloat(int num);
	static UFloat stringToUFloat(std::string num);

	static float uFloatToFloat(UFloat num);
	static int uFloatToInt(UFloat num);
	static std::string uFloatToString(UFloat num);
	
	UFloat();
	UFloat(int num);
	UFloat(float num);
	UFloat(std::string num);
	UFloat(const char* num);

	void add(UFloat& num);
	void sub(UFloat& num);
	void mult(UFloat& num);

	std::string toString();

	void flipPositive();
	void clear();
};