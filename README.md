# Smart Pet Feeder — STM32F407 + ESP32

A remotely controlled automatic pet feeder built using **STM32F407 bare-metal drivers** and **ESP32 Arduino framework**.

The system uses a dual-MCU architecture: the STM32F407 handles feeding operations, hardware control, and scheduling, while the ESP32 provides Wi-Fi/BLE connectivity and remote communication through a web server.

## System Architecture

```text
                 Smartphone / Web Browser
                           |
                     Wi-Fi / BLE
                           |
                    ESP32-WROOM-32
                  ┌─────────────────┐
                  │ Wi-Fi Manager   │
                  │ BLE Manager    │
                  │ Web Server     │
                  │ NTP / Schedule │
                  │ UART Interface │
                  └────────┬────────┘
                           |
                     UART 115200
                           |
                  ┌────────▼────────┐
                  │   STM32F407    │
                  │                 │
                  │ Communication  │
                  │ Feeder Control  │
                  │ RTC / Schedule  │
                  │ Button Handler  │
                  │ LED / Buzzer    │
                  └────────┬────────┘
                           |
             ┌─────────────┼─────────────┐
             |             |             |
           Servo        DS3231       Indicators
          (PWM)          (I2C)        (GPIO)
             |             |             |
          Food Gate    Real-Time     LED / Buzzer
                       Clock
```

## Key Features

| Feature | Description |
|---|---|
| Manual Feeding | Trigger feeding through a physical button or remote command |
| Scheduled Feeding | Automatically dispense food according to configured schedules |
| Remote Control | Control feeding operations through a web interface |
| Wi-Fi Connectivity | ESP32 connects the feeder to a wireless network |
| BLE Provisioning | Configure Wi-Fi credentials through Bluetooth Low Energy |
| Real-Time Clock | DS3231 maintains time for scheduled feeding |
| Independent Operation | STM32 handles local feeding operations without requiring the ESP32 to remain connected |

## Hardware Components

| Component | Purpose |
|---|---|
| STM32F407G-DISC1 | Main controller / feeding ECU |
| ESP32-WROOM-32 | Wi-Fi and BLE connectivity |
| SG90 / MG90S Servo | Controls the food dispensing gate |
| DS3231 RTC | Real-time clock for feeding schedules |
| LEDs | System status indication |
| Buzzer | Audible notifications |
| Push Button | Local feeding and provisioning control |

## Software Architecture

### 1. STM32F407 — Main Controller

Developed in C using custom bare-metal peripheral drivers.

| Module | Responsibility |
|---|---|
| Communication | UART communication with ESP32 |
| Feeder | Servo control and feeding operations |
| RTC | DS3231 communication and time management |
| Button | Physical button event handling |
| LED | System status indication |
| Error Handling | Communication and peripheral error management |

**Peripheral configuration:**

| Peripheral | Configuration | Function |
|---|---|---|
| USART2 | PA2 / PA3, AF7, 115200 baud, 8N1 | ESP32 communication |
| I2C1 | PB6 / PB7, AF4 | DS3231 communication |
| TIM3_CH1 | PA6, AF2 | Servo PWM |
| GPIO | Configured pins | Buttons, LEDs and indicators |

### 2. ESP32 — Wireless Communication Controller

Developed using the Arduino framework.

| Module | Responsibility |
|---|---|
| Wi-Fi Manager | Wi-Fi connection and network management |
| BLE Manager | Bluetooth Low Energy provisioning |
| Preferences Manager | Persistent storage of configuration |
| Web Server | Remote feeding control and system interaction |
| Schedule Manager | Feeding schedule management |
| NTP Server | Network time synchronization |
| UART Communication | Command exchange with STM32 |

The ESP32 acts as a communication bridge between the remote interface and the STM32 controller.

### 3. Inter-MCU Communication

STM32F407 and ESP32 communicate through UART using a lightweight command-based protocol.

**Communication configuration:**
- USART2 on STM32F407.
- UART1 on ESP32.
- Baud rate: 115200.
- Data format: 8N1.
- Interrupt-driven UART reception on STM32.

**Command examples:**

| Command | Purpose |
|---|---|
| CMD_FEED | Request a feeding operation |
| CMD_FEED_COMPLETE | Report feeding completion |
| CMD_NTP_TIME_REQUEST | Request time synchronization |
| CMD_NTP_TIME_RESPONSE | Deliver synchronized time |
| CMD_STATUS_REQUEST | Request system status |
| CMD_STATUS_RESPONSE | Report system status |
| CMD_UPDATE_WIFI | Update Wi-Fi configuration |
| CMD_INIT | Initiate provisioning workflow |
| CMD_ERROR | Report an error |

The protocol uses compact command/value or command/payload structures depending on the message type.

## Design Decisions

### Why use two microcontrollers?

The initial design considered using the ESP32 as the only controller. However, separating wireless communication from hardware control provides a clearer system architecture.

- **STM32F407:** Responsible for deterministic local control, peripheral management, and feeding operations.
- **ESP32:** Responsible for network connectivity, BLE provisioning, web-server functionality, and remote communication.

This separation allows the feeder to perform local operations and scheduled feeding without depending on an active Wi-Fi connection.

### Interrupt-Driven Event Handling

The STM32 uses an event-driven approach to handle button inputs and UART communication.

Interrupt service routines handle time-critical events and update event flags or communication state. The main application loop processes the corresponding events.

This keeps interrupt handlers short and separates hardware event detection from application-level processing.

## Repository Structure

```text
Smart-Pet-Feeder/
│
├── STM32/
│   ├── Inc/
│   ├── Src/
│   ├── Drivers/
│   └── README.md
│
├── ESP32/
│   ├── ble_manager/
│   ├── preferences_manager/
│   ├── wifi_manager/
│   ├── schedule_manager/
│   ├── ntp_server/
│   ├── usart_com/
│   └── README.md
│
├── Documentation/
│   ├── System_Architecture.md
│   ├── UART_Protocol.md
│   └── Hardware_Connections.md
│
└── README.md
```

## Development Environment

| Component | Tools / Framework |
|---|---|
| STM32 | STM32CubeIDE / GCC ARM Embedded |
| ESP32 | Arduino IDE / Arduino framework |
| Programming Language | C / C++ |
| Version Control | Git / GitHub |
| Debugging | ST-LINK, Logic Analyzer, Oscilloscope |

## Current Development Status

The project has undergone hardware prototyping, firmware development, and debugging.

Current development focuses on:
- Completing hardware and mechanical integration.
- Improving UART communication reliability and error handling.
- Integrating and validating the feeding mechanism.
- Improving system reliability and recovery behavior.
- Further developing the FreeRTOS-based architecture.

Features and hardware configurations should be considered complete only after their corresponding integration tests have been performed.

## Future Improvements

- FreeRTOS task-based application architecture.
- More robust UART protocol with packet validation and acknowledgements.
- Watchdog integration and fault recovery.
- Firmware OTA update support.
- Improved feeding history and system monitoring.
- Complete mechanical enclosure and food dispensing mechanism.

## License

Add an appropriate license after verifying the licensing of all original code and external dependencies.
