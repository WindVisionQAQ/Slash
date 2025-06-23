#pragma once

#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(LOCATION) if (GetWorld()) DrawDebugSphere(GetWorld(), LOCATION, 100.f, 12, FColor::Purple, true);
#define DRAW_SPHERE_SINGLEFRAME(LOCATION) if (GetWorld()) DrawDebugSphere(GetWorld(), LOCATION, 100.f, 12, FColor::Purple, false);
#define DRAW_LINE(START_LOCATION, END_LOCATION) if (GetWorld()) DrawDebugLine(GetWorld(), START_LOCATION, END_LOCATION, FColor::Red, true);
#define DRAW_LINE_SINGLEFRAME(START_LOCATION, END_LOCATION) if (GetWorld()) DrawDebugLine(GetWorld(), START_LOCATION, END_LOCATION, FColor::Red, false);
#define DRAW_POINT(LOCATION) if (GetWorld()) DrawDebugPoint(GetWorld(), LOCATION, 10.0f, FColor::Red, true);
#define DRAW_POINT_SINGLEFRAME(LOCATION) if (GetWorld()) DrawDebugPoint(GetWorld(), LOCATION, 10.0f, FColor::Red, false);
#define DRAW_VECTOR(START_LOCATION, END_LOCATION) if (GetWorld()) \
	{ \
		DRAW_LINE(START_LOCATION, END_LOCATION); \
		DRAW_POINT(END_LOCATION); \
	}
#define DRAW_VECTOR_SINGLEFRAME(START_LOCATION, END_LOCATION) if (GetWorld()) \
	{ \
		DRAW_LINE_SINGLEFRAME(START_LOCATION, END_LOCATION); \
		DRAW_POINT_SINGLEFRAME(END_LOCATION); \
	}