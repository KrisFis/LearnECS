
#pragma once

#include "ECSTypes.h"

class FECSRegistry
{

public: // Constructors

	FECSRegistry() = default;
	~FECSRegistry() = default;

public: // Add

	void Add(TEntityId Id);
	FECSEntity& GetOrAdd(TEntityId Id);
	
public: // Contains

	bool Contains(TEntityId Id) const;
	
public: // Get

	FECSEntity* Find(TEntityId Id);
	const FECSEntity* Find(TEntityId Id) const;

public: // Remove

	void Remove(TEntityId Id);

private:

	std::unordered_map<TEntityId, FECSEntity> Entities;
};