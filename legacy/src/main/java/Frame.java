import java.awt.Color;
import java.awt.Dimension;
import java.awt.Toolkit;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JMenuBar;

public class Frame extends JFrame{//Custom frame with standard operations

    //Here we get the dimensions we are working with
    final int defaultWeidth = 800;
    final int defaultHeight = 600;
    final Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    final int width = (int)screenSize.getWidth();
    final int height = (int)screenSize.getHeight();
    final int logoWidth = 100;//top left icon dimensions
    final int logoHeight = 50;
    JMenuBar menuBar = new JMenuBar();
    

    Frame(String title){
        this.getContentPane().setBackground(Color.DARK_GRAY);
        this.setResizable(true);//Set if it can be resisable
        this.setVisible(false);//Set visibility
        this.setTitle(title);//Set title
        this.setLocation((width - defaultWeidth) / 2, (height - defaultHeight) / 2);//Center location
        this.setDefaultCloseOperation(DISPOSE_ON_CLOSE);//DISPOSE TO AVOID MEMORY BEING USED AFTER CLOSE, DO NOT USE EXIT_ON_CLOSE
        this.setSize(new Dimension(defaultWeidth, defaultHeight));//Set default dimensions
        this.setLayout(null);//Null so that i can set custom dimensions to all jframe objects
        this.setJMenuBar(menuBar);
        menuBar.setBorder(BorderFactory.createLineBorder(Color.BLACK));
    }

}
