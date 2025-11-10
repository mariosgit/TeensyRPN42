mariosgit: This is a Hp calculator emulator with rpn.
find out how free42lib works.
Im interessted where the stack is saved - how to dump it's contents
And where the display pixels are stored - how to access these

Workspace: Collecting workspace information# Free42lib Stack and Display Analysis

## Stack Storage

The calculator stack is stored in a global array managed by the Free42 core. Here's where to find it:

### Stack Location
The stack is defined in core_globals.h:

```c
extern vartype **stack;
extern int sp;
extern int stack_capacity;
```

The actual storage and management is in core_globals.cc around line 572. The stack pointer (`sp`) indicates the current top of the stack, where:
- `sp = -1` means empty stack
- `sp >= 0` means stack has elements from index 0 to sp

### Dumping Stack Contents

To dump the stack, you need to:

1. **Access stack elements** through [`stack[sp]`](lib/Free42lib/common/core_globals.h) which is a `vartype` pointer
2. **Check the type** using the type field (TYPE_REAL, TYPE_COMPLEX, TYPE_STRING, etc.)
3. **Extract values** based on type

Helper function in core_aux.cc at line 280:

```c
int reg2str(char *buf, int buflen, reg_id_t reg_id)
```

This function already converts registers to strings. You can use similar logic to dump the full stack:

```cpp
// Access stack with X at stack[sp], Y at stack[sp-1], etc.
for (int i = sp; i >= 0; i--) {
    vartype *v = stack[i];
    // Check v->type and extract value accordingly
    // TYPE_REAL: ((vartype_real *)v)->x
    // TYPE_COMPLEX: ((vartype_complex *)v)->re, ->im
    // TYPE_STRING: ((vartype_string *)v)->txt()
}
```

---

## Display Pixel Storage

### Display Buffer Location

The display is managed in core_display.cc. The key function is:

```c
void shell_blitter(const char *bits, int bytesperline, int x, int y, 
                   int width, int height)
```

This is implemented in your main.cpp at line 424. The **`bits` parameter contains the pixel data**.

### Accessing Display Contents

The display is **131 pixels wide × 16 pixels tall** (see your main.cpp line 367):

```cpp
#define LCDW 131
#define LCDH 16
char disp[LCDH][LCDW + 1];
```

The pixel data comes packed in bytes. Each byte in `bits` represents 8 horizontal pixels:

```cpp
int b = bits[v * bytesperline + (h >> 3)] & (1 << (h & 7));
disp[v][h] = b ? '#' : ' ';
```

### To Access Display Pixels:

1. **Get the display buffer** via the `bits` parameter in `shell_blitter()`
2. **Pixel at (x,y)** is in byte `[y * bytesperline + (x >> 3)]`, bit `(x & 7)`
3. **Extract bit**: `(byte >> (x & 7)) & 1`

You could add a function to your code:

```cpp
int get_display_pixel(const char *bits, int bytesperline, int x, int y) {
    return (bits[y * bytesperline + (x >> 3)] >> (x & 7)) & 1;
}
```

---

## Integration Points

For direct access from your code, consider:
- Modify `shell_blitter()` to save the bits pointer
- Add functions to dump stack via `core_aux.h` interface
- The core is initialized with `core_init()` and you have access to all globals after that



---


