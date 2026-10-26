export type DateInput = Date | number | string;

export interface LunarState {
  date: Date;
  julianDay: number;
  phase: string;
  phaseIndex: number;
  illuminatedFraction: number;
  moonAgeDays: number;
  elongationDegrees: number;
  phaseAngleDegrees: number;
  lunarLongitudeDegrees: number;
  lunarLatitudeDegrees: number;
  lunarDistanceKm: number;
  lunarAngularDiameterArcminutes: number;
  solarLongitudeDegrees: number;
  solarDistanceAu: number;
  deltaTSeconds: number;
  previousNewMoon: Date;
  nextNewMoon: Date;
}

export interface LunarPhaseEvent {
  phase: "New Moon" | "First Quarter" | "Full Moon" | "Last Quarter";
  phaseIndex: 0 | 1 | 2 | 3;
  date: Date;
  julianDay: number;
}

export interface CreateOptions {
  wasmUrl?: string | URL;
  wasmBinary?: ArrayBuffer | Uint8Array;
}

export interface LunarPhases {
  stateAt(date?: DateInput): LunarState;
  phaseAt(lunation: number, phaseIndex: 0 | 1 | 2 | 3): LunarPhaseEvent;
  phasesForYear(year: number): LunarPhaseEvent[];
}

export function createLunarPhases(options?: CreateOptions): Promise<LunarPhases>;
