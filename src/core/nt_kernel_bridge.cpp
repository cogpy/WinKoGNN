/*++
 * nt_kernel_bridge.cpp — NT Kernel Bridge instantiation
 *--*/
#include "cogpy_bridge.hpp"
#include "nt_cognitive_kernel.hpp"

namespace winkognn {
    static const char* kernel_version = "NT4-compat-1.0";
    const char* get_kernel_version() { return kernel_version; }
}
