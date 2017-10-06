
#pragma once

#include "genotick.h"
#include "jni_genotick_interface.h"
#include "jni_genotick_settings.h"
#include "jni_genotick_timepoint.h"
#include "jni_genotick_weight_mode.h"
#include "jni_genotick_inherited_weight_mode.h"
#include "jni_genotick_chart_mode.h"
#include "jni_genotick_error.h"
#include "jni_exceptions.h"
#include "strlcpy.h"

class CJavaLoaderGenotick;
class CJavaLoader;

namespace jni {
namespace genotick {

class CGenotick : public IGenotickDestructable
{
public:
	CGenotick(CJavaLoader* pJavaLoader, jni::JavaVM* pJavaVM, jni::JNIEnv* pJavaEnv);
	virtual ~CGenotick();

private:
	static TGenotickInt32 GENOTICK_CALL GetInterfaceVersionThis(IGenotick* pThis) {
		return static_cast<const CGenotick*>(pThis)->GetInterfaceVersionInternal();
	};
	static EGenotickResult GENOTICK_CALL GetSettingsThis(IGenotick* pThis, SGenotickMainSettings* pSettings) {
		return static_cast<const CGenotick*>(pThis)->GetSettingsInternal(pSettings);
	};
	static EGenotickResult GENOTICK_CALL ChangeSettingsThis(IGenotick* pThis, const SGenotickMainSettings* pSettings) {
		return static_cast<const CGenotick*>(pThis)->ChangeSettingsInternal(pSettings);
	};
	static EGenotickResult GENOTICK_CALL StartThis(IGenotick* pThis, const SGenotickStartSettings* pSettings) {
		return static_cast<const CGenotick*>(pThis)->StartInternal(pSettings);
	};
	static EGenotickResult GENOTICK_CALL ReleaseThis(IGenotick* pThis) {
		return static_cast<const CGenotick*>(pThis)->ReleaseInternal();
	};
	
	TGenotickInt32 GetInterfaceVersionInternal() const;
	EGenotickResult GetSettingsInternal(SGenotickMainSettings* pSettings) const;
	EGenotickResult ChangeSettingsInternal(const SGenotickMainSettings* pSettings) const;
	EGenotickResult StartInternal(const SGenotickStartSettings* pSettings) const;
	EGenotickResult ReleaseInternal() const;

	template <class D, class S> void ToNative(D& dst, const S src) const {
		dst = static_cast<D>(src);
	}

	template <> void ToNative(TGenotickString& dst, const jni::String src) const {
		if (dst.capacity > 0u) {
			std::string buf = jni::Make<std::string>(m_javaEnv, src);
			strlcpy(dst.utf8_buffer, buf.c_str(), dst.capacity);
		}
	}

	template <> void ToNative(TGenotickTimePoint& dst, const jni::genotick::CTimePoint::TObject src) const {
		dst = static_cast<TGenotickTimePoint>(m_timePoint.getValue(src));
	}

	template <> void ToNative(EGenotickWeightMode& dst, const jni::genotick::CWeightMode::TObject src) const {
		dst = static_cast<EGenotickWeightMode>(m_weightMode.GetEnumValue(src));
	}

	template <> void ToNative(EGenotickInheritedWeightMode& dst, const jni::genotick::CInheritedWeightMode::TObject src) const {
		dst = static_cast<EGenotickInheritedWeightMode>(m_inheritedWeightMode.GetEnumValue(src));
	}

	template <> void ToNative(EGenotickChartMode& dst, const jni::genotick::CChartMode::TObject src) const {
		dst = static_cast<EGenotickChartMode>(m_chartMode.GetEnumValue(src));
	}

	template <class D, class S> D ToJava(const S src) const {
		return static_cast<D>(src);
	}

	template <> jni::String ToJava(const TGenotickString src) const {
		const std::string buf = (src.utf8_buffer != nullptr) ? src.utf8_buffer : "";
		return jni::Make<jni::String>(m_javaEnv, buf);
	}

	template <> jni::genotick::CTimePoint::TObject ToJava(const TGenotickTimePoint src) const {
		return m_timePoint.New(static_cast<jni::jlong>(src));
	}

	template <> jni::genotick::CWeightMode::TObject ToJava(const EGenotickWeightMode src) const {
		return m_weightMode.GetEnumObject(static_cast<jni::jint>(src));
	}

	template <> jni::genotick::CInheritedWeightMode::TObject ToJava(const EGenotickInheritedWeightMode src) const {
		return m_inheritedWeightMode.GetEnumObject(static_cast<jni::jint>(src));
	}

	template <> jni::genotick::CChartMode::TObject ToJava(const EGenotickChartMode src) const {
		return m_chartMode.GetEnumObject(static_cast<jni::jint>(src));
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
	jni::genotick::CErrorCode m_errorCode;
};

} // namespace genotick
} // namespace jni
