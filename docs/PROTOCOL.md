# Serial Communication Protocol

This document describes the message format used for communication between the DCC Command Station-EX and the Display Monitor.

## Overview

- **Medium**: Serial (USB/UART)
- **Baud Rate**: 115200 by default; configurable in `config.h`
- **Input**: Mega `Serial` (USB/pins 0-1) and `Serial1` (pins 18-19)
- **Framing**: DCC-EX responses use `<...>`; custom messages use LF or CR+LF
- **Direction**: Command Station → Display Monitor (one-way)

## Message Format

Native DCC-EX turnout responses are accepted directly:

```
<H 101 1>
<H 101 0>
```

`1` displays the configured turnout as `OPEN` (thrown), and `0` displays
it as `CLOSED`. Only configured turnout IDs are accepted; legacy aliases
apply only to the custom format. A newline is not required between native
responses. Other native response types are not yet interpreted.

The existing custom messages consist of:
```
COMMAND:PARAM1:PARAM2:...
```

Custom messages are case-sensitive and use uppercase.

## Defined Messages

### Power Control

```
POWER:ON
POWER:OFF
```

Indicates whether the main track power is enabled or disabled.

**Example**:
```
POWER:ON
POWER:OFF
```

### Turnout Status

```
TURNOUT:<number>:<state>
```

- `<number>`: the configured turnout IDs (default 101, 102, 103).
  Legacy IDs 1, 2, and 3 address the first, second, and third displayed
  turnouts, respectively, unless an ID is explicitly configured for a
  different turnout. Configured IDs always take precedence.
- `<state>`: `OPEN`, `THROWN`, or `CLOSED`. Both `OPEN` and `THROWN`
  display as `OPEN`; `CLOSED` displays as `CLOSED`.

**Examples**:
```
TURNOUT:101:OPEN
TURNOUT:102:CLOSED
TURNOUT:103:THROWN
```

### Throttle/Speed Control

```
THROTTLE:<loco_id>:<speed>
```

- `<loco_id>`: Locomotive identifier configured in `config.h` (default 3)
- `<speed>`: 0-255 (PWM/speed value)
  - 0 = Stopped
  - 128 = Half speed
  - 255 = Full speed

**Examples**:
```
THROTTLE:3:0      # Loco 3 stopped
THROTTLE:3:128    # Loco 3 at half speed
THROTTLE:3:255    # Loco 3 at full speed
```

## Parsing Rules

1. Native responses end with `>`; custom messages end with newline (`\n`).
2. Carriage returns (`\r`) are stripped.
3. Unsupported native responses are ignored; unknown custom messages are logged.
4. Invalid native turnout responses and oversized messages are logged and discarded.

## Status Update Frequency

The Command Station should send updates:
- **Power**: On state change
- **Turnouts**: On state change
- **Throttle**: On change (not necessarily on every command, can be batched)

Typical frequency: Every 100-200ms or on change, whichever is more frequent.

## Future Extensions

Possible future messages (not yet implemented):

```
SIGNAL:<track_section>:<aspect>     # Traffic signal indication
CONSIST:<loco>:<lead_loco>          # Consist information
ROUTE:<route_id>:<state>            # Route/automation status
SENSOR:<id>:<state>                 # Block occupancy or sensor states
ERROR:<code>:<message>              # Error reporting
```

## Example Communication Sequence

```
System startup:
  → POWER:OFF
  → TURNOUT:101:CLOSED
  → TURNOUT:102:CLOSED
  → TURNOUT:103:CLOSED
  → THROTTLE:3:0

User turns on track power:
  → POWER:ON

User throws turnout 1:
  → TURNOUT:101:THROWN

User controls Loco 3 throttle:
  → THROTTLE:3:50
  → THROTTLE:3:100
  → THROTTLE:3:150
  → THROTTLE:3:200
```

## Implementation Notes

### Arduino Display Side

The display accepts native turnout responses and the custom messages above.
Native power and throttle responses are not yet implemented.

### Command Station-EX Side

The Command Station should send these messages via Serial to the Display Arduino. This can be implemented as:

1. **Via custom DCC Command Station-EX script/plugin**
2. **Via a serial forwarder if Command Station doesn't support custom output**
3. **Via direct serial API if available in Command Station-EX**

Example pseudocode for Command Station:

```cpp
void reportTrackStatus() {
  Serial.println("POWER:" + (trackPowerOn ? "ON" : "OFF"));
  Serial.println("TURNOUT:101:" + (turnout1Thrown ? "THROWN" : "CLOSED"));
  Serial.println("TURNOUT:102:" + (turnout2Thrown ? "THROWN" : "CLOSED"));
  Serial.println("TURNOUT:103:" + (turnout3Thrown ? "THROWN" : "CLOSED"));
  Serial.println("THROTTLE:3:" + throttle3Speed);
}
```

## Validation

Messages are validated by:
1. Checking prefix (POWER:, TURNOUT:, THROTTLE:)
2. Verifying required parameters are present
3. Checking parameter values are in valid range
4. Logging any parse errors to serial console

Invalid messages are logged but do not crash the system.
