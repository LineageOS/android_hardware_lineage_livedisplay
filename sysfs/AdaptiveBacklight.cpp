#include "AdaptiveBacklight.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

// Methods from ::vendor::lineage::livedisplay::V2_0::IAdaptiveBacklight follow.
Return<bool> AdaptiveBacklight::isEnabled() {
    // TODO implement
    return bool {};
}

Return<bool> AdaptiveBacklight::setEnabled(bool enabled) {
    // TODO implement
    return bool {};
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IAdaptiveBacklight* HIDL_FETCH_IAdaptiveBacklight(const char* /* name */) {
    //return new AdaptiveBacklight();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
