
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
#define GENOTICK_ZERO_INIT = {}
#else
#define GENOTICK_ZERO_INIT
#endif

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

typedef signed long    TGenotickInt32;
typedef unsigned char  TGenotickBoolean;
typedef signed char    TGenotickByte;
typedef unsigned short TGenotickChar;
typedef signed short   TGenotickShort;
typedef float          TGenotickFloat;
typedef double         TGenotickDouble;
typedef TGenotickInt64 TGenotickTimePoint;
typedef TGenotickInt32 TGenotickSessionId;
typedef unsigned long  TGenotickSize;

struct SGenotickString
{
	char* utf8_buffer;
	TGenotickSize capacity;
};
typedef struct SGenotickString TGenotickString;

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

#define GenotickPrediction_Up    1
#define GenotickPrediction_Down -1
#define GenotickPrediction_Out   0

#define GenotickResult_Success                0
#define GenotickResult_InvalidArgument        1
#define GenotickResult_JvmDllNotFound         2
#define GenotickResult_JvmExportsNotFound     3
#define GenotickResult_JniError               4
#define GenotickResult_JniDetached            5
#define GenotickResult_JniVersionMismatch     6
#define GenotickResult_JniNoMemory            7
#define GenotickResult_JniExists              8
#define GenotickResult_JniInvalidArgument     9
#define GenotickResult_JavaClassMismatch     10
#define GenotickResult_JavaEnumMismatch      11
#define GenotickResult_JavaException         12
#define GenotickResult_ErrorNoError          13
#define GenotickResult_ErrorNoInput          14
#define GenotickResult_ErrorNoOutput         15
#define GenotickResult_ErrorUnknownArgument  16
#define GenotickResult_ErrorInvalidSession   17
#define GenotickResult_ErrorInsufficientData 18

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

#define GENOTICK_PREDICTION(e) \
	e(Up   , = GenotickPrediction_Up   , (SGenotickEnumMeta("UP"  ))) \
	e(Down , = GenotickPrediction_Down , (SGenotickEnumMeta("DOWN"))) \
	e(Out  , = GenotickPrediction_Out  , (SGenotickEnumMeta("OUT" ))) \

DEFINE_CUSTOM_ENUM_CLASS(EGenotickWeightMode, TGenotickInt32, GENOTICK_ENUM_WEIGHT_MODE, SGenotickEnumMeta)
DEFINE_CUSTOM_ENUM_CLASS(EGenotickInheritedWeightMode, TGenotickInt32, GENOTICK_ENUM_INHERITED_WEIGHT_MODE, SGenotickEnumMeta)
DEFINE_CUSTOM_ENUM_CLASS(EGenotickChartMode, TGenotickInt32, GENOTICK_ENUM_CHART_MODE, SGenotickEnumMeta)
DEFINE_CUSTOM_ENUM_CLASS(EGenotickPrediction, TGenotickInt32, GENOTICK_PREDICTION, SGenotickEnumMeta)

enum class EGenotickResult : TGenotickInt32
{
	Success               = GenotickResult_Success,
	InvalidArgument       = GenotickResult_InvalidArgument,
	JvmDllNotFound        = GenotickResult_JvmDllNotFound,
	JvmExportsNotFound    = GenotickResult_JvmExportsNotFound,
	JniError              = GenotickResult_JniError,
	JniDetached           = GenotickResult_JniDetached,
	JniVersionMismatch    = GenotickResult_JniVersionMismatch,
	JniNoMemory           = GenotickResult_JniNoMemory,
	JniExists             = GenotickResult_JniExists,
	JniInvalidArgument    = GenotickResult_JniInvalidArgument,
	JavaClassMismatch     = GenotickResult_JavaClassMismatch,
	JavaEnumMismatch      = GenotickResult_JavaEnumMismatch,
	JavaException         = GenotickResult_JavaException,
	ErrorNoError          = GenotickResult_ErrorNoError,
	ErrorNoInput          = GenotickResult_ErrorNoInput,
	ErrorNoOutput         = GenotickResult_ErrorNoOutput,
	ErrorUnknownArgument  = GenotickResult_ErrorUnknownArgument,
	ErrorInvalidSession   = GenotickResult_ErrorInvalidSession,
	ErrorInsufficientData = GenotickResult_ErrorInsufficientData,
};

