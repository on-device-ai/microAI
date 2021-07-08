# microAI
This project is the early software development of our ODA-1000 product. Therefore, this project is conceptual, trying to integrate MicroPython and TensorFlow Lite for Microcontrollers on Embedded Linux for the initial development and teaching of TinyML technology.  
Like all Embedded systems, Embedded Linux differs from Desktop or Server Linux in that it is a customized and restricted system. Although the two technologies integrated into this project are using for microcontrollers, it should be appropriate to apply them to embedded Linux in a "crossover" manner.  
One factor is that the microcontroller is a more restricted system and will encounter many difficulties in the early development phase. The Embedded Linux system with more resources can be used as a product prototype and used for evaluation or demonstration.  
This project base on the "[CAM-C1126S2U smart dual-lens camera module](http://en.t-firefly.com/product/dev/camc1126s2u.html)" for the following reasons:  

* 32-bits MPU (Microprocessor), suitable for most applications  
* Embedded Linux system based on Buildroot, the software is provided by open source method  
* Built-in ISP (Image Signal Processor) on the chip, one image source (Camera) can provide multi-path  
  
Based on the manufacturer's [AI_UVC](https://wiki.t-firefly.com/en/CAM-C11262U/Application_scenarios.html#ai-uvc-camera) application, make the following modifications:  

1. Software modification, change Dual Camera to Signal Camera  
2. Add MicroPython and TensorFlow Lite for Microcontrollers software packages to Buildroot  
3. Increase the bridge library from C++11 to C code to reduce the difficulty of integration  
4. Through the ISP's multi-path mechanism, the UVC Camera function and the MicroPython application can obtain the same image source (Camera) at the same time  
  
To know how to building microAI on CAM-C1126S2U, please refer to HowToBuild.md in the rv1126_rv1109_linux_ai_camera directory.