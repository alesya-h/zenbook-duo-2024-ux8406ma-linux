# zenbook-duo-2024-ux8406ma-linux
automatic screen on/off (GNOME-specific), brightness sync, battery limiter

## How to use dualscreen toggle on GNOME

Install https://github.com/jadahl/gnome-monitor-config. I packaged it for NixOS already, but need to submit a pull-request.

Since WLAN button code is used for keyboard attach/detach events first we need to disable it. Open dconf-editor and edit the key /org/gnome/settings-daemon/plugins/media-keys/rfkill-static and remove XF86WLAN from the list:

<img src="https://github.com/alesya-h/zenbook-duo-2024-ux8406ma-linux/assets/209175/3c24bc19-a22f-44ad-88e9-1c099c88e2c3" width="50%">

You MUST log out and log in again after doing it.

After that go to Settings -> Keyboard -> (at the bottom) Keyboard Shortcuts -> View and Customize Shortcuts -> Custom Shortcuts and press +.

* Name: "toggle dualscreen mode" or anything else.
* Command: `/absolute/path/to/this/repo/duo set-displays`.
* Shortcut: press "Set Shortcut..." and attach or detach the keyboard (doesn't matter, it sends the same event).

<img src="https://github.com/alesya-h/zenbook-duo-2024-ux8406ma-linux/assets/209175/54d08da6-cba2-49a8-abcf-9eadcd5869d2" width="50%" height="50%">

Press "Add" in the top right corner, close everything and log out and in again. Unlike usual shortcuts, this one doesn't work before logging out.

You also want to add `duo set-displays` to your startup so it'll set your laptop to single or dual screen when you log in.

For manual screen management there are `duo top`, `duo bottom`, `duo both` and `duo toggle` (toggles between top and both) commands.

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

Once the sudo setup is done you can either run `duo sync-backlight` to sync it once (you may want to bind it to some hotkey) or you can run `duo watch-backlight`, presumably at login, and it will keep syncing your brightness from the top display to the bottom one.

## battery limiter

Requires same sudo setup as for the brightness sync. Most likely you want to run `duo bat-limit` or `duo bat-limit 75` (where 75 is your desired threshold percentage, 80 is used if omited) once at the start of your desktop session.
