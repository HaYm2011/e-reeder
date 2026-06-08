# e-reeder

a e-reader... aka chapri kindle

# 2026-06-08: worked a bit on the case

**Total time spent: 0.10 hours**

found a .step file for the batteries and for the sensors
im gonna start with the case for the reeder in the next entry
![image.png](https://cdn.hackclub.com/019ea6ff-4f88-777c-be1b-9f9bc3e61c54/image.png)

# 2026-06-04: Started the design

**Total time spent: 1 hour**

got the step file for the pcb ran into a few errors like with the step file and found a good f3d file for the display module . im going to start the design for the chassis in the next entry
![image.png](https://cdn.hackclub.com/019e9286-50af-7aa5-ae9b-96d174b2a4c0/image.png)
![image.png](https://cdn.hackclub.com/019e9286-1593-73f9-bf10-6cd869138fd5/image.png)

# 2026-06-04: added the battery source i forgot about😅

**Total time spent: 0.30 hours**

So i just realised i completely forgot about the battery stuff and i had to go back to the .sch add a JST socket retrace a few copper lines and i think about done with the pcb for real this time


![image.png](https://cdn.hackclub.com/019e9256-9334-7c11-879c-bd24a0d2a608/image.png)
![image.png](https://cdn.hackclub.com/019e9258-1393-7515-bca0-4461b7707ef9/image.png)
![image.png](https://cdn.hackclub.com/019e9258-3e86-7441-a0c9-98b6ead2e7b8/image.png)

# 2026-06-03: Finished the PCB!!!!!!!!

**Total time spent: 6 hours**

So i finished all the pcb part i was stuck on finding the footprints for all my components for quite a while and then i realised i had to go back to the .sch file for assigning the footprints and i happened to put the male connector footprint instead of the female connector for the esp32 s3 and the sensors. I then did the copper tracing and i had to fix a few overlays in the .sch file and i finally reviewed my progress in the 3d viewer and i had to figure out how to use the measure tool and the move tool at the same time in kicad(i just figured to enter the x,y coords for the components...)


![Screenshot 2026-06-03 114855.png](https://cdn.hackclub.com/019e8e16-699a-7ad5-9642-3e1318504c6d/Screenshot%202026-06-03%20114855.png)

![Screenshot 2026-06-03 203839.png](https://cdn.hackclub.com/019e8e15-ab84-74f1-83b7-7813f3e793fa/Screenshot%202026-06-03%20203839.png)

![Screenshot 2026-06-03 203854.png](https://cdn.hackclub.com/019e8e17-32b2-7410-85cc-2bdeb92d7c61/Screenshot%202026-06-03%20203854.png)

# 2026-05-29: so i finished the pcb schematics (and did a few other stuff)

**Total time spent: 6 hours**

i finished the schematics i changed the e-ink module im gonna be using to:

Waveshare 4.2-inch e-Ink Paper Display Module with SPI Interface:

https://robu.in/product/4-2-inch-e-ink-paper-display-module-with-spi-interface/

this module is essentially the same thing but 0.1 inches lesser than the previous one i selected but this is gonna be easier for me to connect to my esp32 s3.

regarding the pcb schematics i faced quite a few problems since its been a long time since i last used kicad so i forgot that im not supposed to overlay the wires and i spent about a hour rerouting everything and realised that the xshut pins of the VL53L0X TOF Laser Distance Sensors could not be connected to pins like 48 of the esp32 s3 so i rerouted these pins.

i also spent quite a while replacing the older display connections with the newer ones 


![image.png](https://cdn.hackclub.com/019e746b-3c6c-7873-ab11-187fc061d7f6/image.png)
![image.png](https://cdn.hackclub.com/019e7466-a5aa-795a-a78e-041e51c2eedf/image.png)

# 2026-05-27: research, and making of BOM 

**Total time spent: 5hrs**

So i read about a few other opensource e-reader projects , like this one:

https://github.com/joeycastillo/The-Open-Book

so i realised id like to use a microcontroller for this rather than a micro controller and hence an esp32-s3 for this and a e-ink display i found on robocraze.com :

e-ink module:

https://robocraze.com/products/4-3in-serial-interface-electronic-paper-display?_pos=1&_sid=8128a620e&_ss=r

esp32 s3:

https://robocraze.com/products/7semi-esp32-s3-dev-boardc-1-n8r8-wifi-bluetooth-dual-usb-c-rgb-led?_pos=2&_sid=31f0f64c8&_ss=r

so i decided im gonna 3d print the frame and add a really compact ToF sensor on each side of the reeder and so when any side detects movement the page is flipped respectively 

VL53L0X TOF Laser Distance Sensor:

https://robu.in/product/vl53l0x-tof-based-lidar-laser-distance-sensor/

for the body of this thing im going to design it and 3d print it

![image.png](https://cdn.hackclub.com/019e69c6-d92b-7ba9-9219-a4ead886b5d9/image.png)![image.png](https://cdn.hackclub.com/019e69c7-4354-7782-b8a2-07d265dd9a25/image.png)![image.png](https://cdn.hackclub.com/019e69c7-8f65-73b3-8cc6-7590248acf02/image.png)

