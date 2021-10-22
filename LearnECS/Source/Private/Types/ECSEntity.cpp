
#include "ECSEntity.h"

bool FECSEntity::ContainsFlag(const TEntityFlag& Flag) const
{
	return std::find(Flags.begin(), Flags.end(), Flag) != Flags.end();
}

void FECSEntity::AddFlag(const TEntityFlag& Flag)
{
	auto foundFlag = std::find(Flags.begin(), Flags.end(), Flag);
	if(foundFlag == Flags.end())
		return;
	
	Flags.push_back(Flag);
}

void FECSEntity::RemoveFlag(const TEntityFlag& Flag)
{
	auto foundFlag = std::find(Flags.begin(), Flags.end(), Flag);
	if(foundFlag == Flags.end())
		return;
		
	Flags.erase(foundFlag);
}
