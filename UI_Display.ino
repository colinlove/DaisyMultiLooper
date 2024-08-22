/*
#define ii0    0b0000110000111111 // 0
#define ii1    0b0000000000000110 // 1
#define ii2    0b0000000011011011 // 2
#define ii3    0b0000000010001111 // 3
#define ii4    0b0000000011100110 // 4
#define ii5    0b0010000001101001 // 5
#define ii6    0b0000000011111101 // 6
#define ii7    0b0000000000000111 // 7
#define ii8    0b0000000011111111 // 8
#define ii9    0b0000000011101111 // 9

#define iiA    0b0000000011110111 // A
#define iiB    0b0001001010001111 // B
#define iiC    0b0000000000111001 // C
#define iiD    0b0001001000001111 // D
#define iiE    0b0000000011111001 // E
#define iiF    0b0000000001110001 // F
#define iiG    0b0000000010111101 // G
#define iiH    0b0000000011110110 // H
#define iiI    0b0001001000001001 // I
#define iiJ    0b0000000000011110 // J
#define iiK    0b0010010001110000 // K
#define iiL    0b0000000000111000 // L
#define iiM    0b0000010100110110 // M
#define iiN    0b0010000100110110 // N
#define iiO    0b0000000000111111 // O
#define iiP    0b0000000011110011 // P
#define iiQ    0b0010000000111111 // Q
#define iiR    0b0010000011110011 // R
#define iiS    0b0000000011101101 // S
#define iiT    0b0001001000000001 // T
#define iiU    0b0000000000111110 // U
#define iiV    0b0000110000110000 // V
#define iiW    0b0010100000110110 // W
#define iiX    0b0010110100000000 // X
#define iiY    0b0001010100000000 // Y
#define iiZ    0b0000110000001001 // Z

#define iia    0b0001000001011000 // a
#define iib    0b0010000001111000 // b
#define iic    0b0000000011011000 // c
#define iid    0b0000100010001110 // d
#define iie    0b0000100001011000 // e
#define iif    0b0000000001110001 // f
#define iig    0b0000010010001110 // g
#define iih    0b0001000001110000 // h
#define iii    0b0001000000000000 // i
#define iij    0b0000000000001110 // j
#define iik    0b0011011000000000 // k
#define iil    0b0000000000110000 // l
#define iim    0b0001000011010100 // m
#define iin    0b0001000001010000 // n
#define iio    0b0000000011011100 // o
#define iip    0b0000000101110000 // p
#define iiq    0b0000010010000110 // q
#define iir    0b0000000001010000 // r
#define iis    0b0010000010001000 // s
#define iit    0b0000000001111000 // t
#define iiu    0b0000000000011100 // u
#define iiv    0b0010000000000100 // v
#define iiw    0b0010100000010100 // w
#define iix    0b0010100011000000 // x
#define iiy    0b0010000000001100 // y
#define iiz    0b0000100001001000 // z

#define iiSlash    0b0000110000000000 // /
#define iiBackslash    0b0010000100000000 // backslash
#define iiMinus    0b0000000011000000 // -
*/
int vu_segs[8] = {0,
  0b0000000000110000,
  0b0000000001110000,
  0b0000100101110000,
  0b0001101101111001,
  0b0001101111111001,
  0b0011111111111001,
  0b0011111111111111};

int timeint=0;
int offsx=0;
int offsy=0;
int segarrayx[] = {5,2,0,-2,2,0,-2,2,-2,-4,-4,0,4,4,0};
int segarrayy[] = {-6,-3,-3,-3,3,3,3,0,0,3,-3,-6,-3,3,6};

