// Dark State Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UMGHelper.generated.h"


UCLASS()
class DSSCORE_API UUMGHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="DssCore|UMG")
	static FTransform GetWidget3DBounds(UWidget* Widget)
	{
		if (!IsValid(Widget))
		{
			return FTransform();
		}

		auto Geo = Widget->GetPaintSpaceGeometry();

		auto Transform = Geo.GetAccumulatedRenderTransform();
		auto Location = Geo.GetAbsolutePositionAtCoordinates(FVector2f(0.5f, 0.5f));
		auto Size = Geo.GetAbsoluteSize();

		return FTransform(FRotator::ZeroRotator, FVector(0, Location.X, Location.Y),
		                  FVector(1.f, Size.X, Size.Y));
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="DssCore|UMG")
	static void TransformWidgetToComponentLocalBatch(const FVector& HalfDrawQuad,
	                                                 const FTransform& InvComponentTransform,
	                                                 UPARAM(ref)
	                                                 TMap<FName, FTransform>& InOutChildTransforms)
	{
		for (auto& It : InOutChildTransforms)
		{
			auto& Child = It.Value;
			auto CalculatedLocation = Child.GetTranslation();
			CalculatedLocation -= HalfDrawQuad;
			CalculatedLocation *= (InvComponentTransform.GetScale3D() * -1.f);

			const auto ComponentLocation = InvComponentTransform.GetTranslation();
			CalculatedLocation.Y += ComponentLocation.Y;
			CalculatedLocation.Z += ComponentLocation.Z;

			Child.SetTranslation(CalculatedLocation);

			auto CalculatedScale = Child.GetScale3D();
			CalculatedScale.Y /= (HalfDrawQuad.Y * 2.f);
			CalculatedScale.Z /= (HalfDrawQuad.Z * 2.f);

			Child.SetScale3D(InvComponentTransform.GetScale3D() * CalculatedScale);
		}
	}
};