#else

typedef TGenotickInt32 EGenotickWeightMode;
typedef TGenotickInt32 EGenotickInheritedWeightMode;
typedef TGenotickInt32 EGenotickChartMode;
typedef TGenotickInt32 EGenotickPrediction;
typedef TGenotickInt32 EGenotickResult;

#endif // __cplusplus

struct SGenotickMainSettings
{
#ifdef __cplusplus
	SGenotickMainSettings()
		: weightMode(EGenotickWeightMode::WinCount)
		, inheritedChildWeightMode(EGenotickInheritedWeightMode::Parents)
		, chartMode(EGenotickChartMode::None)
	{}
#endif
	TGenotickTimePoint  startTimePoint                    GENOTICK_ZERO_INIT;
	TGenotickTimePoint  endTimePoint                      GENOTICK_ZERO_INIT;
	TGenotickInt32      populationDesiredSize             GENOTICK_ZERO_INIT;
	TGenotickString     populationDAO                     GENOTICK_ZERO_INIT;
	TGenotickBoolean    performTraining                   GENOTICK_ZERO_INIT;
	TGenotickString     dataDirectory                     GENOTICK_ZERO_INIT;
	TGenotickInt32      minimumRobotInstructions          GENOTICK_ZERO_INIT;
	TGenotickInt32      maximumRobotInstructions          GENOTICK_ZERO_INIT;
	TGenotickInt32      maximumProcessorInstructionFactor GENOTICK_ZERO_INIT;
	TGenotickDouble     maximumDeathByAge                 GENOTICK_ZERO_INIT;
	TGenotickDouble     maximumDeathByWeight              GENOTICK_ZERO_INIT;
	TGenotickDouble     probabilityOfDeathByAge           GENOTICK_ZERO_INIT;
	TGenotickDouble     probabilityOfDeathByWeight        GENOTICK_ZERO_INIT;
	EGenotickWeightMode weightMode;
	TGenotickDouble     weightExponent                    GENOTICK_ZERO_INIT;
	TGenotickDouble     inheritedChildWeight              GENOTICK_ZERO_INIT;
	EGenotickInheritedWeightMode inheritedChildWeightMode;
	TGenotickInt32      maximumDataOffset                 GENOTICK_ZERO_INIT;
	TGenotickInt32      protectRobotsUntilOutcomes        GENOTICK_ZERO_INIT;
	TGenotickDouble     newInstructionProbability         GENOTICK_ZERO_INIT;
	TGenotickDouble     instructionMutationProbability    GENOTICK_ZERO_INIT;
	TGenotickDouble     skipInstructionProbability        GENOTICK_ZERO_INIT;
	TGenotickInt32      minimumOutcomesToAllowBreeding    GENOTICK_ZERO_INIT;
	TGenotickInt32      minimumOutcomesBetweenBreeding    GENOTICK_ZERO_INIT;
	TGenotickBoolean    killNonPredictingRobots           GENOTICK_ZERO_INIT;
	TGenotickDouble     randomRobotsAtEachUpdate          GENOTICK_ZERO_INIT;
	TGenotickDouble     protectBestRobots                 GENOTICK_ZERO_INIT;
	TGenotickBoolean    requireSymmetricalRobots          GENOTICK_ZERO_INIT;
	TGenotickDouble     resultThreshold                   GENOTICK_ZERO_INIT;
	TGenotickInt32      ignoreColumns                     GENOTICK_ZERO_INIT;
	TGenotickInt64      randomSeed                        GENOTICK_ZERO_INIT;
	EGenotickChartMode  chartMode;
};

