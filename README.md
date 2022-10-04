# Third Party Hardware Drivers #

This repo contains example drivers developed by Silicon Labs engineers and are provided as [an SDK extension](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-getting-started/install-sdk-extensions). These drivers interface with different types of external hardware and are tested against a specific device listed in the driver name. These drivers are intended to be fully compatible with Silicon Labs' wireless stacks. All drivers provide a minimal Simplicity Studio project for testing and to show an example usage of the driver. The driver source is provided for modification and extension as needed. All drivers in this repository are considered to be EVALUATION QUALITY which implies that the code provided in the repos is subjected to limited testing and is provided provided as-is. It is not suitable for production environments.


A full list of all drivers in this repository is defined below:

| Driver name | Link to driver | Link to example | Link to documentation |
|:----------|:------------------:|:----------:|:------------------:|
| captouch2_cap1166 | [Click Here](./driver/public/mikroe/captouch2_cap1166) | [Click Here](./app/example/mikroe_captouch2_cap1166) | [Click Here](./app/documentation/example/mikroe_captouch2_cap1166) |
| heartrate2_maxm86161 | [Click Here](./driver/public/mikroe/heartrate2_maxm86161) | [Click Here](./app/example/mikroe_heartrate2_maxm86161) | [Click Here](./app/documentation/example/mikroe_heartrate2_maxm86161) |
| temphum9_shtc3 | [Click Here](./driver/public/mikroe/temphum9_shtc3) | [Click Here](./app/example/mikroe_temphum9_shtc3) | [Click Here](./app/documentation/example/mikroe_temphum9_shtc3) |
| oledw_ssd1306 | [Click Here](./driver/public/mikroe/oledw_ssd1306) | [Click Here](./app/example/oledw_ssd1306) | [Click Here](./app/documentation/example/oledw_ssd1306) |
| buzz2_cmt_8540s_smt | [Click Here](./driver/public/mikroe/buzz2_cmt_8540s_smt) | [Click Here](./app/example/mikroe_buzz2_cmt_8540s_smt) | [Click Here](./app/documentation/example/mikroe_buzz2_cmt_8540s_smt) |
| brushless16_lb11685av | [Click Here](./driver/public/mikroe/brushless16_lb11685av) | [Click Here](./app/example/mikroe_brushless16_lb11685av) | [Click Here](./app/documentation/example/mikroe_brushless16_lb11685av) |
| accel5_bma400 | [Click Here](./driver/public/mikroe/accel5_bma400) | [Click Here](./app/example/mikroe_accel5_bma400) | [Click Here](./app/documentation/example/mikroe_accel5_bma400) |
| dosimeter_type5 | [Click Here](./driver/public/silabs/dosimeter_type5) | [Click Here](./app/example/sparkfun_dosimeter_type5) | [Click Here](./app/documentation/example/sparkfun_dosimeter_type5) |
| micro_oled_ssd1306 | [Click Here](./driver/public/silabs/micro_oled_ssd1306) | [Click Here](./app/example/sparkfun_micro_oled_ssd1306) | [Click Here](./app/documentation/example/sparkfun_micro_oled_ssd1306) |
| rfid_id12la | [Click Here](./driver/public/silabs/rfid_id12la) | [Click Here](./app/example/sparkfun_rfid_id12la) | [Click Here](./app/documentation/example/sparkfun_rfid_id12la) |

## Requirements ##

- Desired Silicon Labs Starter Kit
- Driver specific test hardware
- Simplicity Studio v5.6.0.0 or newer
- Gecko SDK Suite 4.2.0, available via Simplicity Studio

## How to add to Simplicity Studio IDE ##

**Detailed documentation available [here](doc/readme.md)**.


- Clone the repository somewhere on your PC

  `git clone https://github.com/SiliconLabs/platform_hardware_drivers_sdk_extensions.git`

- Add the SDK extension to the GSDK
  
  - In Simplicity Studio go to **Preferences** → **Simplicity Studio** → **SDKs** and select the Gecko SDK Suite to which the SDK extension will be added. Click **Add Extension…**
  - Click **Browse** and navigate to the root folder of the package above and click **Select Folder**.
  - The SDK extension should be displayed in the Detected SDK Extension window with the **Third Party Hardware Drivers** name, version, and path. Click **OK** and then **Trust** and **Apply and Close**.
  - Restart Simplicity Studio

> Make sure that **Evaluation** quality software components are enabled in the Software Component view.


## SDK Outline ##

The Third Party Hardware Drivers SDK contains the following files and folders.

```txt
  +--third_party_hw_drivers.slce  > The UC SDK Extension description file
  +--third_party_hw_drivers.slsdk > File provides the ability to auto-scan protocols from the extension folder
  +--templates.xml                > This file contains meta data for showing the project example on Simplicity Studio
  +--app
  |  +--documentation             > Contains the simple documentations for each application example
  |  +--example                   > Simpler examples to demonstrate specific features
  |-
  +--driver
  |  +--component                 > Contains UC component description files (SLCC)
  |  +--inc                       > Contains Third Party Hardware Driver SDK Extension helpers file
  |  +--peripheral_drivers        > Contains the implementation of the peripheral driver layer's components.
  |-
  +--public                      > Contains the public interfaces for each integrated driver
  |  +--mikroe                   > Contains the public interfaces for integrated drivers from MikroE
  |     +--temphum9_shtc3
  |     +--heartrate2_maxm86161
  |     +--...
  |  +--sparkfun                 > Contains the public interfaces for integrated drivers from SparkFun
  |  +--silabs                   > Contains the public interfaces for integrated drivers from Silabs
  |  +--...
  |-
  +--third_party                 > Contains third-party libraries, and repositories like Mikroe Click library.
  |  +--mikrosdk_click_v2        > Contains the MikroE Click Board™ library sources
  |  +--...
  +--doc                         > Contains the doxygen documentation files
  |-
```

## Reporting Bugs/Issues and Posting Questions and Comments ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of this repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of this repo.
