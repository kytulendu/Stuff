#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#define KEY_F1         0x3B00
#define KEY_F2         0x3C00
#define KEY_F3         0x3D00
#define KEY_F4         0x3E00
#define KEY_F5         0x3F00
#define KEY_F6         0x4000
#define KEY_F7         0x4100
#define KEY_F8         0x4200
#define KEY_F9         0x4300
#define KEY_F10        0x4400

#define KEY_HOME       0x4700
#define KEY_END        0x4F00
#define KEY_PGUP       0x4900
#define KEY_PGDN       0x5100
#define KEY_INS        0x5200
#define KEY_DEL        0x5300
#define KEY_UP         0x4800
#define KEY_DOWN       0x5000
#define KEY_LEFT       0x4B00
#define KEY_RIGHT      0x4D00

#define KEY_ESC        0x1B
#define KEY_SPACE      0x20
#define KEY_ENTER      0xD
#define KEY_TAB        0x9
#define KEY_BACKSPACE  0x8

#define KEY_CTRL_A     0x1
#define KEY_CTRL_B     0x2
#define KEY_CTRL_C     0x3
#define KEY_CTRL_D     0x4
#define KEY_CTRL_E     0x5
#define KEY_CTRL_F     0x6
#define KEY_CTRL_G     0x7
#define KEY_CTRL_H     0x8
#define KEY_CTRL_I     0x9
#define KEY_CTRL_J     0xA
#define KEY_CTRL_K     0xB
#define KEY_CTRL_L     0xC
#define KEY_CTRL_M     0xD
#define KEY_CTRL_N     0xE
#define KEY_CTRL_O     0xF
#define KEY_CTRL_P     0x10
#define KEY_CTRL_Q     0x11
#define KEY_CTRL_R     0x12
#define KEY_CTRL_S     0x13
#define KEY_CTRL_T     0x14
#define KEY_CTRL_U     0x15
#define KEY_CTRL_V     0x16
#define KEY_CTRL_W     0x17
#define KEY_CTRL_X     0x18
#define KEY_CTRL_Y     0x19
#define KEY_CTRL_Z     0x1A
#define KEY_CTRL_F1    0x5E00
#define KEY_CTRL_F2    0x5F00
#define KEY_CTRL_F3    0x6000
#define KEY_CTRL_F4    0x6100
#define KEY_CTRL_F5    0x6200
#define KEY_CTRL_F6    0x6300
#define KEY_CTRL_F7    0x6400
#define KEY_CTRL_F8    0x6500
#define KEY_CTRL_F9    0x6600
#define KEY_CTRL_F10   0x6700
#define KEY_CTRL_HOME  0x7700
#define KEY_CTRL_END   0x7500
#define KEY_CTRL_PGUP  0x8400
#define KEY_CTRL_PGDN  0x7600
#define KEY_CTRL_LEFT  0x7300
#define KEY_CTRL_RIGHT 0x7400

#define KEY_ALT_A      0x1E00
#define KEY_ALT_B      0x3000
#define KEY_ALT_C      0x2E00
#define KEY_ALT_D      0x2000
#define KEY_ALT_E      0x1200
#define KEY_ALT_F      0x2100
#define KEY_ALT_G      0x2200
#define KEY_ALT_H      0x2300
#define KEY_ALT_I      0x1700
#define KEY_ALT_J      0x2400
#define KEY_ALT_K      0x2500
#define KEY_ALT_L      0x2600
#define KEY_ALT_M      0x3200
#define KEY_ALT_N      0x3100
#define KEY_ALT_O      0x1800
#define KEY_ALT_P      0x1900
#define KEY_ALT_Q      0x1000
#define KEY_ALT_R      0x1300
#define KEY_ALT_S      0x1F00
#define KEY_ALT_T      0x1400
#define KEY_ALT_U      0x1600
#define KEY_ALT_V      0x2F00
#define KEY_ALT_W      0x1100
#define KEY_ALT_X      0x2D00
#define KEY_ALT_Y      0x1500
#define KEY_ALT_Z      0x2C00
#define KEY_ALT_F1     0x6800
#define KEY_ALT_F2     0x6900
#define KEY_ALT_F3     0x6A00
#define KEY_ALT_F4     0x6B00
#define KEY_ALT_F5     0x6C00
#define KEY_ALT_F6     0x6D00
#define KEY_ALT_F7     0x6E00
#define KEY_ALT_F8     0x6F00
#define KEY_ALT_F9     0x7000
#define KEY_ALT_F10    0x7100

#define KEY_SHIFT_F1   0x5400
#define KEY_SHIFT_F2   0x5500
#define KEY_SHIFT_F3   0x5600
#define KEY_SHIFT_F4   0x5700
#define KEY_SHIFT_F5   0x5800
#define KEY_SHIFT_F6   0x5900
#define KEY_SHIFT_F7   0x6A00
#define KEY_SHIFT_F8   0x6B00
#define KEY_SHIFT_F9   0x6C00
#define KEY_SHIFT_F10  0x6D00
#define KEY_SHIFT_TAB  0xF00

int Get_KB( );
int KB_Hit( );
void Write_KB( int key );
int Get_LastKey( );
void Show_Cursor( int x, int y );

int Get_ThaiKey( );
void Set_ThaiKey( int i );
int Get_EngOnlyKey( );
void Set_EngOnlyKey( int i );

int Alt2Chr( int key );

void Set_NumOnlyKey( int i );

void Beep( );

#endif
