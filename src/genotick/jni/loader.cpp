
#include "loader.h"
#include <genotick/jni/genotick.h>
#include <genotick/jni/error.h>
#include <genotick/jni/exceptions.h>
#include <genotick/jni/genotick_list.h>
#include <utils_win.h>
#include <utf8.h>
#include <assert.h>
#include <console.h>
#include <vector>

namespace genotick {
namespace jni {

CLoader::CLoader()
	: JNI_GetDefaultJavaVMInitArgs_FuncPtr(nullptr)
	, JNI_CreateJavaVM_FuncPtr(nullptr)
	, JNI_CreatedJavaVMs_FuncPtr(nullptr)
	, m_jvmModule(0)
{
}

CLoader::~CLoader()
{
	// On destruction this class does not release any Java VM's automatically intentionally.
	// If the destruction is caused by a process shutdown, then the Java DLL's might have been unloaded already.
	// Or the Java VM's might be needed later if the process was not yet shutdown.
}

EGenotickResult CLoader::GenotickCreate(IGenotick** ppInstance, const TGenotickCreationSettings* pSettings)
{
	if (!ppInstance || !pSettings)
		return EGenotickResult::InvalidArgument;

	if (pSettings->javaOptionCount > 0 && !pSettings->javaOptions)
		return EGenotickResult::InvalidArgument;

	EGenotickResult result = LoadJvmModule(pSettings->utf8jvmDllPath);
	if (result != EGenotickResult::Success)
		return result;
	
	::std::string buffer1;
	::std::string buffer2;
	::std::vector<JavaVMOption> options;
	if (::utils::IsValidString(pSettings->javaClassPath))
	{
		buffer1 = ::stl::string_format("-Djava.class.path=%s", pSettings->javaClassPath);
		options.push_back({ const_cast<char*>(buffer1.c_str()), nullptr });
	}
	if (::utils::IsValidString(pSettings->javaDebugAddress))
	{
		buffer2 = ::stl::string_format("-agentlib:jdwp=transport=dt_socket,address=%s,server=y,suspend=y", pSettings->javaDebugAddress);
		options.push_back({ const_cast<char*>(buffer2.c_str()), nullptr });
	}
	const TGenotickSize additionalOptionCount = pSettings->javaOptionCount;
	for (TGenotickSize i = 0; i < additionalOptionCount; ++i)
	{
		const TJavaVMOption& option = pSettings->javaOptions[i];
		options.push_back({ const_cast<char*>(option.optionString), const_cast<void*>(option.extraInfo) });
	}

	JavaVM* pJavaVM = nullptr;
	JNIEnv* pJavaEnv = nullptr;
	JavaVMInitArgs vm_args = { 0 };
	vm_args.version = JNI_VERSION_1_8;
	vm_args.nOptions = static_cast<jint>(options.size());
	vm_args.options = options.data();
	vm_args.ignoreUnrecognized = false;

	jint jniResult = JNI_CreateJavaVM_FuncPtr(&pJavaVM, reinterpret_cast<void**>(&pJavaEnv), &vm_args);
	if (jniResult != JNI_OK)
		return ::genotick::jni::JniErrorToGenotickResult(jniResult);

	assert(pJavaVM != nullptr);
	assert(pJavaEnv != nullptr);
	result = AddNewInstanceFor(*pJavaVM);
	if (result != EGenotickResult::Success)
		return result;

	*ppInstance = m_instancePtrs.back().get();

	return EGenotickResult::Success;
}

EGenotickResult CLoader::GenotickGetInstances(IGenotickList** ppInstances, const TGenotickCreationSettings* pSettings)
{
	if (!ppInstances || !pSettings)
		return EGenotickResult::InvalidArgument;

	EGenotickResult result = LoadJvmModule(pSettings->utf8jvmDllPath);
	if (result != EGenotickResult::Success)
		return result;

	jsize javaVMCount = 0;
	jint jniResult = JNI_CreatedJavaVMs_FuncPtr(nullptr, 0, &javaVMCount);
	if (jniResult != JNI_OK)
		return ::genotick::jni::JniErrorToGenotickResult(jniResult);

	::std::unique_ptr<JavaVM*[]> javaVMs(new JavaVM*[javaVMCount]);
	JavaVM** ppJavaVMs = javaVMs.get();
	jniResult = JNI_CreatedJavaVMs_FuncPtr(ppJavaVMs, javaVMCount, &javaVMCount);
	if (jniResult != JNI_OK)
		return ::genotick::jni::JniErrorToGenotickResult(jniResult);

	for (jsize i = 0; i < javaVMCount; ++i)
	{
		JavaVM& javaVM = *ppJavaVMs[i];
		if (!HasInstanceFor(javaVM))
		{
			result = AddNewInstanceFor(javaVM);
			if (result != EGenotickResult::Success)
				return result;
		}
	}

	if (m_instancePtrs.size() == 0)
	{
		FreeJvmModule();
	}

	*ppInstances = CreateGenotickList();

	return EGenotickResult::Success;
}

EGenotickResult CLoader::GenotickSpawnConsole()
{
	return CConsole::Create() ? EGenotickResult::Success : EGenotickResult::Failure;
}

EGenotickResult CLoader::GenotickDestroyConsole()
{
	return CConsole::Destroy() ? EGenotickResult::Success : EGenotickResult::Failure;
}

IGenotickList* CLoader::CreateGenotickList()
{
	TGenotickSize size = static_cast<TGenotickSize>(m_instancePtrs.size());
	::genotick::jni::CGenotickList* pInstances = new ::genotick::jni::CGenotickList(size);

	for (TGenotickSize index = 0; index < size; ++index)
	{
		pInstances->Set(index, m_instancePtrs[index].get());
	}

	return pInstances;
}

EGenotickResult CLoader::AddNewInstanceFor(JavaVM& javaVM)
{
	JNIEnv& javaEnv = ::jni::GetEnv(javaVM, ::jni::version(JNI_VERSION_1_8));

	try
	{
		TGenotick* pNewInstance = new TGenotick(*this, javaVM, javaEnv);
		m_instancePtrs.push_back(TGenotickPtr(pNewInstance));
		return EGenotickResult::Success;
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return ::genotick::jni::HandleJavaException(javaEnv, exception);
	}
	catch (const ::genotick::jni::EnumMismatchException& exception)
	{
		return ::genotick::jni::HandleEnumMismatchException(exception);
	}
}

EGenotickResult CLoader::ReleaseInstanceFor(JavaVM& javaVM)
{
	TGenotickPtrsIterator instanceIter = FindInstanceFor(javaVM);

	if (instanceIter == m_instancePtrs.end())
		return EGenotickResult::InvalidArgument;

	m_instancePtrs.erase(instanceIter);
	const jint jniResult = javaVM.DestroyJavaVM();

	if (m_instancePtrs.empty())
	{
		FreeJvmModule();
	}
	
	return ::genotick::jni::JniErrorToGenotickResult(jniResult);
}

CLoader::TGenotickPtrsIterator CLoader::FindInstanceFor(JavaVM& javaVM)
{
	auto Compare = [&javaVM](TGenotickPtr& p) { return p->Contains(javaVM); };
	return ::std::find_if(m_instancePtrs.begin(), m_instancePtrs.end(), Compare);
}

bool CLoader::HasInstanceFor(JavaVM& javaVM)
{
	return FindInstanceFor(javaVM) != m_instancePtrs.end();
}

EGenotickResult CLoader::LoadJvmModule(const char* path)
{
	if (JvmModuleLoaded())
		return EGenotickResult::Success;

	const std::wstring wpath = utf8::to_ucs2(path);
	m_jvmModule = ::LoadLibraryW(wpath.c_str());
	if (!JvmModuleLoaded())
		return EGenotickResult::JvmDllNotFound;

	JNI_GetDefaultJavaVMInitArgs_FuncPtr = ::utils::GetProcAddressT<pJNI_GetDefaultJavaVMInitArgs>(m_jvmModule, "JNI_GetDefaultJavaVMInitArgs");
	JNI_CreateJavaVM_FuncPtr = ::utils::GetProcAddressT<pJNI_CreateJavaVM>(m_jvmModule, "JNI_CreateJavaVM");
	JNI_CreatedJavaVMs_FuncPtr = ::utils::GetProcAddressT<pJNI_GetCreatedJavaVMs>(m_jvmModule, "JNI_GetCreatedJavaVMs");

	const bool bValid = (JNI_GetDefaultJavaVMInitArgs_FuncPtr && JNI_CreateJavaVM_FuncPtr && JNI_CreatedJavaVMs_FuncPtr);
	if (!bValid)
	{
		FreeJvmModule();
		return EGenotickResult::JvmExportsNotFound;
	}
	return EGenotickResult::Success;
}

void CLoader::FreeJvmModule()
{
	if (m_jvmModule != 0)
	{
		::FreeLibrary(m_jvmModule);
		m_jvmModule = HMODULE(0);
		JNI_GetDefaultJavaVMInitArgs_FuncPtr = nullptr;
		JNI_CreateJavaVM_FuncPtr = nullptr;
		JNI_CreatedJavaVMs_FuncPtr = nullptr;
	}
}

} // namespace jni
} // namespace genotick
