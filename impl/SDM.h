#ifndef CYNGN_LIVEDISPLAYSDM_H
#define CYNGN_LIVEDISPLAYSDM_H

#include <LiveDisplayBackend.h>
#include <vendor/lineage/livedisplay/1.0/ILiveDisplay.h>

#define DPPS_BUF_SIZE 64

#define FOSS_SUPPORTED "foss:support"
#define FOSS_ON "foss:on"
#define FOSS_OFF "foss:off"
#define FOSS_STATUS "foss:status"

// For use when only sysfs modes are available
#define STANDARD_NODE_ID 600

#define SRGB_NODE "/sys/class/graphics/fb0/srgb"
#define SRGB_NODE_ID 601

#define DCI_P3_NODE "/sys/class/graphics/fb0/dci_p3"
#define DCI_P3_NODE_ID 602

#define PRIV_MODE_FLAG_SDM 1
#define PRIV_MODE_FLAG_SYSFS 2

namespace android {

class SDM : public LiveDisplayBackend {
  public:
    virtual status_t initialize();
    virtual status_t deinitialize();
    virtual bool hasFeature(Feature feature);

    virtual status_t setAdaptiveBacklightEnabled(bool enabled);
    virtual bool isAdaptiveBacklightEnabled();

    virtual status_t setOutdoorModeEnabled(bool /* enabled */) {
        return NO_INIT;
    }
    virtual bool isOutdoorModeEnabled() {
        return false;
    }

    virtual status_t getColorBalanceRange(Range& range);
    virtual status_t setColorBalance(int32_t balance);
    virtual int32_t getColorBalance();

    virtual status_t getDisplayModes(List<sp<DisplayMode>>& profiles);
    virtual status_t setDisplayMode(int32_t modeID, bool makeDefault);
    virtual sp<DisplayMode> getCurrentDisplayMode();
    virtual sp<DisplayMode> getDefaultDisplayMode();

    virtual status_t getPictureAdjustmentRanges(HSICRanges& ranges);
    virtual status_t getPictureAdjustment(HSIC& hsic);
    virtual status_t getDefaultPictureAdjustment(HSIC& hsic);
    virtual status_t setPictureAdjustment(HSIC hsic);

  private:
    status_t loadHALService();
    sp<DisplayMode> getLocalSRGBMode();
    sp<DisplayMode> getLocalDCIP3Mode();
    sp<DisplayMode> getDisplayModeById(int32_t id);
    status_t setModeState(sp<DisplayMode> mode, bool state);
    status_t saveInitialDisplayMode();
    uint32_t getNumDisplayModes();
    uint32_t getNumSDMDisplayModes();

    int64_t mHandle;
    bool mCachedFOSSStatus;
    int32_t mActiveModeId;

    HSIC mDefaultPictureAdjustment;

    sp<::vendor::lineage::livedisplay::V1_0::ILiveDisplay> intf;
};
};

#endif
