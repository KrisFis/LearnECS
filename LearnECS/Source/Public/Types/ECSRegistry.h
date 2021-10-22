
#pragma once

#include "ECSTypes.h"

class FEntityRegistry
{

public: // Constructors

	FEntityRegistry() = default;
	~FEntityRegistry() = default;

public: // Add

	void Add(TEntityId Id);
	FEntity& FindOrAdd(TEntityId Id);
	
public: // Contains

	bool Contains(TEntityId Id) const;
	
public: // Find

	FEntity* Find(TEntityId Id);
	const FEntity* Find(TEntityId Id) const;

public: // Get
		// * Unsafe find

	FORCEINLINE FEntity& Get(TEntityId Id) { return *Find(Id); }
	FORCEINLINE const FEntity& Get(TEntityId Id) const { return *Find(Id); }

public: // Remove

	void Remove(TEntityId Id);

private:

	TFastMap<TEntityId, FEntity> Entities;
};