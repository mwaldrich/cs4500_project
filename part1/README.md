For this project, we decided to stick with CwC for our language of choice to both familiarize ourselves better for 
future projects (with the expectation that they had to be written in CwC) and performance since the CwC is such a low
level language. While we do think we are both more familiar with the language (and C / C++ as well), the manual memory
management, forced expressiveness of the language, and other features made developing with this language exponentially
harder than with other languages. We felt like we were focusing more on fiddling with the language itself to get it to 
work properly than the actual logic of the program. While working with a low level language has its benefits, our lack 
of experience with the language and other factors in hindsight make us wish we had
chosen a higher level language to work with.

For the actual implementation of our project though, we did make extensive use of our String and StrList classes from 
our previous assignment which was very helpful. The logic of our program runs roughly as follows:

1.) Take in command line arguments and parse them against the supported argument flags, extracting out information like
which file to read, number of bytes to read, what operations on file user wishes to perform, etc.

2.) Reading the given SOR file to extract out schema information from the longest row in the first 500 columns of the
file, ie the number of columns to make and the type of each one. 

3.) Reading in the number of bytes from the designated starting position from the parsed command line
arguments (or set defaults), splitting the read file into rows (skipping first and last rows if needed).

4.) For each split row, then parsing it into a list of fields. Then turning the set of rows into a set of columns (from
earlier), filling missing fields with empty fields.

6.) Performing any user requested operations on the created set of columns.

In all, we believe our implementation performs reasonably well, but still suffers from a unoptimal implementation coming
from our lack of experience with the language.