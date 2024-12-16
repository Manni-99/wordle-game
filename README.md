# Wordle-Game
### Wordle
A command-line application that allows you to play Wordle with various options to filter out possible word guesses based on previous guesses and their feedback.
As of the right now this version has a text file of 10000 words that can be filtered based on the users input.
If you would like to insert a bigger txt such as a dictionary that is fine aswell, the program filters out all words that are longer or shorter than five words.

### Features
Word Filtering: Filter out invalid guesses based on wrong letters, correct positions (green), and misplaced positions (yellow).
Test Suite: Unit tests to verify the functionality of the word filtering and other utilities.

### Requirements
C++17 or later
Google Test (for unit testing)

### Building the program
In your terminal type "build" which should build and link all necessary files to use the program.
The main program file requires an input of a text file which in this case should be by default "wordlist.txt".
To run the main program just type "./wordle wordlist.txt" into the terminal after you've built your files and you should be good to go.
After building there is a test file called "wordle_tests" which runs unit tests on the used functions of the program. 
You can run the tests with the command "./wordle_tests". If you encounter any problems building or running the test program please refer to the requirements section.

### Game Input
The game works interactively, allowing you to input guesses and receive feedback. The feedback will show you:

Gray (wrong): The letters that are not in the word at all.
Green: input for the correct letters and their index
Yellow: The letters that are in the word but not in the correct position.

So at startup you're expected to;
First: input the letters that are not in the word.
Second: input the letters and index of the letters that you know are in the correct position (in case you don't have any correct letters you can input an empty string).
Third: input the letters that are in the word but have an incorrect index to the actual word.

Then the program should return a filtered list of possible candidates if matches any of the words in the text file.

