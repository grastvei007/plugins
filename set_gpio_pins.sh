#!/bin/bash

# filename: /usr/local/bin/set_gpio_pins.sh << set as executable (chmod +x)
#
# set GPIO pins 17, 22, and 24 to be usable without root access
#  In fact, make them accessible to the web-server (www-data) and
#   the 'typical' user (pi).  The 'chmod' allows user 'pi' to write
# This script must be run as root (sudo), in root's crontab, or
#   each of the lines in the for-loop need to start with 'sudo'

for pin in 17 18 22 27; do
   /usr/bin/env gpio export ${pin} out
   chown -R www-data:pi /sys/class/gpio/gpio${pin}
   chmod -R g+w /sys/class/gpio/gpio${pin}
done
