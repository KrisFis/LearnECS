
#include "ECSRegistry.h"
#include "ECSEntity.h"

void FECSRegistry::Add(TEntityId Id)
{
	auto foundEn = Entities.find(Id);
	if(foundEn == Entities.end())
		return;
		
	Entities.insert({Id, FECSEntity(Id)});
}

FECSEntity& FECSRegistry::GetOrAdd(TEntityId Id)
{
	auto foundEn = Entities.find(Id);
	if(foundEn == Entities.end())
	{
		auto result = Entities.insert({Id, FECSEntity(Id)});
		return result.first->second;
	}
	else return foundEn->second;
}

bool FECSRegistry::Contains(TEntityId Id) const
{
	return Entities.find(Id) != Entities.end();
}

FECSEntity* FECSRegistry::Find(TEntityId Id)
{
	auto foundEn = Entities.find(Id);
	if(foundEn == Entities.end())
		return nullptr;
		
	return &foundEn->second;
}

const FECSEntity* FECSRegistry::Find(TEntityId Id) const
{
	auto foundEn = Entities.find(Id);
	if(foundEn == Entities.end())
		return nullptr;
		
	return &foundEn->second;
}

void FECSRegistry::Remove(TEntityId Id)
{
	auto foundEn = Entities.find(Id);
	if(foundEn == Entities.end())
		return;
		
	Entities.erase(foundEn);
}
