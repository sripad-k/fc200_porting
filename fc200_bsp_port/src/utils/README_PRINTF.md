# ARM Bare Metal Printf Implementation

## Overview

This directory contains a lightweight, bare-metal printf implementation optimized for ARM embedded systems. The implementation is thread-safe, reentrant, and does not use dynamic memory allocation (malloc), making it ideal for resource-constrained embedded environments.

## Features

- ✅ **ARM Compatible**: Designed for ARM bare-metal environments
- ✅ **Lightweight**: No malloc usage, minimal resource footprint
- ✅ **Thread-Safe**: Reentrant implementation
- ✅ **UART Debug Output**: Outputs to debug console via UART
- ✅ **Full Format Support**: Supports standard printf format specifiers
- ✅ **Float Support**: Includes floating-point number formatting
- ✅ **Multiple Functions**: printf, sprintf, snprintf, vprintf, vsnprintf

## Files

- **printf.h**: Header file with function declarations
- **printf.c**: Core printf implementation by Marco Paland
- **printf_support.c**: Platform-specific UART output implementation

## Quick Start

### 1. Include the Header

The printf functionality is automatically available when you include `type.h`:

```c
#include "type.h"  // printf.h is included automatically
```

Or include it directly:

```c
#include "printf.h"
```

### 2. Use Printf Functions

```c
// Simple string output
printf("Hello from FC200!\n");

// Integer formatting
int value = 42;
printf("Value: %d\n", value);

// Hexadecimal
printf("Address: 0x%08X\n", 0xDEADBEEF);

// Float formatting (if enabled)
float temperature = 25.5f;
printf("Temperature: %.2f°C\n", temperature);

// Multiple arguments
printf("X=%d, Y=%d, Z=%d\n", x, y, z);
```

## UART Debug Console Configuration

### Hardware Setup

The printf implementation outputs to **UART_DEBUG_CONSOLE** (UART channel 0) by default. This is configured in `printf_support.c`:

```c
#define CONSOLE_UART 0  // UART_DEBUG_CONSOLE
```

### Connecting to Debug Port

To read logs from the debug port, you need to:

1. **Connect UART Cable**: Connect a USB-to-UART adapter to the FC200 debug UART pins
   - TX pin (from FC200)
   - RX pin (to FC200, optional for bidirectional communication)
   - GND

2. **Configure Terminal Settings**:
   - **Baud Rate**: Check your UART initialization (typically 115200)
   - **Data Bits**: 8
   - **Parity**: None
   - **Stop Bits**: 1
   - **Flow Control**: None

3. **Open Serial Terminal**:

   **Linux/macOS**:
   ```bash
   # Using screen
   screen /dev/ttyUSB0 115200
   
   # Using minicom
   minicom -D /dev/ttyUSB0 -b 115200
   
   # Using picocom
   picocom -b 115200 /dev/ttyUSB0
   ```

   **Windows**:
   - Use PuTTY, TeraTerm, or other serial terminal software
   - Select the appropriate COM port
   - Set baud rate to 115200 (or as configured)

## Available Functions

### printf()
Standard printf with output to UART debug console.

```c
int printf(const char* format, ...);
```

**Example**:
```c
printf("System initialized\n");
printf("Sensor value: %d\n", sensor_reading);
```

### sprintf()
Format string into a buffer (use with caution - no bounds checking).

```c
int sprintf(char* buffer, const char* format, ...);
```

**Example**:
```c
char buffer[64];
sprintf(buffer, "Status: %s", status_msg);
```

### snprintf()
**Recommended**: Format string with buffer size limit.

```c
int snprintf(char* buffer, size_t count, const char* format, ...);
```

**Example**:
```c
char buffer[32];
snprintf(buffer, sizeof(buffer), "Temp: %.1f", temperature);
```

### vprintf() / vsnprintf()
Variadic argument versions for creating custom logging functions.

```c
int vprintf(const char* format, va_list va);
int vsnprintf(char* buffer, size_t count, const char* format, va_list va);
```

### fctprintf()
Printf with custom output function.

```c
int fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...);
```

## Format Specifiers

The printf implementation supports standard C format specifiers:

| Specifier | Description | Example |
|-----------|-------------|---------|
| `%d`, `%i` | Signed decimal integer | `printf("%d", -42);` → `-42` |
| `%u` | Unsigned decimal integer | `printf("%u", 42);` → `42` |
| `%x` | Hexadecimal (lowercase) | `printf("%x", 255);` → `ff` |
| `%X` | Hexadecimal (uppercase) | `printf("%X", 255);` → `FF` |
| `%o` | Octal | `printf("%o", 8);` → `10` |
| `%b` | Binary | `printf("%b", 5);` → `101` |
| `%f`, `%F` | Floating-point | `printf("%f", 3.14);` → `3.140000` |
| `%e`, `%E` | Exponential notation | `printf("%e", 1000.0);` → `1.0e+03` |
| `%g`, `%G` | Shortest of %f or %e | `printf("%g", 0.001);` → `0.001` |
| `%c` | Character | `printf("%c", 'A');` → `A` |
| `%s` | String | `printf("%s", "hello");` → `hello` |
| `%p` | Pointer | `printf("%p", ptr);` → `0xBEEF` |
| `%%` | Literal % | `printf("%%");` → `%` |

### Modifiers

