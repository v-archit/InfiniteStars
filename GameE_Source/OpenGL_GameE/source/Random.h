#pragma once

#include<random>

class Random
{
public:

	static float RandGen(int min, int max)
	{
		float randNum = rand() % (max - min + 1) + min;
		return randNum;
	}
	
};