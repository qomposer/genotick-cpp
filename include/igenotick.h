
#ifndef I_GENOTICK_H_
#define I_GENOTICK_H_

#include <common/types.h>

#define GENOTICK_INTERFACE_VERSION 1

#define GENOTICK_EXPORT __declspec(dllexport)
#define GENOTICK_IMPORT __declspec(dllimport)
#define GENOTICK_CALL __cdecl

#ifdef GENOTICK_DLL
#ifdef GENOTICK_IMPL
#define GENOTICK_IMPORT_EXPORT GENOTICK_EXPORT
#else
#define GENOTICK_IMPORT_EXPORT GENOTICK_IMPORT
#endif
#else
#define GENOTICK_IMPORT_EXPORT
#endif

#ifdef __cplusplus
#define ZERO_INIT = {}
#define FTHIS(type)               struct type*
#define FTHISC(type)        const struct type*
#define ITYPE(type, name) typedef struct type name;
#else
#define ZERO_INIT
#define FTHIS(type)               const struct type##Functions*
#define FTHISC(type)              const struct type##Functions*
#define ITYPE(type, name) typedef const struct type##Functions name;
#endif
#ifdef ZORRO_LITE_C
#define FPTR(name)  GENOTICK_CALL  name
#else
#define FPTR(name) (GENOTICK_CALL* name)
#endif

#pragma pack(push, 4)

typedef int32_t  TGenotickInt32;
typedef int64_t  TGenotickInt64;
typedef uint8_t  TGenotickBoolean;
typedef int8_t   TGenotickByte;
typedef uint16_t TGenotickChar;
typedef int16_t  TGenotickShort;
typedef float    TGenotickFloat;
typedef double   TGenotickDouble;
typedef int64_t  TGenotickTimePoint;
typedef int32_t  TGenotickSessionId;
typedef uint32_t TGenotickSize;

struct SGenotickString
{
	char* utf8buffer       ZERO_INIT;
	TGenotickSize capacity ZERO_INIT;
};
typedef struct SGenotickString TGenotickString;

#define GenotickFalse 0
#define GenotickTrue 1

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

#define GenotickResult_Success                 0
#define GenotickResult_Failure                 1
#define GenotickResult_InvalidArgument         2
#define GenotickResult_ThreadNotAttached       3
#define GenotickResult_ThreadAlreadyAttached   4
#define GenotickResult_JvmDllNotFound        100
#define GenotickResult_JvmExportsNotFound    101
#define GenotickResult_JniError              200
#define GenotickResult_JniDetached           201
#define GenotickResult_JniVersionMismatch    202
#define GenotickResult_JniNoMemory           203
#define GenotickResult_JniExists             204
#define GenotickResult_JniInvalidArgument    205
#define GenotickResult_JavaClassMismatch     300
#define GenotickResult_JavaEnumMismatch      301
#define GenotickResult_JavaException         302
#define GenotickResult_ErrorNoInput          400
#define GenotickResult_ErrorNoOutput         401
#define GenotickResult_ErrorUnknownArgument  402
#define GenotickResult_ErrorInvalidSession   403
#define GenotickResult_ErrorDuplicateSession 404
#define GenotickResult_ErrorInsufficientData 405

#ifdef __cplusplus

#include <common/enum.h>

struct SGenotickEnumMeta
{
	constexpr SGenotickEnumMeta(const char* const javaValueName)
		: javaValueName(javaValueName) {}
	const char* const javaValueName;
};

#define GENOTICK_WEIGHT_MODE_LIST(e) \
	e( WinCount     , = GenotickWeightMode_WinCount     , (SGenotickEnumMeta("WIN_COUNT"    )) ) \
	e( WinRate      , = GenotickWeightMode_WinRate      , (SGenotickEnumMeta("WIN_RATE"     )) ) \
	e( ProfitCount  , = GenotickWeightMode_ProfitCount  , (SGenotickEnumMeta("PROFIT_COUNT" )) ) \
	e( ProfitFactor , = GenotickWeightMode_ProfitFactor , (SGenotickEnumMeta("PROFIT_FACTOR")) ) \

DEFINE_DATA_ENUM_CLASS(EGenotickWeightMode, CGenotickWeightMode, TGenotickInt32, GENOTICK_WEIGHT_MODE_LIST, SGenotickEnumMeta)


