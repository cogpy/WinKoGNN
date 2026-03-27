/*++
 * main.cpp — WinKoGNN AGI-OS Command-Line Interface
 *
 * Interactive REPL for the unified AGI-OS system.
 * Supports cog0 script commands and interactive exploration.
 *--*/

#include "cog0_orchestrator.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    using namespace winkognn::orchestrator;

    std::cout << "WinKoGNN Unified AGI-OS v1.0.0\n";
    std::cout << "cogpy-hpp ⊗ ATenSpace ⊗ NT Kernel ⊗ cog0\n";
    std::cout << "Type 'help' for commands, 'quit' to exit.\n\n";

    Cog0Orchestrator orc;

    /* If script file provided, execute it */
    if (argc > 1) {
        FILE* f = fopen(argv[1], "r");
        if (f) {
            std::string script;
            char buf[1024];
            while (fgets(buf, sizeof(buf), f)) {
                script += buf;
            }
            fclose(f);
            std::cout << orc.execute_script(script);
            return 0;
        }
    }

    /* Interactive REPL */
    std::string line;
    while (true) {
        std::cout << "agios> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "quit" || line == "exit") break;
        if (line.empty()) continue;

        std::string output = orc.execute_script(line + "\n");
        if (!output.empty()) {
            std::cout << output;
        }
    }

    std::cout << "\nFinal status:\n" << orc.status_string();
    return 0;
}
