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
    - door open remains in IDLE unless previously paused (resumes where it left off).
- PREWASH runs for 90 ticks.
- WASH runs for 240 ticks, during which the heater turns on below $53.0^\circ\mathrm{C}$, off above $55.0^\circ\mathrm{C}$.
- DRY runs the fan until $40$% RH or at most 600 ticks.
- DONE returns to IDLE when door opens.