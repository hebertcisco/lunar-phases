# Lunar Phases WebAssembly

Client-side JavaScript bindings for the [`lunar-phases`](../)
C11 astronomy core. Calculations run in WebAssembly and do not require a
server, API key, or third-party runtime.

## Build

From this directory, with Clang installed:

```sh
npm run build
npm test
```

The build writes `dist/lunar-phases.js`, `dist/lunar-phases.d.ts`, and
`dist/lunar-phases.wasm`.

## Browser usage

```js
import { createLunarPhases } from "@hebertcisco/lunar-phases-wasm";

const lunar = await createLunarPhases();
const state = lunar.stateAt(new Date("2026-10-25T12:30:00Z"));

console.log(state.phase, state.illuminatedFraction);
console.log(lunar.phasesForYear(2026));
```

For a bundler or a custom asset URL:

```js
const lunar = await createLunarPhases({
  wasmUrl: new URL("./lunar-phases.wasm", import.meta.url)
});
```

## Publishing

The repository workflow `.github/workflows/npm-publish.yml` publishes this
package when a GitHub Release is published with a matching `vX.Y.Z` tag. Add an
`NPM_TOKEN` repository secret with permission to publish the package, then
publish a release whose tag matches `wasm/package.json`.

The workflow can also be run manually. Manual runs default to a dry run and
execute the build, tests, and `npm pack` without publishing.

`stateAt` accepts a `Date`, a millisecond timestamp, or an ISO date string.
All input and phase results are UTC. The astronomy model is the same
educational Meeus/ELP-derived model as the C project and is intended for
planning and general use, not observatory-grade ephemerides.
