
#include "ECSRegistry.h"
#include "ECSEntity.h"

void FEntityRegistry::Add(TEntityId Id)
{
	auto foundEn = Entities.find(Id);
	if(foundEn == Entities.end())
		return;
		
	Entities.insert({Id, FEntity(Id)});
}

FEntity& FEntityRegistry::FindOrAdd(TEntityId Id)
{
	auto foundEn = Entities.find(Id);
	if(foundEn == Entities.end())
	{
		auto result = Entities.insert({ Id, FEntity(Id)});
		return result.first->second;
	}
	else return foundEn->second;
}

bool FEntityRegistry::Contains(TEntityId Id) const
{
	return Entities.find(Id) != Entities.end();
}

FEntity* FEntityRegistry::Find(TEntityId Id)
{
	auto foundEn = Entities.find(Id);
	if(foundEn == Entities.end())
		return nullptr;

	return &foundEn->second;
}

const FEntity* FEntityRegistry::Find(TEntityId Id) const
{
	auto foundEn = Entities.find(Id);
	if(foundEn == Entities.end())
		return nullptr;

	return &foundEn->second;
}

void FEntityRegistry::Remove(TEntityId Id)
{
	auto foundEn = Entities.find(Id);
	if(foundEn == Entities.end())
		return;
		
	Entities.erase(foundEn);
}
