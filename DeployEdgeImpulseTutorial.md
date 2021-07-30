# Deploy Edge Impulse's Tutorial  
This article explains how to deploy Edge Impulse's "[Adding sight to your sensors](https://docs.edgeimpulse.com/docs/image-classification)" tutorial to [microAI](https://github.com/on-device-ai/microAI).  
First, import the finished project "Tutorial: Adding sight to your sensors" import to your Edge Impulse Studio. Then enter the "Deployment" menu, select the "OpenMV library" option, and click the "Build" button to building the library :  
![210729_1](https://user-images.githubusercontent.com/44540872/127596970-f8b6b6c7-91bc-4279-b5b9-2fabb2c035f0.png)  
Decompress the compressed file of the completed library and take out the "trained.tflite" file:  
![210729_2](https://user-images.githubusercontent.com/44540872/127597000-2a1bbac3-5b05-42e4-85ff-01e7b80f0329.png)  
Because microAI is not compatible with OpenMV's MicroPython script, you need to write your script (such as [ei\_image\_classification.py](https://github.com/on-device-ai/microAI/blob/main/ei_image_classification.py)) to correspond to microAI. Then upload the "trained.tflite" and the written script:  
`adb push trained.tflite /userdata`  
`adb push ei_image_classification.py /userdata`  
Use the following command to execute the script:  
`adb shell micropython /userdata/ei_image_classification.py`  
The script running results are as follows:  
![210730](https://user-images.githubusercontent.com/44540872/127600173-ade56193-95ae-49a9-ba21-9f5fcacd668b.gif)  