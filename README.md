# Project_Anukul
This repository contains the files related to the Miniature version of Anukul. We will be regularly updating this project with more code and information.
To get regular updates and also to discuss any doubts or ideas related to this project please use the facebook group:Project_Anukul. https://www.facebook.com/groups/1496000903746140/
The circuit that we have designed is easily reproducible. In this version we have used “Arduino” as the micro-controller. This miniature version is scalable to large farms by using microcontrollers and multiplexers.
The code has been written for arduino uno and all the uploaded libraries are arduino libraries. The circuit diagram has been created using Fritzing and has also been uploaded.
This project is designed with the purpose of reducing water wastage from agricultural activities and increasing the agricultural productivity so that we can save more water for drinking purposes during adverse conditions such as droughts and cater the water demands of future generations.
Basically the circuit consists of a temperature sensor, soil moisture sensor, light sensor, LCD, SD card module, all attached to a single arduino. RTC time module has been used to keep track of time.
The code has been written such that all of the inputs from the sensors will be stored on the SD Card and also vital information i.e. time and soil moisture value will be displayed on the LCD monitor.
Out of all the factors that affect the growth of the crop, we can only control soil moisture level. 
So here soil moisture level [V0] is a dependent variable. And sunlight value [Vs], temperature [Vt]  values are independent variables.(Note although optimal moisture level depends on other factors as well, for explanation purpose we are just considering 2 factors here)
Now there exists some unknown relation between these factors. Where [Ws] and [Wt] are the matrices that represent the unknown relation. We can predict optimal moisture level [V0] if we have [Ws] & [Wt]
                            [V0] = ([Ws]* [Vs]) + ([Wt]* [Vt]) 
When we do the experimental part on a farm, we will maintain different moisture levels in different sections of the farm. The section for which we got the highest yield corresponds to the optimal moisture value. Now we will import the data set of this section to machine. Using this data we can calculate matrices [Ws] & [Wt] with some error.
As we keep on feeding the machine with more and more data, we will reduce the error  in [Ws] & [Wt] i.e. we can predict optimal moisture level more accurately.
As the years passes this will grow exponentially as is evident with any Machine Learning system and we might also be able to predict some unforeseen pattern.
We would appreciate it if anybody could contribute to this project by writing the code related to machine learning.
This miniature model can be scaled up. People can use this idea on a Global scale to regulate the water supply given to agricultural land so as to conserve water. This will also improve the agricultural yield of farmlands. 
