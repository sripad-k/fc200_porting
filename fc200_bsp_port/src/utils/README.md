# FC200 Utils Directory

This directory contains utility functions for the FC200 bare-metal firmware.

## Contents

### Printf Implementation

A lightweight, ARM-compatible, bare-metal printf implementation for debugging via UART.

**📚 Documentation:**
- **[README_PRINTF.md](README_PRINTF.md)** - Complete documentation and guide
- **[PRINTF_QUICK_REF.md](PRINTF_QUICK_REF.md)** - Quick reference card
- **[printf_example.c](printf_example.c)** - Working example code

**📁 Files:**
- `printf.h` - Printf function declarations
- `printf.c` - Printf implementation (by Marco Paland)
- `printf_support.c` - Platform-specific UART output implementation

**🚀 Quick Start:**
```c
#include "type.h"  // printf is included automatically

// Initialize UART debug console
uart_init(UART_DEBUG_CONSOLE);

// Use printf for debugging
printf("Hello from FC200!\n");
printf("Sensor value: %d\n", sensor_value);
printf("Temperature: %.2f°C\n", temp);
```

**📡 Reading Logs:**
Connect to UART0 debug port at 115200 baud, 8N1:
```bash
# Linux/macOS
screen /dev/ttyUSB0 115200

# Windows - use PuTTY, TeraTerm, etc.
```

See [README_PRINTF.md](README_PRINTF.md) for complete details.

### Math Utilities

**Files:**
- `math_util.h` / `math_util.c` - Mathematical utility functions

### Generic Utilities

**Files:**
- `generic_util.h` / `generic_util.c` - General-purpose utility functions

## Usage

All utilities are available by including the main type header:

```c
#include "type.h"
```

This automatically includes:
- Printf functionality
- Common type definitions
- Standard utility functions

## Support

For printf-related issues, see the troubleshooting section in [README_PRINTF.md](README_PRINTF.md).
