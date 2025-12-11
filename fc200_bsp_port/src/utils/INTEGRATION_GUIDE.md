# Printf Integration Guide

This guide shows you how to integrate printf debugging into your FC200 application.

## Step-by-Step Integration

### Step 1: Verify Printf Support is Available

The printf implementation is already integrated into the FC200 BSP. You can verify by checking:

1. `printf.h` and `printf.c` exist in `fc200_bsp_port/src/utils/`
2. `printf_support.c` implements the `_putchar()` function using UART
3. `type.h` includes `printf.h`, making it available everywhere

✅ **No additional setup needed** - printf is ready to use!

### Step 2: Initialize UART Debug Console

Before using printf, initialize the UART debug console. Add this to your `main.c` or initialization code:

```c
#include "type.h"
#include "uart_interface.h"

int main(void)
{
    // Initialize the debug UART (must be done before printf)
    if (!uart_init(UART_DEBUG_CONSOLE)) {
        // UART initialization failed - cannot use printf
        // Handle error (blink LED, etc.)
    }
    
    // Now printf is ready!
    printf("\n=== FC200 System Starting ===\n");
    
    // Rest of your initialization...
    sys_boot();
    gpio_init();
    // ...
}
```

### Step 3: Add Debug Messages

Now you can add printf statements anywhere in your code:

```c
void gpio_configure_all(void)
{
    printf("Configuring GPIOs...\n");
    
    // Your GPIO configuration code
    
    printf("GPIO configuration complete\n");
}

void da_init(void)
{
    printf("Initializing sensors:\n");
    
    printf("  - IMU... ");
    // IMU init code
    printf("OK\n");
    
    printf("  - GPS... ");
    // GPS init code
    printf("OK\n");
    
    printf("Sensor initialization complete\n");
}
```

### Step 4: Connect to Debug Port

#### Hardware Connection

Connect a USB-to-UART adapter to the FC200 debug UART:
- **TX** (from FC200) → **RX** (on USB-UART adapter)
- **RX** (to FC200) → **TX** (on USB-UART adapter) [optional, for bidirectional]
- **GND** → **GND**

#### Software - Open Serial Terminal

**Linux/macOS:**
```bash
# Option 1: Using screen
screen /dev/ttyUSB0 115200

# Option 2: Using picocom
picocom -b 115200 /dev/ttyUSB0

# Option 3: Using minicom
minicom -D /dev/ttyUSB0 -b 115200

# To exit screen: Ctrl+A, then K
# To exit picocom: Ctrl+A, then Ctrl+X
```

**Windows:**
1. Open Device Manager to find COM port (e.g., COM3)
2. Use PuTTY:
   - Connection type: Serial
   - Serial line: COM3 (or your port)
   - Speed: 115200
3. Or use TeraTerm, RealTerm, etc.

**Settings:**
- Baud rate: 115200
- Data bits: 8
- Parity: None
- Stop bits: 1
- Flow control: None

### Step 5: Example - Enhanced Main.c

Here's a complete example of an enhanced `main.c` with debug output:

```c
#include "type.h"
#include "sys_srv_interface.h"
#include "gpio_interface.h"
#include "pwm_interface.h"
#include "da_interface.h"
#include "ach_interface.h"
#include "udp_interface.h"
#include "mavlink_io.h"
#include "fcs_mi_interface.h"
#include "uart_interface.h"

#define ONE_MSEC (1000000UL)

// Debug configuration
#define DEBUG_ENABLED 1
#define DEBUG_VERBOSE 0  // Set to 1 for more detailed output

int main(void)
{
    // Initialize debug UART first
    uart_init(UART_DEBUG_CONSOLE);
    
    printf("\n\n");
    printf("================================================\n");
    printf("  FC200 Flight Computer\n");
    printf("  Firmware Version: 1.0.0\n");
    printf("  Build Date: %s %s\n", __DATE__, __TIME__);
    printf("================================================\n\n");
    
    // FC-200 initialization
    printf("[INIT] System boot...\n");
    sys_boot();
    printf("[INIT] System boot complete\n");
    
    // Initialize default GPIO configuration
    printf("[INIT] Initializing GPIO...\n");
    gpio_init();
    gpio_configure_all();
    printf("[INIT] GPIO initialized\n");
    
    // Initialize default PWM Configuration
    printf("[INIT] Initializing PWM...\n");
    pwm_init();
    printf("[INIT] PWM initialized\n");
    
    // Initialize all Sensors to Acquire data
    printf("[INIT] Initializing sensors...\n");
    da_init();
    printf("[INIT] Sensors initialized\n");
    
    // Initialize Actuator Control Hub
    printf("[INIT] Initializing actuator control...\n");
    ach_init();
    printf("[INIT] Actuator control initialized\n");
    
    // Initialize Flight Control System
    printf("[INIT] Initializing flight control system...\n");
    fcs_mi_init();
    printf("[INIT] Flight control system initialized\n");
    
    // Set tick period to 1000 timer units (1 ms)
    printf("[INIT] Setting system tick period to 1ms...\n");
    sys_set_tick_period(ONE_MSEC);
    
    // Initialize default MAVLINK IO
    printf("[INIT] Initializing MAVLink I/O...\n");
    mav_io_init();
    printf("[INIT] MAVLink I/O initialized\n");
    
    printf("\n");
    printf("================================================\n");
    printf("  Initialization Complete - Starting Main Loop\n");
    printf("================================================\n\n");
    
    uint32_t loop_count = 0;
    
    while (1)
    {
        // Periodic debug output (every 1000 iterations)
        #if DEBUG_VERBOSE
        if ((loop_count % 1000) == 0) {
            printf("[LOOP] Iteration: %u\n", loop_count);
        }
        #endif
        
        // Call periodically to check if UDP messages are received
        udp_sync_periodic();
        
        // Run Periodic Data Acquisition of all Sensors
        da_periodic();
        
        // read the inbound mavlink message
        mavlink_io_recv_periodic();
        
        // periodic flight control task processing
        fcs_mi_periodic();
        
        // Issue commands to the actuators periodically
        ach_cmd_periodic();
        
        // Run Periodic Actuator Control Hub read
        ach_read_periodic();
        
        // tx the outbound mavlink message
        mavlink_io_send_periodic();
        
        sys_sleep();
        
        loop_count++;
    }
}
```

