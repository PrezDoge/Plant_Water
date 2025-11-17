# Arduino IDE Setup Guide

## Arduino IDE Installation

### Step 1: Download Arduino IDE
1. Go to [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
2. Download Arduino IDE for Windows
3. Choose between:
   - **Arduino IDE 2.x** (Recommended - newer interface)
   - **Arduino IDE 1.8.x** (Classic version)

### Step 2: Install Arduino IDE
1. Run the downloaded installer
2. Follow installation wizard
3. Allow Windows to install device drivers when prompted

### Step 3: First Launch
1. Open Arduino IDE
2. Go to **File > Preferences**
3. Note the "Sketchbook location" - this is where your projects will be saved

## Board Setup

### Configure SparkFun RedBoard
1. Connect SparkFun RedBoard to computer via USB cable
2. In Arduino IDE:
   - Go to **Tools > Board > Arduino AVR Boards > Arduino Uno** (RedBoard is Uno-compatible)
   - Go to **Tools > Port** and select the COM port (e.g., COM3, COM4)
   - Note: RedBoard uses the same settings as Arduino Uno

### Verify Connection
1. Go to **Tools > Get Board Info**
2. Should display board information if connected properly

## Opening the Plant Watering Project

### Method 1: Direct Open
1. Go to **File > Open**
2. Navigate to your project folder
3. Select `plant_water.ino`
4. Click **Open**

### Method 2: Using Sketchbook
1. Copy project folder to Arduino sketchbook location
2. Go to **File > Sketchbook > plant_water**

## Compiling and Uploading

### Compile (Verify) Code
1. Click the **✓ Verify** button (or Ctrl+R)
2. Check for any compilation errors in output window
3. Fix any errors before proceeding

### Upload to Arduino
1. Ensure Arduino is connected and correct port is selected
2. Click the **→ Upload** button (or Ctrl+U)
3. Wait for "Done uploading" message

## Using Serial Monitor

### Open Serial Monitor
1. Click **Tools > Serial Monitor** (or Ctrl+Shift+M)
2. Set baud rate to **9600** (bottom right corner)
3. Select "Both NL & CR" for line endings

### Monitor Output
- System status messages will appear here
- Use for debugging and monitoring sensor values
- Check for error messages or warnings

## Troubleshooting

### Common Issues

#### "Port not found" Error
- **Cause**: RedBoard not connected or driver issues
- **Solution**: 
  - Check USB cable connection
  - Try different USB port
  - Install/update FTDI drivers (RedBoard uses FTDI USB-to-Serial chip)
  - RedBoard should appear as "USB Serial Port" in Device Manager

#### Compilation Errors
- **Cause**: Syntax errors or missing libraries
- **Solution**:
  - Check error messages carefully
  - Verify all brackets and semicolons
  - Ensure proper variable declarations

#### Upload Fails
- **Cause**: Wrong board/port selected or Arduino busy
- **Solution**:
  - Verify board and port settings
  - Close Serial Monitor before uploading
  - Press Arduino reset button and try again

### Getting Help
- **Arduino Documentation**: [https://docs.arduino.cc/](https://docs.arduino.cc/)
- **Arduino Forum**: [https://forum.arduino.cc/](https://forum.arduino.cc/)
- **Project Issues**: Check README.md troubleshooting section

## Best Practices

### Code Development
1. Always verify code before uploading
2. Use meaningful variable names
3. Comment your code changes
4. Test small changes incrementally

### Hardware Safety
1. Disconnect power when changing wiring
2. Double-check connections before powering on
3. Use proper voltage levels for components
4. Keep Serial Monitor open during testing

### Project Management
1. Save your work frequently (Ctrl+S)
2. Make backup copies of working code
3. Document any modifications you make
4. Test thoroughly before leaving unattended