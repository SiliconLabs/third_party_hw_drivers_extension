# L9958 - DC Motor 24 Click (Mikroe) #

## Summary ##

This project aims to show the hardware driver that is used to interface with the DC motor driver using the L9958 via APIs of GSDK.

DC Motor 24 Click is a compact add-on board with a brushed DC motor driver. This board features the L9958, an SPI-controlled H-bridge from STMicroelectronics. The L9958 is rated for an operating voltage range from 4V to 28V, with direct PWM motor control and current regulation threshold set by the SPI interface from 2.5A to 8.6A. It also has complete diagnostic and protection capabilities supporting robust and reliable operation. This Click board™ is suitable for controlling DC and stepper motors in safety-critical automotive applications and under extreme environmental conditions.

## Required Hardware ##

- [BGM220 Bluetooth Module Explorer Kit](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit?tab=overview)

- [DC Motor 24 Click](https://www.mikroe.com/dc-motor-24-click)

**NOTE:**
Tested boards for working with this example:

| Board ID | Description  |
| -------- | ------------ |
| BRD2703A | [EFR32xG24 Explorer Kit - XG24-EK2703A](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)    |
| BRD4314A | [BGM220 Bluetooth Module Explorer Kit - BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit?tab=overview)  |
| BRD4108A | [EFR32BG22 Explorer Kit Board](https://www.silabs.com/development-tools/wireless/bluetooth/bg22-explorer-kit?tab=overview)  |

## Hardware Connection ##

The DC Motor 24 Click board supports mikroBUS™, so it can connect easily to the header of the BGM220 Bluetooth Module Explorer Kit.

During the assembly process, make sure that the 45-degree corner of the Click board matches the 45-degree white line of the Silicon Labs Explorer Kit.

The hardware connection is shown in the image below:

![board](image/hardware_connection.png)

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add the BRD4314A to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with filter 'l9958'.

2. Click **Create** button on the **Third Party Hardware Drivers - L9958 - DC Motor 24 Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.
![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "BGM220 Explorer Kit" using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_dcmotor24_l9958/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

        - **[Services] → [IO Stream] → [IO Stream: EUSART]** → default instance name: vcom
        - **[Application] → [Utility] → [Log]**
        - **[Services] → [Timers] → [Sleep Timer]**
        - **[Third Party Hardware Drivers] → [Motor Control] → [L9958 - DC MOTOR 24 Click (Mikroe)]**

4. Build and flash this example to the board.

**Note:**

- Make sure that the SDK extension already be installed. If not please follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- Third Party Hardware Drivers extension must be enabled for the project to install the "L9958 - DC MOTOR 24 Click (Mikroe)" component.

## How It Works ##

This example demonstrates the use of the DC Motor 24 Click board. DC Motor 24 Click communicates with the BGM220 via SPI interface.
It shows moving in the left direction from slow to fast speed and from fast to slow speed then switching the direction.
The speed and the direction of the motor are shown in the USART Terminal where you can track these changes.

You can launch Console which is integrated into Simplicity Studio or use a third-party terminal tool like Putty to receive the data from the USB. A screenshot of the console output and an actual test image are shown in the figure below.

![logging_screen](image/log.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
