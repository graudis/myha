#ifndef __RENEW_GEOM_H__
#define __RENEW_GEOM_H__


#include "rntype.h"
#include <algorithm>
#include <math.h>


#define PI			3.141592653589793238462643383f


inline tFLOAT radianToDegree( tFLOAT radian )
{
	return (radian * 180.0) / PI;
}

inline tFLOAT degreeToRadian( tFLOAT degree )
{
	return PI * (degree / 180.0);
}


template <typename T> inline tBOOL fequal( T x, T y ) { return std::max(x, y) - std::min(x, y) < (T)0.000001f; }


namespace rnSqrt
{
	inline tFLOAT exactDistance(const tFLOAT sx, const tFLOAT sy, const tFLOAT ex, const tFLOAT ey)
	{
		return sqrt((ex - sx) * (ex - sx) + (ey - sy) * (ey - sy));
	}

	inline tFLOAT getDistance(const tFLOAT sx, const tFLOAT sy, const tFLOAT sz, const tFLOAT ex, const tFLOAT ey, const tFLOAT ez)
	{
		tFLOAT dx = ex - sx;
		tFLOAT dy = ey - sy;
		tFLOAT dz = ez - sz;
		return sqrt((dx*dx) + (dy*dy) + (dz*dz));
	}
};


#endif

