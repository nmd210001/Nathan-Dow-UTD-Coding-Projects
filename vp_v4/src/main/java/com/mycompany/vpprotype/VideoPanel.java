/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.vpprotype;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.io.File;
import javax.swing.JPanel;
import uk.co.caprica.vlcj.player.component.EmbeddedMediaPlayerComponent;

/**
 *
 * @author Nathan Dow
 */
public class VideoPanel extends JPanel{
    
    private final EmbeddedMediaPlayerComponent mediaPlayerComponent;
    
    public VideoPanel(){
        super();
        
        setLayout(new BorderLayout());
        //Set up the video context
        
        mediaPlayerComponent = new EmbeddedMediaPlayerComponent();
        
        setPreferredSize(new Dimension(600,400));
        add(mediaPlayerComponent);
        
        this.setVisible(true);
        
        //mediaPlayerComponentPlayer().media().play(File);
    }
    
    public void play(File file){
        
        mediaPlayerComponent.mediaPlayer().media().play(file.toString());
        
    }
}
