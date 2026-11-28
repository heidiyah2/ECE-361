# ECE-361 Final Project
## My Device: Dishwasher

### States and Operation:
| State   | Meaning                              |
|---------|--------------------------------------|
| IDLE    | waiting for START (initial state)    |
| PREWASH | cold water spray                     |
| WASH    | hot water spray, heater on as needed |
| DRY     | fan on until dry                     |
| DONE    | program finished                     |

Operation and reactions:
- START with...
    - door closed begins PREWASH cycle.
    - door open begins a paused PREWASH.
- PREWASH runs for 90 ticks.
- WASH runs for 240 ticks, during which the heater turns on below $53.0^\circ\mathrm{C}$, off at $55.0^\circ\mathrm{C}$.
- DRY runs the fan until below $40$% RH or at most 600 ticks.
- DONE returns to IDLE when door opens.


### Rules and Decisions:
Clock:
1. Has it's own module so unit tests can reach it, and main is not cluttered.
Answer to question from handout "what would happen to your device on 19 January 2038 if time_t were 32 bits?": This is the same issue with Y2K, there would likely be an integer overflow and time would roll over to 1901, but signed overflow is undefined behavior so who knows depending on the system. We will need to update systems before 2038.

Record Keeping:
1. If two records with the same timestamp occur, both will be kept and stored in order of arrival.

Heater:
1. At the beginning of the WASH cycle, the temperature will be measured to determine if the heater should be on or not, so that if the water is already above $55.0^\circ\mathrm{C}$ when WASH starts, the heater will stay off. If temp is above $53.0^\circ\mathrm{C}$ keep heater off.
2. For after pause, if heater was on turn on again, if off check if below $53.0^\circ\mathrm{C}$ and turn on if so.
3. If temp is somehow $>= 55.0^\circ\mathrm{C}$ and heater was previously on, allow heater to resume on for another tick and evaluate the temp on the next tick (step 5 under Pause).

Pause:
1. The event of the door opening while in a state triggers a pause action, which freezes the timer and remembers the state of things. Closing resumes.
2. Only door events are recognized while in pause.
3. Remember start of cycle, start of pause, add pause length to phase start when pause ends (update elapsed time).
4. If door is opened in last tick of phase, upon resume time limit is checked and will end the phase without resuming first (taking up a tick).
5. If temp is somehow $>= 55.0^\circ\mathrm{C}$ and heater was previously on, allow heater to resume on for another tick and evaluate the temp on the next tick (step 3 under Heater).
6. Resuming from a pause at last phase tick starts the next phase on resume.

Random (sort later):
- Code that turns a reading and switch register into events lives in fsm module.
- event to state and state to action functions live in fsm, as they deal directly with states
- Several events in one tick:
    - Door takes priority, then START, time up, dry enough, temperature.
- Phase lengths exactly t + 89 (90 total traces)
- A_UNDEFINED appears: turn everything off, go back to IDLE, return error (from fsm to main), main handles stderr print.
- RGB: IDLE-black, PREWASH-white, WASH-blue, DRY-yellow, DONE-green
- START at very first tick counts as press.
- START is lost if the door closes on the same tick