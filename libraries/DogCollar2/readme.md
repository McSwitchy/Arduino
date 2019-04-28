
# Difference between DogCollar / DogCollar2

You can use this library like the library DogCollar,
external syntax is completely same!

This version is IMPROVED, the differences are:
* way less code space (FLASH) and global variable space (RAM) needed
  (saves 948 Bytes FLASH / 739 Bytes RAM - A LOT on small Arduinos
  like the 328p with only 2048 Bytes RAM)
* no extra library "RFControl" needed any more, handles all itself
  (except for sniffing the unique key of the remote)
* missing the method "repeatLastCmd"

Big thanks are going to smouldery which code parts i took generally
to combine the usability of "DogCollar" as a library and the smart usage
of ressources in "transmitter_vars.ino" in this project:
https://github.com/smouldery/shock-collar-control

# What is it good for

A Library to let the Arduino be a RF-transmitter which controls
a dog collar training device. 
There are cheap versions of "zogin dog training" devices out there
which the library aims at, a set contains of:
* one remote control with buttons "1/2","light","mode","Y" and "UP"/"DOWN" on the side
* one or two receivers with two electrodes and a collar strap to attach it on throat? of the dog

# What you need

works with 433 MHz cheap sender module (~1$) connected to arduino

You need:
* Arduino (i prefer "Pro Mini" or "Pro Micro")
* 433Mhz module (like this: https://www.amazon.com/s/ref=nb_sb_noss_2?url=search-alias%3Delectronics&field-keywords=433mhz+transmitter)
  (the 3-pin version is the sender)

Possibly also this, if you got a device with another uniqueKey:
* RFControl library
* 433Mhz receiver module (like this, 4-pin module shipped in combination of transmitter: https://www.amazon.com/s?k=433Mhz+receiver)
* the INO-script to get the code -> examples/GetUniqueKey.ino
* original remote by hand to press any button (in each RF command is the key included)
  (you may have to hold a button for a few seconds to show up uniqueKey in Terminal)

# Instructions to use the library

1. place this folder into you arduino library path
   OR in the root of your arduino project (where the *.ino lies)
   OR Arduino IDE -> Sketch > Include Library > Add .ZIP Library
2. get the RFControl library from https://github.com/pimatic/RFControl
   and repeat the procedure with this library too
3. find out YOUR uniqueKey with sketch examples/GetUniqueKey.ino
   and change it in example file DogCollar-*.ino to match yours
   (you need the 433MHz receiving module here)
4. connect arduino with some digital pin to DATA of transmitter,
   to run an unchanged example it has to be pin 7

# Reminders

* the tranining receiver will turn power off when no motion detected
for two minutes and no rf signal was received
* before you can use the training device with an arduino, you
  have to pair it (see printed manual delivered with the set):
.1. turn on remote control by pressing "MODE" button
.2. switch to channel you want to use with "1/2" button
.3. press the "RESET" on the receiver
.4. press "MODE" and then "Y" on the remote control
.5. when you press "Y" again, it should now be paired and you can send
    commands.
* the example "KeepAlive" sends an blink-command after each 60 seconds,
  so the receiver will not power off when you don't want it to do...
  BUT: the battery will drain, so plug your arduino out when done with
  "training" 
* the arduino and the remote can't wake up the receiver collar with
  RF-commands... so be sure to wake it with an initial "shake"
