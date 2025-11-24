/**
 * @file printf_example.c
 * @brief Example code demonstrating the use of printf for ARM bare-metal debugging
 * 
 * This file provides practical examples of using the printf implementation
 * to output debug information to the UART debug console.
 * 
 * To use this example:
 * 1. Connect a USB-to-UART adapter to the FC200 debug UART (UART0)
 * 2. Open a serial terminal (115200 baud, 8N1)
 * 3. Call the example functions from your main code
 * 
 * @note This is example code - integrate the relevant parts into your application
 */

#include "type.h"
#include "uart_interface.h"

/**
 * @brief Initialize the debug UART for printf output
 * 
 * This function must be called before using printf to ensure
 * the UART hardware is properly configured.
 * 
 * @return true if initialization successful, false otherwise
 */
bool debug_uart_init(void)
{
    // Initialize the debug console UART (UART channel 0)
    bool result = uart_init(UART_DEBUG_CONSOLE);
    
    if (result) {
        printf("\n\n");
        printf("========================================\n");
        printf("  FC200 Debug Console Initialized\n");
        printf("========================================\n");
        printf("UART: UART_DEBUG_CONSOLE (Channel 0)\n");
        printf("Baud: 115200, 8N1\n");
        printf("========================================\n\n");
    }
    
    return result;
}

/**
 * @brief Example 1: Basic printf usage
 * 
 * Demonstrates simple string and integer output
 */
void example_basic_printf(void)
{
    printf("=== Example 1: Basic Printf ===\n");
    
    // Simple string
    printf("Hello from FC200 bare metal!\n");
    
    // Integer values
    int counter = 42;
    printf("Counter value: %d\n", counter);
    
    // Unsigned integer
    uint32_t timestamp = 1234567890;
    printf("Timestamp: %u\n", timestamp);
    
    // Negative numbers
    int temperature = -15;
    printf("Temperature: %d degrees\n", temperature);
    
    printf("\n");
}

/**
 * @brief Example 2: Hexadecimal and binary output
 * 
 * Demonstrates formatting for hardware debugging
 */
void example_hex_binary(void)
{
    printf("=== Example 2: Hexadecimal & Binary ===\n");
    
    // Memory addresses
    uint32_t base_addr = 0x80000000;
    printf("Base address: 0x%08X\n", base_addr);
    
    // Register values
    uint32_t status_reg = 0xDEADBEEF;
    printf("Status register: 0x%08X\n", status_reg);
    
    // Byte values in hex
    uint8_t device_id = 0xA5;
    printf("Device ID: 0x%02X\n", device_id);
    
    // Binary output (useful for bit fields)
    uint8_t flags = 0b10110101;
    printf("Flag register: 0b%08b (hex: 0x%02X)\n", flags, flags);
    
    // Pointer addresses
    void* ptr = (void*)0xCAFEBABE;
    printf("Pointer: %p\n", ptr);
    
    printf("\n");
}

/**
 * @brief Example 3: Floating-point numbers
 * 
 * Demonstrates formatting of float and double values
 */
void example_floating_point(void)
{
    printf("=== Example 3: Floating-Point ===\n");
    
    // Basic float
    float pi = 3.14159265f;
    printf("Pi: %f\n", pi);
    
    // Precision control
    printf("Pi (2 decimals): %.2f\n", pi);
    printf("Pi (4 decimals): %.4f\n", pi);
    
    // Scientific notation
    float small_value = 0.00012345f;
    printf("Small value: %e\n", small_value);
    printf("Small value: %.2e\n", small_value);
    
    // Sensor data example
    float temperature = 25.75f;
    float pressure = 1013.25f;
    float humidity = 65.5f;
    
    printf("\nSensor Readings:\n");
    printf("  Temperature: %6.2f °C\n", temperature);
    printf("  Pressure:    %7.2f hPa\n", pressure);
    printf("  Humidity:    %5.1f %%\n", humidity);
    
    printf("\n");
}

/**
 * @brief Example 4: Formatted tables
 * 
 * Demonstrates creating aligned output for tabular data
 */
void example_formatted_tables(void)
{
    printf("=== Example 4: Formatted Tables ===\n");
    
    // Sensor data table
    printf("\n%-10s | %8s | %8s | %8s\n", "Sensor", "X-axis", "Y-axis", "Z-axis");
    printf("-----------|----------|----------|----------\n");
    printf("%-10s | %8d | %8d | %8d\n", "Accel", 1024, -512, 2048);
    printf("%-10s | %8d | %8d | %8d\n", "Gyro", 50, -25, 75);
    printf("%-10s | %8d | %8d | %8d\n", "Mag", 320, 480, -150);
    
    // Status flags table
    printf("\n%-15s | %6s | %-20s\n", "Subsystem", "Status", "Details");
    printf("----------------|--------|---------------------\n");
    printf("%-15s | %6s | 0x%04X\n", "IMU", "OK", 0x0001);
    printf("%-15s | %6s | 0x%04X\n", "GPS", "ACTIVE", 0x0002);
    printf("%-15s | %6s | 0x%04X\n", "MAG", "OK", 0x0004);
    
    printf("\n");
}

/**
 * @brief Example 5: Using snprintf for string building
 * 
 * Demonstrates safe string formatting into buffers
 */
