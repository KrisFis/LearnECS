
#pragma once

#include "ECSTypes.h"

class FECSEntity
{

private: // Typedefs

	typedef void* ComponentId;

public: // Constructors

	explicit FECSEntity(TEntityId InId)
		: Id(InId)
		, Flags()
	{}

public: // Getters
	
	TEntityId GetId() const { return Id; }
	bool ContainsFlag(const TEntityFlag& Flag) const;
	
public: // Setters

	void AddFlag(const TEntityFlag& Flag);
	void RemoveFlag(const TEntityFlag& Flag);
	
public:

	template<typename ComponentType, typename... ArgTypes>
	bool AddComponent(ArgTypes&&... Args)
	{
		const ComponentId componentId = GetComponentInternalId<ComponentType>();
	
		auto foundReg = LookupCache.find(componentId);
		if(foundReg != LookupCache.end())
			return false;
		
		uint8* elementAddress = nullptr;
		constexpr uint64 elementBytes = sizeof(ComponentType);

		// Attempt to use old free memory
		for(auto it = UnusedCache.begin(); it != UnusedCache.end(); ++it)
		{
			uint8* unusedAddress = it->first;
			const uint64& unusedBytes = it->second;
		
			if(unusedBytes >= elementBytes)
			{
				elementAddress = unusedAddress;
			
				// Remove or partially un-mark unused memory size
				if(unusedBytes > elementBytes) 
				{
					// element address is 1 byte pointer, so counting by bytes
					UnusedCache.insert({elementAddress + elementBytes, unusedBytes - elementBytes});
				}
				
				UnusedCache.erase(it);
				break; // break after first found memory
			}
		}
		
		// Reserve and use new buffer size if needed
		if(!elementAddress)
		{
			elementAddress = (&*Buffer.insert(Buffer.end(), elementBytes, 0));
		}
		
		new(elementAddress) ComponentType(std::forward<ArgTypes>(Args)...);
		LookupCache.insert({componentId, elementAddress});
		
		return true;
	}
	
	template<typename ComponentType>
	FORCEINLINE bool ContainsComponent()
	{
		return LookupCache.find(GetComponentInternalId<ComponentType>()) != LookupCache.end();
	}
	
	template<typename ComponentType>
	ComponentType* FindComponent()
	{
		const ComponentId componentId = GetComponentInternalId<ComponentType>();
		
		auto foundReg = LookupCache.find(componentId);
		if(foundReg == LookupCache.end())
			return nullptr;
		
		return (ComponentType*)foundReg->second;
	}
	
	template<typename ComponentType>
	bool RemoveComponent()
	{
		const ComponentId componentId = GetComponentInternalId<ComponentType>();
		
		auto foundReg = LookupCache.find(componentId);
		if(foundReg == LookupCache.end())
			return false;

		((ComponentType*)foundReg->second)->~ComponentType();
		
		UnusedCache.insert({foundReg->second, (uint64)sizeof(ComponentType)});
		LookupCache.erase(foundReg);
		return true;
	}

private: // Internal methods

	template<typename ComponentType>
	ComponentId GetComponentInternalId()
	{
		// Works only inside same module
		static ComponentType* uniqueId = nullptr;
		return &uniqueId;
	}

private: // Fields

	TEntityId Id;
	TArray<TEntityFlag> Flags;

	// Id, ptr to buffer
	TFastMap<ComponentId, uint8*> LookupCache;
	
	// ptr to buffer, size of free bytes
	TFastMap<uint8*, uint64> UnusedCache;
	
	// Buffer containing all components
	TArray<uint8> Buffer;
};