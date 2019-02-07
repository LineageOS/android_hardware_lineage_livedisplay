#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V2_0_DISPLAYCOLORCALIBRATION_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V2_0_DISPLAYCOLORCALIBRATION_H

#include <vendor/lineage/livedisplay/2.0/IDisplayColorCalibration.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct DisplayColorCalibration : public IDisplayColorCalibration {
    // Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayColorCalibration follow.
    Return<int32_t> getMaxValue() override;
    Return<int32_t> getMinValue() override;
    Return<void> getCalibration(getCalibration_cb _hidl_cb) override;
    Return<bool> setCalibration(const hidl_vec<int32_t>& rgb) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IDisplayColorCalibration* HIDL_FETCH_IDisplayColorCalibration(const char* name);

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V2_0_DISPLAYCOLORCALIBRATION_H
