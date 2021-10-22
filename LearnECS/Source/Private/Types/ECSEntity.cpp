
#include "ECSEntity.h"

bool FEntity::ContainsFlag(const TEntityFlag& Flag) const
{
	return std::find(Flags.begin(), Flags.end(), Flag) != Flags.end();
}

void FEntity::AddFlag(const TEntityFlag& Flag)
{
	auto foundFlag = std::find(Flags.begin(), Flags.end(), Flag);
	if(foundFlag == Flags.end())
		return;
	
	Flags.push_back(Flag);
}

void FEntity::RemoveFlag(const TEntityFlag& Flag)
{
	auto foundFlag = std::find(Flags.begin(), Flags.end(), Flag);
	if(foundFlag == Flags.end())
		return;
		
	Flags.erase(foundFlag);
}
