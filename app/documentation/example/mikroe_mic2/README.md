# MIC 2 Click #

## Summary ##

This example project shows an example of Mikroe MIC 2 Click board driver integration with the Silicon Labs Platform.

MIC 2 click is equipped with a small electret microphone, accompanied by a suitable pre-amplifying circuit. The small omnidirectional electret microphone features a reasonably flat frequency response, and it can be used within the frequency range between 100 Hz and 20 kHz. This range is perfectly suited for audio and/or speech applications. It can be used for the development of various audio and speech-related applications: speech or audio digitizing, digital audio monitoring and surveillance, digital "bugs", etc.

This example can be used to determine the sound intensity of your location. The combination of the EFR32xG24 Explorer Kit and Mikroe MIC 2 Click can act as a sound intensity measurement device.

## Required Hardware ##

- [**EFR32xG24-EK2703A** EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview).

- [Mikroe MIC 2 Click](https://www.mikroe.com/mic-2-click).

**NOTE:**
Tested boards for working with this example:

| Board ID | Description  |
| ---------------------- | ------ |
| BRD2703A | [xG24-EK2703A - EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)    |
| BRD4314A | [BGM220-EK4314A - BGM220 Bluetooth Module Explorer Kit](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit?tab=overview)  |
| BRD4108A | [BG22-EK4108A - EFR32BG22 Explorer Kit Board](https://www.silabs.com/development-tools/wireless/bluetooth/bg22-explorer-kit?tab=overview)  |

## Hardware Connection ##

The MIC 2 Click board supports MikroBus, so it can connect easily to EFR32xG24 Explorer Kit's MikroBus header. Be sure that the board's 45-degree corner matches the Explorer Kit's 45-degree white line.

The hardware connection is shown in the image below:

![hardware_connection](image/hardware_connection.png)

## Setup ##

You can either create a project based on an example project or start with an empty example project.

1. From the Launcher Home, add the BRD2703A to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with the filter **"mic 2"**.

2. Click **Create** button on the **Third Party Hardware Drivers - MIC 2 Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.
![create_project](image/create_example.png)

### Start with an empty example project ###

1. Create an "Empty C Project" for the "EFR32xG24 Explorer Kit Board" using Simplicity Studio v5. Use the default project settings.

2. Copy the file app/example/mikroe_mic2/app.c (overwriting the existing file), into the project root folder.

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

        - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: **vcom**

        - [Application] → [Utility] → [Log]

        - [Services] → [Timers] → [Sleep Timer]

        - [Third Party Hardware Drivers] → [Audio & Voice] → [MIC 2 Click (Mikroe)] → use the default configuration.

4. Install printf float

    - Open Properties of the Project.

    - Select C/C++ Build → Settings → Tool Settings → GNU ARM C Linker → General. Check Printf float.
    ![install_float](image/install_float.png)

5. Build and flash this example to the board.

**Note:**

- Make sure the SDK extension already be installed. If not please follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- SDK Extension must be enabled for the project to install the "MIC 2 Click (Mikroe)" component. Selecting this component will also include the "I2CSPM" component with instance "mikroe" and "IADC" component.

## How It Works ##

### Driver Layer Diagram ###

![software_layer](image/software_layer.png)

### Testing ###

This example reads the ADC value and converts this value to Decibel (dB) value. With decibel value, you can perceive the noise level of the surrounding environment.

You can launch Console that's integrated into Simplicity Studio or use a third-party terminal tool like TeraTerm to receive the data from the USB. A screenshot of the console output is shown in the figure below.

![console_log](image/console_log.png)

This example uses the I2CSPM diver with the "Mikroe" instance and IADC0 to read the analog signal. After the sensor is initialed successfully, the application reads the analog signal every 500 milliseconds. Right after getting this value, it will be converted to a Decibel value.

To test the functionality, you can change the sound intensity surrounding the sensor in many ways. You can talk louder or applaud near the sensor. Then you will see the Decibel value change on the console in Simplicity Studio or other third-party terminal tools.


## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.