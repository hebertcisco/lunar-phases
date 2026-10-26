#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
OUT="$ROOT/dist"
CORE="$ROOT/.."
CC=${CC:-clang}
WASM_LD=${WASM_LD:-$(command -v wasm-ld 2>/dev/null || true)}

if [ -z "$WASM_LD" ]; then
  echo "error: wasm-ld is required; install LLVM or set WASM_LD=/path/to/wasm-ld" >&2
  exit 1
fi

mkdir -p "$OUT"

"$CC" --target=wasm32 -O3 -ffreestanding -fno-builtin \
  -I"$ROOT/src/wasm-compat" -I"$CORE/include" -I"$ROOT/src" \
  -Wl,--no-entry -Wl,--export-memory -Wl,--export-table \
  -Wl,--export=lunar_wasm_state \
  -Wl,--export=lunar_wasm_phase_time \
  -Wl,--export=lunar_wasm_calendar_to_julian \
  -Wl,--allow-undefined \
  "$CORE/src/calendar.c" "$CORE/src/astronomy.c" "$CORE/src/phases.c" "$ROOT/src/wasm_api.c" \
  -fuse-ld="$WASM_LD" -o "$OUT/lunar-phases.wasm"

cp "$ROOT/src/lunar-phases.d.ts" "$OUT/lunar-phases.d.ts"
cp "$ROOT/src/lunar-phases.js" "$OUT/lunar-phases.js"
