# Printf Debug Support for FC200

## Quick Answer: Yes, there is an ARM-compatible bare-metal printf implementation!

The FC200 repository includes a lightweight, optimized printf implementation specifically designed for ARM bare-metal embedded systems. It outputs debug logs via UART and does not use malloc or any standard library dependencies.

## 📍 Location

All printf-related files are in: **`fc200_bsp_port/src/utils/`**

## 🚀 Quick Start

### 1. Initialize UART Debug Console

Add to your `main.c` before using printf:

```c
#include "type.h"
#include "uart_interface.h"

int main(void)
{
    // Initialize UART for debug console
    uart_init(UART_DEBUG_CONSOLE);
    
    // Now you can use printf!
    printf("Hello from FC200!\n");
    printf("System initialized\n");
    
    // Your code here...
}
```

### 2. Connect to Debug Port

**Hardware:**
- Connect USB-to-UART adapter to FC200 UART0 debug port
- Connect: TX → RX, RX → TX, GND → GND

**Software - Open Terminal:**

Linux/macOS:
```bash
screen /dev/ttyUSB0 115200
# or
picocom -b 115200 /dev/ttyUSB0
```

Windows:
- Use PuTTY or TeraTerm
- Settings: 115200 baud, 8N1, no flow control

### 3. Use Printf

```c
// Simple messages
printf("Sensor initialized\n");

// Numbers
printf("Value: %d\n", 42);
printf("Address: 0x%08X\n", 0xDEADBEEF);

// Floats
printf("Temperature: %.2f°C\n", 25.5);

// Debug info
printf("X=%d, Y=%d, Z=%d\n", x, y, z);
```

## 📚 Documentation

| Document | Purpose |
|----------|---------|
| **[PRINTF_QUICK_REF.md](fc200_bsp_port/src/utils/PRINTF_QUICK_REF.md)** | Quick reference card - format specifiers, common patterns |
| **[README_PRINTF.md](fc200_bsp_port/src/utils/README_PRINTF.md)** | Complete documentation - all features, configuration |
| **[INTEGRATION_GUIDE.md](fc200_bsp_port/src/utils/INTEGRATION_GUIDE.md)** | Step-by-step integration into your code |
| **[printf_example.c](fc200_bsp_port/src/utils/printf_example.c)** | Working example code with 8+ examples |

## 🔧 Key Features

✅ **ARM Compatible** - Designed for ARM Cortex processors  
✅ **Bare Metal** - No OS or standard library required  
✅ **No malloc** - Static allocation only  
✅ **Thread-Safe** - Reentrant implementation  
✅ **UART Output** - Outputs to debug console via UART0  
✅ **Full Support** - All standard format specifiers (%d, %x, %f, %s, etc.)  
✅ **Float Support** - Includes floating-point formatting  
✅ **Lightweight** - ~2-4KB code size (basic), ~5-7KB with floats  

## 📁 Files

```
fc200_bsp_port/src/utils/
├── printf.h              # Printf function declarations
├── printf.c              # Core printf implementation
├── printf_support.c      # UART output implementation (_putchar)
├── printf_example.c      # Example code demonstrating usage
├── README_PRINTF.md      # Complete documentation
├── PRINTF_QUICK_REF.md   # Quick reference
├── INTEGRATION_GUIDE.md  # Integration instructions
└── README.md             # Utils directory overview
```

## 💡 Common Use Cases

### Debug Messages
```c
printf("Initializing IMU...\n");
// init code
printf("IMU initialization complete\n");
```

### Error Reporting
```c
if (!sensor_init()) {
    printf("[ERROR] Sensor init failed: 0x%02X\n", error_code);
}
```

### Sensor Data
```c
printf("Accel: X=%d Y=%d Z=%d\n", accel_x, accel_y, accel_z);
printf("Temp: %.2f°C, Pressure: %.2f hPa\n", temp, pressure);
```

### Hex Dump
```c
printf("Register: 0x%08X\n", reg_value);
printf("Data: 0x%02X (binary: %08b)\n", data, data);
```

## 🎯 Format Specifiers

| Type | Example | Output |
|------|---------|--------|
| Integer | `printf("%d", 42)` | `42` |
| Hex | `printf("0x%08X", 0xBEEF)` | `0x0000BEEF` |
| Float | `printf("%.2f", 3.14159)` | `3.14` |
| String | `printf("%s", "hello")` | `hello` |
| Binary | `printf("%b", 5)` | `101` |

See [PRINTF_QUICK_REF.md](fc200_bsp_port/src/utils/PRINTF_QUICK_REF.md) for complete list.

## ⚙️ Configuration

The UART debug console uses:
- **Channel:** UART_DEBUG_CONSOLE (UART0)
- **Baud Rate:** 115200 (check `uart_ps_cfg.c` or `uart_pl_cfg.c`)
- **Format:** 8 data bits, No parity, 1 stop bit (8N1)
- **Flow Control:** None

To change the console UART channel, edit `CONSOLE_UART` in `printf_support.c`.

## 🐛 Troubleshooting

**No output?**
1. Check `uart_init(UART_DEBUG_CONSOLE)` is called
2. Verify UART TX/RX/GND connections
3. Check terminal baud rate (115200)
4. On Linux, check device permissions: `ls -l /dev/ttyUSB0`

**Garbled text?**
- Wrong baud rate - both sides must use 115200
- Check 8N1 settings in terminal

See the troubleshooting section in [README_PRINTF.md](fc200_bsp_port/src/utils/README_PRINTF.md) for more details.

## 📖 Example Code

Full working examples are in [printf_example.c](fc200_bsp_port/src/utils/printf_example.c):
- Basic printf usage
- Hexadecimal and binary formatting
- Floating-point numbers
- Formatted tables
- Safe string formatting with snprintf
- Logging system
- Sensor data monitoring
- Memory dumps

## 🔗 References

- **Implementation:** Based on [mpaland/printf](https://github.com/mpaland/printf)
- **License:** MIT License
- **Author:** Marco Paland

## 📞 Support

For detailed information:
1. Start with [PRINTF_QUICK_REF.md](fc200_bsp_port/src/utils/PRINTF_QUICK_REF.md) for quick answers
2. Read [INTEGRATION_GUIDE.md](fc200_bsp_port/src/utils/INTEGRATION_GUIDE.md) for step-by-step integration
3. See [README_PRINTF.md](fc200_bsp_port/src/utils/README_PRINTF.md) for complete documentation
4. Check [printf_example.c](fc200_bsp_port/src/utils/printf_example.c) for working code examples

---

**TL;DR:** Yes, printf is available! Initialize with `uart_init(UART_DEBUG_CONSOLE)`, connect terminal at 115200 baud, and use `printf()` normally. See docs in `fc200_bsp_port/src/utils/` for details.
