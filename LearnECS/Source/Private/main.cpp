
#include "ECS.h"

#include <iostream>
#include <assert.h>

namespace NTest
{
	class FScene
	{
	public:

		FECSRegistry Registry;
	};

	struct FMyBasicComponent
	{
		std::uint8_t A;
		std::uint8_t B;
	};

	struct FMyComponent
	{
		inline explicit FMyComponent(const std::string& InStr)
				: Str(InStr)
		{}

		std::string Str;
	};

	class FMyComplexComponent : public FMyComponent, public FMyBasicComponent
	{
	public:
		inline explicit FMyComplexComponent(const std::string& InStr, std::uint8_t InA, const char* InMyTxt, std::uint8_t InB = 5)
				: FMyComponent(InStr), MyTxt(InMyTxt)
		{
			A = InA;
			B = InB;
		}

		const char* MyTxt;
	};
	
	bool TestECS()
	{
		FScene myScene;
	
		if(!myScene.Registry.GetOrAdd(2).AddComponent<FMyComplexComponent>("Hello World2", 8, "Hello3")) return false;
		
		FMyComplexComponent& myComplexComponent = *myScene.Registry.Find(2)->FindComponent<FMyComplexComponent>();
		myComplexComponent.MyTxt = "Yes";
		
		if(!myScene.Registry.Find(2)->RemoveComponent<FMyComplexComponent>()) return false;
		if(!myScene.Registry.Find(2)->AddComponent<FMyComponent>("Hello World2")) return false;
		if(!myScene.Registry.Find(2)->AddComponent<FMyBasicComponent>()) return false;
		
		FMyComponent& myComponent = *myScene.Registry.Find(2)->FindComponent<FMyComponent>();
		myComponent.Str = "Yes";
		
		FMyBasicComponent& myBasicComponent = *myScene.Registry.Find(2)->FindComponent<FMyBasicComponent>();
		myBasicComponent.A = 10;
		
		return true;
	}
}

int main()
{
	if(!NTest::TestECS())
	{
		assert(false);
		return 1;
	}
	
	return 0;
}