struct SGenotickDataColumns
{
	TGenotickTimePoint     time           GENOTICK_ZERO_INIT;
	TGenotickDouble        open           GENOTICK_ZERO_INIT;
	TGenotickDouble        high           GENOTICK_ZERO_INIT;
	TGenotickDouble        low            GENOTICK_ZERO_INIT;
	TGenotickDouble        close          GENOTICK_ZERO_INIT;
	TGenotickDouble        volume         GENOTICK_ZERO_INIT;
	const TGenotickDouble* otherData      GENOTICK_ZERO_INIT;
	unsigned int           otherDataCount GENOTICK_ZERO_INIT;
};

struct SGenotickAssetData
{
	const char*                        assetName   GENOTICK_ZERO_INIT;
	const struct SGenotickDataColumns* columns     GENOTICK_ZERO_INIT;
	TGenotickSize                      columnCount GENOTICK_ZERO_INIT;
};

struct SGenotickStartArgs
{
	const char* const* elements     GENOTICK_ZERO_INIT;
	TGenotickSize      elementCount GENOTICK_ZERO_INIT;
};

struct SGenotickLoadSettings
{
	const char* utf8_jvmDllPath    GENOTICK_ZERO_INIT;
	const char* utf8_javaClassPath GENOTICK_ZERO_INIT;
};

typedef struct SGenotickMainSettings TGenotickMainSettings;
typedef struct SGenotickDataColumns  TGenotickDataColumns;
typedef struct SGenotickAssetData    TGenotickAssetData;
typedef struct SGenotickStartArgs    TGenotickStartArgs;
typedef struct SGenotickLoadSettings TGenotickLoadSettings;

#ifdef ZORRO_LITE_C

struct SGenotickTimePoints
{
	TGenotickBoolean   GENOTICK_CALL FindIndex(const struct SGenotickTimePoints* pThis, TGenotickTimePoint timePoint, TGenotickInt32* pIndex);
	TGenotickTimePoint GENOTICK_CALL GetElement(const struct SGenotickTimePoints* pThis, TGenotickInt32 index);
	TGenotickSize      GENOTICK_CALL GetElementCount(const struct SGenotickTimePoints* pThis);
	void               GENOTICK_CALL Release(const struct SGenotickTimePoints* pThis);
};

struct SGenotickPredictions
{
	EGenotickPrediction GENOTICK_CALL GetElement(const struct SGenotickPredictions* pThis, TGenotickInt32 index);
	TGenotickSize       GENOTICK_CALL GetElementCount(const struct SGenotickPredictions* pThis);
	void                GENOTICK_CALL Release(const struct SGenotickPredictions* pThis);
};

typedef const struct SGenotickTimePoints IGenotickTimePoints;
typedef const struct SGenotickPredictions IGenotickPredictions;

struct SGenotick
{
	TGenotickInt32  GENOTICK_CALL GetInterfaceVersion(const struct SGenotick* pThis);
	EGenotickResult GENOTICK_CALL CreateSession(const struct SGenotick* pThis, TGenotickSessionId sessionId);
	EGenotickResult GENOTICK_CALL RemoveSession(const struct SGenotick* pThis, TGenotickSessionId sessionId);
	EGenotickResult GENOTICK_CALL RemoveAllSessions(const struct SGenotick* pThis);
	EGenotickResult GENOTICK_CALL GetSettings(const struct SGenotick* pThis, TGenotickSessionId sessionId, TGenotickMainSettings* pSettings);
	EGenotickResult GENOTICK_CALL ChangeSettings(const struct SGenotick* pThis, TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings);
	EGenotickResult GENOTICK_CALL SetAssetData(const struct SGenotick* pThis, TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData);
	EGenotickResult GENOTICK_CALL Start(const struct SGenotick* pThis, TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs);
	EGenotickResult GENOTICK_CALL GetTimePoints(const struct SGenotick* pThis, TGenotickSessionId sessionId, TGenotickTimePoints** ppTimePoints);
	EGenotickResult GENOTICK_CALL GetPredictions(const struct SGenotick* pThis, TGenotickSessionId sessionId, TGenotickPredictions** ppPredictions);
	EGenotickResult GENOTICK_CALL GetNewestTimePoint(const struct SGenotick* pThis, TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint);
	EGenotickResult GENOTICK_CALL GetNewestPrediction(const struct SGenotick* pThis, TGenotickSessionId sessionId, EGenotickPrediction* pPrediction);
	EGenotickResult GENOTICK_CALL Release(const struct SGenotick* pThis);
};

