# SPS30 - Particulate Matter Sensor (Sparkfun) #

## Summary ##

This project aims to show the hardware driver that is used to interface with the SPS30 sensor via GSDK APIs. This driver is based on [SPS30 Sensor API](https://github.com/Sensirion/embedded-sps) from Sensirion.

The Sensirion Particulate Matter Sensor SPS30 is a compact, high quality, optical particle sensor that uses laser scattering and the innovative contamination resistance technology of Sensirion to achieve superior binning and particle measurement. This sensor allows users to measure mass concentration and number of particles of 1 µg/m^3, 2.5 µg/m^3, 4 µg/m^3, and 10 µg/m^3.

The compact form factor, measuring just under 41mm x 41mm x 12mm, combined with a sensor lifetime of over 8 years and a self cleaning procedure, makes the SPS30 the perfect sensor for projects in difficult to access locations. An easy to use 5-pin cable is included to make using the SPS30 a snap. The 1.5mm connector is broken out into 5 breadboard friendly wires color coded to make hooking up the SPS30 easy.

The SPS30 has a five-pin interface that can communicate over two different protocols: UART and I2C. The SPS30 requires a 5V power supply, but can work with 3.3V and 5V microcontrollers. The logic is 5V and 3.3V tolerant.

## Required Hardware ##

- [**EFR32xG24-EK2703A** EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview).

- [**Particulate Matter Sensor - SPS30**](https://www.sparkfun.com/products/15103)

**NOTE:**
Tested boards for working with this example:

| Board ID | Description  |
| ---------------------- | ------ |
| BRD2703A | [EFR32xG24 Explorer Kit - XG24-EK2703A](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)    |
| BRD4314A | [BGM220 Bluetooth Module Explorer Kit - BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit?tab=overview)  |
| BRD4108A | [EFR32BG22 Explorer Kit Board](https://www.silabs.com/development-tools/wireless/bluetooth/bg22-explorer-kit?tab=overview)  |

## Hardware Connection ##
The hardware connection is shown in the image below:

![hw_connection_1](image/hw_connection_1.png)
![hw_connection](image/hw_connection.png)

----------------------------------------------------------
| Pin | SPS30      | Connect to | EFR32xG24 Explorer Kit |
| --- | ---------- | ---------- | ---------------------- |
| 1.  | VDD        | <========> | 5V                     |
| 2.  | RX/SDA     | <========> | TX/SDA                 |
| 3.  | TX/SCL     | <========> | RX/SCL                 |
| 4.  | SEL        | <========> | Leave floating to select UART or Pull to GND to select I2C                                               |
| 5.  | GND        | <========> | GND                    |
----------------------------------------------------------

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add the BRD2703A to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with the filter: 'sps30'.

2. Click **Create** button on the example:

    - **Third Party Hardware Drivers - SPS30 - Particulate Matter Sensor (Sparkfun) - I2C** if using the I2C interface.  

    - **Third Party Hardware Drivers - SPS30 - Particulate Matter Sensor (Sparkfun) - UART** if using the UART interface.

    Example project creation dialog pops up -> click Create and Finish and Project should be generated.
    ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "EFR32xG24 Explorer Kit" using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/sparkfun_particulate_matter_sensor_sps30/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

        - [Services] → [Timers] → [Sleep Timer]
        - [Services] → [IO Stream] → [IO Stream: EUSART] → default instance name: vcom
        - [Application] → [Utility] → [Log]
        - If using the I2C interface: [Third Party Hardware Drivers] → [Sensors] → [SPS30 - Particulate Matter Sensor (Sparkfun) - I2C] → use default configuration
        - If using the UART interface: [Third Party Hardware Drivers] → [Sensors] → [SPS30 - Particulate Matter Sensor (Sparkfun) - UART] → use default configuration

4. Install **printf float**

    - Open Properties of the project.

    - Select C/C++ Build → Settings → Tool Settings → GNU ARM C Linker → General. Check Printf float.

    ![install_float](image/install_float.png)

5. Build and flash this example to the board.


**Note:**

- Make sure that the SDK extension is already installed. If not, please follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- Third-party Drivers Extension must be enabled for the project to install "SPS30 - Particulate Matter Sensor (Sparkfun) - I2C" or "SPS30 - Particulate Matter Sensor (Sparkfun) - UART" component.

## How It Works ##

Driver Layer Diagram is shown in the image below:

![driver_layer](image/driver_layer.png)

After you flash the code to the Explorer Kit and power the connected boards, the application starts running automatically. Use Putty/Tera Term (or another program) to read the values of the serial output. Note that the EFR32xG24 Explorer Kit board uses the default baud rate of 115200.

In the image below, you can see an example how the output is displayed. The main program performs a sensor probing at first, then it reads the serial number and feature set version. The main program starts the measurements and continuously receives measured values ​​(pm1.0, pm2.5, pm4.0, pm10.0 ... concentration).

There is a periodic timer in the code, which determines the sampling intervals; the default sensor sampling rate is 3000 ms. If you need more frequent sampling, it is possible to change the corresponding timer value of the "app.c" file.

![log_i2c](image/log_i2c.png "USB Debug Output Data")
![log_uart](image/log_uart.png "USB Debug Output Data")
## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
