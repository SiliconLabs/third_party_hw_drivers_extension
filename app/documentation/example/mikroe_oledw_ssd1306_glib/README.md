# SSD1306 - Mikroe OLEDW Click (Mikroe) with GLIB #

## Summary ##

This project shows the implementation of the OLED driver using a **Mikroe OLED W Click** with **BGM220 Explorer Kit** based on I2C communication.

MikroE OLED W Click is based on the MI9639BO-W OLED module which has a size of 19.3x7.8 mm and resolution of 96x39 pixels. This white light monochrome, passive matrix OLED display is from Multi-Inno Technology. The MI9639BO-W display features an SSD1306 device that is a 128x64 pixel, dot-matrix OLED/PLED segment/common driver with a controller. It can be used for applications where bright and crisp white text or icons are needed.

For more information about the SSD1306 controller, see the [specification page](https://www.mikroe.com/oled-w-click).

>Please note that this is a monochrome OLED driver.

## Required Hardware ##

- [**BGM220-EK4314A** BGM220 Bluetooth Module Explorer Kit (BRD4314A BGM220 Explorer Kit Board)](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit)

- [**Mikroe OLED W Click** board based on SSD1306 IC](https://www.mikroe.com/oled-w-click).

## Hardware Connection ##

You simply connect a *Mikroe OLED W Click board* to a *BGM220 Explorer Kit* board using a Qwiic cable.

**Note:** There are several jumpers on *Mikroe OLED W Click board* that can be changed to facilitate several different functions. The first of which is the ADDR jumper. The ADDR jumper can be used to change the I2C address of the board. The default jumper is open by default, pulling the address pin high and giving us an I2C address of 0X3D. Closing this jumper will ground the address pin, giving us an I2C address of 0X3C.

The second of which is the I2C pull-up jumper. If multiple boards are connected to the I2C bus, the equivalent resistance goes down, increasing your pull up strength. If multiple boards are connected on the same bus, make sure only one board has the pull-up resistors connected.

![hardware_connection](image/hardware_connection.png)

## Setup ##

To test this application, you should connect the BMG220 Explorer Kit Board to the PC using a microUSB cable.

### Create a project based on an example project ###

1. From the Launcher Home, add the BRD4314A to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with filter "mikroe", "oledw".

2. Click **Create** button on the **Third Party Hardware Drivers - SSD1306 - Mikroe OLEDW Click (Mikroe) with GLIB** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.
![create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create a "Platform - Empty C Example" project for the "BGM220 Explorer Kit Board" using Simplicity Studio 5. Use the default project settings. Be sure to connect and select the BGM220 Explorer Kit Board from the "Debug Adapters" on the left before creating a project.

2. Copy the file `app/example/mikroe_oledw_ssd1306_glib/app.c` into the project root folder (overwriting existing file).

3. Install the software components:
    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

        - [Services] → [Timers] → [Sleep Timer]
        - [Platform] → [Driver]→ [Button] → [Simple Button] → default instance name: **btn0**.
        - [Third Party Hardware Drivers] → [Display & LED] → [SSD1306 - OLED W Click (Mikroe) - SPI]  → use default config
        - [Third Party Hardware Drivers] → [Services] → [GLIB - OLED Graphics Library]

4. Build and flash the project to your device.

**Note:**

- Make sure that the SDK extension already be installed. If not please follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- SDK Extension must be enabled for the project to install "SSD1306 - OLED W Click (Mikroe) - SPI" component.

## How It Works ##

After initialization, the OLED displays the text with the font 6x8. If Button 0 is pressed the OLED will display an image with some graphic effects.

The OLED display will look something like the GIF below.

![testing](image/testing_result.gif)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
