# AS3935 - Thunder Click (Mikroe) #

## Summary ##

This project shows the driver implementation of the Thunder Click (SPI) with the Silicon Labs Platform.

Thunder Click is a compact add-on board that detects the presence and proximity of potentially hazardous lightning activity in the vicinity. This board features the AS3935, a programmable fully integrated lightning sensor from ScioSense with a sensitive coil antenna, and the MA5532 from Coilcraft. Besides detecting potentially hazardous lightning activity, this Click board™ also provides information on the estimated distance to the center of the storm on the noise level. The AS3935 can detect lightning up to 40km away with an accuracy of 1km to the storm front with a sensitive antenna tuned to pick up lightning events in the 500kHz band. This Click board™ is suitable for weather station applications by adding lightning detection to your design to ensure you are aware of any potentially hazardous weather heading your way.

## Required Hardware ##

- [EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit)

- [Thunder Click](https://www.mikroe.com/thunder-click)

## Hardware Connection ##

The Thunder Click board supports MikroBus, so it can connect easily to EFR32xG24 Explorer Kit. Be sure that the board's 45-degree corner matches the Explorer Kit's 45-degree white line.

The hardware connection is shown in the image below:

![board](image/hardware_connection.png)

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add the BRD2703A to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with filter thunder.

2. Click **Create** button on the **Third Party Hardware Drivers - Thunder Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.
![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "EFR32xG24 Explorer Kit" using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_thunder_as3935/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

        - [Services] → [Timers] → [Sleep Timer]
        - [Services] → [IO Stream] → [IO Stream: EUSART] → default instance name: vcom
        - [Application] → [Utility] → [Log]
        - [Third Party Hardware Drivers] → [Sensors] → [AS3935 - Thunder Click (Mikroe)]

4. Modify the SPIDRV configuration of Mikroe instance as below.
    ![mikroe spi config](image/mikroe_spi_config.png)

5. Build and flash this example to the board.

**Note:**

- Make sure that the SDK extension already be installed. If not please follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- Third-party Drivers Extension must be enabled for the project to install "Thunder Click (Mikroe)" component.

## How It Works ##

After you flashed the code to the Explorer Kit and powered the connected boards, the application starts running automatically. Use Putty/Tera Term (or another program) to read the values of the serial output. Note that the EFR32xG24 Explorer Kit board uses the default baud rate of 115200.

In the below image, you can see an example of how the output is displayed. The application returns the energy and the distance of the storm if lighting is detected. Otherwise, it will display "noise" or "disturber" if one of them is detected.

There is a timer in the code, which determines the sampling intervals; the default sensor sampling rate is 1 second. If you need more frequent sampling, it is possible to change the corresponding timer value of the app.c file.

![logging_screen](image/result.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
