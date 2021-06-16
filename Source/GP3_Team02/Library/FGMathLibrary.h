#pragma once

class FGMathLibrary
{
public:
	static bool IsPointInBox(const FVector& Point, const FVector& Min, const FVector& Max)
	{
		if(Point.X >= Min.X && Point.Y >= Min.Y && Point.Z >= Min.Z &&
			Point.X <= Max.X && Point.Y <= Max.Y && Point.Z <= Max.Z)
				return true;

		return false;
	}
	
	static bool IsPointInBoxIgnoreZ(const FVector& Point, const FVector2D& Min, const FVector2D& Max)
	{
		if(Point.X >= Min.X && Point.Y >= Min.Y &&
			Point.X <= Max.X && Point.Y <= Max.Y)
				return true;

		return false;
	}

	static FVector2D InverseLerp(const FVector2D A, const FVector2D B, const FVector2D V)
	{
		return ( V - A ) / ( B - A );
	}

	static FVector2D Lerp(const FVector2D A, const FVector2D B, const FVector2D T)
	{
		return FVector2D( FMath::Lerp( A.X, B.X, T.X ), FMath::Lerp( A.Y, B.Y, T.Y ) );
	}
	
	static FVector2D Remap(const FVector2D IMin, const FVector2D IMax, const FVector2D OMin, const FVector2D OMax, const FVector2D Value)
	{
		const FVector2D T = InverseLerp( IMin, IMax, Value );
		return Lerp( OMin, OMax, T );
	}
};