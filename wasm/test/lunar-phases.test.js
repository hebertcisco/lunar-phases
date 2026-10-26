import test from "node:test";
import assert from "node:assert/strict";
import { createLunarPhases } from "../src/lunar-phases.js";

test("exports an async WebAssembly client factory", () => {
  assert.equal(typeof createLunarPhases, "function");
});

test("rejects invalid dates before calling the module", async () => {
  await assert.rejects(
    async () => (await createLunarPhases({ wasmBinary: new Uint8Array([0]) })).stateAt("not-a-date"),
    /WebAssembly|compile|Invalid date/
  );
});
