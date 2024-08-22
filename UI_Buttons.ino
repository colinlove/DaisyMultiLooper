#define TopRow1 petal.buttons[0].RisingEdge()
#define TopRow2 petal.buttons[1].RisingEdge()
#define TopRow3 petal.buttons[2].RisingEdge()
#define TopRow4 petal.buttons[3].RisingEdge()
#define TopRow5 petal.buttons[4].RisingEdge()

#define BottomRow1 petal.buttons[5].RisingEdge()
#define BottomRow2 petal.buttons[6].RisingEdge()
#define BottomRow3 petal.buttons[7].RisingEdge()
#define BottomRow4 petal.buttons[8].RisingEdge()
#define BottomRow5 petal.buttons[9].RisingEdge()

void UI_Menu_Buttons() {
  if ((UI==UI_Welcome) || (UI==UI_Anim) || (UI==UI_xx)) {
    if (TopRow5) {
      UI=UI_FX1;
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }

  } else if (UI==UI_Loop_Play_Rec) {
    if (TopRow1) {
      switch (loop1State) {
        case idle_empty:
          loop1State=armed;
          break;
        case armed:
          loop1State=idle_empty;
          break;
        case rec_first:
          mod = len;
          len = 0;
          loop1State = overdub;
          break;
        case play:
        case idle:
          loop1State = overdub;
          break;
        case overdub:
          loop1State = play;
          break;
      }
      if (loop2State == overdub) loop2State = play; // downgrade any recording loops to playing
      if (loop3State == overdub) loop3State = play;
      if (loop4State == overdub) loop4State = play;
    } else if (TopRow2) {
      switch (loop2State) {
        case idle_empty:
          if (loop1State==idle_empty || loop1State==armed) break;
        case idle:
        case play:
          loop2State = overdub;
          break;
        case (overdub):
          loop2State = play;
          break;
      }
      if (loop1State == rec_first) {
        mod = len;
        len = 0;
        loop1State = play;
      }
      if (loop1State == overdub) loop1State = play; // downgrade any recording loops to playing
      if (loop3State == overdub) loop3State = play;
      if (loop4State == overdub) loop4State = play;
    } else if (TopRow3) {
      switch (loop3State) {
        case idle_empty:
          if (loop1State==idle_empty || loop1State==armed) break;
        case idle:
        case play:
          loop3State = overdub;
          break;
        case overdub:
          loop3State = play;
          break;
      }
      if (loop1State == rec_first) {
        mod = len;
        len = 0;
        loop1State = play;
      }
      if (loop1State == overdub) loop1State = play; // downgrade any recording loops to playing
      if (loop2State == overdub) loop2State = play;
      if (loop4State == overdub) loop4State = play;
    } else if (TopRow4) {
      switch (loop4State) {
        case idle_empty:
          if (loop1State==idle_empty || loop1State==armed) break;
        case idle:
        case play:
          loop4State = overdub;
          break;
        case overdub:
          loop4State = play;
          break;
      }
      if (loop1State == rec_first) {
        mod = len;
        len = 0;
        loop1State = play;
      }
      if (loop1State == overdub) loop1State = play; // downgrade any recording loops to playing
      if (loop2State == overdub) loop2State = play;
      if (loop3State == overdub) loop3State = play;
    } else if (TopRow5) {
      UI=UI_FX1;
    } else if (BottomRow1) {
      switch (loop1State) {
        case idle_empty:
          UI=UI_Loop1;
          break;
        case rec_first:
          mod = len;
          len = 0;
          loop1State = play;
          break;
        case overdub:
        case idle:
          loop1State = play;
          break;
        case play:
          loop1State = idle;
          break;
      }
    } else if (BottomRow2) {
      switch (loop2State) {
        case idle_empty:
          UI=UI_Loop2;
          break;
        case overdub:
        case idle:
          loop2State = play;
          break;
        case play:
          loop2State = idle;
          break;
      }
    } else if (BottomRow3) {
      switch (loop3State) {
        case idle_empty:
          UI=UI_Loop3;
          break;
        case overdub:
        case idle:
          loop3State = play;
          break;
        case play:
          loop3State = idle;
          break;
      }
    } else if (BottomRow4) {
      switch (loop4State) {
        case idle_empty:
          UI=UI_Loop4;
          break;
        case overdub:
        case idle:
          loop4State = play;
          break;
        case play:
          loop4State = idle;
          break;
      }
    } else if (BottomRow5) {
      UI=UI_Loop_Options;
    }
    if (petal.buttons[0].TimeHeldMs() >= 1000 && loop1State != idle_empty) { // hold top left
      ResetBuffer();
      loop1State = idle_empty;
      loop2State = idle_empty;
      loop3State = idle_empty;
      loop4State = idle_empty;
    }

  } else if (UI==UI_FX1) {
    if (TopRow1) {
      UI=UI_Dist;
    } else if (TopRow2) {
      UI=UI_Verb;
    } else if (TopRow3) {
      UI=UI_Echo;
    } else if (TopRow4) {
      UI=UI_Trem;
    } else if (TopRow5) {
      UI=UI_FX2;
    } else if (BottomRow1) {
      toggle_level(inp_dist, last_dist); // Turn Dist On/Off
    } else if (BottomRow2) {
      toggle_level(inp_verb, last_verb); // Turn Verb On/Off
    } else if (BottomRow3) {
      toggle_level(inp_echo, last_echo); // Turn Echo On/Off
    } else if (BottomRow4) {
      toggle_level(inp_trem, last_trem); // Turn Trem On/Off
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }
       
  } else if (UI==UI_FX2) {
    if (TopRow1) {
      UI=UI_Comp;
    } else if (TopRow2) {
      UI=UI_Octv;
    } else if (TopRow3) {
      UI=UI_Mell;
    } else if (TopRow4) {
      UI=UI_Chor;
    } else if (TopRow5) {
      UI=UI_VU;
    } else if (BottomRow1) {
      UI=UI_xx; // Turn Comp On/Off
    } else if (BottomRow2) {
      UI=UI_xx; // Turn Octv On/Off
    } else if (BottomRow3) {
      inp_mell=1-inp_mell; // Turn Mell On/Off
    } else if (BottomRow4) {
      UI=UI_xx; // Turn Chor On/Off
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }
  } else if (UI==UI_VU) {
    if (TopRow5) {
      UI=UI_FX1;
    }
  } else if (UI==UI_Loop1) {
    if (TopRow1) {
      UI=UI_xx; // Turn NoSt On/Off (Loop 1)
    } else if (TopRow2) {
      UI=UI_xx; // Turn NoKl On/Off (Loop 1)
    } else if (TopRow3) {
      UI=UI_xx; // Switch Half/4th/8th/16th/Off (Loop 1)
    } else if (TopRow4) {
      UI=UI_xx; // Turn Dlay On/Off (Loop 1)
    } else if (TopRow5) {
      UI=UI_FX1;
    } else if (BottomRow1) {
      UI=UI_xx; // Turn Lnk2 On/Off (Loop 1)
    } else if (BottomRow2) {
      UI=UI_xx; // Turn Lnk3 On/Off (Loop 1)
    } else if (BottomRow3) {
      UI=UI_xx; // Turn Lnk4 On/Off (Loop 1)
    } else if (BottomRow4) {
      UI=UI_Anim; // A Random Setting We can Add Later
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }

  } else if (UI==UI_Loop2) {
    if (TopRow1) {
      UI=UI_xx; // Turn NoSt On/Off (Loop 2)
    } else if (TopRow2) {
      UI=UI_xx; // Turn NoKl On/Off (Loop 2)
    } else if (TopRow3) {
      UI=UI_xx; // Switch Half/4th/8th/16th/Off (Loop 2)
    } else if (TopRow4) {
      UI=UI_xx; // Turn Dlay On/Off (Loop 2)
    } else if (TopRow5) {
      UI=UI_FX1;
    } else if (BottomRow1) {
      UI=UI_xx; // Turn Lnk1 On/Off (Loop 2)
    } else if (BottomRow2) {
      UI=UI_xx; // Turn Lnk3 On/Off (Loop 2)
    } else if (BottomRow3) {
      UI=UI_xx; // Turn Lnk4 On/Off (Loop 2)
    } else if (BottomRow4) {
      UI=UI_Anim; // A Random Setting We can Add Later
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }

  } else if (UI==UI_Loop3) {
    if (TopRow1) {
      UI=UI_xx; // Turn NoSt On/Off (Loop 3)
    } else if (TopRow2) {
      UI=UI_xx; // Turn NoKl On/Off (Loop 3)
    } else if (TopRow3) {
      UI=UI_xx; // Switch Half/4th/8th/16th/Off (Loop 3)
    } else if (TopRow4) {
      UI=UI_xx; // Turn Dlay On/Off (Loop 3)
    } else if (TopRow5) {
      UI=UI_FX1;
    } else if (BottomRow1) {
      UI=UI_xx; // Turn Lnk1 On/Off (Loop 3)
    } else if (BottomRow2) {
      UI=UI_xx; // Turn Lnk2 On/Off (Loop 3)
    } else if (BottomRow3) {
      UI=UI_xx; // Turn Lnk4 On/Off (Loop 3)
    } else if (BottomRow4) {
      UI=UI_Anim; // A Random Setting We can Add Later
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }

  } else if (UI==UI_Loop4) {
    if (TopRow1) {
      UI=UI_xx; // Turn NoSt On/Off (Loop 4)
    } else if (TopRow2) {
      UI=UI_xx; // Turn NoKl On/Off (Loop 4)
    } else if (TopRow3) {
      UI=UI_xx; // Switch Half/4th/8th/16th/Off (Loop 4)
    } else if (TopRow4) {
      UI=UI_xx; // Turn Dlay On/Off (Loop 4)
    } else if (TopRow5) {
      UI=UI_FX1;
    } else if (BottomRow1) {
      UI=UI_xx; // Turn Lnk1 On/Off (Loop 4)
    } else if (BottomRow2) {
      UI=UI_xx; // Turn Lnk2 On/Off (Loop 4)
    } else if (BottomRow3) {
      UI=UI_xx; // Turn Lnk3 On/Off (Loop 4)
    } else if (BottomRow4) {
      UI=UI_Anim; // A Random Setting We can Add Later
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }

  } else if (UI==UI_Loop_Options) {
    if (TopRow1) {
      UI=UI_xx; // Solo Loop 1
    } else if (TopRow2) {
      UI=UI_xx; // Solo Loop 2
    } else if (TopRow3) {
      UI=UI_xx; // Solo Loop 3
    } else if (TopRow4) {
      UI=UI_xx; // Solo Loop 4
    } else if (TopRow5) {
      UI=UI_FX1;
    } else if (BottomRow1) {
      UI=UI_xx; // Start All
    } else if (BottomRow2) {
      UI=UI_xx; // Stop All
    } else if (BottomRow3) {
      UI=UI_xx; // Tape Stop
    } else if (BottomRow4) {
      UI=UI_xx; // Kill
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }

  } else if (UI==UI_Dist) {
    if (TopRow1) {
      UI=UI_FX1; 
    } else if (TopRow2) {
      UI=UI_FX1; 
    } else if (TopRow3) {
      UI=UI_FX1; 
    } else if (TopRow4) {
      UI=UI_FX1; 
    } else if (TopRow5) {
      UI=UI_FX2;
    } else if (BottomRow1) {
      set_dist(1); // Set Dist to Level 1
      UI=UI_FX1;
    } else if (BottomRow2) {
      set_dist(2); // Set Dist to Level 2
      UI=UI_FX1;
    } else if (BottomRow3) {
      set_dist(3); // Set Dist to Level 3
      UI=UI_FX1;
    } else if (BottomRow4) {
      set_dist(4); // Set Dist to Level 4
      UI=UI_FX1;
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }


  } else if (UI==UI_Verb) {
    if (TopRow1) {
      UI=UI_FX1; 
    } else if (TopRow2) {
      UI=UI_FX1; 
    } else if (TopRow3) {
      UI=UI_FX1; 
    } else if (TopRow4) {
      UI=UI_FX1; 
    } else if (TopRow5) {
      UI=UI_FX2;
    } else if (BottomRow1) {
      set_verb(1);
      UI=UI_FX1; // Set Verb to Level 1
    } else if (BottomRow2) {
      set_verb(2);
      UI=UI_FX1; // Set Verb to Level 2
    } else if (BottomRow3) {
      set_verb(3);
      UI=UI_FX1; // Set Verb to Level 3
    } else if (BottomRow4) {
      set_verb(4);
      UI=UI_FX1; // Set Verb to Level 4
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }
 

  } else if (UI==UI_Echo) {
    if (TopRow1) {
      UI=UI_FX1; 
    } else if (TopRow2) {
      UI=UI_FX1; 
    } else if (TopRow3) {
      UI=UI_FX1; 
    } else if (TopRow4) {
      UI=UI_FX1; 
    } else if (TopRow5) {
      UI=UI_FX2;
    } else if (BottomRow1) {
      set_echo(1);
      UI=UI_FX1; // Set Echo to Level 1
    } else if (BottomRow2) {
      set_echo(2);
      UI=UI_FX1; // Set Echo to Level 2
    } else if (BottomRow3) {
      setEchoLoopFraction();
      UI=UI_FX1; // Set Echo to Level 3
    } else if (BottomRow4) {
      tap_tempo();
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }


  } else if (UI==UI_Trem) {
    if (TopRow1) {
      UI=UI_FX1; 
    } else if (TopRow2) {
      UI=UI_FX1; 
    } else if (TopRow3) {
      UI=UI_FX1; 
    } else if (TopRow4) {
      UI=UI_FX1; 
    } else if (TopRow5) {
      UI=UI_FX2;
    } else if (BottomRow1) {
      set_trem(1);
      UI=UI_FX1; // Set Trem to Level 1
    } else if (BottomRow2) {
      set_trem(2);
      UI=UI_FX1; // Set Trem to Level 2
    } else if (BottomRow3) {
      set_trem(3);
      UI=UI_FX1; // Set Trem to Level 3
    } else if (BottomRow4) {
      set_trem(4);
      UI=UI_FX1; // Set Trem to Level 4
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }


  } else if (UI==UI_Comp) {
    if (TopRow1) {
      UI=UI_FX2; 
    } else if (TopRow2) {
      UI=UI_FX2; 
    } else if (TopRow3) {
      UI=UI_FX2; 
    } else if (TopRow4) {
      UI=UI_FX2; 
    } else if (TopRow5) {
      UI=UI_FX1;
    } else if (BottomRow1) {
      UI=UI_xx; // Set Comp to Level 1
    } else if (BottomRow2) {
      UI=UI_xx; // Set Comp to Level 2
    } else if (BottomRow3) {
      UI=UI_xx; // Set Comp to Level 3
    } else if (BottomRow4) {
      UI=UI_xx; // Set Comp to Level 4
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }


  } else if (UI==UI_Octv) {
    if (TopRow1) {
      UI=UI_FX2; 
    } else if (TopRow2) {
      UI=UI_FX2; 
    } else if (TopRow3) {
      UI=UI_FX2; 
    } else if (TopRow4) {
      UI=UI_FX2; 
    } else if (TopRow5) {
      UI=UI_FX1;
    } else if (BottomRow1) {
      UI=UI_xx; // Set Octv to Level 1
    } else if (BottomRow2) {
      UI=UI_xx; // Set Octv to Level 2
    } else if (BottomRow3) {
      UI=UI_xx; // Set Octv to Level 3
    } else if (BottomRow4) {
      UI=UI_xx; // Set Octv to Level 4
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }


  } else if (UI==UI_Mell) {
    if (TopRow1) {
      UI=UI_FX2; 
    } else if (TopRow2) {
      UI=UI_FX2; 
    } else if (TopRow3) {
      UI=UI_FX2; 
    } else if (TopRow4) {
      UI=UI_FX2; 
    } else if (TopRow5) {
      UI=UI_FX1;
    } else if (BottomRow1) {
      UI=UI_xx; // Set Mell to Level 1
    } else if (BottomRow2) {
      UI=UI_xx; // Set Mell to Level 2
    } else if (BottomRow3) {
      UI=UI_xx; // Set Mell to Level 3
    } else if (BottomRow4) {
      UI=UI_xx; // Set Mell to Level 4
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }


  } else if (UI==UI_Chor) {
    if (TopRow1) {
      UI=UI_FX2; 
    } else if (TopRow2) {
      UI=UI_FX2; 
    } else if (TopRow3) {
      UI=UI_FX2; 
    } else if (TopRow4) {
      UI=UI_FX2; 
    } else if (TopRow5) {
      UI=UI_FX1;
    } else if (BottomRow1) {
      UI=UI_xx; // Set Chor to Level 1
    } else if (BottomRow2) {
      UI=UI_xx; // Set Chor to Level 2
    } else if (BottomRow3) {
      UI=UI_xx; // Set Chor to Level 3
    } else if (BottomRow4) {
      UI=UI_xx; // Set Chor to Level 4
    } else if (BottomRow5) {
      UI=UI_Loop_Play_Rec;
    }

  }
  
  if(petal.buttons[0].Pressed() || petal.buttons[9].Pressed() && (UI!=UI_BOOT)) {
    if ((petal.buttons[0].TimeHeldMs() >= 2000) && (petal.buttons[9].TimeHeldMs() >= 2000)) {
      UI = UI_BOOT;
    }
  }
}