void UI_Display() {
  animloop++;
  if ((UI==UI_Welcome) || (UI==UI_xx)) {
    //Serial.println("Welcome or xx");
    display(0, iiSlash, iiSlash, iiSlash, iiSlash);
    display(1, iiL, iiO, iiO, iiP);
    display(2,0b0000001010000011 , 0b0000000011100011, 0b0000000011100011,0b0000000000100001 );
    display(3, iiBackslash, iiBackslash, iiBackslash, iiBackslash);
    display(4, iiBackslash, iiBackslash, iiBackslash, iiBackslash);
    display(5, 0b0000000000001100, iio, iio, 0b0001000001011000);
    display(6, 0b0000000011011110, iiO, iiO, 0b0000000000000111);
    display(7, iiSlash, iiSlash, iiSlash, iiSlash);

  } else if (UI==UI_Loop_Play_Rec) {
    displayloopstatus(0,4,loop1State);
    displayloopstatus(1,5,loop2State);
    displayloopstatus(2,6,loop3State);
    displayloopstatus(3,7,loop4State);
    Serial.println(" Play Rec");
  } else if (UI==UI_Loop_Options) {
    //Serial.println("Loop Options");
    display(0, iiS, iio, iil, iio);
    display(1, iiS, iio, iil, iio);
    display(2, iiS, iio, iil, iio);
    display(3, iiS, iio, iil, iio);
    display(4, iiS, iit, iir, iit);
    display(5, iiS, iit, iio, iip);
    display(6, iiT, iia, iip, iie);
    display(7, iiK, iii, iil, iil);

  } else if (UI==UI_Loop1) {
    //Serial.println("Loop1");
    display(0, iin, iio, iis, iit);
    display(1, iin, iio, iik, iil);
    display(2, iiH, iia, iil, iif);
    display(3, iid, iil, iia, iiy);
    display(4, iil, iin, iik, ii2);
    display(5, iil, iin, iik, ii3);
    display(6, iil, iin, iik, ii4);
    display(7, iiV, iio, iil, 0);

  } else if (UI==UI_Loop2) {
    //Serial.println("Loop2");
    display(0, iin, iio, iis, iit);
    display(1, iin, iio, iik, iil);
    display(2, iiH, iia, iil, iif);
    display(3, iid, iil, iia, iiy);
    display(4, iil, iin, iik, ii1);
    display(5, iil, iin, iik, ii3);
    display(6, iil, iin, iik, ii4);
    display(7, iiV, iio, iil, 0);

  } else if (UI==UI_Loop3) {
    //Serial.println("Loop3");
    display(0, iin, iio, iis, iit);
    display(1, iin, iio, iik, iil);
    display(2, iiH, iia, iil, iif);
    display(3, iid, iil, iia, iiy);
    display(4, iil, iin, iik, ii1);
    display(5, iil, iin, iik, ii2);
    display(6, iil, iin, iik, ii4);
    display(7, iiV, iio, iil, 0);

  } else if (UI==UI_Loop4) {
    //Serial.println("Loop4");
    display(0, iin, iio, iis, iit);
    display(1, iin, iio, iik, iil);
    display(2, iiH, iia, iil, iif);
    display(3, iid, iil, iia, iiy);
    display(4, iil, iin, iik, ii1);
    display(5, iil, iin, iik, ii2);
    display(6, iil, iin, iik, ii3);
    display(7, iiV, iio, iil, 0);

  } else if (UI==UI_FX1) {
    //Serial.println("FX1");
    display(0, iiD, iii, iis, iit);
    display(1, iiV, iie, iir, iib);
    display(2, iiE, iic, iih, iio);
    display(3, iiT, iir, iie, iim);
    display_level(4, inp_dist);
    display_level(5, inp_verb);
    display_level(6, inp_echo);
    display_level(7, inp_trem);

  } else if (UI==UI_FX2) {
    //Serial.println("FX2");
    display(0, iiC, iio, iim, iip);
    display(1, iiO, iic, iit, iiv);
    display(2, iiM, iie, iil, iil);
    display(3, iiC, iih, iio, iir);
    display(4, iiMinus, iiMinus, iiMinus, iiMinus);
    display(5, iiMinus, iiMinus, iiMinus, iiMinus);
    display(6, iiMinus, iiMinus, iiMinus, iiMinus);
    display(7, iiMinus, iiMinus, iiMinus, iiMinus);
  
  } else if (UI==UI_Dist) {
    //Serial.println("Dist");
    display(0, iiD, iii, iis, iit);
    display(1, iio, iir, iit, iii);
    display(2, iio, iin, 0, 0);
    display(3, 0, 0, 0, 0);
    display(4, 0, iiL, ii1, 0);
    display(5, 0, iiL, ii2, 0);
    display(6, 0, iiL, ii3, 0);
    display(7, 0, iiL, ii4, 0);
  
  } else if (UI==UI_Verb) {
    //Serial.println("Verb");
    display(0, iiR, iie, iiv, iie);
    display(1, iir, iib, 0, 0);
    display(2, 0, 0, 0, 0);
    display(3, 0, 0, 0, 0);
    display(4, 0, iiL, ii1, 0);
    display(5, 0, iiL, ii2, 0);
    display(6, 0, iiL, ii3, 0);
    display(7, 0, iiL, ii4, 0);

  } else if (UI==UI_Echo) {
    //Serial.println("Echo");
    display(0, iiE, iic, iih, iio);
    display(1, 0, 0, 0, 0);
    display(2, 0, 0, 0, 0);
    display(3, 0, 0, 0, 0);
    display(4, 0, iiL, ii1, 0);
    display(5, 0, iiL, ii2, 0);
    display(6, ii1, iiSlash, ii1, ii6);
    display(7, iiT, iiA, iiP, 0);

  } else if (UI==UI_Trem) {
    //Serial.println("Tremolo");
    display(0, iiT, iir, iie, iim);
    display(1, iio, iil, iio, 0);
    display(2, 0, 0, 0, 0);
    display(3, 0, 0, 0, 0);
    display(4, 0, iiL, ii1, 0);
    display(5, 0, iiL, ii2, 0);
    display(6, 0, iiL, ii3, 0);
    display(7, 0, iiL, ii4, 0);

  } else if (UI==UI_Comp) {
    //Serial.println("Comp");
    display(0, iiC, iio, iim, iip);
    display(1, iir, iie, iis, iis);
    display(2, iii, iio, iin, 0);
    display(3, 0, 0, 0, 0);
    display(4, 0, iiL, ii1, 0);
    display(5, 0, iiL, ii2, 0);
    display(6, 0, iiL, ii3, 0);
    display(7, 0, iiL, ii4, 0);

  } else if (UI==UI_Octv) {
    //Serial.println("Octave");
    display(0, iiO, iic, iit, iia);
    display(1, iiv, iie, 0, 0);
    display(2, 0, 0, 0, 0);
    display(3, 0, 0, 0, 0);
    display(4, 0, iiL, ii1, 0);
    display(5, 0, iiL, ii2, 0);
    display(6, 0, iiL, ii3, 0);
    display(7, 0, 0, 0, 0);

  } else if (UI==UI_Mell) {
    //Serial.println("Mellow");
    display(0, iiM, iie, iil, iil);
    display(1, iio, iiw, 0, 0);
    display(2, 0, 0, 0, 0);
    display(3, 0, 0, 0, 0);
    display(4, 0, iiL, ii1, 0);
    display(5, 0, iiL, ii2, 0);
    display(6, 0, iiL, ii3, 0);
    display(7, 0, iiL, ii4, 0);

  } else if (UI==UI_Chor) {
    //Serial.println("Chorus");
    display(0, iiC, iih, iio, iir);
    display(1, iiu, iis, 0, 0);
    display(2, 0, 0, 0, 0);
    display(3, 0, 0, 0, 0);
    display(4, 0, iiL, ii1, 0);
    display(5, 0, iiL, ii2, 0);
    display(6, 0, iiL, ii3, 0);
    display(7, 0, iiL, ii4, 0);


  } else if (UI==UI_VU) {
    //int in_int=(int)(vu_in_max*10000.0);
    int in_int=128-(int)(5.57*log(vu_in_max)/0.693147);
    int out_int=128-(int)log(1/vu_out_max);
    Serial.print("vu_in_max: ");
    Serial.println(vu_in_max);
    Serial.print("vu_in_max*1000.0: ");
    Serial.println(vu_in_max*1000.0);
    Serial.print("vu_in_max*1000000.0: ");
    Serial.println(vu_in_max*1000000.0);
    Serial.print("vu_in_max*1000000000.0: ");
    Serial.println(vu_in_max*1000000000.0);
    //Serial.print(" ");
    //Serial.print(1/vu_in_max);
    //Serial.print(log(1/vu_in_max));
    //Serial.print((int)log(1/vu_in_max));
    
    display(0, vu_seg(in_int), vu_seg(in_int-8), vu_seg(in_int-16), vu_seg(in_int-24)); in_int-=32;
    display(1, vu_seg(in_int), vu_seg(in_int-8), vu_seg(in_int-16), vu_seg(in_int-24)); in_int-=32;
    display(2, vu_seg(in_int), vu_seg(in_int-8), vu_seg(in_int-16), vu_seg(in_int-24)); in_int-=32;
    display(3, vu_seg(in_int), vu_seg(in_int-8), vu_seg(in_int-16), vu_seg(in_int-24));
    display(4, vu_seg(out_int), vu_seg(out_int-8), vu_seg(out_int-16), vu_seg(out_int-24)); out_int-=32;
    display(5, vu_seg(out_int), vu_seg(out_int-8), vu_seg(out_int-16), vu_seg(out_int-24)); out_int-=32;
    display(6, vu_seg(out_int), vu_seg(out_int-8), vu_seg(out_int-16), vu_seg(out_int-24)); out_int-=32;
    display(7, vu_seg(out_int), vu_seg(out_int-8), vu_seg(out_int-16), vu_seg(out_int-24)); 
    vu_in_max*=0.9;
    vu_out_max=0.0;
  } else if (UI==UI_Anim) {
    timeint+=1;
    if (timeint>400) {
      timeint=0;
    }
    //offsx=random(-100,100);
    //offsy=random(-100,100);
    offsy=-abs(sin((float)timeint/20)*70)+60;
    offsx=-(timeint-200);
    
    for (int displayrow=0; displayrow<2; displayrow++) {
      for (int displaycol=0; displaycol<4; displaycol++) {
        
        for (int digit=0;digit<4;digit++){
          int digitx=displaycol*75+digit*13-132+offsx;
          int digity=(displayrow*-36)+18+offsy;
          int segraw=0;
          for (int seg=0;seg<15;seg++) {
            segraw*=2;
            int segx = digitx+segarrayx[seg];
            int segy = digity+segarrayy[seg];
            int distsq = (segx*segx)+(segy*segy);
            if (((distsq<2000)&&(distsq>1500)) || 
                (((distsq<800)&&(distsq>400))&&(segy<0)) || 
                ((segx>10) && (segx<20) && (segy>10) && (segy<20)) || 
                ((segx<-10) && (segx>-20) && (segy>10) && (segy<20))) {
              segraw+=1;
            }
          }
          disp[displayrow*4+displaycol].writeDigitRaw(digit, segraw);
        }
        
        disp[displayrow*4+displaycol].writeDisplay();
        disp[displayrow*4+displaycol].clear();
      }
    }
    delay(5);
  } else if (UI==UI_BOOT) {
    display(0, iiB, iiO, iiO, iiT);
    display(1, iiB, iiO, iiO, iiT);
    display(2, iiB, iiO, iiO, iiT);
    display(3, iiB, iiO, iiO, iiT);
    display(4, 0,0,0,0);
    display(5, 0,0,0,0);
    display(6, 0,0,0,0);
    display(7, 0,0,0,0);
    Serial.println("Boot Mode");
    DAISY.end();
    petal.ResetToBootloader();
  }
}

