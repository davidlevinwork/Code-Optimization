# Code-Optimization

C program that aims to improve and optimize a piece of code of another programmer. <br />

The original code is `original_code.c` and the optimized code is `optimized_code.c`. <br />

### Code improvements

During the optimization process, i used `Gprof`. <br />

* Using Registers int insted int for variables that are in often use during the function
* Using macro expression, and make the math operation on the expression instead of doing it during compilation time.
* Use #define instead "small functions" - save the ru-time call for function  

Before each function and significant action in the code, you can read the relevant notes. <br />
Besides those notes, you can see the improvments in the run-time: <br />

Original run-time code: <br />
<img src="Original_Time.png" width = "230"> 

Optimized run-time code: <br />
<img src="Optimized_Time.png" width = "230"> 
