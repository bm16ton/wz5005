A esp8266 firmware to control the wz5005 (small no builtin psu nor wifi support model, other may work to, untested) Currently the on/off button on page acts like a hardware switch in debounce. I have no idea how or why but every button on the on/off will cause random amounts of clicks sometimes 12 sometimes 50. noway to know if itll land on on or off.

Original firmwaaree was for dps500X series with totally different protocol, can be found here.  https://github.com/zsellera/dps-wifi

I sniffed the stock wz5005 windows app from manufacturer for protocol and spent a couple minutes with programmers calculator for crc. But a gentleman has posted the protocol. Manufacturer has as well and you can find it on-line but its always for the larger model with more features. I cant remember if/what may have been different between the two (except mini not having things like built-in wifi support etc) but behaviors are different. For one the larger model once connected via uart/wifi etc you can no longer control the device via its buttons, the small one you can....tho i can see some possible problems with that. the mans documented protocol is here.  https://github.com/kordian-kowalski/wz5005-control.git   ps: last i knew he didnt have the crc which is simple, add the all hex numbers in message up then divide by 100, instead of dealing with floats etc just addem up and take the last 2 digits.


Inside the folder bens_scripts is the inner workings of an obviously broken mind. 
