# UT-M 7-SEG R Click Driver #

## Summary ##

This project aims to show the hardware driver that is used to interface with  the MAX6969 constant-current LED driver via APIs of GSDK.

UT-M 7-SEG R click carries two SMD ultra thin LED 7-SEG displays and the MAX6969 constant-current LED driver from Analog Devices. The click is designed to run on either 3.3V or 5V power supply. It communicates with the target microcontroller over SPI interface.

## Required Hardware ##

- [A BGM220 Explorer Kit board](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit).

- [UT-M 7-SEG R Click](https://www.mikroe.com/ut-m-7-seg-r-click).

## Hardware Connection ##

The UT-M 7-SEG R Click board support MikroBus, so it can connect easily to BGM220P Explorer Kit's MikroBus header. Be sure that the board's 45-degree corner matches the Explorer Kit's 45-degree white line.

The hardware connection is shown in the image below:

![board](hardware_connection.png "Hardware connection")

## Setup ##

You can either create a project based on a example project or start with an empty example project.

### Create a project based on a example project ###

1. From the Launcher Home, add the BRD4314A to MyProducts, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with filter ut-m

2. Click **Create** button on the **Third Party Hardware Drivers - MAX6969 - UT-M 7-SEG R Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.
![Create_example](create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "BGM220 Explorer Kit Board" using Simplicity Studio v5. Use the default project settings.

2. Copy the file app/example/mikroe_utm7segr_max6969/app.c (overwriting existing file), into the project root folder.

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

        - **[Services] → [IO Stream] → [IO Stream: USART]** → default instance name: vcom
        - **[Application] → [Utility] → [Log]**
        - **[Application] → [Service] → [Simple timer service]**
        - **[Third Party Hardware Drivers] → [Display & LED] → [MAX6969 - UT-M 7-SEG R Click (Mikroe)]**

4. Build and flash this example to the board.

**Note:**

- Make sure the SDK extension already be installed. If not please follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md).

- SDK Extension must be enabled for the project to install "MAX6969 - UT-M 7-SEG R Click (Mikroe)" component. Selecting this component will also include the "SPIDRV" and "PWM" component with default configurated instance: mikroe.

- The example project are built on the BRD4314A board. For another boards, selecting the "MAX6969 - UT-M 7-SEG R Click (Mikroe)" component will include the "SPIDRV" and "PWM" components with unconfigured instance: inst0. This instance should be configurated by users.

## How It Works ##

You can launch Console that's integrated into Simplicity Studio or use a third-party terminal tool like TeraTerm to receive the data from the USB. A screenshot of the console output and  an actual test image are shown in the figure below.

![logging_screen](log.png)

![image_test](image_test.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
