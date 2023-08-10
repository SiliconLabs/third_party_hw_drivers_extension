# MAX30101 & MAX32664 - Pulse Oximeter and Heart Rate Sensor (Sparkfun) #

## Summary ##

The SparkFun Pulse Oximeter and Heart Rate Sensor is an I2C based biometric sensor, utilizing two chips from Maxim Integrated: the MAX32664 Biometric Sensor Hub and the MAX30101 Pulse Oximetry and Heart Rate Module. While the latter does all the sensing, the former is an incredibly small and fast Cortex M4 processor that handles all of the algorithmic calculations, digital filtering, pressure/position compensation, advanced R-wave detection, and automatic gain control.

The MAX30101 does all the sensing by utilizing its internal LEDs to bounce light off the arteries and arterioles in your finger's subcutaneous layer and sensing how much light is absorbed with its photodetectors. This is known as photoplethysmography. This data is passed onto and analyzed by the MAX32664 which applies its algorithms to determine heart rate and blood oxygen saturation (SpO2). SpO2 results are reported as the percentage of hemoglobin that is saturated with oxygen. It also provides useful information such as the sensor's confidence in its reporting as well as a handy finger detection data point. To get the most out of the sensor we've written an driver that compatible with EFR32 MCU to make it easy to adjust all the possible configurations.

## Required Hardware ##

- [A EFR32xG24 Explorer Kit board.](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit)

