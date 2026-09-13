import java.awt.Color;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.time.LocalTime;
import java.util.Random;

import javax.swing.BorderFactory;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;

public class Sudoku extends Thread{

    static Frame sudoku = null;//The frame that we will be working with
    final static int defaultNums = 25;//The default sudoku's given numbers
    final static LocalTime time = LocalTime.now();//Setting up time variables for the random generator
    final static int hours = time.getHour();
    final static int minutes = time.getMinute();
    final static int seconds = time.getSecond();
    static int givenNums = defaultNums;
    static int[][] unsolved = new int[9][9];//Creating the unsolved grid holder
    static int[][] solved = new int[9][9]; //Creating the solved grid 
    static SudokuButton[][] buttonHolder = new SudokuButton[9][9];
    static boolean isSolved = false;
    final static Random random = new Random(hours + minutes + seconds);

    public static void main(String[] args) throws InterruptedException{
        Sudoku thread = new Sudoku();//Initiating the main thread that hosts the jframe
        thread.start();
        SudokuThread2 thread2 = new SudokuThread2(10, givenNums);//Secondary thread for calculating algorithm
        thread2.start();

        sudoku = new Frame("Sudoku");//creating the frame and the menu
        JMenu menu = new JMenu("File");//Initiating menubar and items
        JMenuItem newSudoku = new JMenuItem("New Sudoku");
        sudoku.setLayout(new GridLayout(9, 9));
        menu.setBorder(BorderFactory.createLineBorder(Color.BLACK));
        newSudoku.setBorder(BorderFactory.createLineBorder(Color.BLACK));

        newSudoku.addActionListener(new ActionListener(){//Adding functionality to tab "New Sudoku"
            @Override
            public void actionPerformed(ActionEvent e){
                String[] options = new String[] {"Easy", "Medium", "Hard", "Custom", "cancel"};//Initiating difficulty window option
                int response = JOptionPane.showOptionDialog(null, "Please select difficulty!", "New game", JOptionPane.DEFAULT_OPTION, JOptionPane.PLAIN_MESSAGE, null, options, options[0]);
                if(response <= 2){//hard 22, medium 27, easy 32
                    unsolved = new int[9][9];//Resetting previous tables to avoid problems
                    solved = new int[9][9];
                    SudokuThread2 newThread = new SudokuThread2(10, 32 - 5 * response);//Initiating secondary thread
                    sudoku.setVisible(false);
                    newThread.start();
                    JOptionPane.showConfirmDialog(null, "Please wait for the sudoku to load", "Sudoku loading", JOptionPane.DEFAULT_OPTION);
                    while(newThread.isAlive()){

                    }
                    generateUI(false);//Generate buttons
                    sudoku.setVisible(true);//Make frame visible once its ready
                    //print2D(solved);
                }else if(response == 3){
                    JSpinner spinner = new JSpinner(new SpinnerNumberModel(25, 17, 40, 1));
                    int numOption = JOptionPane.showOptionDialog(null, spinner, "Enter number between 17-40(invalid numbers will be set to 25)", JOptionPane.OK_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE, null, null, null);
                    if(numOption == JOptionPane.OK_OPTION){//If it is
                        int num = (Integer)spinner.getValue();//Get spinner value
                        solved = new int[9][9];//same job as previous
                        unsolved = new int[9][9];
                        SudokuThread2 newThread = new SudokuThread2(10, num);
                        sudoku.setVisible(false);
                        newThread.start();
                        JOptionPane.showConfirmDialog(null, "Please wait for the sudoku to load", "Sudoku loading", JOptionPane.DEFAULT_OPTION);
                        while(newThread.isAlive()){
                            
                        }
                        generateUI(false);
                        sudoku.setVisible(true);
                    }
                }
            }
        });

        menu.add(newSudoku);//Adding menu items to the frame
        sudoku.menuBar.add(menu);
        sudoku.repaint();//Update frame

        JOptionPane.showConfirmDialog(null, "Please wait for the sudoku to load", "Sudoku loading", JOptionPane.DEFAULT_OPTION);

        while(thread2.isAlive()){
            sleep(20);
        }

        generateUI(true);
        sudoku.setVisible(true);
        //print2D(solved);
    }

