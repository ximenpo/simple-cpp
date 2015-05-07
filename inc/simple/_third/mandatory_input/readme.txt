mandatory_input 
utility class for stream input

Intro:  
------
Processing formatted input such as comma separated lists, ISBN numbers, or phone numbers
is a very common task.   Unfortunately, there is no easy and consistent way to control
fixed formatting characters in C++ input streams. 

The C++ class mandatory_input make controlled formatted input as easy as C's standard
scanf()/fscanf(), and without the complexity of more powerful solutions such as for 
example regular expressions or parser generators.

Files: 
------
license.txt          license conditions of this source file
mandatory_input.h    header to be included when the class shall be used
mandatory_input.cpp  implementation to be included in the project using the class
test.cpp	         demo, simple test programmes, and benchmark
text.txt             test data for the benchmark

Remark: 
-------
The source code is commented or doxygen documentation
			
