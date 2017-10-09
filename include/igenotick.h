
// Genotick interface for C & C++
// Supports lib, dll, Win32, x64
// All given strings are expected to be null-terminated

#ifndef I_GENOTICK_H
#define I_GENOTICK_H

#define GENOTICK_INTERFACE_VERSION 1

#define GENOTICK_EXPORT __declspec(dllexport)
#define GENOTICK_IMPORT __declspec(dllimport)
#define GENOTICK_CALL __cdecl

#ifdef GENOTICK_DLL
#ifdef GENOTICK_IMPL
#define GENOTICK_IMPORT_OR_EXPORT GENOTICK_EXPORT
#else
#define GENOTICK_IMPORT_OR_EXPORT GENOTICK_IMPORT
#endif
#else
#define GENOTICK_IMPORT_OR_EXPORT
#endif

#ifdef __cplusplus
template <class T>
inline void GenotickSafeRelease(T* p) {
	if (p) {
		p->Release();
		p = nullptr;
	}
}
#else
#define GENOTICK_SAFE_RELEASE(p) if(p) { p->Release(p); p = 0; }
#endif

struct SGenotickString
{
	char* utf8_buffer;
	unsigned long capacity;
};
typedef struct SGenotickString TGenotickString;

#ifdef ZORRO_LITE_C
struct longlong
{
	signed long hi;
	signed long lo;
};
typedef struct longlong  TGenotickInt64;
#else
typedef signed long long TGenotickInt64;
#endif // ZORRO_LITE_C

typedef signed long      TGenotickInt32;
typedef unsigned char    TGenotickBoolean;
typedef signed char      TGenotickByte;
typedef unsigned short   TGenotickChar;
typedef signed short     TGenotickShort;
typedef float            TGenotickFloat;
typedef double           TGenotickDouble;
typedef TGenotickInt64   TGenotickTimePoint;

const TGenotickBoolean GenotickFalse = 0;
const TGenotickBoolean GenotickTrue = 1;

#define GenotickWeightMode_WinCount     0
#define GenotickWeightMode_WinRate      1
#define GenotickWeightMode_ProfitCount  2
#define GenotickWeightMode_ProfitFactor 3

#define GenotickInheritedWeightMode_Parents      0
#define GenotickInheritedWeightMode_Ancestors    1
#define GenotickInheritedWeightMode_AncestorsLog 2

#define GenotickChartMode_None                 0
#define GenotickChartMode_Draw                 (1 << 0)
#define GenotickChartMode_Save                 (1 << 1)
#define GenotickChartMode_JFreeChart           (1 << 2)
#define GenotickChartMode_JFreeChart_Draw      (GenotickChartMode_JFreeChart | GenotickChartMode_Draw)
#define GenotickChartMode_JFreeChart_Save      (GenotickChartMode_JFreeChart | GenotickChartMode_Save)
#define GenotickChartMode_JFreeChart_Draw_Save (GenotickChartMode_JFreeChart | GenotickChartMode_Draw | GenotickChartMode_Save)

#define GenotickResult_Success               0
#define GenotickResult_InvalidArgument       1
#define GenotickResult_JvmDllNotFound        2
#define GenotickResult_JvmExportsNotFound    3
#define GenotickResult_JniError              4
#define GenotickResult_JniDetached           5
#define GenotickResult_JniVersionMismatch    6
#define GenotickResult_JniNoMemory           7
#define GenotickResult_JniExists             8
#define GenotickResult_JniInvalidArgument    9
#define GenotickResult_JavaClassMismatch    10
#define GenotickResult_JavaEnumMismatch     11
#define GenotickResult_JavaException        12
#define GenotickResult_ErrorNoError         13
#define GenotickResult_ErrorNoInput         14
#define GenotickResult_ErrorNoOutput        15
#define GenotickResult_ErrorUnknownArgument 16

#ifdef __cplusplus

#include "enumdef.h"

struct SGenotickEnumMeta
{
	constexpr SGenotickEnumMeta(const char* const javaValueName) : javaValueName(javaValueName) {}
	const char* const javaValueName;
};

#define GENOTICK_ENUM_WEIGHT_MODE(e) \
	e(WinCount     , = GenotickWeightMode_WinCount     , (SGenotickEnumMeta("WIN_COUNT"    ))) \
	e(WinRate      , = GenotickWeightMode_WinRate      , (SGenotickEnumMeta("WIN_RATE"     ))) \
	e(ProfitCount  , = GenotickWeightMode_ProfitCount  , (SGenotickEnumMeta("PROFIT_COUNT" ))) \
	e(ProfitFactor , = GenotickWeightMode_ProfitFactor , (SGenotickEnumMeta("PROFIT_FACTOR"))) \

