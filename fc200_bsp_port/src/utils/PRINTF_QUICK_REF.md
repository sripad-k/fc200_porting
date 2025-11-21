# Printf Quick Reference Card

## 🚀 Quick Start

```c
#include "type.h"  // printf is automatically included

// Initialize UART debug console first!
uart_init(UART_DEBUG_CONSOLE);

// Now you can use printf
printf("Hello FC200!\n");
```

## 📡 Reading Debug Output

### Terminal Setup
- **Baud Rate:** 115200 (check uart_ps_cfg.c or uart_pl_cfg.c if different)
- **Settings:** 8 data bits, No parity, 1 stop bit (8N1)
- **Flow Control:** None

### Linux/macOS
```bash
screen /dev/ttyUSB0 115200
# or
picocom -b 115200 /dev/ttyUSB0
```

### Windows
- Use PuTTY, TeraTerm, or any serial terminal
- Select correct COM port
- Set baud to 115200

## 📝 Common Format Specifiers

| Type | Specifier | Example | Output |
|------|-----------|---------|--------|
| Integer | `%d` | `printf("%d", 42);` | `42` |
| Unsigned | `%u` | `printf("%u", 42);` | `42` |
| Hex (lower) | `%x` | `printf("%x", 255);` | `ff` |
| Hex (upper) | `%X` | `printf("%X", 255);` | `FF` |
| Hex padded | `%08X` | `printf("%08X", 0xBEEF);` | `0000BEEF` |
| Binary | `%b` | `printf("%b", 5);` | `101` |
| Float | `%f` | `printf("%f", 3.14);` | `3.140000` |
| Float (prec) | `%.2f` | `printf("%.2f", 3.14159);` | `3.14` |
| Scientific | `%e` | `printf("%e", 1000.0);` | `1.0e+03` |
| Character | `%c` | `printf("%c", 'A');` | `A` |
| String | `%s` | `printf("%s", "hi");` | `hi` |
| Pointer | `%p` | `printf("%p", ptr);` | `0xBEEF` |

## 🎯 Common Use Cases

### Debugging Values
```c
int value = 42;
printf("Value = %d\n", value);

uint32_t addr = 0x80000000;
printf("Address = 0x%08X\n", addr);
```

### Sensor Data
```c
float temp = 25.5;
printf("Temperature: %.2f°C\n", temp);
```

### Multi-value Output
```c
printf("X=%d, Y=%d, Z=%d\n", x, y, z);
```

### Hex Dump
```c
uint8_t data = 0xA5;
printf("Data: 0x%02X (binary: %08b)\n", data, data);
```

### Safe String Formatting
```c
char buf[32];
snprintf(buf, sizeof(buf), "Value: %d", value);
```

## ⚡ Best Practices

### ✅ DO
```c
// Use UART initialization
uart_init(UART_DEBUG_CONSOLE);

// Use snprintf for safety
snprintf(buf, sizeof(buf), "x=%d", x);

// Check buffer size
if (len >= sizeof(buf)) {
    printf("Truncated!\n");
}

// Use in main loop, not in ISRs
void main_loop() {
    printf("Status: %d\n", status);
}
```

### ❌ DON'T
```c
// Don't use in ISRs
void timer_isr() {
    printf("Tick\n");  // ❌ BAD
}

// Don't use sprintf (no bounds check)
sprintf(buf, "Long string...");  // ❌ BAD - use snprintf

// Don't forget null termination space
char buf[5];
snprintf(buf, 5, "Hello");  // Only "Hell" fits
```

## 🔧 Width & Alignment

```c
// Right-aligned (default)
printf("%5d", 42);      // "   42"

// Left-aligned
printf("%-5d", 42);     // "42   "

// Zero-padded
printf("%05d", 42);     // "00042"

// Width for hex
printf("0x%08X", 0xFF); // "0x000000FF"

// Precision for floats
printf("%.3f", 3.14159);// "3.142"
```

## 📊 Example: Formatted Table

```c
printf("%-10s | %8s | %6s\n", "Name", "Value", "Status");
printf("-----------|----------|--------\n");
printf("%-10s | %8d | %6s\n", "Sensor1", 1024, "OK");
printf("%-10s | %8d | %6s\n", "Sensor2", 2048, "OK");
```

Output:
```
Name       |    Value | Status
-----------|----------|--------
Sensor1    |     1024 |     OK
Sensor2    |     2048 |     OK
```

## 🐛 Troubleshooting

| Problem | Solution |
|---------|----------|
| No output | Check UART connection, baud rate, and initialization |
| Garbled text | Verify baud rate matches (115200) |
| Missing characters | Check UART FIFO size, add delays if needed |
| Truncated output | Use snprintf, check buffer sizes |

## 📚 See Also

- `README_PRINTF.md` - Complete documentation
- `printf_example.c` - Full examples
- `printf.h` - Function declarations
- `printf_support.c` - UART implementation

## 🔌 UART Channels

```c
typedef enum {
    UART_DEBUG_CONSOLE = 0,  // Debug printf output
    UART_RADALT,
    UART_SBUS,
    UART_INS,
    UART_SERVO,
    UART_ADS,
    UART_LOS,
    UART_MAX_PERIPHERAL
} UART_peripherals_t;
```

Default console: **UART_DEBUG_CONSOLE** (Channel 0)