- **Width**: `printf("%5d", 42);` → `   42`
- **Left-align**: `printf("%-5d", 42);` → `42   `
- **Zero-padding**: `printf("%05d", 42);` → `00042`
- **Precision**: `printf("%.2f", 3.14159);` → `3.14`
- **Precision (strings)**: `printf("%.5s", "hello world");` → `hello`
- **Plus sign**: `printf("%+d", 42);` → `+42`
- **Space**: `printf("% d", 42);` → ` 42`
- **Hash**: `printf("%#x", 255);` → `0xff`

### Length Modifiers

- `hh` - char
- `h` - short
- `l` - long
- `ll` - long long
- `z` - size_t
- `t` - ptrdiff_t
- `j` - intmax_t

## Example Code

Here's a complete example demonstrating printf usage:

```c
#include "type.h"

void debug_example(void)
{
    // System startup message
    printf("\n=== FC200 Debug Console ===\n");
    printf("Firmware Version: %d.%d.%d\n", 1, 0, 0);
    
    // Sensor readings
    int16_t accel_x = 1024;
    int16_t accel_y = -512;
    int16_t accel_z = 2048;
    
    printf("\nAccelerometer Data:\n");
    printf("  X: %6d\n", accel_x);
    printf("  Y: %6d\n", accel_y);
    printf("  Z: %6d\n", accel_z);
    
    // Float values
    float temperature = 25.5f;
    float pressure = 1013.25f;
    
    printf("\nEnvironmental Data:\n");
    printf("  Temperature: %6.2f°C\n", temperature);
    printf("  Pressure:    %7.2f hPa\n", pressure);
    
    // Hexadecimal debugging
    uint32_t status_reg = 0xDEADBEEF;
    printf("\nStatus Register: 0x%08X\n", status_reg);
    
    // Binary debugging
    uint8_t flags = 0b10110101;
    printf("Flags: 0b%08b (0x%02X)\n", flags, flags);
    
    // Pointer debugging
    void* ptr = (void*)0x80000000;
    printf("Base address: %p\n", ptr);
}
```

## Best Practices

### 1. Use snprintf() for Buffer Safety
```c
// ✅ Good - bounds-checked
char buf[32];
snprintf(buf, sizeof(buf), "Value: %d", value);

// ❌ Bad - potential buffer overflow
sprintf(buf, "Value: %d", value);
```

### 2. Avoid Printf in ISRs
```c
// ❌ Bad - printf in interrupt handler
void timer_isr(void)
{
    printf("Timer interrupt!\n");  // UART might be busy
}

// ✅ Good - set flag, print in main loop
volatile bool timer_event = false;

void timer_isr(void)
{
    timer_event = true;
}

void main_loop(void)
{
    if (timer_event) {
        timer_event = false;
        printf("Timer event occurred\n");
    }
}
```

### 3. Use Appropriate Format Specifiers
```c
uint32_t value = 12345;

// ✅ Correct
printf("Value: %u\n", value);  // unsigned

// ❌ Wrong
printf("Value: %d\n", value);  // signed (undefined for large values)
```

### 4. Check Return Values
```c
char buffer[32];
int len = snprintf(buffer, sizeof(buffer), "Long string...");

if (len >= sizeof(buffer)) {
    // String was truncated
    printf("Warning: output truncated\n");
}
```

## Configuration Options

The printf implementation can be configured by defining macros before including `printf.h` or in `printf_config.h`:

```c
// Disable floating-point support (saves code space)
#define PRINTF_DISABLE_SUPPORT_FLOAT

// Disable exponential notation
#define PRINTF_DISABLE_SUPPORT_EXPONENTIAL

// Disable long long support
#define PRINTF_DISABLE_SUPPORT_LONG_LONG

// Adjust buffer sizes
#define PRINTF_NTOA_BUFFER_SIZE 32U
#define PRINTF_FTOA_BUFFER_SIZE 32U
```

## Troubleshooting

### No Output on Terminal

1. **Check UART Connection**: Verify TX/RX/GND connections
2. **Check Baud Rate**: Ensure terminal baud rate matches UART configuration
3. **Check UART Initialization**: Verify `uart_init(UART_DEBUG_CONSOLE)` is called
4. **Check Terminal Device**: On Linux, verify correct `/dev/ttyUSB*` or `/dev/ttyACM*`

### Garbled Output

1. **Baud Rate Mismatch**: Most common cause - verify both sides use same baud rate
2. **Wrong Parity/Stop Bits**: Should be 8N1 (8 data bits, no parity, 1 stop bit)
3. **Flow Control**: Disable hardware/software flow control

### Truncated Output

1. **Buffer Overflow**: Use `snprintf()` instead of `sprintf()`
2. **UART FIFO Full**: Add delays or implement flow control
3. **Interrupt Priority**: Ensure UART interrupts have appropriate priority

## Performance Considerations

- **Code Size**: Basic printf adds ~2-4KB, float support adds ~2-3KB
- **Speed**: Optimized for speed, but UART transmission is the bottleneck
- **Stack Usage**: Uses stack for conversion buffers (~64 bytes)
- **Thread Safety**: Reentrant, but UART access may need protection in multi-threaded environments

## References

- Original implementation: [mpaland/printf](https://github.com/mpaland/printf)
- License: MIT License
- Author: Marco Paland

## Support

For issues or questions:
1. Check UART configuration in `uart_ps_cfg.c` or `uart_pl_cfg.c`
2. Verify `_putchar()` implementation in `printf_support.c`
3. Review UART driver in `fc200_bsp_port/bsp/soc/uart/`
