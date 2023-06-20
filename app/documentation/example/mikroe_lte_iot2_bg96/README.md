# LTE IoT 2 Click #

## Summary ##

This project shows the implementation of an LTE IoT 2 Click driver using Quectel BG96 LTE module from Mikroe Integrated with EFR32xG24 Explorer Kit based on UART communication.
LTE IoT 2 Click is a Click board™ that allows connection to the LTE networks, featuring Quectel BG96 LTE module, which offers two LTE technologies aimed at Machine to Machine communication (M2M) and the Internet of Things (IoT). This module is an embedded IoT communication solution that supports the LTE Cat M1 and NB1 technologies, offering an alternative to similar Low Power Wide Area Network (LPWAN) solutions, such as the ones provided by Sigfox and LoRa. The LTE CAT1 and NB1 technologies are designed with specific requirements of the IoT network in mind. LTE IoT 2 click also offers various other features, allowing simple and reliable connection to these new 3GPP IoT technologies.

## Required Hardware ##

- [**EFR32xG24-EK2703A** EFR32xG24 Explorer Kit (BRD2703A xG24 Explorer Kit Board)](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview).

- [**LTE IoT 2 Click** board based on Quectel BG96 LTE module from Mikroe Integrated](https://www.mikroe.com/lte-iot-2-click).

**NOTE:**
Tested boards for working with this example:

| Board ID | Description  |
| ---------------------- | ------ |
| BRD2703A | [EFR32xG24 Explorer Kit - XG24-EK2703A ](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)    |
| BRD4314A | [BGM220 Bluetooth Module Explorer Kit - BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit?tab=overview)  |
| BRD4108A | [EFR32BG22 Explorer Kit Board](https://www.silabs.com/development-tools/wireless/bluetooth/bg22-explorer-kit?tab=overview)  |

## Hardware Connection ##

The LTE IoT 2 Click board can just be "clicked" into its place. Be sure that the board's 45-degree corner matches the Explorer Kit's 45-degree white line. The GSM antenna needs to be connected to the LTE IOT 2 Click board to ensure that the mobile network signal can be received. And also a SIM card is properly attached to the module. If the user using the GPS function then the GPS antenna needs to be connected to the LTE IoT 2 Click board to ensure that the satellite signals can be received.

![board_1](image/hardware_connection_1.png "BRD2703A xG24 Explorer Kit Board and LTE IoT 2 Click Board")
![board_2](image/hardware_connection_2.png "BRD2703A xG24 Explorer Kit Board and LTE IoT 2 Click Board")
![board_3](image/hardware_connection_3.png "BRD2703A xG24 Explorer Kit Board and LTE IoT 2 Click Board")

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add the BRD2703A  to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with the filter 'lte iot 2'.

2. Click **Create** button on the **Third Party Hardware Drivers - BG96 - LTE IoT 2 CLick (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.
![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "EFR32xG24 Explorer Kit Explorer Kit Board" using Simplicity Studio v5. Use the default project settings.

2. Copy the file [app.c](https://github.com/SiliconLabs/third_party_hw_drivers_extension/tree/master/app/example/mikroe_lte_iot2_bg96) (overwriting existing file), [app_iostream_cli.c](https://github.com/SiliconLabs/third_party_hw_drivers_extension/tree/master/app/example/mikroe_lte_iot2_bg96) and [app_iostream_cli.h](https://github.com/SiliconLabs/third_party_hw_drivers_extension/tree/master/app/example/mikroe_lte_iot2_bg96) into the project root folder.

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

        - [Services] → [Sleep Timer]
        - [Services] → [IO Stream] → [IO Stream: EUSART] → default instance name: vcom
        - [Application] → [Utility] → [Log]
        - [Application] → [Utility] → [Application Queue]
        - [Third-Party Hardware Drivers] → [Sensors] → [BG96 - LTE IoT 2 Click (Mikroe)]

4. Build and flash this example to the board.

**Note:**

- Make sure the Third-party Hardware Drivers Extension already be installed. If not please follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md).

- Third-party Hardware Drivers Extension must be enabled for the project to install "BG96- LTE IoT 2 Click (Mikroe)" component.
- The example project is built on the BRD2703A board. For other boards, selecting the "BG96 - LTE IoT 2 Click (Mikroe)" component will include the "UART" component with an unconfigured instance. This instance should be configurated by users.

## How It Works ##

- The Third Party Hardware Drivers - BG96 - LTE IoT 2 CLick (Mikroe) driver consists of three driver layers and an application layer on the top. The hardware peripheral configuration and low-level handling can be found on the lowest level.
- The core driver schedules the sending of AT commands, maintains a command queue and handles the received responses. It also achieves an error handling that works similarly to exception handling in high-level programming languages. If any error occurs during the communication it can interrupt the command scheduler and report the appropriate error message to the high-level programming interface.
- The high-level functions prepare the AT commands, extend them with parameters if necessary, put them into the command queue and start the scheduler. The basic functions like LTE network configuration, sending mobile sms, data send/receive on a TCP/IP socket and GPS positioning are already implemented but it is possible to write custom high-level functions too. You will find the detailed documentation in the code and a brief example of usage below.

### BG96 - LTE IoT 2 Click general high-level functions ###

- *bg96_init()*
- *bg96_is_alive()*
- *bg96_sleep()*
- *bg96_wake_up()*

### BG96 - LTE IoT 2 Click functional high-level functions ###

- *bg96_nb_init(void)*
- *bg96_nb_open_connection()*
- *bg96_network_registration()*
- *bg96_nb_close_connection()*
- *bg96_nb_send_data()*
- *bg96_nb_receive_data()*
- *bg96_read_ip()*
- *bg96_read_imei()*
- *bg96_read_infor()*
- *bg96_get_operator()*
- *bg96_set_te_gsm()*
- *bg96_config_service_domain()*
- *bg96_set_sms_mode()*
- *bg96_send_sms_text()*
- *bg96_send_sms_pdu()*
- *bg96_set_sim_apn()*
- *bg96_gnss_start()*
- *bg96_gnss_get_position()*
- *bg96_gnss_stop()*

## Driver Layer Overview ##

![layer](image/layer.drawio.png)

## Driver Extension Guide ##

### Adding New AT Command ###

To add new AT command to the parser you need to define an AT command line callback which depends on the required response of the command.
The possible AT command responses are defined in the [User manual](https://docs.particle.io/assets/pdfs/Quectel_BG96_AT_Commands_Manual_V2.1.pdf) of the BG96 module. In some cases, one of the predefined line callbacks can be used. e.g.: *at_ok_error_cb()* if the command response is OK or ERROR.

1. Define the appropriate line callback function in *at_parser_core.c*. You can find an example below.

    ```c
    void at_custom_cb(uint8_t *new_line, uint8_t call_number)
      //call_number refers the number of new lines received as response
      //new_line points to a string with the new line data
      switch (call_number) {
      case 1:
        //the value of the new line can be parsed
        if (has_substring(new_line, "OK")) {
            //in case of the last response arrived scheduler can be stepped forward
            at_parser_scheduler_next_cmd();
        }
        if (has_substring(new_line, "ERROR")) {
            //data can be reported to the caller
            at_parser_report_data(new_line);
            //in case of error scheduler can be stopped and error code can be reported
            at_parser_scheduler_error(SL_STATUS_FAIL);
        }
        break;
      default:
        //default case always SHALL be defined
        at_parser_scheduler_error(SL_STATUS_FAIL);
        break;
      }
    ```

2. Add the core process function to the main loop.

    ```c
    void app_process_action (void)
    {
      at_parser_process ();
      ...
    }
    ```

3. The definition of AT command SHALL be implemented in a high-level function. See the section **Creating New High Level Function**.

### Creating New High Level Function ###

Creating a high-level function means a single AT command or a list of AT commands which should be sent after each other to the BG96 IoT module and process their response, indicate errors and define the user data. The predefined high-level function can be found in *mikroe_bg96.c* files.

  ```c
  sl_status_t custom_high_level_function( user_data_t *custom_user_data,          //you can use user data
                                          at_scheduler_status_t *output_object)   //you MUST define an output_object to receive the response
  {
      //define command status variable as SL_STATUS_OK, this will collect the errors of low level functions
    sl_status_t cmd_status = SL_STATUS_OK;
    //define the base and additional parts of the AT command, additional parts can be prameters inherited from user data 
    uint8_t extend_str[]= "0,0",
    uint8_t base_cmd[] = "AT+QICLOSE=";

    //create a STATIC command descriptor, this descriptor MUST be allocated during the AT command parsing process!
    static at_cmd_desc_t at_custom = { .cms_string="",                     //use an empty cmd_string if there are additional parameters
                                      .ln_cb=at_custom_cb,                //add the appropriate line callback function 
                                      .timeout_ms = AT_DEFAULT_TIMEOUT    //define a timeout for the command (check the BG96 datasheet for the decent timeout)
                                      };
      //prepare the command descriptor (clears the command string)
      at_parser_clear_cmd(&at_custom);

      //for the following functions use validate() function to indicate the errors!
      //extend command descriptor command string with the base command
      validate(cmd_status, at_parser_extend_cmd(&at_custom, base_cmd));
      //extend command descriptor command string with the additional parts
      validate(cmd_status, at_parser_extend_cmd(&at_custom, conn_string));
      //add the command descriptor to the command queue, queue is available only if previous high level function finished
      validate(cmd_status, at_parser_add_cmd_to_q(&at_custom));
      //start command scheduler, scheduler runs until the command queue is empty or error is occourred.
      validate(cmd_status, at_parser_start_scheduler(output_object));
    return cmd_status;
  }
  ```

### Creating New CLI Command ###

1. Add a new element to the *cli_cmds[]* array in *app_iostream_cli.c*. The new element SHALL contain a CLI command string and a perform function. Use a short command and DO NOT use spaces and special characters! The maximum length of a CLI command is defined in *CLI_CMD_LENGTH* macro in *app_iostream_cli.h* and is 10 by default.

    ```c
    static cli_cmd_t cli_cmds[] = {
                                    { "imei", imei },
                                    { "ip", ip },
                                    //you can define new CLI commands here
                                    {"custom_cmd", custom_cmd}
                                    };
    ```

2. Define the CLI command to perform a function. This is the function defined in the new element of *cli_cmd[]* array. Use static keywords!

    ```c
    static void custom_cmd()
    {
        //prepare an output object for receiving data from a high level function
        at_parser_init_output_object(&output_object);
        //call a high level function
        custom_high_level_function(&output_object);
        //create an event listener for the output object and define the event handler function
        at_listen_event(&output_object.status,      //pointer to a variable to check
                        SL_STATUS_OK,               //the state the status variable should be equal to
                        custom_cmd_handler,         //event handler function
                        (void*) &output_object);    //parameter of event handler function

        //you can write a short log if command perform function has run. Do not forget the \r\n at the end of the message!
        printf("Custom command has been performed!\r\n");
    }
    ```

3. Define the event handler function you gave in the event listener parameter list.

    ```c
    static void custom_cmd_handler(void *handler_data)
    {
        //handler data is a pointer to the user data passed by the event listener
        //at_scheduler_state is a good option to propagate because it contains the AT command response
        at_scheduler_status_t *l_output = (at_scheduler_status_t*) handler_data;

        //check the error code and handle it
        if (l_output->error_code) {
            printf("Error while running custom command %d\r\n", l_output->error_code);
        } else {
            //print the response data if it was reported by the AT command line callback
            printf("Custom command response: %s\r\n", l_output->response_data);
        }
    }
    ```

4. Add the event listener and IOstream process functions to the main loop.

    ```c
      void app_process_action (void)
      {
          app_iostream_usart_process_action ();
          at_event_process ();
          ...
      }
    ```

### Testing ###

- You can launch Console that's integrated into Simplicity Studio or use a third-party terminal tool like Tera Term to receive the data from the USB. In this example, we build a simple command line interface application. There are commonly used commands predefined in the "app_iostream_cli.c" file. When the user wants to test a feature, just call the corresponding commands.
- First, after the chip has finished booting, the user needs to start the LTE IoT 2 module. Enter the command "wakeup" into Tera Term and end with an "Enter" key. A logline is printed confirming the command has been executed. The lower layer library will control the GPIO pin connected to the PWK pin to power the LTE IoT 2 module. After a few seconds the module boots up, and it will respond with a successful boot confirmation.
- After the LTE IoT 2 module boots up successfully, users can check basic parameters such as getting imei code, and getting revision by "imei", "infor" commands. The user then calls the "service" command to configure the service domain type CS and PS. Next, the user calls the command "gsm" to select TE character set to GSM.
- In this example, we send a text message from LTE IoT 2 Click module to a given phone number. There are two operating modes "textmode" and "pdumode" need to call these 2 commands first to put the module into operation mode respectively. The user can then invoke the "smstext" and "smspdu" commands to send the message. After confirming the successful message delivery, there will be a response, which contains the reference information of that sms. 
- In addition, we also provide an example using the GPS function. First, users need to turn on the GPS module with the "gpsstart" command. After the LTE IoT 2 Click module captures the satellite signal, the user can get the location coordinates with the "location" command. After getting the results, users can use the longitude and latitude values ​​entered into Google Maps to check the actual location. In addition, the command also returns information such as UTC, altitude, date, etc. After use, the user can turn off the GPS function with the command "gpsstop", this is to save energy for the system.
- A screenshot of the console output is shown in the figure below:

  ![usb_debug](image/log.png "USB Debug Output Data")

  ![usb_debug_gps](image/gps_log.png "USB Debug Output Data GPS")
  
  ![sms_img](image/sms_receive.png "SMS Receive")

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.