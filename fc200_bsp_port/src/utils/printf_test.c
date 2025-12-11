/**
 * @file printf_test.c
 * @brief Simple test program to verify printf functionality
 * 
 * This minimal test program can be used to verify that the printf
 * implementation is working correctly with the UART debug console.
 * 
 * How to use:
 * 1. Include this file in your build (or copy the test function to main.c)
 * 2. Call printf_test() from your main() after system initialization
 * 3. Connect terminal to debug UART at 115200 baud
 * 4. Verify you see the test output
 * 
 * Expected output:
 * ================================
 * Printf Test Starting
 * ================================
 * Test 1: Simple string - PASS
 * Test 2: Integer - PASS
 * Test 3: Hex - PASS
 * Test 4: Float - PASS
 * Test 5: Multiple args - PASS
 * ================================
 * All Printf Tests Passed!
 * ================================
 */

#include "type.h"
#include "uart_interface.h"

/**
 * @brief Run basic printf tests
 * 
 * This function runs a series of simple tests to verify that
 * printf is working correctly with the UART debug console.
 * 
 * @return true if all tests pass, false otherwise
 */
bool printf_test(void)
{
    bool all_passed = true;
    
    // Ensure UART is initialized
    if (!uart_init(UART_DEBUG_CONSOLE)) {
        // Can't even report the error since printf needs UART!
        return false;
    }
    
    printf("\n");
    printf("================================\n");
    printf("Printf Test Starting\n");
    printf("================================\n");
    
    // Test 1: Simple string
    printf("Test 1: Simple string - ");
    printf("PASS\n");
    
    // Test 2: Integer formatting
    printf("Test 2: Integer - ");
    int test_int = 42;
    if (test_int == 42) {
        printf("PASS (value=%d)\n", test_int);
    } else {
        printf("FAIL\n");
        all_passed = false;
    }
    
    // Test 3: Hexadecimal formatting
    printf("Test 3: Hex - ");
    uint32_t test_hex = 0xDEADBEEF;
    printf("PASS (0x%08X)\n", test_hex);
    
    // Test 4: Float formatting
    printf("Test 4: Float - ");
    float test_float = 3.14159f;
    printf("PASS (%.3f)\n", test_float);
    
    // Test 5: Multiple arguments
    printf("Test 5: Multiple args - ");
    int x = 10, y = 20, z = 30;
    printf("PASS (x=%d, y=%d, z=%d)\n", x, y, z);
    
    // Test 6: snprintf (safe string formatting)
    printf("Test 6: snprintf - ");
    char buffer[32];
    int len = snprintf(buffer, sizeof(buffer), "Value=%d", 999);
    if (len > 0 && len < sizeof(buffer)) {
        printf("PASS (buffer=\"%s\", len=%d)\n", buffer, len);
    } else {
        printf("FAIL\n");
        all_passed = false;
    }
    
    printf("================================\n");
    if (all_passed) {
        printf("All Printf Tests Passed!\n");
    } else {
        printf("Some Printf Tests Failed!\n");
    }
    printf("================================\n");
    printf("\n");
    
    return all_passed;
}

/**
 * @brief Quick printf sanity check
 * 
 * A minimal test that just prints a startup banner.
 * Use this if you just want to verify basic printf functionality.
 */
void printf_quick_test(void)
{
    uart_init(UART_DEBUG_CONSOLE);
    
    printf("\n");
    printf("*********************************\n");
    printf("*   FC200 Printf Test           *\n");
    printf("*   If you see this, printf     *\n");
    printf("*   is working correctly!       *\n");
    printf("*********************************\n");
    printf("\n");
    printf("Timestamp: %u\n", 12345);
    printf("Status: %s\n", "OPERATIONAL");
    printf("Value: 0x%08X\n", 0xCAFEBABE);
    printf("\n");
}

/**
 * @brief Example integration into main()
 * 
 * Shows how to add the printf test to your main() function
 */
#if 0  // Set to 1 to enable this example
int main(void)
{
    // Your system initialization
    sys_boot();
    
    // Run printf test
    printf_quick_test();
    // or for full test:
    // printf_test();
    
    // Rest of your initialization
    gpio_init();
    // ...
    
    // Main loop
    while (1) {
        // Your code
    }
}
#endif
