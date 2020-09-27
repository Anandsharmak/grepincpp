Clone the repossitory
To make executable file in linux : 
use "g++ grep.cpp -o grep"
Then use command " grep < input.txt > output.txt "

Where input. txt contains the input of format :
-[options] pattern filename.ext
example =1.  -wi line is sample.txt
	 2.   line is sample.txt

Input.txt can contain multiple lines with inputs such as above 
Output.txt algorithm outputs all of them with a '\n' between each output for better readability.

We have used KMP algorithm :
Used for finding patterns in a text

Time Complexity is : O(m+n) where m is total length of sample file and n is length of pattern
Space Complexity is : O(m) 


In KMP is a standard algorithm for pattern search
It is a standard algorithm.

We have modified comparison in KMP so that -w case in which we have to extract a word which can even have .,?,! 
connected to it. For this we add " "+pattern+" " for search

We have also provided a testcase function for checking testcases locally