typedef const struct SGenotick IGenotick;

#else

struct SGenotickFunctions;
struct SGenotick;
struct SGenotickTimePointsFunctions;
struct SGenotickTimePoints;
struct SGenotickPredictionsFunctions;
struct SGenotickPredictions;

#ifdef __cplusplus
typedef const struct SGenotick IGenotick;
typedef const struct SGenotickTimePoints IGenotickTimePoints;
typedef const struct SGenotickPredictions IGenotickPredictions;
#else
typedef const struct SGenotickFunctions IGenotick;
typedef const struct SGenotickTimePointsFunctions IGenotickTimePoints;
typedef const struct SGenotickPredictionsFunctions IGenotickPredictions;
#endif

struct SGenotickTimePointsFunctions
{
	TGenotickBoolean   (GENOTICK_CALL* FindIndex)(IGenotickTimePoints* pThis, TGenotickTimePoint timePoint, TGenotickInt32* pIndex);
	TGenotickTimePoint (GENOTICK_CALL* GetElement)(IGenotickTimePoints* pThis, TGenotickInt32 index);
	TGenotickSize      (GENOTICK_CALL* GetElementCount)(IGenotickTimePoints* pThis);
	void               (GENOTICK_CALL* Release)(IGenotickTimePoints* pThis);
};

struct SGenotickPredictionsFunctions
{
	EGenotickPrediction (GENOTICK_CALL* GetElement)(IGenotickPredictions* pThis, TGenotickInt32 index);
	TGenotickSize       (GENOTICK_CALL* GetElementCount)(IGenotickPredictions* pThis);
	void                (GENOTICK_CALL* Release)(IGenotickPredictions* pThis);
};

struct SGenotickFunctions
{
	TGenotickInt32  (GENOTICK_CALL* GetInterfaceVersion)(IGenotick* pThis);
	EGenotickResult (GENOTICK_CALL* CreateSession)(IGenotick* pThis, TGenotickSessionId sessionId);
	EGenotickResult (GENOTICK_CALL* RemoveSession)(IGenotick* pThis, TGenotickSessionId sessionId);
	EGenotickResult (GENOTICK_CALL* RemoveAllSessions)(IGenotick* pThis);
	EGenotickResult (GENOTICK_CALL* GetSettings)(IGenotick* pThis, TGenotickSessionId sessionId, TGenotickMainSettings* pSettings);
	EGenotickResult (GENOTICK_CALL* ChangeSettings)(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings);
	EGenotickResult (GENOTICK_CALL* SetAssetData)(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData);
	EGenotickResult (GENOTICK_CALL* Start)(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs);
	EGenotickResult (GENOTICK_CALL* GetTimePoints)(IGenotick* pThis, TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints);
	EGenotickResult (GENOTICK_CALL* GetPredictions)(IGenotick* pThis, TGenotickSessionId sessionId, IGenotickPredictions** ppPredictions);
	EGenotickResult (GENOTICK_CALL* GetNewestTimePoint)(IGenotick* pThis, TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint);
	EGenotickResult (GENOTICK_CALL* GetNewestPrediction)(IGenotick* pThis, TGenotickSessionId sessionId, EGenotickPrediction* pPrediction);
	EGenotickResult (GENOTICK_CALL* Release)(IGenotick* pThis);
};

