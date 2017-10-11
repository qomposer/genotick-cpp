
#pragma once

#include "genotick.h"
#include "jni_genotick_interface.h"
#include "jni_genotick_settings.h"
#include "jni_genotick_data_lines.h"
#include "jni_genotick_main_app_data.h"
#include "jni_genotick_timepoint.h"
#include "jni_genotick_timepoints.h"
#include "jni_genotick_weight_mode.h"
#include "jni_genotick_inherited_weight_mode.h"
#include "jni_genotick_chart_mode.h"
#include "jni_genotick_error_code.h"
#include "jni_genotick_prediction.h"
#include "jni_genotick_predictions.h"
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
	static EGenotickResult GENOTICK_CALL GetSettingsThis(IGenotick* pThis, TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) {
		return static_cast<const CGenotick*>(pThis)->GetSettingsInternal(sessionId, pSettings);
	};
	static EGenotickResult GENOTICK_CALL ChangeSettingsThis(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) {
		return static_cast<const CGenotick*>(pThis)->ChangeSettingsInternal(sessionId, pSettings);
	};
	static EGenotickResult GENOTICK_CALL StartThis(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) {
		return static_cast<const CGenotick*>(pThis)->StartInternal(sessionId, pArgs);
	};
	static EGenotickResult GENOTICK_CALL ReleaseThis(IGenotick* pThis) {
		return static_cast<const CGenotick*>(pThis)->ReleaseInternal();
	};
	
	TGenotickInt32 GetInterfaceVersionInternal() const;
	EGenotickResult GetSettingsInternal(TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) const;
	EGenotickResult ChangeSettingsInternal(TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) const;
	EGenotickResult StartInternal(TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) const;
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
		dst = EGenotickWeightMode::getByValue(m_weightMode.GetEnumValue(src));
	}

	template <> void ToNative(EGenotickInheritedWeightMode& dst, const jni::genotick::CInheritedWeightMode::TObject src) const {
		dst = EGenotickInheritedWeightMode::getByValue(m_inheritedWeightMode.GetEnumValue(src));
	}

	template <> void ToNative(EGenotickChartMode& dst, const jni::genotick::CChartMode::TObject src) const {
		dst = EGenotickChartMode::getByValue(m_chartMode.GetEnumValue(src));
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
		return m_weightMode.GetEnumObject(src.value());
	}

	template <> jni::genotick::CInheritedWeightMode::TObject ToJava(const EGenotickInheritedWeightMode src) const {
		return m_inheritedWeightMode.GetEnumObject(src.value());
	}

	template <> jni::genotick::CChartMode::TObject ToJava(const EGenotickChartMode src) const {
		return m_chartMode.GetEnumObject(src.value());
	}

	CJavaLoaderGenotick& m_javaLoader;
	JavaVM& m_javaVM;
	JNIEnv& m_javaEnv;
	
	jni::UniqueStringClass m_stringClass;
	jni::genotick::CMainInterface m_mainInterface;
	jni::genotick::CMainSettings m_mainSettings;
	jni::genotick::CDataLines m_dataLines;
	jni::genotick::CMainAppData m_mainAppData;
	jni::genotick::CTimePoint m_timePoint;
	jni::genotick::CTimePoints m_timePoints;
	jni::genotick::CWeightMode m_weightMode;
	jni::genotick::CInheritedWeightMode m_inheritedWeightMode;
	jni::genotick::CChartMode m_chartMode;
	jni::genotick::CErrorCode m_errorCode;
	jni::genotick::CPrediction m_prediction;
	jni::genotick::CPredictions m_predictions;
};

} // namespace genotick
} // namespace jni
