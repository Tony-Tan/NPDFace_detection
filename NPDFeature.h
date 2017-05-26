#ifndef NPDFEATURE_H
#define NPDFEATURE_H
#include "Configuration.h"
class NPDfeature
{
public:
	unsigned char NPDtable[GRAY_LEVEL][GRAY_LEVEL];
	//unsigned char feaPoints[FEATURE_SIZE * 4];
	NPDfeature();
	
};
#endif//NPDFEATURE_H