#define GENOTICK_ENUM_INHERITED_WEIGHT_MODE(e) \
	e(Parents      , = GenotickInheritedWeightMode_Parents      , (SGenotickEnumMeta("PARENTS"      ))) \
	e(Ancestors    , = GenotickInheritedWeightMode_Ancestors    , (SGenotickEnumMeta("ANCESTORS"    ))) \
	e(AncestorsLog , = GenotickInheritedWeightMode_AncestorsLog , (SGenotickEnumMeta("ANCESTORS_LOG"))) \

#define GENOTICK_ENUM_CHART_MODE(e) \
	e(None                 , = GenotickChartMode_None                 , (SGenotickEnumMeta("NONE"                ))) \
	e(Draw                 , = GenotickChartMode_Draw                 , (SGenotickEnumMeta("DRAW"                ))) \
	e(Save                 , = GenotickChartMode_Save                 , (SGenotickEnumMeta("SAVE"                ))) \
	e(JFreeChart           , = GenotickChartMode_JFreeChart           , (SGenotickEnumMeta("JFREECHART"          ))) \
	e(JFreeChart_Draw      , = GenotickChartMode_JFreeChart_Draw      , (SGenotickEnumMeta("JFREECHART_DRAW"     ))) \
	e(JFreeChart_Save      , = GenotickChartMode_JFreeChart_Save      , (SGenotickEnumMeta("JFREECHART_SAVE"     ))) \
	e(JFreeChart_Draw_Save , = GenotickChartMode_JFreeChart_Draw_Save , (SGenotickEnumMeta("JFREECHART_DRAW_SAVE"))) \

DEFINE_CUSTOM_ENUM_CLASS(EGenotickWeightMode, TGenotickInt32, GENOTICK_ENUM_WEIGHT_MODE, SGenotickEnumMeta)
DEFINE_CUSTOM_ENUM_CLASS(EGenotickInheritedWeightMode, TGenotickInt32, GENOTICK_ENUM_INHERITED_WEIGHT_MODE, SGenotickEnumMeta)
DEFINE_CUSTOM_ENUM_CLASS(EGenotickChartMode, TGenotickInt32, GENOTICK_ENUM_CHART_MODE, SGenotickEnumMeta)

enum class EGenotickResult : TGenotickInt32
{
	Success              = GenotickResult_Success,
	InvalidArgument      = GenotickResult_InvalidArgument,
	JvmDllNotFound       = GenotickResult_JvmDllNotFound,
	JvmExportsNotFound   = GenotickResult_JvmExportsNotFound,
	JniError             = GenotickResult_JniError,
	JniDetached          = GenotickResult_JniDetached,
	JniVersionMismatch   = GenotickResult_JniVersionMismatch,
	JniNoMemory          = GenotickResult_JniNoMemory,
	JniExists            = GenotickResult_JniExists,
	JniInvalidArgument   = GenotickResult_JniInvalidArgument,
	JavaClassMismatch    = GenotickResult_JavaClassMismatch,
	JavaEnumMismatch     = GenotickResult_JavaEnumMismatch,
	JavaException        = GenotickResult_JavaException,
	ErrorNoError         = GenotickResult_ErrorNoError,
	ErrorNoInput         = GenotickResult_ErrorNoInput,
	ErrorNoOutput        = GenotickResult_ErrorNoOutput,
	ErrorUnknownArgument = GenotickResult_ErrorUnknownArgument,
};

#else

typedef TGenotickInt32 EGenotickWeightMode;
typedef TGenotickInt32 EGenotickInheritedWeightMode;
typedef TGenotickInt32 EGenotickChartMode;
typedef TGenotickInt32 EGenotickResult;

#endif // __cplusplus