#define GENOTICK_INHERITED_WEIGHT_MODE_LIST(e) \
	e( Parents      , = GenotickInheritedWeightMode_Parents      , (SGenotickEnumMeta("PARENTS"      )) ) \
	e( Ancestors    , = GenotickInheritedWeightMode_Ancestors    , (SGenotickEnumMeta("ANCESTORS"    )) ) \
	e( AncestorsLog , = GenotickInheritedWeightMode_AncestorsLog , (SGenotickEnumMeta("ANCESTORS_LOG")) ) \

DEFINE_DATA_ENUM_CLASS(EGenotickInheritedWeightMode, CGenotickInheritedWeightMode, TGenotickInt32, GENOTICK_INHERITED_WEIGHT_MODE_LIST, SGenotickEnumMeta)

#define GENOTICK_CHART_MODE_LIST(e) \
	e( None                 , = GenotickChartMode_None                 , (SGenotickEnumMeta("NONE"                )) ) \
	e( Draw                 , = GenotickChartMode_Draw                 , (SGenotickEnumMeta("DRAW"                )) ) \
	e( Save                 , = GenotickChartMode_Save                 , (SGenotickEnumMeta("SAVE"                )) ) \
	e( JFreeChart           , = GenotickChartMode_JFreeChart           , (SGenotickEnumMeta("JFREECHART"          )) ) \
	e( JFreeChart_Draw      , = GenotickChartMode_JFreeChart_Draw      , (SGenotickEnumMeta("JFREECHART_DRAW"     )) ) \
	e( JFreeChart_Save      , = GenotickChartMode_JFreeChart_Save      , (SGenotickEnumMeta("JFREECHART_SAVE"     )) ) \
	e( JFreeChart_Draw_Save , = GenotickChartMode_JFreeChart_Draw_Save , (SGenotickEnumMeta("JFREECHART_DRAW_SAVE")) ) \

DEFINE_DATA_ENUM_CLASS(EGenotickChartMode, CGenotickChartMode, TGenotickInt32, GENOTICK_CHART_MODE_LIST, SGenotickEnumMeta)

#define GENOTICK_PREDICTION_LIST(e) \
	e( Up   , = GenotickPrediction_Up   , (SGenotickEnumMeta("UP"  )) ) \
	e( Down , = GenotickPrediction_Down , (SGenotickEnumMeta("DOWN")) ) \
	e( Out  , = GenotickPrediction_Out  , (SGenotickEnumMeta("OUT" )) ) \

DEFINE_DATA_ENUM_CLASS(EGenotickPrediction, CGenotickPrediction, TGenotickInt32, GENOTICK_PREDICTION_LIST, SGenotickEnumMeta)

#undef GENOTICK_WEIGHT_MODE_LIST
#undef GENOTICK_INHERITED_WEIGHT_MODE_LIST
#undef GENOTICK_CHART_MODE_LIST
#undef GENOTICK_PREDICTION_LIST

