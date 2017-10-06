
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
#define GENOTICK_SAFE_RELEASE(p) if(p) { p->Release(); p = 0; }
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

#ifdef ZORRO_LITE_C

typedef TGenotickInt32 EGenotickWeightMode;
typedef TGenotickInt32 EGenotickInheritedWeightMode;
typedef TGenotickInt32 EGenotickChartMode;
typedef TGenotickInt32 EGenotickResult;

#define eGenotickWeightMode_WinCount     0
#define eGenotickWeightMode_WinRate      1
#define eGenotickWeightMode_ProfitCount  2
#define eGenotickWeightMode_ProfitFactor 3

#define eGenotickInheritedWeightMode_Parents      0
#define eGenotickInheritedWeightMode_Ancestors    1
#define eGenotickInheritedWeightMode_AncestorsLog 2

#define eGenotickChartMode_None                 0
#define eGenotickChartMode_Draw                 (1 << 0)
#define eGenotickChartMode_Save                 (1 << 1)
#define eGenotickChartMode_JFreeChart           (1 << 2)
#define eGenotickChartMode_JFreeChart_Draw      (eGenotickChartMode_JFreeChart | eGenotickChartMode_Draw)
#define eGenotickChartMode_JFreeChart_Save      (eGenotickChartMode_JFreeChart | eGenotickChartMode_Save)
#define eGenotickChartMode_JFreeChart_Draw_Save (eGenotickChartMode_JFreeChart | eGenotickChartMode_Draw | eGenotickChartMode_Save)

#define eGenotickResult_Success               0
#define eGenotickResult_InvalidArgument       1
#define eGenotickResult_JvmDllNotFound        2
#define eGenotickResult_JvmExportsNotFound    3
#define eGenotickResult_JniError              4
#define eGenotickResult_JniDetached           5
#define eGenotickResult_JniVersionMismatch    6
#define eGenotickResult_JniNoMemory           7
#define eGenotickResult_JniExists             8
#define eGenotickResult_JniInvalidArgument    9
#define eGenotickResult_JavaClassMismatch    10
#define eGenotickResult_JavaEnumMismatch     11
#define eGenotickResult_JavaException        12
#define eGenotickResult_ErrorNoError         13
#define eGenotickResult_ErrorNoInput         14
#define eGenotickResult_ErrorNoOutput        15
#define eGenotickResult_ErrorUnknownArgument 16

#else

#define GENOTICK_OPEN_ENUM(name) typedef enum name {
#define GENOTICK_CLOSE_ENUM(name) } name;

#define GENOTICK_UNROLL_ENUM_NAME(cppEnumName, ...)                                    cppEnumName,
#define GENOTICK_UNROLL_ENUM_NAME_AND_VALUE(cppEnumName, javaEnumName, enumValue, ...) cppEnumName = enumValue,
#define GENOTICK_UNROLL_ENUM_DUMMY_NAME(cppEnumName, ...)                              eDummy_##cppEnumName,

#define GENOTICK_DEFINE_ENUM(name, list) \
	GENOTICK_OPEN_ENUM(name) list(GENOTICK_UNROLL_ENUM_NAME) GENOTICK_CLOSE_ENUM(name)

#define GENOTICK_DEFINE_ENUM_WITH_VALUE(name, list) \
	GENOTICK_OPEN_ENUM(name) list(GENOTICK_UNROLL_ENUM_NAME_AND_VALUE) GENOTICK_CLOSE_ENUM(name)

#define GENOTICK_DEFINE_ENUM_WITH_COUNT(name, list, count) \
	GENOTICK_OPEN_ENUM(name) list(GENOTICK_UNROLL_ENUM_NAME) count, GENOTICK_CLOSE_ENUM(name)

#define GENOTICK_DEFINE_ENUM_WITH_VALUE_COUNT(name, list, count) \
	GENOTICK_OPEN_ENUM(name) list(GENOTICK_UNROLL_ENUM_DUMMY_NAME) count, list(GENOTICK_UNROLL_ENUM_NAME_AND_VALUE) GENOTICK_CLOSE_ENUM(name)

#define GENOTICK_ENUM_WEIGHT_MODE(f) \
	f(eGenotickWeightMode_WinCount     , WIN_COUNT    ) \
	f(eGenotickWeightMode_WinRate      , WIN_RATE     ) \
	f(eGenotickWeightMode_ProfitCount  , PROFIT_COUNT ) \
	f(eGenotickWeightMode_ProfitFactor , PROFIT_FACTOR) \

#define GENOTICK_ENUM_INHERITED_WEIGHT_MODE(f) \
	f(eGenotickInheritedWeightMode_Parents      , PARENTS      ) \
	f(eGenotickInheritedWeightMode_Ancestors    , ANCESTORS    ) \
	f(eGenotickInheritedWeightMode_AncestorsLog , ANCESTORS_LOG) \

#define GENOTICK_ENUM_CHART_MODE(f) \
	f(eGenotickChartMode_None                 , NONE                 , (0)                                                                                ) \
	f(eGenotickChartMode_Draw                 , DRAW                 , (1 << 0)                                                                           ) \
	f(eGenotickChartMode_Save                 , SAVE                 , (1 << 1)                                                                           ) \
	f(eGenotickChartMode_JFreeChart           , JFREECHART           , (1 << 2)                                                                           ) \
	f(eGenotickChartMode_JFreeChart_Draw      , JFREECHART_DRAW      , (eGenotickChartMode_JFreeChart | eGenotickChartMode_Draw)                          ) \
	f(eGenotickChartMode_JFreeChart_Save      , JFREECHART_SAVE      , (eGenotickChartMode_JFreeChart | eGenotickChartMode_Save)                          ) \
	f(eGenotickChartMode_JFreeChart_Draw_Save , JFREECHART_DRAW_SAVE , (eGenotickChartMode_JFreeChart | eGenotickChartMode_Draw | eGenotickChartMode_Save)) \

GENOTICK_DEFINE_ENUM_WITH_COUNT(EGenotickWeightMode, GENOTICK_ENUM_WEIGHT_MODE, eGenotickWeightMode_Count)
GENOTICK_DEFINE_ENUM_WITH_COUNT(EGenotickInheritedWeightMode, GENOTICK_ENUM_INHERITED_WEIGHT_MODE, eGenotickInheritedWeightMode_Count)
GENOTICK_DEFINE_ENUM_WITH_VALUE_COUNT(EGenotickChartMode, GENOTICK_ENUM_CHART_MODE, eGenotickChartMode_Count)

GENOTICK_OPEN_ENUM(EGenotickResult)
	eGenotickResult_Success = 0,
	eGenotickResult_InvalidArgument,
	eGenotickResult_JvmDllNotFound,
	eGenotickResult_JvmExportsNotFound,
	eGenotickResult_JniError,
	eGenotickResult_JniDetached,
	eGenotickResult_JniVersionMismatch,
	eGenotickResult_JniNoMemory,
	eGenotickResult_JniExists,
	eGenotickResult_JniInvalidArgument,
	eGenotickResult_JavaClassMismatch,
	eGenotickResult_JavaEnumMismatch,
	eGenotickResult_JavaException,
	eGenotickResult_ErrorNoError,
	eGenotickResult_ErrorNoInput,
	eGenotickResult_ErrorNoOutput,
	eGenotickResult_ErrorUnknownArgument,
GENOTICK_CLOSE_ENUM(EGenotickResult)

#endif // ZORRO_LITE_C

struct SGenotickMainSettings
{
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
