# LM75A - Temperature Sensor #

## Summary ##

This project shows the integration of the [LM75A Temp Sensor](https://www.ti.com/lit/ds/symlink/lm75a.pdf?ts=1771677975375&ref_url=https://www.ti.com/product/LM75A) with I2C driver APIs using Silicon Labs platform.

The LM75A is a temperature-to-digital converter that uses an on-chip band-gap temperature sensor and a sigma-delta analog-to-digital conversion technique. The device also functions as a thermal detector with an over-temperature detection output.
## Table Of Contents ##

- [Required Hardware](#required-hardware)
- [Hardware Connection](#hardware-connection)
- [Setup](#setup)
  - [Create a project based on an example project](#create-a-project-based-on-an-example-project)
  - [Start with an empty example project](#start-with-an-empty-example-project)
- [How It Works](#how-it-works)
- [Report Bugs & Get Support](#report-bugs--get-support)

## Required Hardware ##

- 1x [Silicon Labs Series 2 Development Kit](https://www.silabs.com/development-tools/wireless/) based on the EFR32 SoC, such as:
  - [XG24-RB4187c](https://www.silabs.com/development-tools/wireless/xg24-rb4187c-efr32xg24-wireless-gecko-radio-board?tab=overview) + [Si-MB4002A](https://www.silabs.com/development-tools/wireless/wireless-pro-kit-mainboard?tab=overview)
  - [XG24-RB4186C](https://www.silabs.com/development-tools/wireless/xg24-rb4186c-efr32xg24-wireless-gecko-radio-board?tab=overview) + [Si-MB4002A](https://www.silabs.com/development-tools/wireless/wireless-pro-kit-mainboard?tab=overview)
  - [xG21-SLWRB4181C](https://www.silabs.com/development-tools/wireless/slwrb4181c-efr32xg21-wireless-gecko-radio-board?tab=overview) + [Si-MB4002A](https://www.silabs.com/development-tools/wireless/wireless-pro-kit-mainboard?tab=overview)
  - [xG22-SLWRB4182A](https://www.silabs.com/development-tools/wireless/slwrb4182a-efr32xg22-wireless-gecko-radio-board?tab=overview) + [Si-MB4002A](https://www.silabs.com/development-tools/wireless/wireless-pro-kit-mainboard?tab=overview)
  - [xG23-RB4204D](https://www.silabs.com/development-tools/wireless/xg23-rb4204d-efr32xg23-868-915-mhz-14-dbm-radio-board?tab=overview) + [Si-MB4002A](https://www.silabs.com/development-tools/wireless/wireless-pro-kit-mainboard?tab=overview)
  - [xG28-RB4401C](https://www.silabs.com/development-tools/wireless/xg28-rb4401c-efr32xg28-2-4-ghz-ble-and-20-dbm-radio-board?tab=overview) + [Si-MB4002A](https://www.silabs.com/development-tools/wireless/wireless-pro-kit-mainboard?tab=overview)

  *or*

- 1x [Silicon Labs Wi-Fi Development Kit](https://www.silabs.com/development-tools/wireless/wi-fi) based on SiWG917, such as:
  - [SiW917Y-RB4343A](https://www.silabs.com/development-tools/wireless/wi-fi/siw917y-rb4343a-wi-fi-6-bluetooth-le-8mb-flash-radio-board-for-module?tab=overview) + [Si-MB4002A](https://www.silabs.com/development-tools/wireless/wireless-pro-kit-mainboard?tab=overview)
  - [SiWx917-DK2605A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit?tab=overview)
  - [SiWx917-EK2708A](https://www.silabs.com/development-tools/wireless/wi-fi/siw917y-ek2708a-explorer-kit?tab=overview)

- 1x [LM75A Temperature Sensor](https://www.ti.com/lit/ds/symlink/lm75a.pdf?ts=1771677975375&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLM75A)
## Hardware Connection ##

In this setup, the LM75A temperature sensor is interfaced with the Silicon Labs board using female to female jumper wires, as shown in the figure.

![connection](image/connection.png)

The tables below provide an overview of the pin connections.

**Silicon Labs Series 2 Development Kit:**

| Description | BRD4187C | BRD4186C | BRD4181C | BRD4182A | BRD4204D | BRD4401C | ↔ | LM75A Temperature Sensor Breakout |
| --- | --- | --- | --- | --- | --- | --- | --- |  --- |
| I2C_SDA | PC3 | PC3 | PC3 | PC3 | PC3 | PA13 | ↔ | SDA |
| I2C_SCL | PD2 | PD2 | PD2 | PD2 | PD2 | PD12 | ↔ | SCL |

**Silicon Labs Wi-Fi Development Kit:**

| Description | BRD4343A + BRD4002A | BRD2605A | BRD2708A | ↔ | LM75A Temperature Sensor Breakout |
| --- | --- | --- | --- | --- | --- |
| I2C_SDA | ULP_GPIO_6 [EXP_16] | ULP_GPIO_6 | GPIO_6 | ↔ | SDA |
| I2C_SCL | ULP_GPIO_7 [EXP_15] | ULP_GPIO_7 | GPIO_7 | ↔ | SCL |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
>
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add used board BRD4187C(XG24) or BRD4343A(SiWx917)  to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **LM75A**.

2. Click **Create** button on the **Third Party Hardware Drivers - LM75A - Temperature Sensor** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/lm75a_temp_sensor/app.c` into the project root folder (overwriting the existing file).

3. Open the .slcp file. Select the **SOFTWARE COMPONENTS** tab and install the following components:

   - **If the Series 2 Development Kit is used:**
     - [Services] → [Timers] → [Sleep Timer]
     - [Services] → [IO Stream] → [IO Stream: USART] → instance name: **vcom**
     - [Application] →  [Utility] → [Log]
     - [Platform] → [Board] → [Board Control] → Enable **Virtual COM UART**
     - [Platform] → [Driver] → [I2C] → [I2CSPM] → default instance name: **i2c_sensor** → Select the corresponding pins according to the table provided in [Hardware Connection](#hardware-connection)
     - [Third Party Hardware Drivers] → [Sensors] → [**LM75A - Temperature Sensor**]

   - **If the Wi-Fi Development Kit is used:**
     - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Services] → [Sleep Timer for Si91x]
     - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [I2C] → [i2c2] → Select the corresponding pins according to the table provided in [Hardware Connection](#hardware-connection)
     - [Third Party Hardware Drivers] → [Sensors] → [**LM75A - Temperature Sensor**]

4. Build and flash the project to your device.

## How It Works ##

This example demonstrates how a microcontroller communicates with the **LM75A digital temperature sensor** over the **I2C bus** to measure ambient temperature and display it in human-readable form.

### Working Principle ###

The **LM75A** is a temperature sensor that continuously measures temperature internally and stores the result in a **temperature register**. The host MCU (SI91x or Series 2 device) acts as an **I2C master**, while the LM75A acts as an **I2C slave**.

The overall workflow is:

1. **I2C Initialization**
  - The application selects the I2C instance to be used for communication with the LM75A sensor.
  - The selected I2C handle is passed to the driver through the initialization API.
  - The driver initializes the selected I2C interface and prepares communication with the LM75A sensor.

2. **Register-Based Communication**
  - The LM75A is accessed using register addressing.
  - To read temperature:
    - The MCU first writes the **temperature register** address to the sensor.
    - Then it performs a read operation to retrieve the temperature data.

3. **Raw Data Format**
  - The LM75A returns temperature as a **2-byte** value.
  - This value is:
    - Left-aligned
    - Encoded in a **signed format**
  - The driver shifts and scales this raw value to obtain a meaningful temperature.

4. **Temperature Conversion**
  - The raw value is converted into **degrees Celsius** using sensor resolution (0.5°C per step in this implementation).
  - The application also converts Celsius to **Fahrenheit** for display.

5. This application periodically monitors the temperature and displays the temperature in Celsius and Fahrenheit every second for 30 seconds, then displays the temperature once every minute thereafter.

### API Overview ###

The LM75A driver is designed with a **simple and minimal API** set, focused on initialization, data acquisition, and utility conversion.

**APIs Provided by the Driver**

`sl_status_t lm75_init(void *i2c_instance, void *i2c_config);` - **Initialization API**
  
**Purpose:**

- Initializes the LM75A driver using the I2C instance selected by the application.
- Allows users to select the desired I2C interface from the application layer.
- For SI91x, the application passes both the I2C instance and the corresponding I2C configuration.
- For Series 2, the application passes the desired I2CSPM instance and `NULL` for the configuration parameter.

**Note:** To use a different I2C interface, update the I2C instance selection in `app.c`. For SI91x devices, update both the I2C instance and the corresponding I2C configuration. For Series 2 devices, update the I2CSPM instance used by the application.

`sl_status_t lm75_get_temperature(int16_t *temp_c);` - **Temperature Read API**

**Purpose:**

- Reads the current temperature from the LM75A by performing an I2C write-read transaction.
- Converts the raw sensor data into fixed-point Celsius × 100 format.
- Returns the converted temperature through the output pointer.
- Returns an appropriate status code to indicate success or failure.

### Testing ###

Use PuTTY, Tera Term, or the Serial Console (right-click on the hardware under Debug Adapters → Launch Console → Serial1) to view the serial output. Configure the connection with the default serial settings: **baud rate 115200**, **8 data bits**, **no parity**, **1 stop bit**, and Enabling **flow control**. You should see output similar to the example below.

![console](image/console.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
