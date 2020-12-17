Datakomm prosjekt, webpage.

CONTENTS OF THIS FILE:

 * REQUIREMENTS
 * INSTALLATION
 * STARTUP VALUES
 * OPTIONS



# Requirements
-----------

Raspberry Pi(v3+) with latest Raspbian OS installed

This server requires modules as described under dependencies installed in this order:
SSH
VNC(optional)
UFW
Fail2Ban
DuckDNS(optional)
Nginx
Certbot and letsencrypt


-------------------------------

# Installation
Follow the installation of Raspberry Pi and OS:
https://www.raspberrypi.org/software/

Install each dependency in order.

--------------------------------

Setup values:
Values that needs to be changed for each dependency is described in their respective instructions.
For Raspberry Pi:
Check IPv4:
sudo hostname -I
Setup:
sudo ifconfig
- Name
- Password
Check External IP:
https://www.whatsmyip.org/

---------------------------------

Options:
Changing username/password, open ports, enabling/disabling IP/MAC, apply DNS, configure and control clients.

--------------------------------

Made by Datakomm gruppe 02. 
Inquiries about the website can be sent to mabron@stud.ntnu.no
