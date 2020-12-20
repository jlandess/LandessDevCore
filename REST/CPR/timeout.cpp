#include "timeout.h"

#include <limits>
#include <stdexcept>
#include <type_traits>
#include <string>

namespace cpr {

// No way around since curl uses a long here.
// NOLINTNEXTLINE(google-runtime-int)
long Timeout::Milliseconds() const {
    static_assert(std::is_same<std::chrono::milliseconds, decltype(ms)>::value,
                  "Following casting expects milliseconds.");

    // No way around since curl uses a long here.
    // NOLINTNEXTLINE(google-runtime-int)
    if (ms.count() > std::numeric_limits<long>::max()) {
        throw std::overflow_error(
                "cpr::Timeout: timeout value overflow: " + std::to_string(ms.count()) + " ms.");
    }
    // No way around since curl uses a long here.
    // NOLINTNEXTLINE(google-runtime-int)
    if (ms.count() < std::numeric_limits<long>::min()) {
        throw std::underflow_error(
                "cpr::Timeout: timeout value underflow: " + std::to_string(ms.count()) + " ms.");
    }

    // No way around since curl uses a long here.
    // NOLINTNEXTLINE(google-runtime-int)
    return static_cast<long>(ms.count());
}

} // namespace cpr