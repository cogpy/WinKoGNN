/*++
 * cog0_orchestrator.cpp — cog0 Orchestrator instantiation
 *--*/
#include "cog0_orchestrator.hpp"

namespace winkognn {
    static const char* orchestrator_version = "cog0-1.0";
    const char* get_orchestrator_version() { return orchestrator_version; }
}
