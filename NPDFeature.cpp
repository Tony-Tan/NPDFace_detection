#include "NPDFeature.h"
#ifdef _OPENMP
#include <omp.h>
#endif
NPDfeature::NPDfeature()
{
	
	Message("NPDfeature::NPDfeature", "Create NPD Table;");
	for (int j = 0; j < GRAY_LEVEL; j++)
	{
		for (int i = 0; i < GRAY_LEVEL; i++)
		{
			if (0!=i+j)
			{
				NPDtable[j][i] = (unsigned char)((GRAY_LEVEL-1.0)*(double(i) / double(i + j)));
			}
			else
			{
				NPDtable[j][i] = 0;
			}
		}
	}
	
	/*int id = 0;
	for (int feaI = 0; feaI < SAMPLE_SIZE*SAMPLE_SIZE; feaI++)
	{
		int x_1, x_2, y_1, y_2;
		x_1 = feaI % SAMPLE_SIZE;
		y_1 = feaI / SAMPLE_SIZE;
		for (int feaJ = feaI + 1; feaJ < SAMPLE_SIZE*SAMPLE_SIZE; feaJ++)
		{
			x_2 = feaJ % SAMPLE_SIZE;
			y_2 = feaJ / SAMPLE_SIZE;
			feaPoints[id * 4 + 0] = (unsigned char)x_1;
			feaPoints[id * 4 + 1] = (unsigned char)y_1;
			feaPoints[id * 4 + 2] = (unsigned char)x_2;
			feaPoints[id * 4 + 3] = (unsigned char)y_2;
			id++;
		}
	}*/
}
