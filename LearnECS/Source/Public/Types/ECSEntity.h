
#pragma once

#include "ECSTypes.h"

// TODO(kristian.fisera): Find proper implementation without RTTI
#define WITH_RTTI 1

#if WITH_RTTI
#include <typeinfo>
#endif

namespace NComponentsContainer
{
	template<typename ComponentType>
	uint32 CalculateHash()
	{
		#if WITH_RTTI
		return typeid(ComponentType).hash_code();
		#else
		return 0;
		#endif
	}
}

class FComponentsContainer
{

public: // Constructor

	FComponentsContainer() = default;
	~FComponentsContainer() = default;

public:

	template<typename ComponentType, typename ...ArgTypes>
	bool Add(ArgTypes&&... Args)
	{
		uint32 componentHashCode = NComponentsContainer::CalculateHash<ComponentType>();
	
		auto foundReg = LookupCache.find(componentHashCode);
		if(foundReg == LookupCache.end())
		{
			std::uint8_t* elementP = &(*Buffer.end());
			Buffer.insert(Buffer.end(), sizeof(ComponentType), 0);
			((ComponentType*)elementP)->ComponentType(std::forward<ArgTypes>(Args)...);
			
			LookupCache.insert({componentHashCode, elementP});
			return true;
		}
		else if(!foundReg->second)
		{
			std::uint8_t* elementP = foundReg->second;
			((ComponentType*)elementP)->ComponentType(std::forward<ArgTypes>(Args)...); // overwrite with new
		}
		
		return false;
	}
	
	template<typename ComponentType>
	bool Contains()
	{
		return LookupCache.find(NComponentsContainer::CalculateHash<ComponentType>()) != LookupCache.end();
	}
	
	template<typename ComponentType>
	ComponentType* Get()
	{
		uint32 componentHashCode = NComponentsContainer::CalculateHash<ComponentType>();
		
		auto foundReg = LookupCache.find(componentHashCode);
		if(foundReg == LookupCache.end())
			return nullptr;
		
		return (ComponentType*)foundReg->second;
	}
	
	template<typename ComponentType>
	bool Remove()
	{
		uint32 componentHashCode = NComponentsContainer::CalculateHash<ComponentType>();
		
		auto foundReg = LookupCache.find(componentHashCode);
		if(foundReg == LookupCache.end())
			return false;

		// Once component has been added, it has still reserved memory
		//* This memory is just reused
		(ComponentType*)(foundReg->second)->~ComponentType();
		foundReg->second = nullptr;
		LookupCache.erase(foundReg);
	}


private: // Fields

	// Hash, ptr to buffer
	TFastMap<uint32, uint8*> LookupCache;
	
	// Buffer containing all components
	TArray<uint8> Buffer;
};

class FEntity
{
	
public: // Constructors

	FORCEINLINE explicit FEntity(TEntityId InId)
		: Id(InId)
		, Flags()
		, Container()
	{}

public: // Operators

	FComponentsContainer& operator->() { return GetComponents(); }
	const FComponentsContainer& operator->() const { return GetComponents(); }

public: // Getters
	
	FORCEINLINE TEntityId GetId() const { return Id; }
	bool ContainsFlag(const TEntityFlag& Flag) const;
	
	FORCEINLINE FComponentsContainer& GetComponents() { return Container; }
	FORCEINLINE const FComponentsContainer& GetComponents() const { return Container; }
	
public: // Setters

	void AddFlag(const TEntityFlag& Flag);
	void RemoveFlag(const TEntityFlag& Flag);
	
private:

	TEntityId Id;
	TArray<TEntityFlag> Flags;
	
	FComponentsContainer Container;
};