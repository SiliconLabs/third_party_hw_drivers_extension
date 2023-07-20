# MQ131 - Ozone 2 Click (Mikroe) #

## Summary ##

This example project shows an example for Mikroe Ozone 2 Click board driver integration with Silicon Labs Platform.

Ozone 2 click is an Ozone detection (O3) sensor, based on the MQ131 gas sensor. This gas sensor has a sensitive layer made of SnO2, which changes its resistance when exposed to ozone. The sensor is able to sense O3 concentration in the range of 10 ppm to 1000 ppm. The Click board™ is also equipped with a trimmer potentiometer, used to adjust the sensitivity and offset of the MQ131 sensor. Low cross-influence of other oxidation gases such as CL2 and NO2, ensures a good accuracy of the sensor.

The analog output of the sensor is sampled by a high-resolution 22-bit A/D converter (ADC), which offers the conversion result on the SPI interface. The analog signal from the sensor is also available, allowing an external ADC to be used.

## Required Hardware ##

- [BGM220 Bluetooth Module Explorer Kit - BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit?tab=overview).
- [**Mikroe Ozone 2 click** board based on MQ131 gas sensor](https://www.mikroe.com/ozone-2-click).

**NOTE:**
Tested boards for working with this example:

| Board ID | Description  |
| ---------------------- | ------ |
| BRD2703A | [EFR32xG24 Explorer Kit - XG24-EK2703A](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)    |
| BRD4314A | [BGM220 Bluetooth Module Explorer Kit - BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit?tab=overview)  |
| BRD4108A | [EFR32BG22 Explorer Kit Board](https://www.silabs.com/development-tools/wireless/bluetooth/bg22-explorer-kit?tab=overview)  |

## Hardware Connection ##

The Ozone 2 click supports MikroBus, so it can connect easily to EFR32xG24 Explorer Kit's MikroBus header. Be sure that the board's 45-degree corner matches the Explorer Kit's 45-degree white line.

The hardware connection is shown in the image below:

![hardware_connection](images/hardware_connection.png)

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add the BRD4314A to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with filter **mq131**.
2. Click **Create** button on the **Third Party Hardware Drivers - MQ131 - Ozone 2 Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.
![create_project](images/create_example.png)

### Start with an empty example project ###

1. Create an "Empty C Project" for the "BGM220 Bluetooth Module Explorer Kit Board" using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_ozone2_mq131/app.c` into the project root folder (overwriting existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

        - [Services] → [Timers] → [Sleep Timer]
        - [Services] → [IO Stream] → [IO Stream: EUSART] → default instance name: **vcom**
        - [Application] → [Utility] → [Log]
        - [Platform] → [Driver] → [SPI] → [SPIDRV] → default instance name: **mikroe** → You need to change the SPI master chip select (CS) control scheme to "CS controlled by application"
        ![spidrv_config](images/spidrv_config.png)
        - [Third Party Hardware Drivers] → [Sensors] → [MQ131 - Ozone 2 Click (Mikroe)] → You may select the ADC module to use in a component configuration as the image shown below
          - Internal ADC: Use EFR32 MCU ADC
          - External ADC: Use MCP3551 ADC on the Ozone 2 click board
        ![mg131_config](images/mq131_config.png)

4. Build and flash this example to the board.

**Note :**

- Make sure that the SDK extension already be installed. If not please follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- SDK Extension must be enabled for the project to install "MQ131 - Ozone 2 Click (Mikroe)" component.

## How It Works ##

The application reads the level of ozone in the air every repetition of 1 second. This driver is able to get the level of ozone gas in the range from 10 to 1000 ppm.
Be sure that you correctly set the AD convertor which you want to use.
You can launch Console that's integrated into Simplicity Studio or use a third-party terminal tool like TeraTerm to receive the data from the USB. A screenshot of the console output is shown as the figure below.

![usb_deblogug](images/log.png "USB Debug Output Data")

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