enum class EGenotickResult : TGenotickInt32
{
	Success               = GenotickResult_Success,
	Failure               = GenotickResult_Failure,
	InvalidArgument       = GenotickResult_InvalidArgument,
	ThreadNotAttached     = GenotickResult_ThreadNotAttached,
	ThreadAlreadyAttached = GenotickResult_ThreadAlreadyAttached,
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
	ErrorNoInput          = GenotickResult_ErrorNoInput,
	ErrorNoOutput         = GenotickResult_ErrorNoOutput,
	ErrorUnknownArgument  = GenotickResult_ErrorUnknownArgument,
	ErrorInvalidSession   = GenotickResult_ErrorInvalidSession,
	ErrorDuplicateSession = GenotickResult_ErrorDuplicateSession,
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
	TGenotickDouble     maximumDeathByAge                 ZERO_INIT;
	TGenotickDouble     maximumDeathByWeight              ZERO_INIT;
	TGenotickDouble     probabilityOfDeathByAge           ZERO_INIT;
	TGenotickDouble     probabilityOfDeathByWeight        ZERO_INIT;
	TGenotickDouble     weightExponent                    ZERO_INIT;
	TGenotickDouble     inheritedChildWeight              ZERO_INIT;
	TGenotickDouble     newInstructionProbability         ZERO_INIT;
	TGenotickDouble     instructionMutationProbability    ZERO_INIT;
	TGenotickDouble     skipInstructionProbability        ZERO_INIT;
	TGenotickDouble     randomRobotsAtEachUpdate          ZERO_INIT;
	TGenotickDouble     protectBestRobots                 ZERO_INIT;
	TGenotickDouble     resultThreshold                   ZERO_INIT;
	TGenotickTimePoint  startTimePoint                    ZERO_INIT;
	TGenotickTimePoint  endTimePoint                      ZERO_INIT;
	TGenotickInt64      randomSeed                        ZERO_INIT;
	TGenotickString     dataDirectory                     ZERO_INIT;
	TGenotickString     populationDAO                     ZERO_INIT;
	TGenotickInt32      populationDesiredSize             ZERO_INIT;
	TGenotickInt32      minimumRobotInstructions          ZERO_INIT;
	TGenotickInt32      maximumRobotInstructions          ZERO_INIT;
	TGenotickInt32      maximumProcessorInstructionFactor ZERO_INIT;
	TGenotickInt32      maximumDataOffset                 ZERO_INIT;
	TGenotickInt32      protectRobotsUntilOutcomes        ZERO_INIT;
	TGenotickInt32      minimumOutcomesToAllowBreeding    ZERO_INIT;
	TGenotickInt32      minimumOutcomesBetweenBreeding    ZERO_INIT;
	TGenotickInt32      ignoreColumns                     ZERO_INIT;
	EGenotickWeightMode          weightMode;
	EGenotickInheritedWeightMode inheritedChildWeightMode;
	EGenotickChartMode           chartMode;
	TGenotickBoolean    performTraining                   ZERO_INIT;
	TGenotickBoolean    killNonPredictingRobots           ZERO_INIT;
	TGenotickBoolean    requireSymmetricalRobots          ZERO_INIT;
	TGenotickByte       padding[1]                        ZERO_INIT;
};
typedef struct SGenotickMainSettings     TGenotickMainSettings;

struct SGenotickDataPoint
{
	TGenotickTimePoint     time            ZERO_INIT;
	TGenotickDouble        open            ZERO_INIT;
	TGenotickDouble        high            ZERO_INIT;
	TGenotickDouble        low             ZERO_INIT;
	TGenotickDouble        close           ZERO_INIT;
	const TGenotickDouble* optionalColumns ZERO_INIT;
};
typedef struct SGenotickDataPoint TGenotickDataPoint;
#define GenotickMinColumnCount 5u

struct SGenotickAssetData
{
	const char*                      assetName              ZERO_INIT;
	const struct SGenotickDataPoint* dataPoints             ZERO_INIT;
	TGenotickSize                    dataPointCount         ZERO_INIT;
	TGenotickSize                    optionalColumnCount    ZERO_INIT;
	TGenotickBoolean                 firstDataPointIsNewest ZERO_INIT;
	TGenotickByte                    padding[3]             ZERO_INIT;
};
typedef struct SGenotickAssetData TGenotickAssetData;

struct SGenotickStartArgs
{
	const char* const* elements     ZERO_INIT;
	TGenotickSize      elementCount ZERO_INIT;
};
typedef struct SGenotickStartArgs TGenotickStartArgs;

struct SJavaVMOption
{
	const char* optionString ZERO_INIT;
	const void* extraInfo    ZERO_INIT;
};
typedef struct SJavaVMOption TJavaVMOption;

struct SGenotickCreationSettings
{
	const char*                 utf8jvmDllPath   ZERO_INIT; // mandatory path to jvm.dll
	const char*                 javaClassPath    ZERO_INIT; // optional [ex: "genotick.jar"]
	const char*                 javaDebugAddress ZERO_INIT; // optional [ex: "127.0.0.1:8888"]
	const struct SJavaVMOption* javaOptions      ZERO_INIT; // optional regular Java VM options
	TGenotickSize               javaOptionCount  ZERO_INIT;
};
typedef struct SGenotickCreationSettings TGenotickCreationSettings;