    public static void generateTable(int nums){//This will generate the solved and unsolved arrays
        int[][] table = new int[9][9];//Creating potential solution table
        int count = 0;//This variable will count how many numbers are placed on the current table
        while(true){
            table = new int[9][9];
            count= 0;
            while(count < nums){//Making a random grid that behaves to sudoku's rules;
                int nextX = random.nextInt(9);
                int nextY = random.nextInt(9);
                int nextVal = random.nextInt(10);
                if(isAllowed(table, nextX, nextY, nextVal)){
                    table[nextX][nextY] = nextVal;
                    count++;
                    if(count >= nums){
                        break;
                    }
                }
            }

            for(int i = 0; i < 9; i++){//Copying the unsolved grid for later use;
                for(int j = 0; j < 9; j++){
                    unsolved[i][j] = table[i][j];
                }
            }


            if(isSolvable(table, 0, 0)){//If there is a solution end task
                return;
            }

        }
    }

    public static boolean isAllowed(int[][] array, int x, int y, int val){//This will return true when we can place the val into the array[x][y]
        if(array[x][y] != 0){//if place is already taken;
            return false;
        }
        for(int i = 0; i < 9; i++){//If the place is taken vertically or horizontally;
            if(array[x][i] == val){
                return false;
            }

            if(array[i][y] == val){
                return false;
            }
        }

        int multX = x - x % 3;//These 2 lines 
        int multY = y - y % 3;//Find the block that the number belongs

        for(int i = 0; i < 3; i++){//Check if elligible for the block;
            for(int j = 0; j < 3; j++){
                if(array[i + multX][j + multY] == val){
                    return false;
                }
            }
        }

        return true;
    }

    public static boolean isSolvable(int[][] grid, int i, int j){
        if(i >= 9){
            i = 0;//Change column;
            j = j + 1;
        }
        if(j>=9){//The sudoku given has a solution;
            for(int i1 = 0; i1 < 9; i1++){
                for(int j1 = 0; j1 < 9; j1++){
                    solved[i1][j1] = grid[i1][j1];
                }
            }
            return true;
        }

        if(grid[i][j] != 0){
            return isSolvable(grid, i + 1, j);//Move to the next variable that isn't 0;
        }else{
            for(int k = 1; k < 10; k++){
                if(isAllowed(grid, i, j, k)){//Continue for all values that are allowed for that position;
                    grid[i][j] = k;
                    if(isSolvable(grid, i + 1, j)){
                        return true;
                    }
                }
                grid[i][j] = 0;//Reset the value for the next one to be used;
            }
        }
        return false;
    }

    public static void generateUI(boolean firstTime){
        if(!firstTime){//Delete older buttons if its not the first time operating
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    sudoku.remove(buttonHolder[i][j]);
                }
            }
        }
        buttonHolder = new SudokuButton[9][9];
        for(int i = 0; i < 9; i++){//Create new buttons
            for(int j = 0; j < 9; j++){
                SudokuButton button = new SudokuButton(solved[i][j], unsolved[i][j]);
                sudoku.add(button);
                buttonHolder[i][j] = button;
            }
        }
        sudoku.repaint();
    }

    public static void print2D(int[][] arr){//Printing 2D array for debug purposes
        System.out.println();
        for(int i = 0; i < arr.length; i++){
            for(int j = 0; j < arr[0].length; j++){
                System.out.print(" " + arr[i][j]);
            }
            System.out.println();
        }
        return;
    }

    public static void checkIfSolved(){//This will run each time a value is added to the table
        int count = 0;
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                if(buttonHolder[i][j].isCorrect){//Getting count of correct numbers
                    count++;
                }
            }
        }
        if(count == 81 && !isSolved){//If everything is correct, the sudoku is solved
            JOptionPane.showConfirmDialog(null, "Excellent work! Sudoku solved!", "You solved it!", JOptionPane.PLAIN_MESSAGE, JOptionPane.DEFAULT_OPTION);
            isSolved = true;
        }
        //System.out.println(count);
    }
}
