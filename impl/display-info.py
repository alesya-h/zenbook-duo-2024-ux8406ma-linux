#!/usr/bin/env python3

"""Report what mutter knows about an internal panel.

backend-gnome used to carry one block of constants per panel variant with the
wrong one commented out, so a fresh checkout only worked on whichever machine it
had last been edited for, and getting it wrong moved windows to the wrong place
rather than failing outright. Mutter already knows the mode, the scale in force
and the EDID for whatever panel is actually fitted, and answers the same way on
the 3K OLED and on the FHD+ unit.

    display-info.py panel <connector>   mode id, height, scale, VRR support
    display-info.py spec  <connector>   the EDID triple GNOME keys inputs on
"""

import json
import subprocess
import sys

CALL = [
    "busctl", "--user", "call",
    "org.gnome.Mutter.DisplayConfig",
    "/org/gnome/Mutter/DisplayConfig",
    "org.gnome.Mutter.DisplayConfig",
    "GetCurrentState",
    "--json=short",
]


def current_state():
    result = subprocess.run(CALL, capture_output=True, text=True)
    if result.returncode != 0:
        sys.exit(f"display-info: cannot reach mutter: {result.stderr.strip()}")
    data = json.loads(result.stdout)["data"]
    # (serial, monitors, logical monitors, properties)
    return data[1], data[2]


def prop(props, name, default=None):
    entry = props.get(name)
    return default if entry is None else entry.get("data", default)


def find_monitor(monitors, connector):
    for monitor in monitors:
        if monitor[0][0] == connector:
            return monitor
    sys.exit(f"display-info: no monitor on connector {connector}")


def preferred_mode(modes, connector):
    for mode in modes:
        if prop(mode[6], "is-preferred"):
            return mode
    for mode in modes:
        if prop(mode[6], "is-current"):
            return mode
    sys.exit(f"display-info: {connector} reports no usable mode")


def logical_scale(logical_monitors, connector):
    for entry in logical_monitors:
        if any(spec[0] == connector for spec in entry[5]):
            return entry[2]
    # The panel is fitted but not currently driven, which is exactly the state
    # `duo both` is about to leave, so fall back to what the mode asks for.
    return None


def main(argv):
    if len(argv) != 3:
        sys.exit(__doc__.strip())
    command, connector = argv[1], argv[2]

    monitors, logical_monitors = current_state()
    spec, modes, _props = find_monitor(monitors, connector)

    if command == "spec":
        _connector, vendor, product, serial = spec
        print(vendor, product, serial)
        return

    if command != "panel":
        sys.exit(__doc__.strip())

    mode = preferred_mode(modes, connector)
    mode_id, height, mode_scale = mode[0], mode[2], mode[4]
    scale = logical_scale(logical_monitors, connector)
    if scale is None:
        scale = mode_scale
    variable = prop(mode[6], "refresh-rate-mode") == "variable"

    print(mode_id, height, f"{scale:g}", 1 if variable else 0)


if __name__ == "__main__":
    main(sys.argv)
