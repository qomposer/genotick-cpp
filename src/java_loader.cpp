
#include "java_loader.h"
#include "jni_genotick.h"
#include "jni_error.h"
#include "utils.h"
#include <string>
#include <array>
#include <assert.h>
#include <filesystem>

#define MISMATCH_MESSAGE "Mismatching Data Type Found"
static_assert(sizeof(TGenotickInt32)   == sizeof(jni::jint)    , MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickInt64)   == sizeof(jni::jlong)   , MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickByte)    == sizeof(jni::jbyte)   , MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickBoolean) == sizeof(jni::jboolean), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickChar)    == sizeof(jni::jchar)   , MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickShort)   == sizeof(jni::jshort)  , MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickFloat)   == sizeof(jni::jfloat)  , MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickDouble)  == sizeof(jni::jdouble) , MISMATCH_MESSAGE);
#undef MISMATCH_MESSAGE

namespace
{
	template<typename F>
	F GetProcAddressT(HMODULE hmod, const char* name)
	{
		return reinterpret_cast<F>(::GetProcAddress(hmod, name));
	}

	std::string GetJavaOptionString(const char* option, const char* value)
	{
		return ::stl::string_format("%s=%s", option, value);
	}
}

CJavaLoader::CJavaLoader()
	: JNI_GetDefaultJavaVMInitArgs_FuncPtr(nullptr)
	, JNI_CreateJavaVM_FuncPtr(nullptr)
	, JNI_CreatedJavaVMs_FuncPtr(nullptr)
	, m_jvmModule(0)
{
}

CJavaLoader::~CJavaLoader()
{
	ReleaseAllJvmInstances();
}

EGenotickResult CJavaLoader::LoadGenotick(IGenotick** ppInstance, const SGenotickJvmSettings* pSettings)
{
	if (!ppInstance || !pSettings)
		return eGenotickResult_InvalidArgument;

	EGenotickResult result = LoadJvmModule(pSettings->jvmDllPath);
	if (result == eGenotickResult_Success)
	{
		const std::string javaClassPathOption = GetJavaOptionString("-Djava.class.path", pSettings->javaClassPath);

		JavaVM* pJvm = nullptr;
		JNIEnv* pEnv = nullptr;
		JavaVMInitArgs vm_args = { 0 };
		JavaVMOption options[] = { const_cast<char*>(javaClassPathOption.c_str()) };
		vm_args.version = JNI_VERSION_1_8;
		vm_args.nOptions = utils::GetArrayLength(options);
		vm_args.options = options;
		vm_args.ignoreUnrecognized = false;

		const jni::jint jniResult = JNI_CreateJavaVM_FuncPtr(&pJvm, reinterpret_cast<void**>(&pEnv), &vm_args);
		result = jni::JniErrorToGenotickResult(jniResult);

		if (result == eGenotickResult_Success)
		{
			assert(pJvm);
			assert(pEnv);
			try
			{
				jni::genotick::CGenotick* pNewInstance = new jni::genotick::CGenotick(this, pJvm, pEnv);
				m_instances.push_back(TGenotickPtr(pNewInstance));
				*ppInstance = pNewInstance;
			}
			catch (jni::PendingJavaException)
			{
				jni::ExceptionDescribe(*pEnv);
				jni::ExceptionClear(*pEnv);
				result = eGenotickResult_JavaClassMismatch;
			}
		}
	}
	return result;
}

void CJavaLoader::RemoveInstance(const IGenotick* pInstance)
{
	auto predicate = [pInstance](TGenotickPtr& p) { return p.get() == pInstance; };
	stl::find_and_erase_if(m_instances, predicate);

	if (m_instances.empty())
	{
		FreeJvmModule();
	}
}

EGenotickResult CJavaLoader::LoadJvmModule(const wchar_t* jvmDllPath)
{
	if (JvmModuleLoaded())
		return eGenotickResult_Success;

	m_jvmModule = ::LoadLibraryW(jvmDllPath);

	if (!JvmModuleLoaded())
		return eGenotickResult_JvmDllNotFound;

	JNI_GetDefaultJavaVMInitArgs_FuncPtr = GetProcAddressT<pJNI_GetDefaultJavaVMInitArgs>(m_jvmModule, "JNI_GetDefaultJavaVMInitArgs");
	JNI_CreateJavaVM_FuncPtr = GetProcAddressT<pJNI_CreateJavaVM>(m_jvmModule, "JNI_CreateJavaVM");
	JNI_CreatedJavaVMs_FuncPtr = GetProcAddressT<pJNI_GetCreatedJavaVMs>(m_jvmModule, "JNI_GetCreatedJavaVMs");

	const bool bValid = (JNI_GetDefaultJavaVMInitArgs_FuncPtr && JNI_CreateJavaVM_FuncPtr && JNI_CreatedJavaVMs_FuncPtr);
	if (!bValid)
	{
		FreeJvmModule();
		return eGenotickResult_JvmExportsNotFound;
	}
	return eGenotickResult_Success;
}

void CJavaLoader::FreeJvmModule()
{
	if (m_jvmModule != 0)
	{
		::FreeLibrary(m_jvmModule);
		m_jvmModule = 0;
		JNI_GetDefaultJavaVMInitArgs_FuncPtr = nullptr;
		JNI_CreateJavaVM_FuncPtr = nullptr;
		JNI_CreatedJavaVMs_FuncPtr = nullptr;
	}
}

EGenotickResult CJavaLoader::ReleaseAllJvmInstances()
{
	EGenotickResult result = eGenotickResult_Success;
	while (!m_instances.empty())
	{
		IGenotick* pInstance = m_instances.back().get();
		result = pInstance->Release();
		assert(result == eGenotickResult_Success);
	}
	return jni::JniErrorToGenotickResult(result);
}
