# Qwiic Keypad (Sparkfun) #

## Summary ##

This project shows the implementation of a Keypad module that is integrated on the SparkFun Qwiic Keypad board.

The SparkFun Qwiic Keypad board combines the convenience of the Qwiic connection system and a matrix keypad with a contoller IC, so you don't need to mess with any pull-up resistors or configuring multiple GPIO pins. You only need to connect the Qwiic interfaces together and initialize the I2C peripheral on your development board.

For more information about the SparkFun Qwiic Keypad, see the [specification page](https://learn.sparkfun.com/tutorials/qwiic-keypad-hookup-guide).

## Required Hardware ##

- [An EFR32xG24 Explorer Kit - xG24-EK2703A](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview#user-guides)

- [A SparkFun Qwiic keypad Board.](https://www.sparkfun.com/products/15290)
  
## Hardware Connection ##

The Sparkfun Qwiic Keypad board can be easily connected by using a [Qwiic cable](https://www.sparkfun.com/products/17259).

![hardware_connection](image/hardware_connection.png)

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add the BRD2703A to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with filter "keypad".

2. Click **Create** button on the **Third Party Hardware Drivers - Qwiic Keypad (SparkFun)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.
![Create_example](image/create_example.png)

3. In this step you can decide if you want to use the INT I/O. If you don't want to use it, then you don't need to modify anything, just continue on with the guide. If you want to use it, then uncomment the 17th line in sparkfun_qwiic_keypad.h and connect the INT pin on the Keypad to the B1 pin on the Explorer Kit.

4. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "EFR32xG24 Explorer Kit" using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/sparkfun_qwiic_keypad/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

   - Open the .slcp file in the project.

   - Select the SOFTWARE COMPONENTS tab.

   - Install the following components:

      - [Services] → [Timers] → [Sleep Timer]
      - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: vcom
      - [Third Party] → [Tiny printf]
      - [Third Party Hardware Drivers] → [Human Machine Interface] → [Qwiic Keypad (Sparkfun)]

4. In this step you can decide if you want to use the INT I/O. If you don't want to use it, then you don't need to modify anything, just continue on with the guide. If you want to use it, then uncomment the 22th line in app.h and connect the INT pin on the Keypad to the B1 pin on the Explorer Kit.

5. Build and flash the project to your device.

**Note:**

- Make sure that the SDK extension already be installed. If not please follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- SDK Extension must be enabled for the project to install "Qwiic Keypad (Sparkfun)" component.

## How It Works ##

### API Overview ###

- *sparkfun_keypad_init()*: Initialize the keypad module. This function should be called before the main loop.

- *sparkfun_keypad_get_firmware_version()*: Read Firmware Version from the Keypad. Helpful for tech support.

- *sparkfun_keypad_set_address()*: Set new I2C address for keypad.

- *sparkfun_keypad_get_address()*: Get current I2C address used of keypad.

- *sparkfun_keypad_scan_address()*: Scan I2C address of all Keypad that connected on the I2C bus.

- *sparkfun_keypad_select_device()*: Select device on the I2C bus based on its I2C address.

- *sparkfun_keypad_read_last_button()*: Read the first element from the fifo.

- *sparkfun_keypad_time_since_last_button_event()*: Get the time elapsed since the last button was pressed in miliseconds.

- *sparkfun_keypad_update_fifo()*: Send a command to the keypad to update the contents of the fifo inside the device.

- *sparkfun_keypad_present()*: Check whether a keypad is present on the I2C bus or not.

- *sparkfun_keypad_send_command()*: Send a command and read the result over the I2C bus.

### Testing ###

This example demonstrates some of the available features of the Keypad module. Follow the below steps to test the example:

1. On your PC open a terminal program, such as the Console that is integrated in Simplicity Studio or a third-party tool terminal like TeraTerm to receive the logs from the virtual COM port.

2. Press one or more button on the keypad and check the logs on the terminal.

![logging_screen](image/logs.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
