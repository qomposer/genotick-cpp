
#pragma once

#define GENOTICK_IMPL
#include <igenotick.h>
#include <jni.h>

namespace genotick {

struct IGenotickDestructable : public IGenotick
{
	virtual ~IGenotickDestructable() {};
	virtual bool Contains(JavaVM& javaVM) const = 0;
};

} // namespace genotick
