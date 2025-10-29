# Patch xil_printf.c to output line feed after every carriage return
# Define the input and output file paths
$inputFile = "136T-2200-113050-001-F11-02/psu_cortexr5_0/standalone_domain/bsp/psu_cortexr5_0/libsrc/standalone_v7_3/src/xil_printf.c"
$outputFile = "output.txt"
$insertText = "            if (*ctrl == 0x0A) outbyte((char8)0x0D);"

# Read the file line by line and process it
$lines = Get-Content $inputFile
$found = $false
$count = 0

# Open output file for writing
$lines | ForEach-Object {
    # Check if the current line matches the search term
    if ($_ -eq "        if (*ctrl != '%') {" -and !$found) {
        # Set to skip next 3 lines
        $found = $true
        $count = 3
    }
    # Write the current line to the output file
    Add-Content $outputFile $_
    #check if 3 lines passed since match
    if ($count -gt 0) {
        $count = $count - 1
        if ($count -eq 0) {
          # If so add the new line
          Add-Content $outputFile $insertText
        } 
    }        
}

# Replace the original file with the modified one
Move-Item -Force $outputFile $inputFile