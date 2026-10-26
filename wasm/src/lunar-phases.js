const PHASE_NAMES = ["New Moon", "First Quarter", "Full Moon", "Last Quarter"];
const CURRENT_PHASE_NAMES = [
  "New", "Waxing Crescent", "First Quarter", "Waxing Gibbous",
  "Full", "Waning Gibbous", "Last Quarter", "Waning Crescent"
];
const BYTES_PER_STATE = 8 * 14 + 4;

function toJulianDay(value) {
  const date = value instanceof Date ? value : new Date(value);
  if (Number.isNaN(date.getTime())) throw new TypeError("Invalid date");
  return date.getTime() / 86400000 + 2440587.5;
}

function fromJulianDay(julianDay) {
  return new Date((julianDay - 2440587.5) * 86400000);
}

function defaultWasmUrl() {
  return new URL("./lunar-phases.wasm", import.meta.url);
}

async function instantiate(options) {
  const url = options.wasmUrl ?? defaultWasmUrl();
  const source = options.wasmBinary ?? url;
  const imports = { env: {
    fmod: (a, b) => a % b, floor: Math.floor, sin: Math.sin, cos: Math.cos,
    asin: Math.asin, acos: Math.acos, atan2: Math.atan2, fabs: Math.abs, pow: Math.pow
  } };
  if (source instanceof ArrayBuffer || ArrayBuffer.isView(source)) {
    return WebAssembly.instantiate(source, imports);
  }
  const response = await fetch(source);
  if (!response.ok) throw new Error(`Unable to load WebAssembly (${response.status})`);
  if (WebAssembly.instantiateStreaming) {
    try { return await WebAssembly.instantiateStreaming(response, imports); } catch (_) { /* MIME fallback */ }
  }
  return WebAssembly.instantiate(await response.arrayBuffer(), imports);
}

function createClient(instance) {
  const { exports } = instance;
  const memory = new DataView(exports.memory.buffer);

  function stateAt(date = new Date()) {
    const julianDay = toJulianDay(date);
    const pointer = exports.lunar_wasm_state(julianDay);
    const values = [];
    for (let offset = 0; offset < BYTES_PER_STATE - 4; offset += 8) values.push(memory.getFloat64(pointer + offset, true));
    const phaseIndex = memory.getInt32(pointer + BYTES_PER_STATE - 4, true);
    return {
      date: fromJulianDay(values[11]), julianDay: values[11], phase: CURRENT_PHASE_NAMES[phaseIndex], phaseIndex,
      illuminatedFraction: values[8], moonAgeDays: values[9], elongationDegrees: values[6], phaseAngleDegrees: values[7],
      lunarLongitudeDegrees: values[0], lunarLatitudeDegrees: values[1], lunarDistanceKm: values[2],
      lunarAngularDiameterArcminutes: values[3], solarLongitudeDegrees: values[4], solarDistanceAu: values[5],
      deltaTSeconds: values[10], previousNewMoon: fromJulianDay(values[12]), nextNewMoon: fromJulianDay(values[13])
    };
  }

  function phaseAt(lunation, phaseIndex) {
    if (!Number.isInteger(lunation) || !Number.isInteger(phaseIndex) || phaseIndex < 0 || phaseIndex > 3) {
      throw new RangeError("lunation must be an integer and phaseIndex must be 0..3");
    }
    const julianDay = exports.lunar_wasm_phase_time(lunation, phaseIndex);
    return { phase: PHASE_NAMES[phaseIndex], phaseIndex, date: fromJulianDay(julianDay), julianDay };
  }

  function phasesForYear(year) {
    if (!Number.isInteger(year) || year < -4712 || year > 10000) throw new RangeError("year must be an integer");
    const firstLunation = Math.floor((year - 2000) * 12.3685) - 2;
    const events = [];
    for (let lunation = firstLunation; lunation < firstLunation + 17; lunation++) {
      for (let phaseIndex = 0; phaseIndex < 4; phaseIndex++) {
        const event = phaseAt(lunation, phaseIndex);
        if (event.date.getUTCFullYear() === year) events.push(event);
      }
    }
    return events.sort((a, b) => a.julianDay - b.julianDay);
  }

  return Object.freeze({ stateAt, phaseAt, phasesForYear });
}

export async function createLunarPhases(options = {}) {
  if (typeof WebAssembly === "undefined") throw new Error("WebAssembly is not supported");
  const result = await instantiate(options);
  return createClient(result.instance ?? result);
}
