public class SudokuThread2 extends Thread{//This will be the thread that will run
        long minPrime;
        int nums;                         //the sudoku generate() algorithm
        SudokuThread2(long minPrime, int nums){
            this.minPrime = minPrime;
            this.nums = nums;
            this.setName("Thread2");
        }

        public void run(){
            Sudoku.generateTable(this.nums);//Generate sudoku with given or default nums
        }
    }