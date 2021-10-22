
#include "ECS.h"

#include <iostream>

namespace NTest
{
	class FScene
	{
	public:

		FECSRegistry Registry;
	};

	struct FMyBasicComponent
	{
		uint8 A;
		uint8 B;
	};

	struct FMyComponent
	{
		FORCEINLINE explicit FMyComponent(FString InStr)
				: Str(InStr)
		{}

		FString Str;
	};

	class FMyComplexComponent : public FMyComponent, public FMyBasicComponent
	{
	public:
		FORCEINLINE explicit FMyComplexComponent(FString InStr, uint8 InA, const char* InMyTxt, uint8 InB = 5)
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
	
		ENSURE_RET(myScene.Registry.GetOrAdd(2).AddComponent<FMyComplexComponent>("Hello World2", 8, "Hello3"), false);
		
		FMyComplexComponent& myComplexComponent = *myScene.Registry.Find(2)->FindComponent<FMyComplexComponent>();
		myComplexComponent.MyTxt = "Yes";
		
		ENSURE_RET(myScene.Registry.Find(2)->RemoveComponent<FMyComplexComponent>(), false);
		ENSURE_RET(myScene.Registry.Find(2)->AddComponent<FMyComponent>("Hello World2"), false);
		ENSURE_RET(myScene.Registry.Find(2)->AddComponent<FMyBasicComponent>(), false);
		
		FMyComponent& myComponent = *myScene.Registry.Find(2)->FindComponent<FMyComponent>();
		myComponent.Str = "Yes";
		
		FMyBasicComponent& myBasicComponent = *myScene.Registry.Find(2)->FindComponent<FMyBasicComponent>();
		myBasicComponent.A = 10;
		
		return true;
	}
}

int main()
{
	ENSURE_RET(NTest::TestECS(), 1);
	return 0;
}