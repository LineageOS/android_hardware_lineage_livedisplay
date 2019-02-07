#include "ReadingEnhancement.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

// Methods from ::vendor::lineage::livedisplay::V2_0::IReadingEnhancement follow.
Return<bool> ReadingEnhancement::isEnabled() {
    // TODO implement
    return bool {};
}

Return<bool> ReadingEnhancement::setEnabled(bool enabled) {
    // TODO implement
    return bool {};
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IReadingEnhancement* HIDL_FETCH_IReadingEnhancement(const char* /* name */) {
    //return new ReadingEnhancement();
//}
//
}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
