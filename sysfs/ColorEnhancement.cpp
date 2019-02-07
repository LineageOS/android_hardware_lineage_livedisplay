#include "ColorEnhancement.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

// Methods from ::vendor::lineage::livedisplay::V2_0::IColorEnhancement follow.
Return<bool> ColorEnhancement::isEnabled() {
    // TODO implement
    return bool {};
}

Return<bool> ColorEnhancement::setEnabled(bool enabled) {
    // TODO implement
    return bool {};
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IColorEnhancement* HIDL_FETCH_IColorEnhancement(const char* /* name */) {
    //return new ColorEnhancement();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
