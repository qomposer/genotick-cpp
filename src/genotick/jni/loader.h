
#pragma once

#include <genotick/jni/genotick.h>
#include <jni.h>
#include <Windows.h>
#include <vector>
#include <memory>

namespace genotick {
namespace jni {

using TGenotick = CGenotickFunctions<CGenotick>;

class CLoader
{
private:
	using TJNI_GetDefaultJavaVMInitArgs = jint (JNICALL*)(void* args);
	using TJNI_CreateJavaVM = jint (JNICALL*)(JavaVM** pvm, void** penv, void* args);
	using TJNI_GetCreatedJavaVMs = jint (JNICALL*)(JavaVM** pvm, jsize size, jsize* psize);

	using TGenotickPtr = std::unique_ptr<TGenotick>;
	using TGenotickPtrs = std::vector<TGenotickPtr>;
	using TGenotickPtrsIterator = typename TGenotickPtrs::iterator;

public:
	CLoader();
	~CLoader();

	EGenotickResult GenotickCreate(IGenotick** ppInstance, const SGenotickCreationSettings* pSettings);
	EGenotickResult GenotickGetInstances(IGenotickList** ppInstances, const TGenotickCreationSettings* pSettings);
	EGenotickResult GenotickSpawnConsole();
	EGenotickResult GenotickDestroyConsole();

protected:
	EGenotickResult ReleaseInstanceFor(JavaVM& javaVM);

private:
	EGenotickResult       LoadJvmModule(const char* path);
	void                  FreeJvmModule();
	IGenotickList*        CreateGenotickList();
	EGenotickResult       AddNewInstanceFor(JavaVM& javaVM);
	TGenotickPtrsIterator FindInstanceFor(JavaVM& javaVM);
	bool                  HasInstanceFor(JavaVM& javaVM);
	
	inline bool JvmModuleLoaded() { return m_jvmModule != 0; }

	TJNI_GetDefaultJavaVMInitArgs JNI_GetDefaultJavaVMInitArgs_FuncPtr;
	TJNI_CreateJavaVM JNI_CreateJavaVM_FuncPtr;
	TJNI_GetCreatedJavaVMs JNI_CreatedJavaVMs_FuncPtr;

	HMODULE m_jvmModule;
	TGenotickPtrs m_instancePtrs;
};

class CLoaderFriend : public CLoader
{
public:
	using CLoader::ReleaseInstanceFor;
};

} // namespace jni
} // namespace genotick
