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

int timeint=0;
int offsx=0;
int offsy=0;
int segarrayx[] = {5,2,0,-2,2,0,-2,2,-2,-4,-4,0,4,4,0};
int segarrayy[] = {-6,-3,-3,-3,3,3,3,0,0,3,-3,-6,-3,3,6};

void UI_Menu_Buttons() {
  if ((UI==UI_Welcome) || (UI==UI_Welcome)) {

    if (petal.buttons[4].RisingEdge()) {
      UI=UI_FX;
    } else if (petal.buttons[9].RisingEdge()) {
      UI=UI_Loop;
    }

  } else if (UI==UI_FX) {

    if (petal.buttons[4].RisingEdge()) {
      UI=UI_Anim;
    } else if (petal.buttons[9].RisingEdge()) {
      UI=UI_Loop;
    }

  } else if (UI==UI_Loop) {

    if (petal.buttons[4].RisingEdge()) {
      UI=UI_FX;
    } else if (petal.buttons[9].RisingEdge()) {
      UI=UI_Loop2;
    }

  } else if (UI==UI_Loop2) {

    if (petal.buttons[4].RisingEdge()) {
      UI=UI_FX;
    } else if (petal.buttons[9].RisingEdge()) {
      UI=UI_Loop;
    }

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
  }
  if (petal.buttons[0].TimeHeldMs() >= 3000 && petal.buttons[9].TimeHeldMs() >= 3000) resetToBootloader();
}

void UI_Display() {
  if (UI==UI_Welcome) {
    display(0, iiSlash, iiSlash, iiSlash, iiSlash);
    display(1, iiL, iiO, iiO, iiP);
    display(2, 0b0000000000100001, 0b0000000011100011, 0b0000000011100011, 0b0000001010000011);
    display(3, iiBackslash, iiBackslash, iiBackslash, iiBackslash);
    display(4, iiBackslash, iiBackslash, iiBackslash, iiBackslash);
    display(5, 0b0000000000100001, 0b0000000011100011, 0b0000000011100011, 0b0000001010000011);
    display(6, iid, iiO, iiO, 0b0000000000000111);
    display(7, iiSlash, iiSlash, iiSlash, iiSlash);
  } else if (UI==UI_FX) {
    display(0, iiD, iii, iis, iit);
    display(1, iiV, iie, iir, iib);
    display(2, iiE, iic, iih, iio);
    display(3, iiT, iir, iie, iim);
    display(4, iiMinus, iiMinus, iiMinus, iiMinus);
    display(5, iiMinus, iiMinus, iiMinus, iiMinus);
    display(6, iiMinus, iiMinus, iiMinus, iiMinus);
    display(7, iiMinus, iiMinus, iiMinus, iiMinus);
  } else if (UI==UI_Loop) {
    display(0, iiR, iie, iic, 0);
    display(1, iiR, iie, iic, 0);
    display(2, iiR, iie, iic, 0);
    display(3, iiR, iie, iic, 0);
    display(4, iiP, iil, iia, iiY);
    display(5, iiP, iil, iia, iiY);
    display(6, iiP, iil, iia, iiY);
    display(7, iiP, iil, iia, iiY);
  } else if (UI==UI_Loop2) {
    display(0, iiS, iio, iil, iio);
    display(1, iiS, iio, iil, iio);
    display(2, iiS, iio, iil, iio);
    display(3, iiS, iio, iil, iio);
    display(4, iiA, iil, iil, 0);
    display(5, ii2, ii3, iiSlash, ii4);
    display(6, iiT, iia, iip, iie);
    display(7, iiK, iii, iil, iil);
  }
}