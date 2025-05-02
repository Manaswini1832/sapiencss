# Sapiencss

This is a simple programming language that's similar to human speech and can currently draw simple shapes like a rectangle, circle and a line to the screen based on the x-coordinate, y-coordinate, width, height, radius, length and other mentioned attributes.

Here's an example of the syntax:

```
MAKE RECTANGLE "r1" WITH color "red", x "10", y "10", width "5", height "10";
MAKE CIRCLE "c1" WITH color "yellow", x "20", y "30", radius "5";
MAKE LINE "l1" WITH color "teal", x "5", y "15", length "15";
```

This block of code generates the following image :

![Hello world result](https://github.com/Manaswini1832/sapiencss/blob/master/hello_world_sapiencss.png?raw=true)

## File structure

1. test.cpp and header files in the headers folder show all the classes and how they're used separately
2. sapienCSSCompiler.cpp contains the final compilation function that's used in the web application
3. While development, any changes can be made and tested using the test.cpp and the header files. Then the final changes should be updated in the sapienCSSCompiler.cpp file, which is then compiled to WASM and a js module to be used in the React Web App
