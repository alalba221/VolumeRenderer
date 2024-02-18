#pragma once
#include "Mesh.h"
#include "Alalba_8190/Core/Grid/SparseGrid.inl"
namespace Alalba
{
#define INF 114514.0


	// BVH 树节点
	struct BVHNode {
		BVHNode* left = NULL;       // 左右子树索引
		BVHNode* right = NULL;
		int n, index;               // 叶子节点信息               
		lux::Vector AA, BB;                // 碰撞盒
	};

	// 求交结果
	struct HitResult {
		Triangle* triangle = NULL;
		float distance = INF;
	};

	//class LevelSet:public SparseGrid<float>
	//{

	//};

	std::shared_ptr<SparseGridVolume<float>> LevelSet(const Mesh& mesh, INT3 resolution, int partionSize,int bandwidth=3);
}


