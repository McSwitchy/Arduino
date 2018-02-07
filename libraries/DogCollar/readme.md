
# What is it good for

destinated to be used with dog collar tranining device, there
are cheap versions of "zogin dog training" devices out there... 

# What you need

works with 433 MHz cheap sender module (~1$) connected to arduino

You need:
* Arduino (i prefer "Pro Mini" or "Pro Micro")
* 433Mhz module (like this: https://www.amazon.com/s/ref=nb_sb_noss_2?url=search-alias%3Delectronics&field-keywords=433mhz+transmitter)
  (the 3-pin version is the sender)
  
# Instructions to use the library

1. place this folder into you arduino library path
   or in the root of your arduino project (where the *.ino lies)
2. get the RfControl library from https://github.com/pimatic/RFControl
   and repeat the procedure with this library too
3. connect arduino with some digital pin to DATA of transmitter,
   to run an unchanged example it has to be pin 7

# Reminders

 the tranining receiver will turn power off when no motion detected
  for two minutes and no rf signal was received
* the example "KeepAlive" sends an blink-command after each 20 seconds,
  so the receiver will not power off when you don't want it to do...
  BUT: the battery will drain, so plug your arduino out when done with
  "training" 
