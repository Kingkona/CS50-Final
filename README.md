# Automated Garden Watering System Prototype
#### Video Demo: https://www.youtube.com/watch?v=h3nPeAaQ8pc
#### Description:
My CS50 final project is a prototype of an automated garden irrigation system.  I love vegetable gardening, but watering can be a chore under time pressure.  Therefore, I thought it would be interesting to build a system that automates the process of watering based on sensors reading the soil moisture level.

The system responds to the soil conditions, either dry or wet.  As the soil dries out, conductivity between the sensor probes is reduced to a point that causes the micro-controller to send a voltage signal to the relay to switch it on.  When the relay is closed, current flows to the sprinkler valve causing it to open and water to flow.  The valve can be connected to a drip irrigation system to control the water volume.  This way, the soil slowly absorbs water to ensure a good soaking.  If at least one sensor detects the soil is sufficiently soaked based on the amount of current flowing between the probes, the micro-controller stops sending the signal to the relay.  This causes the relay to switch open, eliminating the current being sent to the sprinkler valve, and shutting the water off.

There is a single program file written with the Arduino IDE that encompasses the micro-controller initialization, sensor management, network configuration, program logic, and the web server.  I added numerous comments in the code to explain each part of the program and the logic.

My program design is based on a logical 'or' condition between the sensors.  In other words, the micro-controller will make a decision based on the input of either sensor rather than both.  Assuming uniform soil composition and exposure to sunlight, it's likely that the rate of water evaporation will be even across the garden bed.  Therefore, I expect the sensors will detect wet and dry conditions consistently and keep the garden bed adequately watered.  However, what if one sensor happens to be placed in an isolated spot of the soil that retains water longer (e.g., soil with a higher clay content)?

Because of the logical 'or' condition that determines when the water valve is turned on and off, it is conceivable that the sensor in the soil retaining water longer will prevent the valve from opening and starve the rest of the garden of water.  Therefore, one way to overcome the inadvertent flaw of the logical 'or' condition would be to add a timer; upon expiration of the timer, the sprinkler valve would open to water the garden for a period of time.  Optionally, using a more advanced micro-controller that supports multiple analog inputs would allow for higher resolution data collection.  This would provide the ability to create a decision making algorithm, for example, based on an average moisture level, that triggers the sprinkler valve to open or close.  The ESP 8266 micro-controller that I used for my project only supports one analog input that is not sufficient for both sensors.  Therefore, I had to use the digital general purpose I/O (GPIO) interfaces that only support two conditions, moist or dry (i.e., on or off). 

Such a system is obtainable thanks to low cost micro-controllers and accessories used for prototyping a project such as my automated garden watering system.
