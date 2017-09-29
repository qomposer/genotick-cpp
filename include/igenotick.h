
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

#undef SAFE_RELEASE
#define SAFE_RELEASE(p) if(p) { p->Release(); p = 0; }

#ifdef __cplusplus
extern "C" {
#endif

struct IGenotickFunctions;
struct IGenotick;

#ifdef __cplusplus
typedef struct IGenotick IGenotick;
#else
typedef const struct IGenotickFunctions* IGenotick;
#endif

typedef signed long      TGenotickInt32;
typedef signed long long TGenotickInt64;
typedef unsigned char    TGenotickBoolean;
typedef signed char      TGenotickByte;
typedef unsigned short   TGenotickChar;
typedef signed short     TGenotickShort;
typedef float            TGenotickFloat;
typedef double           TGenotickDouble;
typedef TGenotickInt64   TGenotickTimePoint;

const TGenotickBoolean GenotickFalse = 0;
const TGenotickBoolean GenotickTrue = 1;

typedef struct TGenotickString
{
	char* utf8_buffer;
	unsigned long capacity;
} TGenotickString;

typedef enum EGenotickWeightMode
{
	eGenotickWeightMode_WinCount = 0,
	eGenotickWeightMode_WinRate,
	eGenotickWeightMode_ProfitCount,
	eGenotickWeightMode_ProfitFactor,
} EGenotickWeightMode;

typedef enum EGenotickInheritedWeightMode
{
	eGenotickInheritedWeightMode_Parents = 0,
	eGenotickInheritedWeightMode_Ancestors,
	eGenotickInheritedWeightMode_AncestorsLog,
} EGenotickInheritedWeightMode;

typedef enum EGenotickChartMode
{
	eGenotickChartMode_None                 = 0,
	eGenotickChartMode_Draw                 = 1<<0,
	eGenotickChartMode_Save                 = 1<<1,
	eGenotickChartMode_JFreeChart           = 1<<2,
	eGenotickChartMode_JFreeChart_Draw      = eGenotickChartMode_JFreeChart | eGenotickChartMode_Draw,
	eGenotickChartMode_JFreeChart_Save      = eGenotickChartMode_JFreeChart | eGenotickChartMode_Save,
	eGenotickChartMode_JFreeChart_Draw_Save = eGenotickChartMode_JFreeChart | eGenotickChartMode_Draw | eGenotickChartMode_Save,
} EGenotickChartMode;

typedef enum EGenotickResult
{
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
	eGenotickResult_ErrorContinue,
	eGenotickResult_ErrorNoError,
	eGenotickResult_ErrorNoInput,
	eGenotickResult_ErrorNoOutput,
	eGenotickResult_ErrorUnknownArgument,
	eGenotickResult_ErrorUnknown,
} EGenotickResult;

typedef struct SGenotickMainSettings
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
} SGenotickMainSettings;

typedef struct SGenotickStartSettings
{
	const char** parameters;
	unsigned int parameterCount;
} SGenotickStartSettings;

struct IGenotickFunctions
{
	TGenotickInt32(GENOTICK_CALL* GetInterfaceVersion)(IGenotick* pThis);
	EGenotickResult(GENOTICK_CALL* GetSettings)(IGenotick* pThis, SGenotickMainSettings* pSettings);
	EGenotickResult(GENOTICK_CALL* ChangeSettings)(IGenotick* pThis, const SGenotickMainSettings* pSettings);
	EGenotickResult(GENOTICK_CALL* Start)(IGenotick* pThis, const SGenotickStartSettings* pSettings);
	EGenotickResult(GENOTICK_CALL* Release)(IGenotick* pThis);
};

struct IGenotick
{
	const struct IGenotickFunctions* functions;
#ifdef __cplusplus
	TGenotickInt32 GetInterfaceVersion() {
		return functions->GetInterfaceVersion(this);
	}
	EGenotickResult GetSettings(SGenotickMainSettings* pSettings) {
		return functions->GetSettings(this, pSettings);
	}
	EGenotickResult ChangeSettings(const SGenotickMainSettings* pSettings) {
		return functions->ChangeSettings(this, pSettings);
	}
	EGenotickResult Start(const SGenotickStartSettings* pSettings) {
		return functions->Start(this, pSettings);
	}
	EGenotickResult Release() {
		return functions->Release(this);
	}
protected:
	IGenotick() {}
	virtual ~IGenotick() {}
#endif
};

typedef struct
{
	const char* utf8_jvmDllPath;
	const char* utf8_javaClassPath;
} SGenotickJvmSettings;

// Unfortunately as of Java 8, JNI allows for one JavaVM instance per process only - ever.
// After releasing a JavaVM, you cannot even start a new one (shame).
// So keep the returned instance sacred and use it until the process dies.
GENOTICK_IMPORT_OR_EXPORT EGenotickResult GENOTICK_CALL LoadGenotick(IGenotick** pInstance, const SGenotickJvmSettings* pSettings);

#ifdef __cplusplus
}
#endif

#endif // I_GENOTICK_H
