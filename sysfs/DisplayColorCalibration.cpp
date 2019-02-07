#include "DisplayColorCalibration.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

// Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayColorCalibration follow.
Return<int32_t> DisplayColorCalibration::getMaxValue() {
    // TODO implement
    return int32_t {};
}

Return<int32_t> DisplayColorCalibration::getMinValue() {
    // TODO implement
    return int32_t {};
}

Return<void> DisplayColorCalibration::getCalibration(getCalibration_cb _hidl_cb) {
    // TODO implement
    return Void();
}

Return<bool> DisplayColorCalibration::setCalibration(const hidl_vec<int32_t>& rgb) {
    // TODO implement
    return bool {};
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IDisplayColorCalibration* HIDL_FETCH_IDisplayColorCalibration(const char* /* name */) {
    //return new DisplayColorCalibration();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
