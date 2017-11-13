
#pragma once

#include <genotick/interface.h>
#include <jni.h>
#include <Windows.h>
#include <vector>
#include <memory>

namespace genotick {
namespace jni {

class CLoader
{
private:
	typedef jint (JNICALL* pJNI_GetDefaultJavaVMInitArgs)(void* args);
	typedef jint (JNICALL* pJNI_CreateJavaVM)(JavaVM** pvm, void** penv, void* args);
	typedef jint (JNICALL* pJNI_GetCreatedJavaVMs)(JavaVM** pvm, jsize size, jsize* psize);

	typedef ::std::unique_ptr<IGenotickDestructable> TGenotickPtr;
	typedef ::std::vector<TGenotickPtr> TGenotickPtrs;
	typedef typename TGenotickPtrs::iterator TGenotickPtrsIterator;

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

	pJNI_GetDefaultJavaVMInitArgs JNI_GetDefaultJavaVMInitArgs_FuncPtr;
	pJNI_CreateJavaVM JNI_CreateJavaVM_FuncPtr;
	pJNI_GetCreatedJavaVMs JNI_CreatedJavaVMs_FuncPtr;

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
