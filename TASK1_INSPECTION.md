# Task 1 Inspection: I/O Skeleton Structure

## Files And Classes Found

| Class | File(s) | Status |
| --- | --- | --- |
| `CGA_Screen` | `machine/cgascr.h`, `machine/cgascr.cc` | Skeleton exists, mostly empty |
| `Stringbuffer` | `object/strbuf.h`, `object/strbuf.cc` | Skeleton exists, methods missing |
| `O_Stream` | `object/o_stream.h`, `object/o_stream.cc` | Skeleton exists, inheritance, operators, and manipulators missing |
| `CGA_Stream` | `device/cgastr.h`, `device/cgastr.cc` | Skeleton exists, inheritance and output glue missing |
| `Keyboard_Controller` | `machine/keyctrl.h`, `machine/keyctrl.cc` | Mostly implemented, several hardware I/O methods stubbed |
| `Key` | `machine/key.h` | Implemented inline in header |

## Missing / Stubbed Methods

### `CGA_Screen`

- Missing private state, likely video memory pointer and cursor/control ports.
- Constructor initializer is stubbed.
- Missing screen operations such as character output, cursor positioning, clearing, scrolling, and attribute handling.
- `machine/cgascr.cc` contains only the include and a placeholder.

### `Stringbuffer`

- Missing buffer storage and current write position/index.
- Missing constructor and buffering methods.
- Missing `put`/append behavior.
- Missing pure virtual `flush()` as described by the file comment.

### `O_Stream`

- Missing inheritance from `Stringbuffer`.
- Missing `operator<<` overloads for characters, strings, integer types, and manipulators.
- Missing declarations/definitions for `endl`, `bin`, `oct`, `dec`, and `hex`.
- Missing number base state and conversion logic.

### `CGA_Stream`

- Missing inheritance from `O_Stream` and likely `CGA_Screen`.
- Missing constructor.
- Missing `flush()` implementation that transfers buffered output to the CGA screen.

### `Keyboard_Controller`

- `key_decoded()` is implemented.
- `get_ascii_code()` is implemented.
- Constructor and `reboot()` are implemented.
- Stubs remain in:
  - `Keyboard_Controller::key_hit()`
  - `Keyboard_Controller::set_repeat_rate(int speed, int delay)`
  - `Keyboard_Controller::set_led(char led, bool on)`

### `Key`

- No obvious stubs.
- Already stores ASCII code, scan code, modifier bits, validity state, and selected scan-code constants.

## Expected Class Relationships

```text
Stringbuffer
  ^
  |
O_Stream
  ^
  |
CGA_Stream  ---- uses/inherits ----> CGA_Screen

Keyboard_Controller ---- returns ----> Key
```

- `Stringbuffer` is the device-independent character buffer.
- `O_Stream` builds formatted stream output on top of `Stringbuffer`.
- `CGA_Screen` is the low-level CGA hardware abstraction.
- `CGA_Stream` connects formatted stream output to the CGA screen, probably by deriving from `O_Stream` and `CGA_Screen`.
- `Keyboard_Controller` decodes keyboard controller bytes into `Key` objects.
- `Key` is a small value object used by `Keyboard_Controller`.

## Likely Implementation Order

1. Implement `CGA_Screen` low-level screen access: video memory, cursor ports, clearing, scrolling, and cursor positioning.
2. Implement `Stringbuffer`: fixed buffer, write position, `put`, automatic flush trigger, and pure virtual `flush`.
3. Implement `O_Stream`: stream operators, number base conversion, and manipulators.
4. Implement `CGA_Stream`: connect `O_Stream` output to `CGA_Screen` and implement `flush`.
5. Implement remaining `Keyboard_Controller` hardware methods: `key_hit`, `set_repeat_rate`, and `set_led`.
6. Wire globals/application/main once output works, likely including `kout`, keyboard objects, and `Application::action()`.

## Note

The requested skeletons already exist. Do not create parallel duplicate classes.
