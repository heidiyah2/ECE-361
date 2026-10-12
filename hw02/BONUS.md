# Bonus: Recursive Maximum Depth

The following results were produced by running `bonus.sh` on my computer.

| Executable | Function | Largest safe `n` | First crashing `n` | Gap |
|---|---:|---:|---:|---:|
| `depth-o0` | `stats_max` (0) | 163,840 | 180,224 | 10% |
| `depth-o0` | `stats_max_tail` (1) | 245,760 | 262,144 | 6% |
| `depth-o2` | `stats_max` (0) | 1,441,792 | 1,572,864 | 9% |
| `depth-o2` | `stats_max_tail` (1) | 2,013,265,920 | 2,147,483,648 | 6% |

## System details

- Operating system: Fedora Linux 44
- Stack size: 8 MB
- Compiler: GCC 16.2.1
- CPU architecture: x86_64

The `depth-o0` executable was compiled with `-O0`, and `depth-o2` was
compiled with `-O2`. The search stopped when the gap between the largest safe
value and the first crashing value was 12% or less.

## Explanation of Results
`stats_max` will be referred to as "non-tail" and `stats_max_tail` as "tail" functions.

- Tail vs non-tail functions with flag `-O0`: 
Both functions must hold information in a frame on the stack for each call of the recursion (Tail-Call Optimization is not utilized), so the number of frames will be the same between them. However, the tail function merely passes along a single value in `best_value` and does not need to store information on which recursion step a value was stored with, as the non-tail function does in order to back-calculate. Because of this, it is expected that the tail function would reach a higher `n` value, it appears on my machine to be almost double that of the non-tail function.

- Non-tail function with `-O0` vs `-O2`: 
The largest `n` reached is significantly higher with the `-O2` flag, but the increase is not as drastic as with the tail function. This is primarily because there is still more information to store than the tail function with each frame, and although the optimization reduces the load on the stack, it cannot overcome the base inefficiency compared with the tail function.

- Tail function with `-O0` vs `-O2`:
Here the max `n` dramatically increases, and this is primarily because the `-O2` flag enables Tail-Call Optimization (TCO), which recognizes the tail call and rewrites them into a loop that utilizes the same stack frame. However, the max `n` reached is exactly the maximum size of a 32-bit `int` plus 1, which points to integer overflow as the cause of a max `n` being found at all, since a negative value is not a valid `n` for my function and the 8 MB stack size limit was surpassed. The tail function run with `-O2` would theoretically run forever if not limited by type sizes and computer limits.

## Implications
The results showcase the benefits of using a tail call in recursions. They could be especially useful or even necessary in cases such as a binary tree search that is unbalanced (if the height of the tree is equal to the number of items), where a stack overflow is likely if not guaranteed without TCO.