- [SparkFun Pulse Oximeter and Heart Rate Sensor - MAX30101 & MAX32664 (Qwiic)](https://www.sparkfun.com/products/15219)

**NOTE:**
Tested boards for working with this example:

| Board ID | Description  |
| ---------------------- | ------ |
| BRD2704A | [SparkFun Thing Plus Matter - MGM240P - BRD2704A](https://www.sparkfun.com/products/20270) |
| BRD2601B | [EFR32xG24 Dev Kit - xG24-DK2601B](https://www.silabs.com/development-tools/wireless/efr32xg24-dev-kit?tab=overview)   |
| BRD2703A | [EFR32xG24 Explorer Kit - XG24-EK2703A](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)    |
| BRD4108A | [BG22 Bluetooth SoC Explorer Kit - BG22-EK4108A](https://www.silabs.com/development-tools/wireless/bluetooth/bg22-explorer-kit?tab=overview)    |
| BRD4314A | [BGM220 Bluetooth Module Explorer Kit - BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit?tab=overview)    |

## Hardware Connection ##

An MAX30101 & MAX32664 sensor board can be easily connected up with two I2C wires (SDA and SCL) along with 3v3 and GND. For the designated boards, SparkFun Qwiic compatible STEMMA QT connectors can be used.

![hardware_connection](image/hardware_connection.png)

**Note:** This board has two additional pins on its header: the RESET and MFIO pin. These pins are required for the board to function because they determine if the board enters data collection mode or not.

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ##

1. From the Launcher Home, add the BRD2703A to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with filter MAX32664.

2. Click **Create** button on the **Third Party Hardware Drivers - MAX30101 & MAX32664 - Pulse Oximeter and Heart Rate Sensor (Sparkfun)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.
![create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ##

1. Create a "Platform - Empty C Project" project for the "xG24 Explorer Kit (xG24-EK2703A)" using Simplicity Studio v5. Use the default project settings. Be sure to connect and select the xG24 Explorer Kit (xG24-EK2703A) Board from the "Debug Adapters" on the left before creating a project.

2. Copy the file `app/example/sparkfun_hr_po_max30101_max32664/app.c` into the project root folder (overwriting existing file).

3. Set the test mode in the *app.c* file.

4. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

      - [Services] → [IO Stream] → [IO Stream: USART] with the default instance name: vcom.
      - [Services] → [Timer] → [Sleep Timer]
      - [Application] → [Utility] → [Log].
      - [Third Party Hardware Drivers] → [Sensors] → [MAX30101 & MAX32664 - Pulse Oximeter and Heart Rate Sensor (Sparkfun)] → use default configuration

5. Build and flash the project to your device.

**Note:**

- Make sure that the SDK extension already be installed. If not please follow [this documentation](https://github.com/SiliconLabs/platform_hardware_drivers_sdk_extensions/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- SDK Extension must be enabled for the project to install "MAX30101 & MAX32664 - Pulse Oximeter and Heart Rate Sensor (Sparkfun)" component.

## How It Works ##

### MAX30101 - Pulse Oximeter and Heart Rate Monitor ###

The MAX30101 gets your heart rate (BPM) and blood oxygen levels (SpO2) through the process of photoplethysmography, which is the process of obtaining the aforementioned biometric data with light. The SparkFun Pulse Oximeter works by placing your finger gently on the sensor which shines red, infrared, and sometimes green light through your skin. The capillaries filled with blood under your skin will absorb this light, or not, and the MAX30101 sensor will read which light comes back. This light data will then be sent back to the Biometric Sensor Hub which handles all the calculations to determine heart rate and blood oxygen levels.

### MAX32664 - Biometric Sensor Hub ###

The MAX32664 Biometric Sensor Hub is a very small Cortex M4 microcontroller dedicated to receiving the data it receives from the MAX30101 and running the calculations to determine heart rate and blood oxygen. When you're interfacing with the SparkFun Pulse Oximeter and Heart Rate Monitor, you are in effect interfacing with this wicked fast microcontroller. There are a multitude of settings to tailor the sensor to the persons you'll be monitoring made available through the driver library.

### Reference Tables and Sensor Settings ###

#### BioData Information ####

As shown below, the library uses a type that is unique to the SparkFun Pulse Oximeter and Heart Rate Monitor. The name of this type is bio_hub_data_t which is described in the table below

| BioData Members | Information |
| --- | --- |
| body.heart_rate | Heart rate |
| body.oxygen | Blood Oxygen Levels |
| body.confidence | The Sensor's confidence in the reported data. |
| body.status | Finger Detection |
| body.ir_led | Number of IR LED Samples |
| body.red_led | Number of Red LED Samples |

#### BioData Mode 2 ####

In addition to the information above, mode 2 also gives the following two data points.
| BioData Members | Information |
| --- | --- |
| body.r_value | Sp02 r Value |
| body.extStatus | Extended Finger Status Message |

#### Finger Status ####

Below is a reference table for the body.status member which tells you if the sensor has detected a finger or some other object that is not a finger. It relays this information with four numbers: 0-3.
| Status Number | Description |
| --- | --- |
| 0 | No Object Detected |
| 1 | Object Detected |
| 2 | Object Other Than Finger Detected |
| 3 | Finger Detected |

Below is a reference table for the body.exStatus member which is an expansion of the first finger status messaging. This is enabled in mode 2 and contains 8 different values.
| Status Number | Description |
| --- | --- |
| 0 | Success
| 1 | Not Ready
| -1 | Object Detected
| -2 | Excessive Sensor Device Motion
| -3 | No object detected
| -4 | Pressing too hard
| -5 | Object other than finger detected
| -6 | Excessive finger motion

#### Pulse Width vs. Sample Collection ####

There is trade off between higher resolution (i.e. longer pulse width) and the number of samples that you can collect per second. The table below shows how the resolution and sample rate interact.
| Samples/Second | Pulse  | Width | (uS) |
| --- | --- | --- | --- |
| 69 | 118 | 215 | 411
| 50 | O | O | O | O
| 100 | O | O | O | O
| 200 | O | O | O | O
| 400 | O | O | O |
| 800 | O | O | O |
| 1000 | O | O |
| 1600 | O |
| Resolution (bits) | 15 | 16 | 17 | 18 |

### Testing ###

This example demonstrates some of the available features of the Pulse Oximeter and Heart Rate Sensor. Follow the below steps to test the example:

1. On your PC open a terminal program, such as the Console that is integrated in Simplicity Studio or a third-party tool terminal like TeraTerm to receive the logs from the virtual COM port.

2. Try to put your finger on the sensor with a proper pressure (solid contact between the finger and the sensor without optical leakage and don’t press with force) and check the logs on the terminal.

![logging_screen](image/logs.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
