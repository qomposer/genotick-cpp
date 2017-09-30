
#pragma once

#include <igenotick.h>
#include "igenotick_destructable.h"
#include <jni.h>
#include <Windows.h>
#include <vector>
#include <memory>

class CJavaLoader
{
public:
	CJavaLoader();
	~CJavaLoader();

	EGenotickResult LoadGenotick(IGenotick** ppInstance, const SGenotickJvmSettings* pSettings);

protected:
	EGenotickResult RemoveInstance(const IGenotick* pInstance, JavaVM& javaVM);

private:
	EGenotickResult LoadJvmModule(const char* path);
	void FreeJvmModule();
	EGenotickResult ReleaseAllJvmInstances();
	
	static std::string MakeJavaOptionString(const char* option, const char* value);
	inline bool JvmModuleLoaded() { return m_jvmModule != 0; }

	typedef jint (JNICALL *pJNI_GetDefaultJavaVMInitArgs)(void *args);
	typedef jint (JNICALL *pJNI_CreateJavaVM)(JavaVM **pvm, void **penv, void *args);
	typedef jint (JNICALL *pJNI_GetCreatedJavaVMs)(JavaVM **pvm, jsize size, jsize *psize);

	typedef std::unique_ptr<IGenotickDestructable> TGenotickPtr;
	typedef std::vector<TGenotickPtr> TGenotickPtrs;

	pJNI_GetDefaultJavaVMInitArgs JNI_GetDefaultJavaVMInitArgs_FuncPtr;
	pJNI_CreateJavaVM JNI_CreateJavaVM_FuncPtr;
	pJNI_GetCreatedJavaVMs JNI_CreatedJavaVMs_FuncPtr;

	HMODULE m_jvmModule;
	TGenotickPtrs m_instances;
};

class CJavaLoaderGenotick : public CJavaLoader
{
public:
	using CJavaLoader::RemoveInstance;
};
