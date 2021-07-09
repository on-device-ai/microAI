# How to execute MicroPython script  
Here are two scripts: **hello_world.py** and **person_detection.py**, which correspond to the TensorFlow Lite for Microcontrollers examples.  
Since the "[CAM-C1126S2U smart dual-lens camera module](http://en.t-firefly.com/product/dev/camc1126s2u.html)" uses adb ([Android Debug Bridge](https://developer.android.com/studio/command-line/adb)) to connect to the computer, you may need to install additional drivers or utility software on the computer.  
So, please make sure that the adb connection between the computer and CAM-C1126S2U is correct before executing the script.  
The first is hello_world.py, which corresponds to the "hello_world" example. Upload script to CAM-C1126S2U:  
`adb push hello_world.py /userdata`  
Execute the script:  
`adb shell micropython /userdata/hello_world.py`  
The video of the execution process is as follows:  
![hello_world](https://user-images.githubusercontent.com/44540872/125034515-3fe2ba00-e0c3-11eb-9252-95f62af19c92.gif)  
Then execute person_detection.py, which corresponds to the "person_detection" example, and the execution process is the same as that of hello_world.py:  
`adb push person_detection.py /userdata`  
`adb shell micropython /userdata/person_detection.py`  
The video of the execution process is as follows:
![person_detection](https://user-images.githubusercontent.com/44540872/125034312-0742e080-e0c3-11eb-92de-61bf714f06a6.gif)  
Note: Since CAM-C1126S2U can have multiple paths with one camera, the above video will display the UVC Camera video and the inference result of the "person_detection" example at the same time.

  