void display(uint8_t n, uint16_t a, uint16_t b, uint16_t c, uint16_t d) {
  disp[n].writeDigitRaw(0, a);
  disp[n].writeDigitRaw(1, b);
  disp[n].writeDigitRaw(2, c);
  disp[n].writeDigitRaw(3, d);
  disp[n].writeDisplay();
}

void displayloopstatus(uint8_t d1, uint8_t d2, ChannelState loopstate) {
  switch (loopstate) {
    case idle_empty:
      Serial.print('E');
      display(d1, iiR, iie, iic, 0);
      display(d2, iiO, iiP, iiT, iiS);
      break;
    case armed:
    Serial.print('A');
      display(d1, iiA, iiR, iiM, iiD);
      display(d2, 0, 0, 0, 0);
      break;
    case rec_first:
      Serial.print('1');
    case overdub:
      Serial.print('O');
      display(d1, iiO, iiOverscore, iiOverscore, iiO);
      display(d2, iiP, iiL, iiA, iiY);
      break;
    case play:
      Serial.print('P');
      display(d1, iiO, iid, iiu, iib);
      display(d2, iiS, iit, iio, iip);
      break;
    case idle:
      Serial.print('I');
      display(d1, iiO, iid, iiu, iib);
      display(d2, iiP, iiL, iiA, iiY);
      break;

    }
}

void display_level(uint8_t disp, uint8_t level) {
  switch (level) {
    case 0:
      display(disp, iiMinus, iiMinus, iiMinus, iiMinus);
      break;
    case 1:
      display(disp, iiX, iiMinus, iiMinus, iiMinus);
      break;
    case 2:
      display(disp, iiX, iiX, iiMinus, iiMinus);
      break;
    case 3:
      display(disp, iiX, iiX, iiX, iiMinus);
      break;
    case 4:
      display(disp, iiX, iiX, iiX, iiX);
      break;
  }
}

int vu_seg(int level) {
  if (level<0) return 0;
  if (level>7) return vu_segs[7];
  return vu_segs[level];
}