void example_snprintf(void)
{
    printf("=== Example 5: snprintf (Safe String Formatting) ===\n");
    
    char buffer[64];
    int len;
    
    // Build a status string
    len = snprintf(buffer, sizeof(buffer), "System status: %s", "OPERATIONAL");
    printf("Buffer: %s (length: %d)\n", buffer, len);
    
    // Build a sensor reading string
    float temp = 23.5f;
    uint32_t time = 12345;
    len = snprintf(buffer, sizeof(buffer), "[%u] Temp=%.1f°C", time, temp);
    printf("Buffer: %s (length: %d)\n", buffer, len);
    
    // Demonstrate truncation protection
    char small_buf[16];
    len = snprintf(small_buf, sizeof(small_buf), "This is a very long string that will be truncated");
    printf("Small buffer: %s\n", small_buf);
    printf("Attempted length: %d, Buffer size: %zu\n", len, sizeof(small_buf));
    
    if (len >= sizeof(small_buf)) {
        printf("Warning: String was truncated!\n");
    }
    
    printf("\n");
}

/**
 * @brief Example 6: Error and debug messages with severity levels
 * 
 * Demonstrates a simple logging system using printf
 */
void example_logging_system(void)
{
    printf("=== Example 6: Logging System ===\n\n");
    
    // Different log levels
    printf("[INFO]  System initialization started\n");
    printf("[DEBUG] Initializing UART0 at 115200 baud\n");
    printf("[INFO]  UART0 initialized successfully\n");
    printf("[DEBUG] Loading configuration from flash\n");
    printf("[WARN]  Configuration checksum mismatch, using defaults\n");
    printf("[INFO]  IMU calibration in progress...\n");
    printf("[DEBUG] IMU offset: X=0x%04X, Y=0x%04X, Z=0x%04X\n", 0x0010, 0x0020, 0x0030);
    printf("[ERROR] GPS module not responding (timeout after 5s)\n");
    printf("[INFO]  System ready (3 errors, 1 warning)\n");
    
    printf("\n");
}

/**
 * @brief Example 7: Real-time sensor data monitoring
 * 
 * Demonstrates continuous sensor data output
 */
void example_sensor_monitoring(void)
{
    printf("=== Example 7: Sensor Monitoring ===\n");
    printf("(Press Ctrl+C to stop)\n\n");
    
    // Simulate 5 iterations of sensor readings
    for (int i = 0; i < 5; i++) {
        // Simulate sensor data (in real code, read from actual sensors)
        int16_t accel_x = 1000 + (i * 10);
        int16_t accel_y = -500 + (i * 5);
        int16_t accel_z = 2000 - (i * 8);
        
        int16_t gyro_x = 50 + i;
        int16_t gyro_y = -25 + i;
        int16_t gyro_z = 75 - i;
        
        uint32_t timestamp = 1000 * (i + 1);
        
        printf("[%06u] ACC:(%5d,%5d,%5d) GYRO:(%4d,%4d,%4d)\n",
               timestamp,
               accel_x, accel_y, accel_z,
               gyro_x, gyro_y, gyro_z);
        
        // In real code, you would have a delay here
        // sys_sleep_ms(100);
    }
    
    printf("\n");
}

/**
 * @brief Example 8: Memory dump
 * 
 * Demonstrates hex dump of memory regions
 */
void example_memory_dump(void)
{
    printf("=== Example 8: Memory Dump ===\n");
    
    // Example data (in real code, this would be actual memory)
    uint8_t data[] = {
        0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F,
        0x72, 0x6C, 0x64, 0x21, 0x00, 0xDE, 0xAD, 0xBE,
        0xEF, 0xCA, 0xFE, 0xBA, 0xBE, 0x12, 0x34, 0x56,
        0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x01, 0x02, 0x03
    };
    
    uint32_t base_addr = 0x80000000;
    size_t size = sizeof(data);
    
    printf("\nMemory dump at 0x%08X (%zu bytes):\n", base_addr, size);
    printf("Address    | Hex Values                                      | ASCII\n");
    printf("-----------|------------------------------------------------|------------------\n");
    
    for (size_t i = 0; i < size; i += 16) {
        printf("0x%08X |", base_addr + i);
        
        // Print hex values
        for (size_t j = 0; j < 16; j++) {
            if (i + j < size) {
                printf(" %02X", data[i + j]);
            } else {
                printf("   ");
            }
        }
        
        printf(" | ");
        
        // Print ASCII representation
        for (size_t j = 0; j < 16 && (i + j) < size; j++) {
            uint8_t c = data[i + j];
            printf("%c", (c >= 32 && c < 127) ? c : '.');
        }
        
        printf("\n");
    }
    
    printf("\n");
}

/**
 * @brief Run all printf examples
 * 
 * This function runs all example functions in sequence.
 * Call this from your main() function after initializing the system.
 * 
 * @note Make sure to call debug_uart_init() first!
 */
void run_all_printf_examples(void)
{
    // Make sure UART is initialized
    if (!debug_uart_init()) {
        // If printf doesn't work, we can't even report the error!
        return;
    }
    
    // Run all examples
    example_basic_printf();
    example_hex_binary();
    example_floating_point();
    example_formatted_tables();
    example_snprintf();
    example_logging_system();
    example_sensor_monitoring();
    example_memory_dump();
    
    printf("========================================\n");
    printf("  All examples completed!\n");
    printf("========================================\n\n");
}

/**
 * @brief Minimal integration example
 * 
 * Shows how to add basic printf debugging to your main.c
 */
void minimal_integration_example(void)
{
    // 1. Initialize UART for debug console
    uart_init(UART_DEBUG_CONSOLE);
    
    // 2. Print startup message
    printf("\n=== FC200 Starting ===\n");
    
    // 3. Print debug messages during initialization
    printf("Initializing GPIO... ");
    // gpio_init();
    printf("OK\n");
    
    printf("Initializing sensors... ");
    // da_init();
    printf("OK\n");
    
    printf("System ready!\n\n");
    
    // 4. In your main loop, add debug output as needed
    int loop_count = 0;
    while (loop_count < 3) {  // Normally while(1)
        printf("Main loop iteration: %d\n", loop_count);
        
        // Your application code here
        
        loop_count++;
        // sys_sleep();
    }
}
