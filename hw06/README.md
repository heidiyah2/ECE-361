# HW6: Sensor, Panel, and Scenario Replay

`replay` replays a scenario file through the iom361 I/O module, one tick per second. Each tick it
sets the sensor and switches from the scenario, reads them back through the registers, toggles
LED k for every switch k just pressed, and prints one trace line.

| Module | What it does |
|---|---|
| `sensor.h` / `sensor.c` | reads and converts the temperature and humidity registers into a `reading_t` |
| `panel.h` / `panel.c` | finds pressed and released switches; turns LEDs on, off, or toggles them by read-modify-write |
| `scenario.h` / `scenario.c` | loads a scenario file and gives the line in force at a tick |
| `main.c` | the replay loop and the trace |
| `iom361_r4.h` / `iom361_r4.c` | the I/O module model (given) |

## Build

From the `hw06/` directory:

```sh
make
```

This builds `replay` with `-std=c11 -Wall -Wextra -g -fsanitize=address,undefined`, linked with
`-lm`.

## Run

```sh
./replay scenarios/switches.txt
```

A scenario file has `#` comments and blank lines, one `start <unix timestamp>` line, then lines of
`tick temp rh switches` with the switches in hexadecimal. The first tick must be 0 and ticks must
increase; values hold between lines.

The switches can be written with or without a `0x` or `0X` prefix, and the hex digits can be
uppercase or lowercase: `0xa`, `0XA`, `A`, and `a` all mean switches 1 and 3 are on. The trace always prints
them as uppercase with `0x` (`switches=0xA`). With 4 switches, bits above bit 3 are ignored, so
`0xFF` reads back as `0xF`. `replay` prints one line per tick from 0 to the last tick, in
UTC:

```
2026-10-06 08:00:02 temp=21.5 rh=40.0 switches=0x1 pressed=0x1 leds=0x1
```

If the file can't be opened or a line is wrong, `replay` prints an error with the line number to
standard error and exits with status 1.

## Test

```sh
make test
```

This builds `replay` and the tests if needed, then:

1. runs `./tests/test_hw06`, the unit tests for the sensor, panel, and scenario modules. It prints
   `PASS` or `FAIL` for each check and a summary.
2. runs `sh regress.sh`, the regression test. For each file in `scenarios/` it runs `./replay`
   and compares the output with the file of the same name in `expected/`, printing `PASS` or
   `FAIL` per scenario.

`make test` exits with a nonzero status if any unit test or regression test fails. Run it from
`hw06/`: the unit tests write a temporary scenario file in the current directory, and
`regress.sh` uses paths relative to it.

To add a regression test, put the scenario in `scenarios/name.txt` and its checked trace in
`expected/name.txt`.

## Clean

```sh
make clean
```

This removes the `.o` files, `replay`, and `tests/test_hw06`.
