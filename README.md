 How to use:
 The program "TheShortestQuitFromTheLabyrinth" was made on "Dev-C++" environment.
 For proper work of the program, you should download all available files on the directory. 
 
 About

The main purpose of the program is to find the shortest way in the labyrinth(maze) from start point to finish. It's mean that labyrinth should have at least 2 path quilts and more but the program is looking for the shortest way from point to point. The program realized on a "Wave algorithm(Lee algorithm)" which based on the "Breadth-first search". Realization of this algorithm looks: The start point is marked '0'. After we are looking at the neighbors which situated to the left, right, up, down and assigning them '1'. After that, we are looking at all neighbors of '1' and assigning them '2'. That type of procedure is continued until we reach the quit point from the labyrinth and procedure finished the work.

 User guide

The program takes input data of labyrinth from Text File which named "input_labirint.txt" and it should be at the same directory as the main program file.  The labyrinth in the text file should look like the matrix and consist of '1' - wall and '0' - passing zone. The start point is marking digit '2' and quit point is marking digit '3'. The digits in the file should be separated by a gap between each other. But before the first and after last digit at row should be no gaps. The number of digits should be equal at all rows of the text file. 
After it, the user may run the program. In depending, whether the quit exists or not, the program will show the result at the console window:
- Coordinates of the start and the quit points;
- The message - "We found the shortest path!";
- Coordinates of every one point of the shortest path;
- Displaying the same labyrinth where red crosses - the unpassing zones, grey fields - the passing zones, 'S' and 'F' points of start and finish accordingly and the symbols 'o' realized marking of the shortest way between 'S' and 'F' points.
If the way between 'S' and 'F' points not exists, the program will show the message: "A path from starting position to quit is unreachable!" and the program will finish the work.
