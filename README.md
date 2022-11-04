# Control Devices Using TV Remote
Before using TV Remote Control for controlling a device one must know on which protocol it is working.  
I am having one Chinese TV with me and don't know on which protocol it is working. So here comes the use of Logic Analyzer, i checked the Output of TV Remote using TSOP1738.  
Connected the TSOP1738 output to channel-0 of Logic Analyzer and capture the waveform on pressing Key-1, Key-2 and Key-3. So here are the wave-forms.  

![Key-1 Pressed on TV Remote](https://4.bp.blogspot.com/-lWXkxPRr3Qg/V287MP1FW1I/AAAAAAAAASA/pEJvWnBBw64ce3Qi7lOM50ThLST8esP8gCKgB/s1600/Key1.png)  

![Key-2 Pressed on TV Remote](https://4.bp.blogspot.com/-PDxlGBN2g_k/V287ME0JgjI/AAAAAAAAAS4/BC9b2DeffsAfp7Y_OMTp7RthkemFwd8TACKgB/s1600/Key2.png)  

![Key-3 Pressed on TV Remote](https://2.bp.blogspot.com/-2_kNQD70TaY/V287MptJhHI/AAAAAAAAAS4/UnZC3BL3-D0NYVgnEpgOaw-4_Gx8HwmoQCKgB/s1600/Key3.png)  

So based on this information I figure out that this signal is using Extended-NEC protocol, which is very common, apart from this there are several other protocols used in TV remote such as Sony SIRC, Philips RC5 etc.  
So first we will know something more about the NEC Protocol (though my remote has Extended NEC Protocol)  

### Features
* 8 bit address and 8 bit command length.
* Extended mode available, doubling the address size.
* Address and command are transmitted twice for reliability.
* Pulse distance modulation.
* Carrier frequency of 38kHz.
* Bit time of 1.125ms or 2.25ms.

### Modulation

![Modulation Scheme](https://3.bp.blogspot.com/-ckWO9V8-V6s/V287NDSWSnI/AAAAAAAAAS4/QpyOf9bfphIXUJSQmu-h9IYBQh1Uj1eiwCKgB/s1600/necmodulation.png)

The NEC protocol uses pulse distance encoding of the bits. Each pulse is a 560µs long 38kHz carrier burst (about 21 cycles). A logical "1" takes 2.25ms to transmit, while a logical "0" is only half of that, being 1.125ms.  

### Protocol
![NEC Protocol](https://1.bp.blogspot.com/-0E1xwqf6Xrk/V287OHxLvuI/AAAAAAAAAS4/afK-iVRn8QUqfXepL2stEQQ8tDgZbk2GQCKgB/s1600/nectrain.png)

The picture above shows a typical pulse train of the NEC protocol. With this protocol the LSB is transmitted first. In this case Address $59 and Command $16 is transmitted. A message is started by a 9ms AGC burst, which was used to set the gain of the earlier IR receivers. This AGC burst is then followed by a 4.5ms space, which is then followed by the Address and Command. Address and Command are transmitted twice. The second time all bits are inverted and can be used for verification of the received message. The total transmission time is constant because every bit is repeated with its inverted length. If you're not interested in this reliability you can ignore the inverted values, or you can expand the Address and Command to 16 bits each.
If you see the waveform at the top of this page, you will find that the waveform is exactly opposite of this waveform, that is because i am using TSOP1738 and its output is low when their is a 38KHz burst and else it is high.  

On Logic Analyzer the sequence for Keys are as follow: (This is Extended-NEC that's why no Address Inversion)  
```
Key-1 = 00000000 11101111 10000000 01111111 as LSB is Transmitted first Address = 0xF700 and Command = 0x01
Key-2 = 00000000 11101111 01000000 10111111 as LSB is Transmitted first Address = 0xF700 and Command = 0x02
Key-2 = 00000000 11101111 11000000 00111111 as LSB is Transmitted first Address = 0xF700 and Command = 0x03
```

![Burst Pulse for Repeat Mode](https://2.bp.blogspot.com/-HW0l2sAyRi8/V287NWEAjYI/AAAAAAAAAS4/-s9GGdLmuoM3VmsDv_q3j3Ix84ynnRvcACKgB/s1600/necrepeat.png)

A command is transmitted only once, even when the key on the remote control remains pressed. Every 110ms a repeat code is transmitted for as long as the key remains down. This repeat code is simply a 9ms AGC pulse followed by a 2.25ms space and a 560µs burst.  

![NEC Sequence](https://2.bp.blogspot.com/-I8h4zxOMg_Y/V287NVpcXAI/AAAAAAAAAS4/ip2N5OYXwRMZ0bD9vhgqoN9Q3El-ECZGwCKgB/s1600/necsequence.png)  

### Extended NEC Protocol
The NEC protocol is so widely used that soon all possible addresses were used up. By sacrificing the address redundancy the address range was extended from 256 possible values to approximately 65000 different values. This way the address range was extended from 8 bits to 16 bits without changing any other property of the protocol.  
By extending the address range this way the total message time is no longer constant. It now depends on the total number of 1's and 0's in the message. If you want to keep the total message time constant you'll have to make sure the number 1's in the address field is 8 (it automatically means that the number of 0's is also 8). This will reduce the maximum number of different addresses to just about 13000.  

The command redundancy is still preserved. Therefore each address can still handle 256 different commands.  

![Extended NEC Protocol](https://2.bp.blogspot.com/-3AYyxutfCFQ/V287MvRbnSI/AAAAAAAAAS4/ntbYvqlC4FAQXOd4d2rkscruOYNvPDpMQCKgB/s1600/necexttrain.png)

Keep in mind that 256 address values of the extended protocol are invalid because they are in fact normal NEC protocol addresses. Whenever the low byte is the exact inverse of the high byte it is not a valid extended address.  
So this is all about NEC Protocol, as i have to decode this protocol, that's why i need a micro-controller for that and i select PIC18F4550 (because i have PIC18F4550 development board with me and TSOP1738 is also there).  
Here is the rough schematic diagram of the setup.  

![Schematic](https://4.bp.blogspot.com/-m8Uu--NAfbA/V287MkihwUI/AAAAAAAAAS4/ZwP3QUphXGgJL0Szl_kvYcUxXVRoFJ-lgCKgB/s1600/extendednec.png)

My TV Remote Controller and My PIC18F4550 Development Board.  

![Hardware Setup](https://2.bp.blogspot.com/-Lb201fLA-4w/V287MNUayHI/AAAAAAAAAS4/SlN-CWTgy0obS4H7TjsAKz8ukOiQ0GLpQCKgB/s1600/DevBoard.jpg)

### Working
Whenever a key is pressed on TV Remote Controller, TSOP1738 decodes it 38KHz burst and then micro-controller decodes this signal coming from the remote. And if signal is decoded properly then Address and Command is displayed on the 16x2 LCD of the development board. If there is some problem in the command then 0xFF is displayed which means data not decoded properly and there is some problem.  
TSOP1738 is connected to RB0 pin and my board has 8-LED's connected to PORTB of micro-controller, so i can't use LED connected to RB0 pin and can use LED's connected to RB1 to RB7 pin. So if someone presses key-1 on remote LED connected to RB1 pin glows, similarly on pressing key-2 on remote, LED connected to RB2 pin glows and so on, if someone press key-0 on remote, all LED's connected to PORTB will turn off.  
NOTE: There is some problem with my board LED connected to RB5 that's why it doesn't glow when pressing Key-5 on remote, but if you see the video, data is displayed properly on 16x2 LCD.  

This video illustrates the whole working of the project.  
[YouTube Video](https://www.youtube.com/watch?v%3DchB4imOdL1E)

### References
[NEC Protocol](http://www.sbprojects.com/knowledge/ir/nec.php)
