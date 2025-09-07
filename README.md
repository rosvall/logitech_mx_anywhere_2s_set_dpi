> [!NOTE]
> Moved to [codeberg.org/rosvall/logitech_mx_anywhere_2s_set_dpi](https://codeberg.org/rosvall/logitech_mx_anywhere_2s_set_dpi)
>
> For reasons given [here](https://github.com/rosvall/goodbye-github)


# Hack to keep my Logitech MX Anywhere S2 mouse at 4000 DPI

## Why?

After growing really tired of [Solaar](https://github.com/pwr-Solaar/Solaar) freezing,
forgetting to apply settings, being crap, and currently simply not working on my
system, I tried [libratbag](https://github.com/libratbag/libratbag) which is
absolutely terrific, apart from not working with my mouse.
Or rather, it works, but this mouse doesn't have non-volatile storage for settings,
so it resets to default settings when it goes to sleep after not having been moved
for a couple of minutes.

## How?

This terrible hack simply writes a handful of bytes to the correct `/dev/hidrawN` node
everytime `udev` notices something with the USB PID and VID of this particular mouse
has come online.

The "handful of bytes" is basically just taken from `ratbagd`'s debug output
(which is also a pain to get to, because of the awfulness that DBUS is. God, I hate DBUS.)

I've made *very* little effort to understand the protocol, apart from resolving a few
magic numbers and some structure from the libratbag HID++ 2.0 implmentation. (The files
with names like hidpp20.c in libratbag)

As I've just hardcoded some (probably setup specific) values, it'll probably not work
for anyone else. When it breaks, I just might look into actually implementing the
necessary part of the protocol in a slightly less stupid way.

Pretty much all the effort I've put into this has been getting `udev` to do the right
thing. Which resulted in the wrapper shell script, because `udev` is a complete pain.
