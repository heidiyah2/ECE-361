## Test: A Bad Next
Where `t[OPEN][E_CLOSE].next = 7`.
The output of `fsm_check` for this is

```
invalid next state: OPEN CLOSE
```

and it returns 1.

Say `314` was typed and then the door was closed, so the table look-up is as above where the state is `OPEN` and the event is `E_CLOSE`. But the next state is 7 which is invalid, and lock_event returns -1 for all characters and main ends with `final: ?` and no further attempts to enter a code are allowed. The associated action is still `A_LOCK` however, so there's no way to unlock it again.

## Test: No Way Out
Where there is no way out of `OPEN` (`t[OPEN][E_CLOSE].next = OPEN` and `t[OPEN][E_TIMEOUT].next = OPEN`).
The output of `fsm_check` for this is

```
no way out: OPEN
```

and it returns 1.

In this case even when the timer runs out or the door is closed, the next state is `OPEN` and cannot be locked again. Since digits do nothing when in `OPEN` there is no way to relock at all.

## Test: OPEN is Unreachable
Where `t[GOT2][E_RIGHT].next = LOCKED`.
The output of `fsm_check` for this is

```
unreachable: OPEN
```

and it returns 1.

Here `GOT2` points to `LOCKED` with the right code, but the action remains `A_UNLOCK` so the door physically unlocks while the state goes back to `LOCKED` (which has `A_NONE` so no lock action occurs). This is arguably worse than if the lock did not unlock, since there's a false sense of security regarding the contents of the safe.
