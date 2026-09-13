import java.awt.Color;
import java.awt.Font;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import javax.swing.BorderFactory;
import javax.swing.JButton;

public class SudokuButton extends JButton{
    int correctNum;//The correct value for the button
    int givenNum;//The given and current value
    boolean isCorrect = false;//Initiating isCorrect value
    SudokuButton(int correctNum, int givenNum){
        JButton b = this;
        this.givenNum = givenNum;
        this.correctNum = correctNum;
        this.setBackground(Color.WHITE);//Set background
        this.setBorder(BorderFactory.createLineBorder(Color.BLACK));//Set border
        this.setFont(new Font("arial", Font.BOLD, 20));//Sent font
        if(givenNum == 0){//Give starting values to the button
            this.setText("");
        }else{
            this.setText(String.valueOf(givenNum));
        }
        this.setForeground(Color.RED);//Make the numbers color red
        if(correctNum == givenNum){//if given from the start make color black and make them immutable
            this.setFocusable(false);//Cannot be focused
            this.setEnabled(false);//Cannot be clicked
            this.setForeground(Color.BLACK);
            this.setBackground(Color.LIGHT_GRAY);
            isCorrect = true;
        }else{
            this.addKeyListener(new KeyAdapter() {//Add listener for user input, code is quite self explanatory,
                @Override                         //If Input ranges from 1-9, it checks if it is correct, else its automatically false
                public void keyPressed(KeyEvent e){
                    b.setBackground(Color.LIGHT_GRAY);
                    int val = e.getKeyCode();
                    if(val == KeyEvent.VK_1){
                        b.setText("1");
                        if(correctNum == 1){
                            isCorrect = true;
                        }else{
                            isCorrect = false;
                        }
                    }else if(val == KeyEvent.VK_2){
                        b.setText("2");
                        if(correctNum == 2){
                            isCorrect = true;
                        }else{
                            isCorrect = false;
                        }
                    }else if(val == KeyEvent.VK_3){
                        b.setText("3");
                        if(correctNum == 3){
                            isCorrect = true;
                        }else{
                            isCorrect = false;
                        }
                    }else if(val == KeyEvent.VK_4){
                        b.setText("4");
                        if(correctNum == 4){
                            isCorrect = true;
                        }else{
                            isCorrect = false;
                        }
                    }else if(val == KeyEvent.VK_5){
                        b.setText("5");
                        if(correctNum == 5){
                            isCorrect = true;
                        }else{
                            isCorrect = false;
                        }
                    }else if(val == KeyEvent.VK_6){
                        b.setText("6");
                        if(correctNum == 6){
                            isCorrect = true;
                        }else{
                            isCorrect = false;
                        }
                    }else if(val == KeyEvent.VK_7){
                        b.setText("7");
                        if(correctNum == 7){
                            isCorrect = true;
                        }else{
                            isCorrect = false;
                        }
                    }else if(val == KeyEvent.VK_8){
                        b.setText("8");
                        if(correctNum == 8){
                            isCorrect = true;
                        }else{
                            isCorrect = false;
                        }
                    }else if(val == KeyEvent.VK_9){
                        b.setText("9");
                        if(correctNum == 9){
                            isCorrect = true;
                        }else{
                            isCorrect = false;
                        }
                    }else{
                        b.setBackground(Color.WHITE);
                        b.setText(" ");
                        isCorrect = false;
                    }
                    Sudoku.checkIfSolved();
                }
            });
        }
    }

    public boolean checkIfCorrect(){//Return isCorrect value using this function, its useful since num is static and we cant use this.nums
        return isCorrect;
    }
}
