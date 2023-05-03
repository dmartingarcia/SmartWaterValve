# esp-watervalve
Hacking the Jardibric PNR16 to add wifi to it.

# State of the project
It adds a esp8266 (Wemos d1 mini) in order to control the water valve, removing the IC and hardwiring to the esp8266


## First look

After dissasembling it we had different 3 parts:

<img src="https://user-images.githubusercontent.com/11503528/236049587-1c19bdfa-4ad5-455e-9cc1-1d79379430d1.jpg" width=35% height=35%>

### Top layer

Here we can see that there's a IC, two rotary selectors and the connection to the rain sensor at the bottom right.

<img src="https://user-images.githubusercontent.com/11503528/236049580-4eaeb839-fa56-4c7b-9984-553fb38f9154.jpg" width=35% height=35%>

Both rotary selectors are divided into two equal semicircles, where the connections are mirrored between each side, also mirrored between both, except for the central contact path.

<img src="https://user-images.githubusercontent.com/11503528/236050792-3940232b-9079-4570-b843-937664dae71e.jpg" width=35% height=35%>

### Bottom layer

In this side, we have the valve position sensing part, plus the motor connector and battery contacts

<img src="https://user-images.githubusercontent.com/11503528/236054079-afecc74d-5d7a-43d9-b12c-d7eafca2f687.jpg" width=35% height=35%>



### Wiring

You will need 3 GPIO pins from the esp8266.

 - **Led pin:** I use the same pin that is being used for the lonboard led.
	 - D4 // GPIO2
 - **Valve position:** I choose to pull up this pin, because half of the contacts are not grounded or connected, and it will allow us to detect when we're grounded on the contact pads.
	 - D2 // GPIO4
 - **Motor pin:** It enables the motor, I choose to pull down this pin.
	 - D1 // GPIO5
I also avoided all pins that had some kind of special function for the booting process.

Here's the detail picture of the contacts and where it goes on the IC.

<img src="https://user-images.githubusercontent.com/11503528/236052772-8fa9d1b3-5b07-461d-ad5f-f674fa64cc1f.jpg" width=35% height=35%>

<img src="https://user-images.githubusercontent.com/11503528/236053697-6be824d6-ae81-4822-a1e1-06fe0e858761.jpg" width=35% height=35%>

And this is how I ended wiring everything. I use contact glue to hold the cables and avoid rip any pad or traces.

<img src="https://user-images.githubusercontent.com/11503528/236049569-2262b934-bf73-4f7c-9246-076817b4b0e0.jpg" width=35% height=35%>

#### I want to controll it using the rotary selectors

Okay, that's a bit difficult on this esp8266, because of the lack of GPIO pins, I tried to use a analog multiplexer to be able to manage the outer contacts using just 3 pins, but it ended being a nightmare because I started to use TX and RX pins and it make a bit harsh to develop and upload the code.

<img src="https://user-images.githubusercontent.com/11503528/236049593-7e86091d-8c68-4e82-b95c-7bc7d3abe1cd.jpg" width=35% height=35%>

But it's still doable, but it will also not fit inside of the original case, and that's also a reason to just skip using the rotary encoders.

<img src="https://user-images.githubusercontent.com/11503528/236049590-29cf539a-a79f-4810-b1a5-327a412d6ee4.jpg" width=35% height=35%>
