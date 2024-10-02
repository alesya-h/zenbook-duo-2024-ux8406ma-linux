# zenbook-duo-2024-ux8406ma-linux

Features:
* brightness sync (any)
* battery limiter (any)
* touch/pen panels mapping (GNOME-specific, requires GNOME 46 or a backported Mutter patch)
* automatic bottom screen on/off (GNOME-specific)
* automatic rotation (GNOME-specific)

## panel mapping

`duo set-tablet-mapping` will set necessary dconf settings, but for them to work you need a Mutter with a patch from https://gitlab.gnome.org/GNOME/mutter/-/merge_requests/3556 and libwacom with this patch https://github.com/linuxwacom/libwacom/pull/640 . Both are merged upstream, so you can just wait.

## bottom screen toggle on GNOME

Make sure gnome-monitor, usbutils and inotify-tools are installed, the script relies on the `gnome-monitor-config`, `lsusb` and `inotifywait` commands from them.

Before the next steps, you may need or want to change the scaling settings or change the config at the top of `duo` based on the version of the duo that you have (1080p vs 3k display models)

For automatic screen management run `duo watch-displays` somewhere at the start of your GNOME session.

For manual screen management there are `duo top`, `duo bottom`, `duo both` and `duo toggle` (toggles between top and both) commands.

In addition there's also `duo toggle-bottom-touch` to toggle touch for the bottom screen, so you can draw with a pen while resting your hand on the screen.

## automatic rotation

Make sure iio-sensor-proxy is installed, the script relies on `monitor-sensor` command from it. Once it's installed and you followed the steps above for dualscreen setup just run `duo watch-rotation` somewhere at the start of your GNOME session.

## brightness sync

Brightness control requires root permissions. I prefer to have sudo with a password by default, so I use a hack to have a NOPASSWD sudo for /usr/bin/env which allows to execute any command. Line in /etc/sudoers looks like `%wheel  ALL=(ALL:ALL)    NOPASSWD: /usr/bin/env`. On NixOS the relevant part of the config is this:

```
  security.sudo = {
    enable = true;
    extraRules = [{
      commands = [
        {
          command = "/usr/bin/env";
          options = [ "NOPASSWD" ];
        }
      ];
      groups = [ "wheel" ];
    }];
  };
```

Once the sudo setup is done you can either run `duo sync-backlight` to sync it once (you may want to bind it to some hotkey) or you can run `duo watch-backlight` at login and it will keep syncing your brightness from the top display to the bottom one.

For most linux distros there is an included systemd service file: `brightness-sync.service` that just needs `/path/to/duo` changed before moving it to `/etc/systemd/system` to enable brightness sync in the background.

## battery limiter

Requires same sudo setup as for the brightness sync. Most likely you want to run `duo bat-limit` or `duo bat-limit 75` (where 75 is your desired threshold percentage, 80 is used if omited) once at the start of your desktop session.

## Notes concerning usage on Fedora 40

The steps described above work on Fedora 40 with the following specific changes:
Prerequisities:
`sudo dnf install lm_sensors gnome-monitor-config inotofy-tools`
Libwacom files elan-425a.tablet and elan-425b.tablet should be copied to /usr/share/libwacom
For brightness sync to work properly, line 10 of the duo.sh should be modified to `backlight=card1-eDP-2-backlight`

