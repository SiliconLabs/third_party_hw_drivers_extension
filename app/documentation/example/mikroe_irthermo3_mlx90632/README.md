# MLX90632 - IrThermo 3 Click (Mikroe) #

## Summary ##

This project aims to show the hardware driver that is used to interface with the contactless temperature sensor via APIs of GSDK.

IRThermo 3 click uses the MLX90632 FIR sensor from Melexis as the contactless temperature sensor. This sensor is a thermopile IR sensor. A thermopile sensor is actually a serially connected thermocouple array, with hot junctions located on the heat-absorbing membrane. It uses a highly advanced MLX90632 FIR sensor, which detects the average temperature of objects within its field of view (FOV), which is typically ±25°. It is factory calibrated, with the calibration constants stored in its internal EEPROM. Besides EEPROM used for storing trimming values, device settings, and calibration constants, it has an additional RAM area that can be used for auxiliary measurement data storage. The integrated temperature sensor provides ambient temperature measurement, necessary for proper calibration. The device is calibrated to sense objects with a temperature ranging from -20 ℃ up to 200 ℃, and an ambient temperature ranging up to 80℃. The measurement resolution is 0.02℃. Measured data is available via the industry standard I2C communication bus.

## Required Hardware ##

- [EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview).

- [IrThermo 3 Click](https://www.mikroe.com/ir-thermo-3-click).

**NOTE:**
Tested boards for working with this example:

| Board ID | Description  |
| ---------------------- | ------ |
| BRD2703A | [EFR32xG24 Explorer Kit - XG24-EK2703A ](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)    |
| BRD4314A | [BGM220 Bluetooth Module Explorer Kit - BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit?tab=overview)  |
| BRD4108A | [EFR32BG22 Explorer Kit Board](https://www.silabs.com/development-tools/wireless/bluetooth/bg22-explorer-kit?tab=overview)  |

## Hardware Connection ##

The IrThermo 3 Click board support MikroBus, so it can connect easily to EFR32xG24 Explorer Kit's MikroBus header. Be sure that the board's 45-degree corner matches the Explorer Kit's 45-degree white line.

The hardware connection is shown in the image below:

![board](image/hardware_connection.png "Hardware connection")

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add the BRD2703A to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with filter mlx90632

2. Click **Create** button on the **Third Party Hardware Drivers - MLX90632 - IrThermo 3 Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.
![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "EFR32xG24 Explorer Kit" using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_irthermo3_mlx90632/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

        - **[Services] → [Timer] → [Sleep Timer]**
        - **[Services] → [IO Stream] → [IO Stream: USART]** → default instance name: vcom
        - **[Application] → [Utility] → [Log]**
        - **[Third Party Hardware Drivers] → [Sensors] → [MLX90632 - IrThermo 3 Click (Mikroe)]**  -> use default configuration.
        ![config](image/default_config.png)

4. Install printf float

    - Open Properties of the Project.

    - Select C/C++ Build → Settings → Tool Settings → GNU ARM C Linker → General. Check Printf float.
    ![install_float](image/float.png)

5. Build and flash this example to the board.

**Note:**

- Make sure that the SDK extension already be installed. If not please follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- SDK Extension must be enabled for the project to install "MLX90632 - IrThermo 3 Click (Mikroe)" component.

## How It Works ##

The application measures ambient and object temperatures and displays data in degree centigrade every second.
You can launch Console that's integrated into Simplicity Studio or use a third-party terminal tool like TeraTerm to receive the data from the USB. A screenshot of the console output and an actual test image are shown in the figure below.

![logging_screen](image/log.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
