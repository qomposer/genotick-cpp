
#include "java_loader.h"
#include "jni_genotick.h"
#include "jni_error.h"
#include "utils.h"
#include "utf8.h"
#include <array>
#include <assert.h>

namespace
{
	template<typename F>
	F GetProcAddressT(HMODULE hmod, const char* name)
	{
		return reinterpret_cast<F>(::GetProcAddress(hmod, name));
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
		return EGenotickResult::InvalidArgument;

	EGenotickResult result = LoadJvmModule(pSettings->utf8_jvmDllPath);
	if (result == EGenotickResult::Success)
	{
		const std::string javaClassPathOption = MakeJavaOptionString("-Djava.class.path", pSettings->utf8_javaClassPath);

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

		if (result == EGenotickResult::Success)
		{
			assert(pJvm);
			assert(pEnv);
			try
			{
				jni::genotick::CGenotick* pNewInstance = new jni::genotick::CGenotick(this, pJvm, pEnv);
				m_instances.push_back(TGenotickPtr(pNewInstance));
				*ppInstance = pNewInstance;
			}
			catch (const jni::PendingJavaException& exception)
			{
				result = jni::HandleJavaException(*pEnv, exception);
			}
			catch (const jni::EnumMismatchException& exception)
			{
				result = jni::HandleEnumMismatchException(exception);
			}
		}
	}
	return result;
}

EGenotickResult CJavaLoader::RemoveInstance(const IGenotick* pInstance, JavaVM& javaVM)
{
	auto predicate = [pInstance](TGenotickPtr& p) { return p.get() == pInstance; };
	stl::find_and_erase_if(m_instances, predicate);
	const jint jniResult = javaVM.DestroyJavaVM();

	if (m_instances.empty())
	{
		FreeJvmModule();
	}

	return jni::JniErrorToGenotickResult(jniResult);
}

EGenotickResult CJavaLoader::LoadJvmModule(const char* path)
{
	if (JvmModuleLoaded())
		return EGenotickResult::Success;

	const std::wstring wpath = utf8::to_ucs2(path);
	m_jvmModule = ::LoadLibraryW(wpath.c_str());

	if (!JvmModuleLoaded())
		return EGenotickResult::JvmDllNotFound;

	JNI_GetDefaultJavaVMInitArgs_FuncPtr = GetProcAddressT<pJNI_GetDefaultJavaVMInitArgs>(m_jvmModule, "JNI_GetDefaultJavaVMInitArgs");
	JNI_CreateJavaVM_FuncPtr = GetProcAddressT<pJNI_CreateJavaVM>(m_jvmModule, "JNI_CreateJavaVM");
	JNI_CreatedJavaVMs_FuncPtr = GetProcAddressT<pJNI_GetCreatedJavaVMs>(m_jvmModule, "JNI_GetCreatedJavaVMs");

	const bool bValid = (JNI_GetDefaultJavaVMInitArgs_FuncPtr && JNI_CreateJavaVM_FuncPtr && JNI_CreatedJavaVMs_FuncPtr);
	if (!bValid)
	{
		FreeJvmModule();
		return EGenotickResult::JvmExportsNotFound;
	}
	return EGenotickResult::Success;
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
	EGenotickResult result = EGenotickResult::Success;
	while (!m_instances.empty() && (result == EGenotickResult::Success))
	{
		IGenotick* pInstance = m_instances.back().get();
		result = pInstance->Release();
	}
	return result;
}

std::string CJavaLoader::MakeJavaOptionString(const char* option, const char* value)
{
	return ::stl::string_format("%s=%s", option, value);
}
