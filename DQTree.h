#ifndef NPDFACE_QDTREE_H
#define NPDFACE_QDTREE_H
#include"Configuration.h"

class DQTreeNode
{
public:
	class FeatureID 
	{
	public:
		int x_1,y_1,x_2,y_2;
		FeatureID()
		{
			x_1 = 0;
			y_1 = 0; 
			x_2 = 0;
			y_2 = 0;
		}
	}featureID;
	unsigned char cutPoint[2];
	NodeType nodeType;
	float fit;
	DQTreeNode* leftRoot;
	DQTreeNode* rightRoot;
public:
	DQTreeNode();

};
class DQTree
{
public:
	//DQTreeNode * root;
	double threshold;
	int stageId;// stage num
	int treeId; // tree id in stage
	signed char x_1[(2<<TREE_DEPTH)-1] ;
	signed char y_1[(2<<TREE_DEPTH)-1] ;
	signed char x_2[(2<<TREE_DEPTH)-1] ;
	signed char y_2[(2<<TREE_DEPTH)-1] ;
	int cut_1[(2<<TREE_DEPTH)-1];
	int cut_2[(2<<TREE_DEPTH)-1];
	float fit[(2<<TREE_DEPTH)-1];
public:
	DQTree(std::string treeFilePath,int stageId,int treeId);
public:
	void read(const std::string treeFilePath);
	bool destoryDQTree();

};
#endif//NPDFACE_QDTREE_H