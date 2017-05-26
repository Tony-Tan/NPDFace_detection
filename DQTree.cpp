#include "DQTree.h"
#include <fstream>
#include "NPDFeature.h"
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#ifdef _OPENMP
#include <omp.h>
#endif
DQTreeNode::DQTreeNode()
	:featureID(), leftRoot(NULL), rightRoot(NULL),fit(0.0)
{
}
DQTree::DQTree(std::string treeFilePath,int stageId_,int treeId_)
	:stageId(stageId_),treeId(treeId_)
{
    memset(x_1,0,sizeof(x_1));
    memset(x_2,0,sizeof(x_2));
    memset(y_1,0,sizeof(y_1));
    memset(y_2,0,sizeof(y_2));
    memset(cut_1,0,sizeof(cut_1));
    memset(cut_2,0,sizeof(cut_2));
    memset(fit,0,sizeof(fit));
	read(treeFilePath);
}

/*
*
*
*
*/
static void readNode(DQTree* tree, FILE* treeDefineFile,int arrayPos)
{
	//*root = new DQTreeNode();
    int nodeType=0;
	fscanf(treeDefineFile, "%d ", &nodeType);
	if (nodeType == SPLIT)
	{
		int left_cut, right_cut;
        int x1,x2,y1,y2;
		fscanf(treeDefineFile, "%d %d (%d,%d) (%d,%d)\n",
			&(left_cut), &(right_cut),&x1, &y1,&x2,&y2);
        tree->x_1[arrayPos]=(char)x1;
        tree->y_1[arrayPos]=(char)y1;
        tree->x_2[arrayPos]=(char)x2;
        tree->y_2[arrayPos]=(char)y2;
        tree->cut_1[arrayPos]=left_cut;
        tree->cut_2[arrayPos]=right_cut;
        tree->fit[arrayPos]=0.0;
	}
	else if (nodeType == LEAF)
	{
        float fit;
		fscanf(treeDefineFile, "%f\n", &fit);
        tree->x_1[arrayPos]=-1;
        tree->y_1[arrayPos]=-1;
        tree->x_2[arrayPos]=-1;
        tree->y_2[arrayPos]=-1;
        tree->cut_1[arrayPos]=-1;
        tree->cut_2[arrayPos]=-1;
        tree->fit[arrayPos]=fit;
		return;
	}
	readNode(tree,treeDefineFile,arrayPos*2+1);
	readNode(tree,treeDefineFile,arrayPos*2+2);
}

void DQTree::read(const std::string treeFilePath)
{
	FILE * treeFile = fopen(treeFilePath.c_str(), "r");
	if (treeFile == NULL)
	{
		Message("DQTree::read", "read tree define file fail:" + treeFilePath);
		exit(1);
	}
	
	fscanf(treeFile, "%lf\n", &threshold);
	readNode(this,treeFile,0);
	fclose(treeFile);
}

void destoryDQTreeNode(DQTreeNode** node)
{
	if((*node)->leftRoot!=NULL)
		destoryDQTreeNode(&((*node)->leftRoot));
	if((*node)->rightRoot!=NULL)
		destoryDQTreeNode(&((*node)->rightRoot));
	delete *node;
	*node=NULL;

}

bool DQTree::destoryDQTree()
{
	return true;
}
