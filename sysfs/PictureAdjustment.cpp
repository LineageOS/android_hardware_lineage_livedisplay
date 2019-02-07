#include "PictureAdjustment.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

// Methods from ::vendor::lineage::livedisplay::V2_0::IPictureAdjustment follow.
Return<void> PictureAdjustment::getHueRange(getHueRange_cb _hidl_cb) {
    // TODO implement
    return Void();
}

Return<void> PictureAdjustment::getSaturationRange(getSaturationRange_cb _hidl_cb) {
    // TODO implement
    return Void();
}

Return<void> PictureAdjustment::getIntensityRange(getIntensityRange_cb _hidl_cb) {
    // TODO implement
    return Void();
}

Return<void> PictureAdjustment::getContrastRange(getContrastRange_cb _hidl_cb) {
    // TODO implement
    return Void();
}

Return<void> PictureAdjustment::getSaturationThresholdRange(getSaturationThresholdRange_cb _hidl_cb) {
    // TODO implement
    return Void();
}

Return<void> PictureAdjustment::getPictureAdjustment(getPictureAdjustment_cb _hidl_cb) {
    // TODO implement
    return Void();
}

Return<void> PictureAdjustment::getDefaultPictureAdjustment(getDefaultPictureAdjustment_cb _hidl_cb) {
    // TODO implement
    return Void();
}

Return<bool> PictureAdjustment::setPictureAdjustment(const ::vendor::lineage::livedisplay::V2_0::HSIC& hsic) {
    // TODO implement
    return bool {};
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IPictureAdjustment* HIDL_FETCH_IPictureAdjustment(const char* /* name */) {
    //return new PictureAdjustment();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
