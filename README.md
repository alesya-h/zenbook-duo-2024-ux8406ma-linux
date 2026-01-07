# zenbook-duo-2024-ux8406ma-linux

Features:
* brightness sync (any)
* battery limiter (any)
* touch/pen panels mapping (Hyprland)
* automatic bottom screen on/off (Hyprland)
* automatic rotation (Hyprland)

## Hyprland

The `duo-hypr` script provides automatic display management for Hyprland.

### Dependencies

```bash
sudo pacman -S jq inotify-tools iio-sensor-proxy usbutils
```

### Configuration

Edit the configuration variables at the top of `duo-hypr` to match your hardware:

```bash
preferred_resolution="1920x1200@60"  # or "2880x1800@120" for 3K model
ui_scale=1                            # 1.5 for 3K model
vrr=1                                 # 0 to disable VRR
mpvpaper=0                            # 1 to enable custom mpvpaper reset with orientation change
```

### Usage

Add to your `~/.config/hypr/hyprland.conf`:

```ini
exec-once = /path/to/duo-hypr watch-displays
exec-once = /path/to/duo-hypr watch-rotation
exec-once = /path/to/duo-hypr watch-backlight
exec-once = /path/to/duo-hypr set-tablet-mapping
```

Or configure tablet/touchscreen mapping directly in hyprland.conf:

```ini
device {
    name = elan9008:00-04f3:425b
    output = eDP-1
}
device {
    name = elan9009:00-04f3:425a
    output = eDP-2
}
device {
    name = elan9008:00-04f3:425b-stylus
    output = eDP-1
}
device {
    name = elan9009:00-04f3:425a-stylus
    output = eDP-2
}
```

This will automatically:
- Switch to top screen only when keyboard is attached
- Switch to both screens when keyboard is detached
- Rotate both screens when the device is rotated
- Sync brightness between displays
- Map touchscreen/stylus input to the correct display

### Manual Commands

| Command | Description |
|---------|-------------|
| `duo-hypr top` | Show only top screen (eDP-1) |
| `duo-hypr bottom` | Show only bottom screen (eDP-2) |
| `duo-hypr both` | Show both screens stacked |
| `duo-hypr toggle` | Toggle between top-only and both |
| `duo-hypr status` | Show current display configuration |
| `duo-hypr set-tablet-mapping` | Map touchscreen/stylus to correct displays |
