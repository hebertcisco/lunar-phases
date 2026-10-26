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

## Install

Install the published package with npm:

```sh
npm install lunar-phases-wasm
```

## Browser usage

```js
import { createLunarPhases } from "lunar-phases-wasm";

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

`createLunarPhases()` returns a promise for a client with these methods:

- `stateAt(date?)` returns the lunar state at a `Date`, millisecond timestamp,
  or ISO date string. If omitted, it uses the current time.
- `phaseAt(lunation, phaseIndex)` returns one principal phase event, where
  `phaseIndex` is `0` for new moon, `1` for first quarter, `2` for full moon,
  or `3` for last quarter.
- `phasesForYear(year)` returns all principal phase events in the requested
  calendar year.

Dates and phase results use UTC. TypeScript declarations are included in the
package.

## Publishing

The repository workflow `.github/workflows/npm-publish.yml` publishes this
package when a GitHub Release is published with a matching `vX.Y.Z` tag. Add an
`NPM_TOKEN` repository secret with permission to publish the package, then
publish a release whose tag matches `wasm/package.json`.

The workflow can also be run manually. Manual runs default to a dry run and
execute the build, tests, and `npm pack` without publishing.

The astronomy model is the same educational Meeus/ELP-derived model as the C
project and is intended for planning and general use, not observatory-grade
ephemerides.
