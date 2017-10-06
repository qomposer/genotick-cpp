
#pragma once

#define GENOTICK_IMPL
#include <igenotick.h>

struct IGenotickDestructable : public IGenotick
{
	virtual ~IGenotickDestructable() {};
};
