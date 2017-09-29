
#pragma once

#include <igenotick.h>
#include "igenotick_destructable.h"
#include "jni_genotick_interface.h"
#include "jni_genotick_settings.h"
#include "jni_genotick_timepoint.h"
#include "jni_genotick_weight_mode.h"
#include "jni_genotick_inherited_weight_mode.h"
#include "jni_genotick_chart_mode.h"
#include "strlcpy.h"
#include <exception>

class CJavaLoaderGenotick;
class CJavaLoader;

namespace jni {
namespace genotick {

class EnumMismatchException : public std::exception
{
public:
	explicit EnumMismatchException(char const* const message)
		: std::exception(message)
	{}
};

class CGenotick : public IGenotickDestructable
{
public:
	// Constructor can throw jni::PendingJavaException
	CGenotick(CJavaLoader* pJavaLoader, jni::JavaVM* pJavaVM, jni::JNIEnv* pJavaEnv);
	virtual ~CGenotick();

private:
	static TGenotickInt32 GENOTICK_CALL GetInterfaceVersionThis(IGenotick* pThis) {
		return static_cast<CGenotick*>(pThis)->GetInterfaceVersionInternal();
	};
	static EGenotickResult GENOTICK_CALL GetSettingsThis(IGenotick* pThis, SGenotickMainSettings* pSettings) {
		return static_cast<CGenotick*>(pThis)->GetSettingsInternal(pSettings);
	};
	static EGenotickResult GENOTICK_CALL ChangeSettingsThis(IGenotick* pThis, const SGenotickMainSettings* pSettings) {
		return static_cast<CGenotick*>(pThis)->ChangeSettingsInternal(pSettings);
	};
	static EGenotickResult GENOTICK_CALL StartThis(IGenotick* pThis, const SGenotickStartSettings* pSettings) {
		return static_cast<CGenotick*>(pThis)->StartInternal(pSettings);
	};
	static EGenotickResult GENOTICK_CALL ReleaseThis(IGenotick* pThis) {
		return static_cast<CGenotick*>(pThis)->ReleaseInternal();
	};
	
	TGenotickInt32 GetInterfaceVersionInternal();
	EGenotickResult GetSettingsInternal(SGenotickMainSettings* pSettings);
	EGenotickResult ChangeSettingsInternal(const SGenotickMainSettings* pSettings);
	EGenotickResult StartInternal(const SGenotickStartSettings* pSettings);
	EGenotickResult ReleaseInternal();

	EGenotickResult HandleJavaException();
	EGenotickResult HandleEnumMismatchException();

	template <class D, class S> void ToNative(D& dst, const S src) {
		dst = static_cast<D>(src);
	}

	template <> void ToNative(TGenotickString& dst, const jni::String src) {
		if (dst.capacity > 0u) {
			std::string buf = jni::Make<std::string>(m_javaEnv, src);
			strlcpy(dst.utf8_buffer, buf.c_str(), dst.capacity);
		}
	}

	template <> void ToNative(TGenotickTimePoint& dst, const jni::genotick::CTimePoint::TObject src) {
		dst = static_cast<TGenotickTimePoint>(m_timePoint.getValue(src));
	}

	template <> void ToNative(EGenotickWeightMode& dst, const jni::genotick::CWeightMode::TObject src) {
		dst = static_cast<EGenotickWeightMode>(m_weightMode.ordinal(src));
	}

	template <> void ToNative(EGenotickInheritedWeightMode& dst, const jni::genotick::CInheritedWeightMode::TObject src) {
		dst = static_cast<EGenotickInheritedWeightMode>(m_inheritedWeightMode.ordinal(src));
	}

	template <> void ToNative(EGenotickChartMode& dst, const jni::genotick::CChartMode::TObject src) {
		dst = static_cast<EGenotickChartMode>(m_chartMode.value(src));
	}

	template <class D, class S> D ToJava(const S src) {
		return static_cast<D>(src);
	}

	template <> jni::String ToJava(const TGenotickString src) {
		const std::string buf = (src.utf8_buffer != nullptr) ? src.utf8_buffer : "";
		return jni::Make<jni::String>(m_javaEnv, buf);
	}

	template <> jni::genotick::CTimePoint::TObject ToJava(const TGenotickTimePoint src) {
		return m_timePoint.New(static_cast<jni::jlong>(src));
	}

	template <> jni::genotick::CWeightMode::TObject ToJava(const EGenotickWeightMode src) {
		const auto enumValues = m_weightMode.values();
		const auto index = static_cast<jni::jsize>(src);
		const auto length = enumValues.Length(m_javaEnv);
		if (index < length) {
			return enumValues.Get(m_javaEnv, index);
		}
		throw EnumMismatchException("Mismatch");
	}

	template <> jni::genotick::CInheritedWeightMode::TObject ToJava(const EGenotickInheritedWeightMode src) {
		const auto enumValues = m_inheritedWeightMode.values();
		const auto index = static_cast<jni::jsize>(src);
		const auto length = enumValues.Length(m_javaEnv);
		if (index < length) {
			return enumValues.Get(m_javaEnv, index);
		}
		throw EnumMismatchException("Mismatch");
	}

	template <> jni::genotick::CChartMode::TObject ToJava(const EGenotickChartMode src) {
		const jni::genotick::CChartMode::TObjectArray enumValues = m_chartMode.values();
		const jni::jsize length = enumValues.Length(m_javaEnv);
		const jni::jint expectedValue = static_cast<jni::jint>(src);
		for (jni::jsize i = 0; i < length; ++i) {
			const jni::genotick::CChartMode::TObject enumValue = enumValues.Get(m_javaEnv, i);
			const jni::jint givenValue = m_chartMode.value(enumValue);
			if (expectedValue == givenValue) {
				return enumValue;
			}
		}
		throw EnumMismatchException("Mismatch");
	}

	CJavaLoaderGenotick& m_javaLoader;
	JavaVM& m_javaVM;
	JNIEnv& m_javaEnv;
	
	jni::UniqueStringClass m_stringClass;
	jni::genotick::CMainInterface m_mainInterface;
	jni::genotick::CMainSettings m_mainSettings;
	jni::genotick::CTimePoint m_timePoint;
	jni::genotick::CWeightMode m_weightMode;
	jni::genotick::CInheritedWeightMode m_inheritedWeightMode;
	jni::genotick::CChartMode m_chartMode;
};

} // namespace genotick
} // namespace jni