## Adding Printf to Existing Code

### Example 1: Error Reporting

```c
bool sensor_init(void)
{
    printf("[SENSOR] Initializing sensor...\n");
    
    // Try to communicate with sensor
    if (!sensor_communicate()) {
        printf("[ERROR] Sensor communication failed!\n");
        return false;
    }
    
    // Read sensor ID
    uint8_t id = sensor_read_id();
    printf("[SENSOR] Device ID: 0x%02X\n", id);
    
    if (id != EXPECTED_SENSOR_ID) {
        printf("[ERROR] Unexpected sensor ID: expected 0x%02X, got 0x%02X\n",
               EXPECTED_SENSOR_ID, id);
        return false;
    }
    
    printf("[SENSOR] Initialization successful\n");
    return true;
}
```

### Example 2: Performance Monitoring

```c
void task_with_timing(void)
{
    uint32_t start_time = get_timestamp();
    
    // Do the task
    perform_computation();
    
    uint32_t end_time = get_timestamp();
    uint32_t elapsed = end_time - start_time;
    
    printf("[PERF] Task completed in %u microseconds\n", elapsed);
    
    if (elapsed > MAX_ALLOWED_TIME) {
        printf("[WARN] Task exceeded time budget (%u > %u)\n",
               elapsed, MAX_ALLOWED_TIME);
    }
}
```

### Example 3: State Machine Debugging

```c
typedef enum {
    STATE_IDLE,
    STATE_ARMED,
    STATE_FLYING,
    STATE_LANDING
} flight_state_t;

const char* state_names[] = {
    "IDLE",
    "ARMED",
    "FLYING",
    "LANDING"
};

void change_state(flight_state_t new_state)
{
    printf("[FSM] State transition: %s -> %s\n",
           state_names[current_state],
           state_names[new_state]);
    
    current_state = new_state;
}
```

### Example 4: Data Logging

```c
void log_sensor_data(sensor_data_t* data)
{
    static uint32_t log_count = 0;
    
    // Log header every 10 samples
    if (log_count % 10 == 0) {
        printf("\nTime    | Accel X | Accel Y | Accel Z | Temp\n");
        printf("--------|---------|---------|---------|-------\n");
    }
    
    printf("%7u | %7d | %7d | %7d | %5.1f\n",
           data->timestamp,
           data->accel_x,
           data->accel_y,
           data->accel_z,
           data->temperature);
    
    log_count++;
}
```

## Conditional Debugging

To avoid printf overhead in production builds, use conditional compilation:

```c
// In a header file (e.g., debug.h)
#ifdef DEBUG_BUILD
    #define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

// In your code
DEBUG_PRINT("Debug value: %d\n", value);  // Only prints in debug builds
```

Or use different debug levels:

```c
#define DEBUG_LEVEL 2  // 0=none, 1=errors, 2=warnings, 3=info, 4=verbose

#if DEBUG_LEVEL >= 1
    #define DEBUG_ERROR(fmt, ...) printf("[ERROR] " fmt, ##__VA_ARGS__)
#else
    #define DEBUG_ERROR(fmt, ...) ((void)0)
#endif

#if DEBUG_LEVEL >= 2
    #define DEBUG_WARN(fmt, ...) printf("[WARN] " fmt, ##__VA_ARGS__)
#else
    #define DEBUG_WARN(fmt, ...) ((void)0)
#endif

#if DEBUG_LEVEL >= 3
    #define DEBUG_INFO(fmt, ...) printf("[INFO] " fmt, ##__VA_ARGS__)
#else
    #define DEBUG_INFO(fmt, ...) ((void)0)
#endif

#if DEBUG_LEVEL >= 4
    #define DEBUG_VERBOSE(fmt, ...) printf("[VERBOSE] " fmt, ##__VA_ARGS__)
#else
    #define DEBUG_VERBOSE(fmt, ...) ((void)0)
#endif
```

## Troubleshooting

### No Output Appears

1. **Check UART initialization:**
   ```c
   bool result = uart_init(UART_DEBUG_CONSOLE);
   if (!result) {
       // UART init failed
   }
   ```

2. **Check hardware connections:**
   - Verify TX/RX/GND are connected
   - Check for loose connections

3. **Check terminal settings:**
   - Baud rate: 115200
   - Data: 8 bits
   - Parity: None
   - Stop bits: 1

4. **Check device permissions (Linux):**
   ```bash
   sudo usermod -a -G dialout $USER  # Add user to dialout group
   # Then log out and log back in
   ```

### Garbled Output

- **Wrong baud rate** - most common issue
- Check both ends are using same baud rate (115200)
- Verify UART clock configuration

### Missing Characters

- UART FIFO overflow - add delays between printf calls
- Or implement flow control

## See Also

- [README_PRINTF.md](README_PRINTF.md) - Complete printf documentation
- [PRINTF_QUICK_REF.md](PRINTF_QUICK_REF.md) - Quick reference
- [printf_example.c](printf_example.c) - Example code
