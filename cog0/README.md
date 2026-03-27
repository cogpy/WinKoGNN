# cog0

**Source:** https://github.com/ReZorg/cog0  
**Status:** pending — repository is private and could not be cloned automatically.

## How to integrate

1. Obtain access to `ReZorg/cog0` and clone it here:
   ```
   git clone https://github.com/ReZorg/cog0.git cog0_tmp
   cp -r cog0_tmp/* cog0/
   rm -rf cog0_tmp cog0/.git
   ```

2. Enable the CMake target from the repo root:
   ```
   cmake -DBUILD_COG0=ON ..
   ```

## Expected integration mapping

Once the source tree is available, the C/C++ files from cog0 are expected
to fit into WinKoGNN as follows:

| cog0 path           | Role in WinKoGNN                                          |
|---------------------|-----------------------------------------------------------|
| `include/*.h`       | Public headers → exposed via `cog0_headers` INTERFACE lib |
| `src/*.c` / `*.cpp` | Implementation → compiled into `cog0` OBJECT library      |

The cog0 library is anticipated to complement:
- `aten/aten/src/ATen/atomspace/` — shared cognitive graph substrate
- `private/ntos/` — NT4 kernel-level integration points
- `opencog_workbench/core/` — Python orchestration wrapper

See `ARCHITECTURE.md` for the full integration topology.
