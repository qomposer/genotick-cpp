
#pragma once

#define GENOTICK_IMPL
#include <igenotick.h>

namespace genotick {

struct IGenotickDestructable : public IGenotick
{
	virtual ~IGenotickDestructable() {};
};

} // namespace genotick