struct SGenotickTimePointsFunctions
{
	TGenotickBoolean          FPTR(FindIndex)(FTHISC(SGenotickTimePoints), const TGenotickTimePoint* timePoint, TGenotickSize* pIndex);
	const TGenotickTimePoint* FPTR(GetElement)(FTHISC(SGenotickTimePoints), TGenotickSize index);
	TGenotickSize             FPTR(GetElementCount)(FTHISC(SGenotickTimePoints));
	void                      FPTR(Release)(FTHIS(SGenotickTimePoints));
};
ITYPE(SGenotickTimePoints, IGenotickTimePoints)

struct SGenotickPredictionsFunctions
{
	EGenotickPrediction       FPTR(GetElement)(FTHISC(SGenotickPredictions), TGenotickSize index);
	TGenotickSize             FPTR(GetElementCount)(FTHISC(SGenotickPredictions));
	void                      FPTR(Release)(FTHIS(SGenotickPredictions));
};
ITYPE(SGenotickPredictions, IGenotickPredictions)

struct SGenotickFunctions
{
	TGenotickInt32  FPTR(GetInterfaceVersion)(FTHISC(SGenotick));
	EGenotickResult FPTR(CreateSession)(FTHIS(SGenotick), TGenotickSessionId sessionId);
	EGenotickResult FPTR(RemoveSession)(FTHIS(SGenotick), TGenotickSessionId sessionId);
	EGenotickResult FPTR(RemoveAllSessions)(FTHIS(SGenotick));
	EGenotickResult FPTR(GetSettings)(FTHISC(SGenotick), TGenotickSessionId sessionId, TGenotickMainSettings* pSettings);
	EGenotickResult FPTR(ChangeSettings)(FTHIS(SGenotick), TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings);
	EGenotickResult FPTR(SetAssetData)(FTHIS(SGenotick), TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData);
	EGenotickResult FPTR(Start)(FTHIS(SGenotick), TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs);
	EGenotickResult FPTR(GetTimePoints)(FTHISC(SGenotick), TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints);
	EGenotickResult FPTR(GetPredictions)(FTHISC(SGenotick), TGenotickSessionId sessionId, const char* assetName, IGenotickPredictions** ppPredictions);
	EGenotickResult FPTR(GetNewestTimePoint)(FTHISC(SGenotick), TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint);
	EGenotickResult FPTR(GetNewestPrediction)(FTHISC(SGenotick), TGenotickSessionId sessionId, const char* assetName, EGenotickPrediction* pPrediction);
	EGenotickResult FPTR(AttachCurrentThread)(FTHIS(SGenotick), TGenotickBoolean asDaemon);
	EGenotickResult FPTR(DetachCurrentThread)(FTHIS(SGenotick));
	EGenotickResult FPTR(Release)(FTHIS(SGenotick));
};
ITYPE(SGenotick, IGenotick)

struct SGenotickListFunctions
{
	IGenotick*    FPTR(GetElement)(FTHISC(SGenotickList), TGenotickSize index);
	TGenotickSize FPTR(GetElementCount)(FTHISC(SGenotickList));
	void          FPTR(Release)(FTHIS(SGenotickList));
};
ITYPE(SGenotickList, IGenotickList)

#ifdef __cplusplus
struct SGenotickTimePoints
{
	bool FindIndex(const TGenotickTimePoint* timePoint, TGenotickSize* index) const {
		return m_functions.FindIndex(this, timePoint, index) == GenotickTrue ? true : false;
	}
	const TGenotickTimePoint* GetElement(TGenotickSize index) const {
		return m_functions.GetElement(this, index);
	}
	TGenotickSize GetElementCount() const {
		return m_functions.GetElementCount(this);
	}
	void Release() {
		return m_functions.Release(this);
	}
protected:
	using TInterfaceFunctions = SGenotickTimePointsFunctions;
	const TInterfaceFunctions m_functions ZERO_INIT;
	~SGenotickTimePoints() {}
};

struct SGenotickPredictions
{
	EGenotickPrediction GetElement(TGenotickSize index) const {
		return m_functions.GetElement(this, index);
	}
	TGenotickSize GetElementCount() const {
		return m_functions.GetElementCount(this);
	}
	void Release() {
		return m_functions.Release(this);
	}
protected:
	using TInterfaceFunctions = SGenotickPredictionsFunctions;
	const TInterfaceFunctions m_functions ZERO_INIT;
	~SGenotickPredictions() {}
};

