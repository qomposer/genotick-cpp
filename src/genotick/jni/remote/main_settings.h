
#pragma once

#include <genotick/jni/remote/timepoint.h>
#include <genotick/jni/remote/weight_mode.h>
#include <genotick/jni/remote/inherited_weight_mode.h>
#include <genotick/jni/remote/chart_mode.h>

namespace genotick {
namespace jni {
namespace wrapper {

struct SMainSettingsTag { static constexpr auto Name() { return "com/alphatica/genotick/genotick/MainSettings"; } };

class CMainSettings : public CClass<SMainSettingsTag>
{
public:
	using TIntField = ::jni::IntField<TagType>;
	using TLongField = ::jni::LongField<TagType>;
	using TDoubleField = ::jni::DoubleField<TagType>;
	using TBooleanField = ::jni::BooleanField<TagType>;
	using TStringField = ::jni::StringField<TagType>;
	using TTimePointField = ::jni::Field<TagType, CTimePoint::TObject>;
	using TWeightModeField = ::jni::Field<TagType, CWeightMode::TObject>;
	using TInheritedWeightModeField = ::jni::Field<TagType, CInheritedWeightMode::TObject>;
	using TChartModeField = ::jni::Field<TagType, CChartMode::TObject>;

#define GENOTICK_MAINSETTINGS_FIELDS(f) \
	f(TTimePointField          , startTimePoint                   ) \
	f(TTimePointField          , endTimePoint                     ) \
	f(TIntField                , populationDesiredSize            ) \
	f(TStringField             , populationDAO                    ) \
	f(TBooleanField            , performTraining                  ) \
	f(TStringField             , dataDirectory                    ) \
	f(TIntField                , minimumRobotInstructions         ) \
	f(TIntField                , maximumRobotInstructions         ) \
	f(TIntField                , maximumProcessorInstructionFactor) \
	f(TDoubleField             , maximumDeathByAge                ) \
	f(TDoubleField             , maximumDeathByWeight             ) \
	f(TDoubleField             , probabilityOfDeathByAge          ) \
	f(TDoubleField             , probabilityOfDeathByWeight       ) \
	f(TWeightModeField         , weightMode                       ) \
	f(TDoubleField             , weightExponent                   ) \
	f(TDoubleField             , inheritedChildWeight             ) \
	f(TInheritedWeightModeField, inheritedChildWeightMode         ) \
	f(TIntField                , maximumDataOffset                ) \
	f(TIntField                , protectRobotsUntilOutcomes       ) \
	f(TDoubleField             , newInstructionProbability        ) \
	f(TDoubleField             , instructionMutationProbability   ) \
	f(TDoubleField             , skipInstructionProbability       ) \
	f(TIntField                , minimumOutcomesToAllowBreeding   ) \
	f(TIntField                , minimumOutcomesBetweenBreeding   ) \
	f(TBooleanField            , killNonPredictingRobots          ) \
	f(TDoubleField             , randomRobotsAtEachUpdate         ) \
	f(TDoubleField             , protectBestRobots                ) \
	f(TBooleanField            , requireSymmetricalRobots         ) \
	f(TDoubleField             , resultThreshold                  ) \
	f(TIntField                , ignoreColumns                    ) \
	f(TLongField               , randomSeed                       ) \
	f(TChartModeField          , chartMode                        ) \

	explicit CMainSettings::CMainSettings(::jni::JNIEnv* pJavaEnv)
		: CClass<TagType>(pJavaEnv)
		GENOTICK_MAINSETTINGS_FIELDS(GENOTICK_UNROLL_FIELD_INITIALIZERS)
	{
	}

	GENOTICK_MAINSETTINGS_FIELDS(GENOTICK_UNROLL_SET_FIELD_INLINE_FUNCTIONS)
	GENOTICK_MAINSETTINGS_FIELDS(GENOTICK_UNROLL_GET_FIELD_INLINE_FUNCTIONS)

private:
	GENOTICK_MAINSETTINGS_FIELDS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace wrapper
} // namespace jni
} // namespace genotick
