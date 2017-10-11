
#pragma once

#define GENOTICK_IMPL
#include <igenotick.h>

namespace genotick {

struct IGenotickTimePointsDestructable : public IGenotickTimePoints
{
	virtual ~IGenotickTimePointsDestructable() {};
};

struct IGenotickPredictionsDestructable : public IGenotickPredictions
{
	virtual ~IGenotickPredictionsDestructable() {};
};

struct IGenotickDestructable : public IGenotick
{
	virtual ~IGenotickDestructable() {};
};

} // namespace genotick