struct SGenotick
{
	TGenotickInt32 GetInterfaceVersion() const {
		return m_functions.GetInterfaceVersion(this);
	}
	EGenotickResult CreateSession(TGenotickSessionId sessionId) {
		return m_functions.CreateSession(this, sessionId);
	}
	EGenotickResult RemoveSession(TGenotickSessionId sessionId) {
		return m_functions.RemoveSession(this, sessionId);
	}
	EGenotickResult RemoveAllSessions() {
		return m_functions.RemoveAllSessions(this);
	}
	EGenotickResult GetSettings(TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) const {
		return m_functions.GetSettings(this, sessionId, pSettings);
	}
	EGenotickResult ChangeSettings(TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) {
		return m_functions.ChangeSettings(this, sessionId, pSettings);
	}
	EGenotickResult SetAssetData(TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData) {
		return m_functions.SetAssetData(this, sessionId, pAssetData);
	}
	EGenotickResult Start(TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) {
		return m_functions.Start(this, sessionId, pArgs);
	}
	EGenotickResult GetTimePoints(TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) const {
		return m_functions.GetTimePoints(this, sessionId, ppTimePoints);
	}
	EGenotickResult GetPredictions(TGenotickSessionId sessionId, const char* assetName, IGenotickPredictions** ppPredictions) const {
		return m_functions.GetPredictions(this, sessionId, assetName, ppPredictions);
	}
	EGenotickResult GetNewestTimePoint(TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) const {
		return m_functions.GetNewestTimePoint(this, sessionId, pTimePoint);
	}
	EGenotickResult GetNewestPrediction(TGenotickSessionId sessionId, const char* assetName, EGenotickPrediction* pPrediction) const {
		return m_functions.GetNewestPrediction(this, sessionId, assetName, pPrediction);
	}
	EGenotickResult AttachCurrentThread(bool asDaemon) {
		return m_functions.AttachCurrentThread(this, asDaemon ? GenotickTrue : GenotickFalse);
	}
	EGenotickResult DetachCurrentThread() {
		return m_functions.DetachCurrentThread(this);
	}
	EGenotickResult Release() {
		return m_functions.Release(this);
	}
protected:
	using TInterfaceFunctions = SGenotickFunctions;
	const TInterfaceFunctions m_functions ZERO_INIT;
	~SGenotick() {}
};

struct SGenotickList
{
	IGenotick* GetElement(TGenotickSize index) const {
		return m_functions.GetElement(this, index);
	}
	TGenotickSize GetElementCount() const {
		return m_functions.GetElementCount(this);
	}
	void Release() {
		return m_functions.Release(this);
	}
protected:
	using TInterfaceFunctions = SGenotickListFunctions;
	const TInterfaceFunctions m_functions ZERO_INIT;
	~SGenotickList() {}
};
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

// This function attempts to load a new Java VM and create a new Genotick instance.
// Unfortunately as of Java 8, JNI allows for one JavaVM instance per process only - ever.
// After releasing a JavaVM, you cannot even start a new one.
// So keep the returned instance sacred and use it until the process dies.
// Do not bother calling IGenotick::Release() at any time.
GENOTICK_IMPORT_EXPORT EGenotickResult GENOTICK_CALL GenotickCreate(IGenotick** ppInstance, const TGenotickCreationSettings* pSettings);

// If you use a platform where you cannot keep hold of a IGenotick instance
// from one test run to another while the process itself remains alive,
// then use this function to retrieve previously created Genotick instance(s).
GENOTICK_IMPORT_EXPORT EGenotickResult GENOTICK_CALL GenotickGetInstances(IGenotickList** ppInstances, const TGenotickCreationSettings* pSettings);

// Spawns a console in native code
GENOTICK_IMPORT_EXPORT EGenotickResult GENOTICK_CALL GenotickSpawnConsole();
GENOTICK_IMPORT_EXPORT EGenotickResult GENOTICK_CALL GenotickDestroyConsole();

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#undef ZERO_INIT
#undef FTHIS
#undef FTHISC
#undef FPTR
#undef ITYPE

#endif // I_GENOTICK_H_
