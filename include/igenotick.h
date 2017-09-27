// Author: pascalx
// Genotick interface for C & C++
// Supports lib, dll, Win32, x64

#ifndef _I_GENOTICK_H_
#define _I_GENOTICK_H_

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

typedef long jint_t;

struct IGenotickFunctions_;
struct IGenotick_;

#ifdef __cplusplus
typedef IGenotick_ IGenotick;
#else
typedef const struct IGenotickFunctions_* IGenotick;
#endif

typedef enum
{
	eGenotickResult_Success,
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
	eGenotickResult_JavaException,
	eGenotickResult_ErrorContinue,
	eGenotickResult_ErrorNoError,
	eGenotickResult_ErrorNoInput,
	eGenotickResult_ErrorNoOutput,
	eGenotickResult_ErrorUnknownArgument,
	eGenotickResult_ErrorUnknown,
} EGenotickResult;

typedef struct
{
	const char** parameters;
	unsigned int parameterCount;
} SGenotickStartSettings;

struct IGenotickFunctions_
{
	jint_t(GENOTICK_CALL* GetInterfaceVersion)(IGenotick* pThis);
	EGenotickResult(GENOTICK_CALL* Start)(IGenotick* pThis, const SGenotickStartSettings* pSettings);
	EGenotickResult(GENOTICK_CALL* Release)(IGenotick* pThis);
};

struct IGenotick_
{
	const struct IGenotickFunctions_* functions;
#ifdef __cplusplus
	jint_t GetInterfaceVersion() {
		return functions->GetInterfaceVersion(this);
	}
	EGenotickResult Start(const SGenotickStartSettings* pSettings) {
		return functions->Start(this, pSettings);
	}
	EGenotickResult Release() {
		return functions->Release(this);
	}
protected:
	IGenotick_() {}
	virtual ~IGenotick_() {}
#endif
};

typedef struct
{
	const wchar_t* jvmDllPath;
	const char* javaClassPath;
} SGenotickJvmSettings;

// Unfortunately as of Java 8, JNI allows for one JavaVM instance per process only - ever.
// After releasing a JavaVM, you cannot even start a new one (shame).
// So keep the returned instance sacred and use it until the process dies.
GENOTICK_IMPORT_OR_EXPORT EGenotickResult GENOTICK_CALL LoadGenotick(IGenotick** pInstance, const SGenotickJvmSettings* pSettings);

#ifdef __cplusplus
}
#endif

#endif // _I_GENOTICK_H_
