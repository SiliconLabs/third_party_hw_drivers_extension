# LEA-6S - GPS Click (Mikroe) #

## Summary ##

This project shows the implementation of a GPS Click driver using LEA-6S from Mikroe Integrated with BGM220P Explorer Kit based on UART communication.
GPS click is a compact solution for adding GPS functionality to your device. It carries the u-blox LEA-6S high-performance position engine. The click is designed to run on a 3.3V power supply and communicates with the target MCU through UART. GPS click can simultaneously track up to 16 satellites while searching for new ones. The LEA-6S module’s TTFF (time to first fix) is less than one second — this is the measure of time necessary for a GPS receiver to get satellite signals and navigation data, and based on this information, calculate a position (a fix).

## Required Hardware ##

- [**BGM220-EK4314A** BGM220 Bluetooth Module Explorer Kit (BRD4314A BGM220 Explorer Kit Board)](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit)

- [**GPS Click** board based on LEA-6S from Mikroe Integrated](https://www.mikroe.com/gps-click).

## Hardware Connection ##

The GPS Click board can just be "clicked" into its place. Be sure that the board's 45-degree corner matches the Explorer Kit's 45-degree white line. The GPS antenna needs to be connected to the GPS click board to ensure that the satellite signals can be received.

![board](image/hardware_connection.png "BGM220 Explorer Kit Board and GPS Click Board")

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add the BRD4314A to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with the filter 'gps'.

2. Click **Create** button on the **Third Party Hardware Drivers - LEA-6S - GPS Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.
![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "BGM220P Explorer Kit Board" using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_gpsclick_lea6s/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

        - [Services] → [Timers] → [Sleep Timer]
        - [Services] → [IO Stream] → [IO Stream: EUSART] → default instance name: vcom
        - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: mikroe → Set "Baud rate" to 9600
        - [Application] → [Utility] → [Log]
        - [Application] → [Utility] → [Application Queue]
        - [Third Party Hardware Drivers] → [Sensors] → [LEA-6S - GPS Click (Mikroe)]

4. Build and flash this example to the board.

**Note:**

- Make sure that the SDK extension already be installed. If not please follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- SDK Extension must be enabled for the project to install "LEA-6S - GPS Click (Mikroe)" component.

## How It Works ##

This example reads and processes data from GPS clicks.
You can launch Console that's integrated into Simplicity Studio or use a third-party terminal tool like TeraTerm to receive the data from the USB. A screenshot of the console output is shown in the figure below.

![usb_debug](image/gps_log.png "USB Debug Output Data")

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
