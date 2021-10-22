
#pragma once

#include "ECSTypes.h"

class FECSEntity
{

private: // Typedefs

	typedef void* ComponentId;

public: // Constructors

	inline explicit FECSEntity(TEntityId InId)
		: Id(InId)
		, Flags()
	{}

public: // Getters
	
	inline TEntityId GetId() const { return Id; }
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
		
		std::uint8_t* elementAddress = nullptr;
		constexpr std::size_t elementBytes = sizeof(ComponentType);

		// Attempt to use old free memory
		for(auto it = UnusedCache.begin(); it != UnusedCache.end(); ++it)
		{
			std::uint8_t* unusedAddress = it->first;
			const std::size_t& unusedBytes = it->second;
		
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
	inline bool ContainsComponent()
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
		
		UnusedCache.insert({foundReg->second, (std::size_t)sizeof(ComponentType)});
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
	std::vector<TEntityFlag> Flags;

	// Id, ptr to buffer
	std::unordered_map<ComponentId, std::uint8_t*> LookupCache;
	
	// ptr to buffer, size of free bytes
	std::unordered_map<std::uint8_t*, std::size_t> UnusedCache;
	
	// Buffer containing all components
	std::vector<std::uint8_t> Buffer;
};