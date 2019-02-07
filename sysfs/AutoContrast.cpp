#include "AutoContrast.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

// Methods from ::vendor::lineage::livedisplay::V2_0::IAutoContrast follow.
Return<bool> AutoContrast::isEnabled() {
    // TODO implement
    return bool {};
}

Return<bool> AutoContrast::setEnabled(bool enabled) {
    // TODO implement
    return bool {};
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IAutoContrast* HIDL_FETCH_IAutoContrast(const char* /* name */) {
    //return new AutoContrast();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