#ifdef __cplusplus

struct SGenotickTimePoints
{
	const struct SGenotickTimePointsFunctions functions;

	TGenotickBoolean FindIndex(TGenotickTimePoint timePoint, TGenotickInt32* index) const {
		return functions.FindIndex(this, timePoint, index);
	}
	TGenotickTimePoint GetElement(TGenotickInt32 index) const {
		return functions.GetElement(this, index);
	}
	TGenotickSize GetElementCount() const {
		return functions.GetElementCount(this);
	}
	void Release() const {
		return functions.Release(this);
	}
protected:
	SGenotickTimePoints() : functions{ 0 } {}
	~SGenotickTimePoints() {}
};

struct SGenotickPredictions
{
	const struct SGenotickPredictionsFunctions functions;

	EGenotickPrediction GetElement(TGenotickInt32 index) const {
		return functions.GetElement(this, index);
	}
	TGenotickSize GetElementCount() const {
		return functions.GetElementCount(this);
	}
	void Release() const {
		return functions.Release(this);
	}
protected:
	SGenotickPredictions() : functions{ 0 } {}
	~SGenotickPredictions() {}
};

struct SGenotick
{
	const struct SGenotickFunctions functions;

	TGenotickInt32 GetInterfaceVersion() const {
		return functions.GetInterfaceVersion(this);
	}
	EGenotickResult CreateSession(TGenotickSessionId sessionId) const {
		return functions.CreateSession(this, sessionId);
	}
	EGenotickResult RemoveSession(TGenotickSessionId sessionId) const {
		return functions.RemoveSession(this, sessionId);
	}
	EGenotickResult RemoveAllSessions() const {
		return functions.RemoveAllSessions(this);
	}
	EGenotickResult GetSettings(TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) const {
		return functions.GetSettings(this, sessionId, pSettings);
	}
	EGenotickResult ChangeSettings(TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) const {
		return functions.ChangeSettings(this, sessionId, pSettings);
	}
	EGenotickResult SetAssetData(TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData) const {
		return functions.SetAssetData(this, sessionId, pAssetData);
	}
	EGenotickResult Start(TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) const {
		return functions.Start(this, sessionId, pArgs);
	}
	EGenotickResult GetTimePoints(TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) const {
		return functions.GetTimePoints(this, sessionId, ppTimePoints);
	}
	EGenotickResult GetPredictions(TGenotickSessionId sessionId, IGenotickPredictions** ppPredictions) const {
		return functions.GetPredictions(this, sessionId, ppPredictions);
	}
	EGenotickResult GetNewestTimePoint(TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) const {
		return functions.GetNewestTimePoint(this, sessionId, pTimePoint);
	}
	EGenotickResult GetNewestPrediction(TGenotickSessionId sessionId, EGenotickPrediction* pPrediction) const {
		return functions.GetNewestPrediction(this, sessionId, pPrediction);
	}
	EGenotickResult Release() const {
		return functions.Release(this);
	}
protected:
	SGenotick() : functions{ 0 } {}
	~SGenotick() {}
};

#endif // __cplusplus

#endif // ZORRO_LITE_C

#ifdef __cplusplus
extern "C" {
#endif

// Unfortunately as of Java 8, JNI allows for one JavaVM instance per process only - ever.
// After releasing a JavaVM, you cannot even start a new one (shame).
// So keep the returned instance sacred and use it until the process dies.
GENOTICK_IMPORT_OR_EXPORT EGenotickResult GENOTICK_CALL LoadGenotick(IGenotick** ppInstance, const TGenotickLoadSettings* pSettings);

#ifdef __cplusplus
}
#endif

#undef GENOTICK_ZERO_INIT

#endif // I_GENOTICK_H