struct SGenotickMainSettings
{
#ifdef __cplusplus
	SGenotickMainSettings()
		: startTimePoint()
		, endTimePoint()
		, populationDesiredSize()
		, populationDAO()
		, performTraining()
		, dataDirectory()
		, minimumRobotInstructions()
		, maximumRobotInstructions()
		, maximumProcessorInstructionFactor()
		, maximumDeathByAge()
		, maximumDeathByWeight()
		, probabilityOfDeathByAge()
		, probabilityOfDeathByWeight()
		, weightMode(EGenotickWeightMode::WinCount)
		, weightExponent()
		, inheritedChildWeight()
		, inheritedChildWeightMode(EGenotickInheritedWeightMode::Parents)
		, maximumDataOffset()
		, protectRobotsUntilOutcomes()
		, newInstructionProbability()
		, instructionMutationProbability()
		, skipInstructionProbability()
		, minimumOutcomesToAllowBreeding()
		, minimumOutcomesBetweenBreeding()
		, killNonPredictingRobots()
		, randomRobotsAtEachUpdate()
		, protectBestRobots()
		, requireSymmetricalRobots()
		, resultThreshold()
		, ignoreColumns()
		, randomSeed()
		, chartMode(EGenotickChartMode::None)
	{}
#endif
	TGenotickTimePoint  startTimePoint;
	TGenotickTimePoint  endTimePoint;
	TGenotickInt32      populationDesiredSize;
	TGenotickString     populationDAO;
	TGenotickBoolean    performTraining;
	TGenotickString     dataDirectory;
	TGenotickInt32      minimumRobotInstructions;
	TGenotickInt32      maximumRobotInstructions;
	TGenotickInt32      maximumProcessorInstructionFactor;
	TGenotickDouble     maximumDeathByAge;
	TGenotickDouble     maximumDeathByWeight;
	TGenotickDouble     probabilityOfDeathByAge;
	TGenotickDouble     probabilityOfDeathByWeight;
	EGenotickWeightMode weightMode;
	TGenotickDouble     weightExponent;
	TGenotickDouble     inheritedChildWeight;
	EGenotickInheritedWeightMode inheritedChildWeightMode;
	TGenotickInt32      maximumDataOffset;
	TGenotickInt32      protectRobotsUntilOutcomes;
	TGenotickDouble     newInstructionProbability;
	TGenotickDouble     instructionMutationProbability;
	TGenotickDouble     skipInstructionProbability;
	TGenotickInt32      minimumOutcomesToAllowBreeding;
	TGenotickInt32      minimumOutcomesBetweenBreeding;
	TGenotickBoolean    killNonPredictingRobots;
	TGenotickDouble     randomRobotsAtEachUpdate;
	TGenotickDouble     protectBestRobots;
	TGenotickBoolean    requireSymmetricalRobots;
	TGenotickDouble     resultThreshold;
	TGenotickInt32      ignoreColumns;
	TGenotickInt64      randomSeed;
	EGenotickChartMode  chartMode;
};
typedef struct SGenotickMainSettings TGenotickMainSettings;

struct SGenotickStartSettings
{
	const char** parameters;
	unsigned int parameterCount;
};
typedef struct SGenotickStartSettings TGenotickStartSettings;

#ifdef ZORRO_LITE_C

struct SGenotick
{
	TGenotickInt32  GENOTICK_CALL GetInterfaceVersion(const struct SGenotick* pThis);
	EGenotickResult GENOTICK_CALL GetSettings(const struct SGenotick* pThis, struct SGenotickMainSettings* pSettings);
	EGenotickResult GENOTICK_CALL ChangeSettings(const struct SGenotick* pThis, const struct SGenotickMainSettings* pSettings);
	EGenotickResult GENOTICK_CALL Start(const struct SGenotick* pThis, const struct SGenotickStartSettings* pSettings);
	EGenotickResult GENOTICK_CALL Release(const struct SGenotick* pThis);
};
typedef const struct SGenotick IGenotick;

#else

struct SGenotickFunctions;
struct SGenotick;

#ifdef __cplusplus
typedef struct SGenotick IGenotick;
#else
typedef const struct SGenotickFunctions IGenotick;
#endif

struct SGenotickFunctions
{
	TGenotickInt32 (GENOTICK_CALL* GetInterfaceVersion)(IGenotick* pThis);
	EGenotickResult(GENOTICK_CALL* GetSettings)(IGenotick* pThis, struct SGenotickMainSettings* pSettings);
	EGenotickResult(GENOTICK_CALL* ChangeSettings)(IGenotick* pThis, const struct SGenotickMainSettings* pSettings);
	EGenotickResult(GENOTICK_CALL* Start)(IGenotick* pThis, const struct SGenotickStartSettings* pSettings);
	EGenotickResult(GENOTICK_CALL* Release)(IGenotick* pThis);
};

#ifdef __cplusplus
struct SGenotick
{
	const struct SGenotickFunctions functions;

	TGenotickInt32 GetInterfaceVersion() {
		return functions.GetInterfaceVersion(this);
	}
	EGenotickResult GetSettings(SGenotickMainSettings* pSettings) {
		return functions.GetSettings(this, pSettings);
	}
	EGenotickResult ChangeSettings(const SGenotickMainSettings* pSettings) {
		return functions.ChangeSettings(this, pSettings);
	}
	EGenotickResult Start(const SGenotickStartSettings* pSettings) {
		return functions.Start(this, pSettings);
	}
	EGenotickResult Release() {
		return functions.Release(this);
	}
protected:
	SGenotick() : functions{ 0 } {}
	~SGenotick() {}
};
#endif // __cplusplus

#endif // ZORRO_LITE_C

struct SGenotickJvmSettings
{
	const char* utf8_jvmDllPath;
	const char* utf8_javaClassPath;
};
typedef struct SGenotickJvmSettings TGenotickJvmSettings;

#ifdef __cplusplus
extern "C" {
#endif

// Unfortunately as of Java 8, JNI allows for one JavaVM instance per process only - ever.
// After releasing a JavaVM, you cannot even start a new one (shame).
// So keep the returned instance sacred and use it until the process dies.
GENOTICK_IMPORT_OR_EXPORT EGenotickResult GENOTICK_CALL LoadGenotick(IGenotick** pInstance, const struct SGenotickJvmSettings* pSettings);

#ifdef __cplusplus
}
#endif

#endif // I_GENOTICK_H
