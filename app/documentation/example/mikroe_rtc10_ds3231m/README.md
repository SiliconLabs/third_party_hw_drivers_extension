# DS3231M - RTC 10 Click (Mikroe) #

## Summary ##

This example project shows an example of Mikroe RTC 10 Click board driver integration with the Silicon Labs Platform.

RTC 10 Click is a real-time clock module which has an extremely low power consumption, allowing it to be used with a single button cell battery, for an extended period of time. It features the DS3231M, a low-cost, extremely accurate, I2C real-time clock (RTC) from Analog Devices. The device incorporates a battery input and maintains accurate timekeeping when main power to the device is interrupted. The integration of the microelectromechanical systems (MEMS) resonator enhances the long-term accuracy of the device and reduces the piecepart count in a manufacturing line.

This example can be used to demonstrate the capability of maintaining timekeeping of the Mikroe RTC 10 Click board.

## Required Hardware ##

- [**EFR32xG24-EK2703A** EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview).

- [**Mikroe RTC 10 Click**](https://www.mikroe.com/rtc-10-click) board is based on the DS3231M.

**NOTE:**
Tested boards for working with this example:

| Board ID | Description  |
| ---------------------- | ------ |
| BRD2703A | [xG24-EK2703A - EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)    |
| BRD4314A | [BGM220-EK4314A - BGM220P Bluetooth Module Explorer Kit](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit?tab=overview)  |
| BRD4108A | [BG22-EK4108A - EFR32BG22 Explorer Kit Board](https://www.silabs.com/development-tools/wireless/bluetooth/bg22-explorer-kit?tab=overview)  |

## Hardware Connection ##

The RTC Click board supports MikroBus, so it can connect easily to EFR32xG24 Explorer Kit's MikroBus header. Be sure that the board's 45-degree corner matches the Explorer Kit's 45-degree white line.

The hardware connection is shown in the image below:

![hardware_connection](image/hardware_connection.png)

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add the BRD2703A to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with the filter **"rtc"**.

2. Click **Create** button on the **Third Party Hardware Drivers - DS3231M - RTC10 Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.
![create_project](image/create_project.png)

### Start with an empty example project ###

1. Create an "Empty C Project" for the "EFR32xG24 Explorer Kit Board" using Simplicity Studio v5. Use the default project settings.

2. Copy the app.c file (located in the `app/example/mikroe_rtc10_ds3231m` folder) into the project root folder (overwriting existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

        - [Services] → [Timers] → [Sleep Timer]

        - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: **vcom**

        - [Application] → [Utility] → [Log]

        - [Third Party Hardware Drivers] → [Clock & Timing] → [DS3231M - RTC 10 Click (Mikroe)] → use default configuration

4. Install printf float

    - Open Properties of the project.

    - Select C/C++ Build > Settings > Tool Settings >GNU ARM C Linker > General. Check Printf float.

        ![float](image/print_float.png)

5. Build and flash this example to the board.

- Make sure that the SDK extension already be installed. If not please follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- SDK Extension must be enabled for the project to install the "DS3231M - RTC 10 Click (Mikroe)" component. Selecting this component will also include the "I2CSPM" component with "mikroe" instance.

## How It Works ##

### Driver Layer Diagram ###

![software_layer](image/software_layer.png)

### Testing ###

This example sets time and date for the RTC Click in the initialization step. And then it starts tracking the time and display it every one second. The temperature is displayed every 60 seconds.  

You can launch Console that's integrated into Simplicity Studio or use a third-party terminal tool like TeraTerm to receive the data from the USB. A screenshot of the console output is shown in the figure below.

![console_log](image/console_log.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
