---
title: Controller Documentation
date: 2020-01-29
---

## Key Features:

The **Controller** is a designed for purpose controller board, intended for use with non-critical applications that would benefit from automation. Protected connections and an industrial operational temperature range means that the device can be put to use in harsh environments.

## Specification

*   Industrial operating temperature range
*   8 x protected analogue 0-5v input ports
*   8 x protected digital output ports
*   2 x protected digital input ports
*   1 x Communication port
*   Bluetooth v4 connectivity
*   Small package
*   Small current draw
*   Simple to customize to suit an application

## Controller Operation:

The controller works by routinely evaluating logic that has been sent to it. The controller can be used with various third party interfaces, but all that is needed to get going is a Bluetooth connection via which to send a string of logic.

## Controller Logic:

The logic the device evaluates is made up of a condition to be satisfied and a resulting action. The logic can utilize any of the available inputs and outputs on the device.

e.g. Logic for a simple access control system, where a password must be sent to the controller.

`IF COM = password OR COM = passw0rd THEN O1 = 1 ELSE O1 = 0`

Here, output number #1 "O1", would be wired up to a 5 volt relay connected to an actuator locking mechanism.

e.g. Logic for switching from a solar system to mains supply and back again depending on load.

`IF A1 < 11.5 AND I1 = 1 THEN O1 = 1 ELSE O1 = 0`

`IF A1 > 13 AND I1 = 1 THEN O1 = 0 ELSE O1 = 1`

Here, analogue input #1 `A1` would be a proportionally divided down voltage input from a battery store of a solar system. Digital input #1 `I1` would be a manually operated switch, output #1 `O1` would be connected to a double throw 5 volt relays, rated to 240AC, which would switch between wholesale electricity supply and solar electricity supply.

## Key Words:

| Word    | Description                                                                            |
|---------|----------------------------------------------------------------------------------------|
| HELP    | Get a help string from the microcontroller.                                            |
| NAME    | Set the name of the device. `NAME  a_name`                                             |
| TIMEOUT | Set the timeout to clear the stored string. `TIMEOUT 3`                                |
| LOGIC   | Get a list of all the stored logic strings. `LOGIC`                                    |
| DELETE  | Delete a logic string from the list of stored logic strings. `DELETE 2`                |
| ZERO    | Zero the value of an analogue input. `ZERO A1`                                         |
| GET     | Get the values and setting of the device.                                              |
| SET     | Set a value of an output or calibrate an analogue input. `SET O1 = 1` , `SET A1 = 200` |
| SLAVE   | Turn off logic string evaluation.                                                      |
| MASTER  | Turn on logic string evaluation.                                                       |
| STREAM  | Toggle streaming of the device values.                                                 |
| STR     | Stored string value read from the Bluetooth connection.                                |
| NONE    | A blank string.                                                                        |
| IF      | Used at the start of a ternary logic string.                                           |
| THEN    | Implication. Used in a logic string.                                                   |
| ELSE    | Alternative implication. Used in a logic string.                                       |
| AND     | And conditional. Used in a logic string.                                               |
| OR      | Or conditional. Used in a logic string.                                                |
| !=      | Not equal to. Used in a logic string.                                                  |
| =       | Equal to. Used in a logic string.                                                      |
| >       | Greater than. Used in a logic string.                                                  |
| <       | Less than. Used in a logic string.                                                     |
| O1      | Name for output #1                                                                     |
| O2      | Name for output #2                                                                     |
| O3      | Name for output #3                                                                     |
| O4      | Name for output #4                                                                     |
| O5      | Name for output #5                                                                     |
| O6      | Name for output #6                                                                     |
| O7      | Name for output #7                                                                     |
| O8      | Name for output #8                                                                     |
| A1      | Name for analogue input #1                                                             |
| A2      | Name for analogue input #2                                                             |
| A3      | Name for analogue input #3                                                             |
| A4      | Name for analogue input #4                                                             |
| A5      | Name for analogue input #5                                                             |
| A6      | Name for analogue input #6                                                             |
| A7      | Name for analogue input #7                                                             |
| A8      | Name for analogue input #8                                                             |
| I1      | Name for input #1                                                                      |
| I2      | Name for input #2                                                                      |

