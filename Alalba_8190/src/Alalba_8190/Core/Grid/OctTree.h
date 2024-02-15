#pragma once
#include "Volume.h"
#include "Fields.h"

namespace Alalba
{
	template<class T>
	struct OctTreeNode
	{
		OctTreeNode* children[8];
		lux::Vector m_LLFC;
		lux::Vector m_RURC;
		T m_value;
	};


	template<class T>
	class OctTree
	{

	};
}