void set_dist(int level) {
  inp_dist=level;
  switch (level) {
    case 1:
      dist_SoftGain = 1;
      dist_HardGain = 100;
      dist_OutGain = 0.2;
      break;
    case 2:
      dist_SoftGain = 100;
      dist_HardGain = 1;
      dist_OutGain = 0.05;
      break;
    case 3:
      dist_SoftGain = 10;
      dist_HardGain = 10;
      dist_OutGain = 0.05;
      break;
    case 4:
      dist_SoftGain = 25;
      dist_HardGain = 25;
      dist_OutGain = 0.05;
      break;
  }
}

void set_verb(int level) {
  inp_verb=level;
  switch(level) {
    case 1:
      verb_period=1000;
      verb_feedback=0.5;
      verb_wet=0.3;
      break;
    case 2:
      verb_period=1500;
      verb_feedback=0.7;
      verb_wet=0.3;
      break;
    case 3:
      verb_period=1500;
      verb_feedback=0.9;
      verb_wet=0.2;
      break;
    case 4:
      verb_period=1500;
      verb_feedback=0.5;
      verb_wet=0.5;
      break;
  }
}

void set_echo(int level) {
  inp_echo=level;
  switch(level) {
    case 1:
      echo_feedback=0.20;
      echo_wet=0.3;
      break;
    case 2:
      echo_feedback=0.35;
      echo_wet=0.4;
      break;
  }
}

void setEchoLoopFraction() {

}

void tap_tempo() {
  if (tap_period<0) {
    tap_period=0;
    if (echo_loop_fraction>0) echo_loop_fraction=-echo_loop_fraction;
  } else {
    echo_period=constrain(tap_period, 2000, ECHO_SAMPLE_MAX);
    tap_period=-1;
  }
}

void set_trem(int level) {
  inp_trem=level;
  switch(level) {
    case 1:
      trem_mag = 0.65; // max 1
      trem_period = 0.25;
      break;
    case 2:
      trem_mag = 0.4; // max 1
      trem_period = 0.1;
      break;
    case 3:
      trem_mag = 0.3; // max 1
      trem_period = 0.07;
      break;
    case 4:
      trem_mag = 0.2; // max 1
      trem_period = 0.15;
      break;
  }
}


void toggle_level(int &level, int &old_level) {
  if (level) {
    old_level=level;
    level=0;
  } else {
    level=old_level;
  }
}