# Sudoku
This is a simple implementation of the sudoku backtracking algorithm along with the java swing library in order
to create a simple and efficient GUI based application. The user can choose between multiple difficulties as well as a specific
number of known numbers between 17 and 40. There is a jar file provided that can run with java 8 or higher. A congratulations message is provided 
after the player succesfully completes the puzzle. Internally, during the generation phase the program randomly distributes a set number of numbers and then checks if a possible solution exists. If it does not it starts from scratch. The process repeats until a valid solution is identified. Then, the player is tasked to solve the sudoku. If it does not fit the player's skill-base or mood due to it being too easy or hard, he can regenerate a new sudoku with a different given set of numbers. The program utilizes 2 threads: One for the graphical interface and one for the algorithm runtime.

## Prequisites
Java 8 environment
## How to run
~~~
java -jar sudoku1.0.jar
~~~
## How to compile locally
A pom.xml file is provided to compile using maven.
## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.