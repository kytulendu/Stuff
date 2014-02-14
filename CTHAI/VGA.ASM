_DATA   SEGMENT WORD PUBLIC 'DATA'
        CH_COLOR  DB ?
_DATA   ENDS

_TEXT   SEGMENT BYTE PUBLIC 'CODE'
        ASSUME  DS:_DATA, CS:_TEXT


;************************************************************************
; Common definitions for the next five routines                         *
;************************************************************************

GRAPHICS_CTL    EQU     3CEH            ;Address of Graphics controller
SEQUENCE_CTL    EQU     3C4H            ;Address the Sequencer

XSIZE   EQU     640                     ;Assume 640 pixels across
HBYTES  EQU     (XSIZE/8)               ;Compute bytes per raster
GRAPH_SEG       EQU     0A000H          ;Segment of display buffer

NFN             EQU     20

;************************************************************************
; Set pixel at N to color COLOR                                         *
; using the PLANAR PIXEL WRITE method                                   *
;************************************************************************

N               EQU     [BP+4]          ;Formal parameters on stack
MAS    	       EQU     [BP+6]

        PUBLIC  _PIXEL_W

_PIXEL_W    PROC    NEAR
        PUSH    BP
        MOV     BP,SP
        PUSH    ES
        PUSH    AX
        PUSH    BX
        PUSH    CX
        PUSH    DX

        ; Convert x,y address into OFFSET:SEGMENT and get MASK

        MOV     BX,N                    ;Fetch N coordinate
        MOV     AX,GRAPH_SEG
        MOV     ES,AX
        MOV     CL,MAS                   ;Keep mask in CL


        ; Set mask register to preserve unused 7 bits in a byte

        MOV     DX,GRAPHICS_CTL
        MOV     AL,08H                  ;Select BIT MASK register
        OUT     DX,AL
        MOV     AL,CL                   ;Fetch mask returned earlier
        INC     DX                      ;and set MASK register to it
        OUT     DX,AL
        DEC     DX

        ; Set MAP MASK register to enable all planes and set pixel to 0
        ; then set MAP MASK to enable planes coresponding to color

        MOV     DX,SEQUENCE_CTL
        MOV     AL,02                   ;Select MAP MASK (PLANE SELECT) reg.
        OUT     DX,AL
        INC     DX
        MOV     AL,0FH                  ;Enable all four planes for write
        OUT     DX,AL

        MOV     AH,ES:[BX]              ;Latch data to preserve unused bits
        XOR     AL,AL                   ;Make sure that proper bit is CLEAR
        MOV     ES:[BX],AL              ;Clear all four planes

        MOV     AL,CH_COLOR                ;Fetch color passed on the stack
        AND     AL,0FH                  ;Clear unused bits (use first four)
        OUT     DX,AL                   ;Enable planes as specified in color
        DEC     DX                      ;(MAP MASK reg is still selected)

        ; Set the pixel to new color

        MOV     AL,0FFH                 ;Make sure that proper bit is ON
        MOV     ES:[BX],AL              ;Set pixel color to new value

        ; Restore MASK registers

        MOV     DX,GRAPHICS_CTL
        MOV     AL,08H                  ;Select BIT MASK register
        OUT     DX,AL
        MOV     AL,0FFH                 ;Enable all 8 bits for write
        INC     DX                      ;(default)
        OUT     DX,AL

        MOV     DX,SEQUENCE_CTL
        MOV     AL,02                   ;Select MAP MASK (PLANE SELECT) reg
        OUT     DX,AL
        INC     DX
        MOV     AL,0FH                  ;Enable all four planes for write
        OUT     DX,AL

        ; Restore segment registers and return

        POP     DX
        POP     CX
        POP     BX
        POP     AX
        POP     ES
        MOV     SP,BP
        POP     BP
        RET
_PIXEL_W    ENDP

;************************************************************************
; Out char at X,Y whit color COLOR                                      *
;************************************************************************

x               EQU     [BP+4]
Y               EQU     [BP+6]
COLOR           EQU     [BP+8]
CHR             EQU     [BP+10]
FN_SEG  	       EQU     [BP+12]
FN_OFF  	       EQU     [BP+14]

N     	       EQU WORD PTR [BP-2]
NN     	       EQU BYTE PTR [BP-4]

        PUBLIC  _OUT_CH

_OUT_CH    PROC    NEAR
        PUSH    BP
        MOV     BP,SP
        PUSH    ES

        SUB     SP,6

        MOV     AX,COLOR
        MOV     CH_COLOR,AL

        MOV     BX,X
        MOV     AX,Y
        ADD     AX,DX

        ; Compute SEGMENT:OFFSET pair from x,y pair

        MOV     CX,HBYTES               ;Fetch bytes per raster
        MUL     CX                      ;Compute offset past y rasters
        MOV     NN,BL                   ;Keep copy of x for later
        SHR     BX,1                    ;Get offset within raster as x mod 8
        SHR     BX,1
        SHR     BX,1
        ADD     BX,AX                   ;Add offsets together and keep in BX
        MOV     N,BX


        XOR     DX,DX
LL:
        MOV     AX,FN_SEG
        MOV     ES,AX
        MOV     AX,CHR
        MOV     CL,NFN
        MUL     CL
        MOV     BX,FN_OFF
        ADD     BX,AX
        ADD     BX,DX
        XOR     AH,AH
        MOV     AL,ES:[BX]

        MOV     CH,AL

        MOV     CL,NN
        AND     CL,07H                  ;Compute which bit in a byte
        SHR     AL,CL

        PUSH    AX
        PUSH    N
        CALL    _PIXEL_W

        INC     N

        CMP     CL,0
        JE      LL2

        MOV     AL,CH

        SUB     CL,9
        XOR     CL,0ffH
        AND     CL,07H                  ;Compute which bit in a byte
        SHL     AL,CL

        PUSH    AX
        PUSH    N
        CALL    _PIXEL_W
LL2:
        ADD     N,79
        INC     DX
        CMP     DX,NFN
        JNE     LL

        ; Restore segment registers and return

        POP     ES
        MOV     SP,BP
        POP     BP
        RET
_OUT_CH    ENDP


;************************************************************************
;                                                                       *
;  Scanline(start, end, raster, color):draw line from (start,y) to      *
;       (end,y) with a color 'color'.                                   *
;  Entry :sp + 2 = Start                                                *
;         sp + 4 = Stop                                                 *
;         sp + 6 = y                                                    *
;         sp + 8 = color                                                *
;               ....                                                    *
;                                                                       *
;************************************************************************

Start   EQU     [BP+4]
Stop    EQU     [BP+6]
Raster  EQU     [BP+8]
Color   EQU     [BP+10]

        PUBLIC  _SCANLINE

_SCANLINE       PROC NEAR
        PUSH    BP
        MOV     BP,SP

        PUSH    DI                      ;Preserve segment registers
        PUSH    SI
        PUSH    DS
        PUSH    ES

       ;--- Load SET/RESET registers with current color

        MOV     DX,GRAPHICS_CTL                ; move color into reset register
        XOR     AL,AL
        OUT     DX,AL
        INC     DX
        MOV     AL,Color
        OUT     DX,AL
        DEC     DX                      ; enable use of reset register
        MOV     AL,1
        OUT     DX,AL
        INC     DX
        MOV     AL,0Fh
        OUT     DX,AL
        MOV     DX,SEQUENCE_CTL                ; enable all four planes for writing
        MOV     AL,2
        OUT     DX,AL
        INC     DX
        MOV     AL,0Fh
        OUT     DX,al

        ;--- COMPUTE ADDRESS AND MASK FOR FIRST PIXEL -------------
SL_Address:
        ;--- Convert (x,y) to absolute address in display buffer

        MOV     AX,Raster               ;Compute offset as 80*y+x/8
        MOV     BX,HBYTES
        MUL     BX
        MOV     DI,Start                ; + x/8
        MOV     CL,3
        SHR     DI,CL
        ADD     DI,AX

        MOV     DX,GRAPH_SEG               ;Point DS and ES into display buffer
        MOV     DS,DX
        MOV     ES,DX

        MOV     CX,Stop                 ;Compute dx
        SUB     CX,Start

        ;--- Compute bit mask for the leading partial byte

        MOV     AX,Start                ;Fetch x0
        AND     AX,07H                  ;Check if there is leading partial byte
        JZ      SL_Complete             ;...No, bypass this step
        MOV     BX,0FFH                 ;Compute mask for the first byte by
        PUSH    CX                      ;shifting FF pattern to the right
        MOV     CX,AX                   ;until only partial leading bits are
        SHR     BX,CL                   ;on
        POP     CX
        ADD     CX,AX                   ;Update counter of bits completed
        SUB     CX,8
        JGE     SL_SetMask              ;Modify mask if only one byte
        NEG     CX
        SHR     BX,CL
        SHL     BX,CL
        XOR     CX,CX                   ; restore counter
SL_SetMask:
        MOV     DX,GRAPHICS_CTL                 ;Set mask by loading BIT MASK
        MOV     AL,08h                  ;register in the GRAPHICS controller
        OUT     DX,AL
        INC     DX
        MOV     AL,BL
        OUT     DX,AL

        ;--- Draw pixels from the leading partial byte

        MOV     AL,[DI]                 ;Latch data
        STOSB                           ;Write new data

        ;--- Draw pixels from the middle complete bytes

SL_Complete:                            ; check if any bytes to set
        MOV     BX,CX                   ;Check if there is more than 7 bits
        CMP     CX,8                    ;left to draw
        JL      SL_Trailing             ;... If not, skip this part

        SHR     CX,1                    ;Divide bits by 8 to get byte count
        SHR     CX,1
        SHR     CX,1
        MOV     DX,GRAPHICS_CTL                ; Enable all 8 bits for write
        MOV     AL,08h                  ; by writing to BIT MASK register in
        OUT     DX,AL                   ; the SEQUENCER
        INC     DX
        MOV     AL,0FFH
        OUT     DX,AL

        REP     STOSB                   ;Write new data

        ;--- Compute mask for the trailing partial byte

SL_Trailing:
        AND     BX,07H                  ;Check if there are any bits left to do
        JZ      SL_Done                 ;...No, quit
        MOV     AX,0FFFFH               ;...Yes, compute mask by shifting FFFF
        MOV     CX,BX                   ;by number of bits to be drawn
        SHR     AX,CL
        XOR     AH,0FFH
        MOV     DX,GRAPHICS_CTL                ;Set BIT MASK register in GRAPHICS
        MOV     AL,08h                  ;controller to the mask
        OUT     DX,AL
        INC     DX
        MOV     AL,AH
        OUT     DX,AL

        ;--- Draw pixels from the trailing partial byte

        MOV     AL,[DI]                 ;Latch data
        STOSB                           ;Write new data

        ;--- Restore PLANE ENABLE and BIT MASK registers
SL_Done:
        MOV     DX,GRAPHICS_CTL                ; Enable all 8-bits in a byte for write
        MOV     AL,08h                  ; by setting BIT MASK register to Fhex
        OUT     DX,AL
        INC     DX
        MOV     AL,0FFh
        OUT     DX,AL

        DEC     DX                      ; Disable SET/RESET function
        MOV     AL,1
        OUT     DX,AL
        INC     DX
        XOR     AX,AX
        OUT     DX,AL

        POP     ES                      ;Restore segment registers
        POP     DS
        POP     SI
        POP     DI
        MOV     SP,BP
        POP     BP
        RET
_SCANLINE       ENDP

;************************************************************************
; Line (x0, y0, x1, y1,c ): draw line from (x0,y0) to (x1,y1)           *
;               with a color (c).                                       *
;               This routine is divided into three parts. Horizontal    *
;               lines are done in the first part, vertical lines in     *
;               second part, and rest in the third part.  The lines     *
;               in the third part are done using the Bresenhams         *
;               algorithm.                                              *
; entry : sp + 2 = x0                                                   *
;         sp + 4 = y0                                                   *
;         sp + 6 = x1                                                   *
;         sp + 8 = y1                                                   *
;         sp + 10= color                                                *
;************************************************************************

X0      EQU     [BP+4]
Y0      EQU     [BP+6]
X1      EQU     [BP+8]
Y1      EQU     [BP+10]
Color   EQU     [BP+12]

		PUBLIC  __LINE

__LINE   PROC    NEAR
		PUSH    BP
        MOV     BP,SP
        SUB     SP,6                    ;Declare three local variables

        PUSH    DI
        PUSH    SI
        PUSH    DS
        PUSH    ES

        MOV     AX,X0                   ; make sure that x1 >= x0
        MOV     CX,X1
        CMP     CX,AX
        JGE     Get_Offset
        MOV     BX,Y0
        MOV     DX,Y1
        MOV     X0,CX
        MOV     Y0,DX
        MOV     X1,AX
        MOV     Y1,BX

;-------COMPUTE ADDRESS AND MASK FOR FIRST PIXEL -------------
Get_Offset:
        ;--- Compute offset and save on stack
        MOV     CL,4                    ; offset = 80 * y + x/8
        MOV     AX,Y0
        SHL     AX,CL
        MOV     BX,AX
        SHL     AX,1
        SHL     AX,1
        ADD     AX,BX
        MOV     BX,X0                   ; + x/8
        MOV     CL,3
        SHR     BX,CL
        ADD     AX,BX
        PUSH    AX                      ; save offset on stack, later pop to DI

        ;--- Compute mask and save on the stack

        MOV     CX,X0                   ; compute which bit (x mod 8) to modify
        AND     CL,7
        MOV     BX,80h
        SHR     BX,CL
        PUSH    BX                      ; save mask on stack, later pop to SI
        MOV     DX,GRAPHICS_CTL                ; enable only the bit(within a byte)
        MOV     AL,08H                  ; to be changed
        OUT     DX,AL
        INC     DX
        MOV     AL,BL                   ; ... reg-bx has the correct bit
        OUT     DX,AL

        ;--- Load set/reset registers with current color

        MOV     DX,GRAPHICS_CTL                ; move color into reset register
        XOR     AL,AL
        OUT     DX,AL
        INC     DX
        MOV     AX,Color
        OUT     DX,AL
        DEC     DX                      ; enable use of reset register
        MOV     AL,1
        OUT     DX,AL
        INC     DX
        MOV     AL,0FH
        OUT     DX,AL
        MOV     DX,SEQUENCE_CTL                ; enable all four planes for writting
        MOV     AL,2
        OUT     DX,AL
        INC     DX
        MOV     AL,0FH
        OUT     DX,AL

        ;--- Load segment register

        MOV     DX,0A000h
        MOV     DS,DX
        MOV     ES,DX

;-------COMPUTE  DX AND DY ----------------------------------------
;       DETERMINE IF HORIZONTAL, VERTICAL OR DIAGONNAL LINE

        MOV     AX,80                   ; set raster increment
        MOV     [bp-6],AX
        MOV     SI,X1                   ; compute dx           reg-si
        SUB     SI,X0
        MOV     DI,Y1                   ; compute dy           reg-di
        SUB     DI,Y0
        JGE     DyIsPos
        NEG     AX
        MOV     [bp-6],AX
        NEG     DI
DyIsPos:
        CMP     SI,0                    ; jump according to type of line
        JZ      Vert
        CMP     DI,0
        JZ      Horiz
        JMP     Diag

;-------GENERATE A VERTICAL LINE -----------------

Vert:   MOV     cx,di                   ; set up counter
        INC     cx
        MOV     bx,[bp-6]

        POP     SI                      ; fetch mask
        MOV     DX,GRAPHICS_CTL                ; set mask
        MOV     AL,08h
        OUT     DX,AL
        INC     DX
        MOV     AX,SI
        OUT     DX,AL

        POP     DI                      ; fetch offset
LoopVert:
        MOV     AL,[DI]                 ; latch data(to preserve other 7 bits i
        MOV     [DI],AL                 ; write new data (only one bit will be
        ADD     DI,BX                   ; update offset
        LOOP    LoopVert
        JMP     LineDone

;-------GENERATE A HORIZONTAL LINE ---------------

Horiz:  MOV     CX,SI                   ; set counter of pixels
        POP     SI                      ; fetch mask
        POP     DI                      ; fetch offset

        ;---    draw pixels from the leading partial byte

        MOV     AX,X0
        AND     AX,07h                  ; check for partial byte
        JZ      FullBytes
        MOV     BX,0FFh                 ; compute the mask
        PUSH    CX
        MOV     CX,AX
        SHR     BX,CL
        POP     CX
        ADD     CX,AX                   ; update counter
        SUB     CX,08h
        JGE     MaskSet                 ; modify mask if only one byte
        NEG     CX
        SHR     BX,CL
        SHL     BX,CL
        XOR     CX,CX                   ; restore counter
MaskSet:
        MOV     DX,GRAPHICS_CTL                ; set the mask
        MOV     AL,08h
        OUT     DX,AL
        INC     DX
        MOV     AL,BL
        OUT     DX,AL
        MOV     AH,[DI]                 ; latch data
        MOV     [DI],AL                 ; write new data
        INC     DI                      ; update offset

        ;---    draw pixels from the middle complete bytes
FullBytes:                              ;
        MOV     BX,CX                   ; check if any bytes to set
        CMP     DX,8
        JL      TrailBytes
        SHR     CX,1                    ; compute count
        SHR     CX,1
        SHR     CX,1
        MOV     DX,GRAPHICS_CTL                ; set the mask
        MOV     AL,08h
        OUT     DX,AL
        INC     DX
        MOV     AL,0FFh
        OUT     DX,AL

        REP     STOSB                   ; fill complete bytes

        ;---    draw pixels from the trailing partial byte
TrailBytes:
        AND     BX,07h
        JZ      HorizDone
        MOV     AX,0FFFFh                ; compute mask
        MOV     CX,BX
        SHR     AX,CL
        XOR     AH,0FFh                  ; set the mask
        MOV     DX,GRAPHICS_CTL
        MOV     AL,08h
        OUT     DX,AL
        INC     DX
        MOV     AL,AH
        OUT     DX,AL
        MOV     AL,[DI]                  ; latch data
        MOV     [DI],AL                  ; set new data
HorizDone:
        JMP     LineDone

;----------------------- GENERATE A DIAGONAL LINE -----------------

        ;--- figure out which quarter does the line lie in

Diag:   CMP     SI,DI                   ; Is dy > dx
        JLE     oct12                   ; ...Yes, do processing in octants
                                        ;    1 and 2
        ; Compute constants for octant zero and three
        ; This is where x is the major direction and y is minor

oct03:  MOV     CX,SI                   ; set counter to dx+1
        INC     CX
        SAL     DI,1                    ; d1 = dy*2             reg-di
        MOV     BX,DI                   ; d  = dy*2-dx          reg-bx
        SUB     BX,SI
        NEG     SI                      ; d2 = dy*2-dx-dx       reg-si
        ADD     SI,BX

        MOV     [bp-2],di               ; save d1
        MOV     [bp-4],si               ; save d2
        MOV     DX,GRAPHICS_CTL                ; select BIT MASK register
        MOV     AL,08h
        OUT     DX,AL
        INC     DX
        POP     AX                      ; fetch mask
        POP     DI                      ; fetch address

        ;-------------- GENERATE LINE IN THE OCTANT ZERO AND THREE ------

next0:  OUT     DX,AL                   ; enable a bit in a byte
        MOV     AH,[DI]                 ; latch old data
        MOV     [DI],AH                 ; modify (enabled bits)

        ROR     AL,1                    ; update mask
        ADC     DI,0                    ; update byte address

        TEST    BX,8000H                ; if d >= 0 then ...
        JNZ     dneg0
        ADD     BX,[BP-4]               ; ... d = d + d2
        ADD     DI,[BP-6]               ; update offset to next scan line
        LOOP    next0
        JMP     LineDone

dneg0:  ADD     BX,[BP-2]               ; if d < 0 then d = d + d1
        LOOP    next0
        JMP     LineDone

        ;----------------------------------------------------------------

        ;---    Compute constants for octant one and two

oct12:  MOV     CX,DI                   ; set counter to dy+1
        INC     CX
        SAL     SI,1                    ; d1 = dx * 2
        MOV     BX,SI                   ; d  = dx * 2 - dy
        SUB     BX,DI
        NEG     DI                      ; d2 = -dy + dx * 2 - dy
        ADD     DI,BX

        MOV     [BP-4],DI               ; save d2
        MOV     [BP-2],SI               ; save d1
        MOV     dx,GRAPHICS_CTL                ; select BIT MASK register
        MOV     al,08h
        OUT     DX,AL
        INC     DX
        POP     AX                      ; fetch mask
        POP     DI                      ; fetch address
        OUT     DX,AL                   ; enable a bit in a byte

        ;-----GENERATE A LINE IN THE OCTANT ONE AND TWO -----------------

next1:  MOV     AH,[DI]                 ; latch old data
        MOV     [DI],AH                 ; modify (enabled bits)

        ADD     DI,[BP-6]               ; update offset (y = y+1)

        TEST    BX,8000H                ; if d >= 0 then ...
        JNZ     dneg1

        ADD     BX,[BP-4]               ; ... d = d + d2
        ROR     AL,1                    ; ... update mask (x = x+1)
        ADC     DI,0                    ; ... update offset
        OUT     DX,AL                   ; Enable next bit within a byte
        LOOP    next1
        JMP     LineDone

dneg1:  ADD     BX,[BP-2]               ; if d < 0 then d = d + d1
        LOOP    next1

        ;----------------------------------------------------------------
        ;--- Restore PLANE ENABLE and BIT MASK registers
LineDone:
        MOV     DX,GRAPHICS_CTL                ; Enable all 8-bits in a byte for write
        MOV     AL,08h                  ; by setting BIT MASK register to Fhex
        OUT     DX,AL
        INC     DX
        MOV     AL,0FFh
        OUT     DX,AL

        DEC     DX                      ; Disable SET/RESET function
        MOV     AL,1
        OUT     DX,AL
        INC     DX
        XOR     AX,AX
        OUT     DX,AL

        POP     ES
        POP     DS
        POP     SI
        POP     DI
        MOV     SP,BP
        POP     BP
        RET
__LINE   ENDP


;************************************************************************
; Set Cursor:                                                           *
; This procedure will expand the two cursor masks into                  *
; four planes.  The expanded masks will be stored                       *
; after the last visible scan line at A000:(640/8*350).                 *
; Entry:        AND_Mask - 2x16 bytes with AND mask                     *
;               XOR_Mask - 2x16 bytes with XOR mask                     *
;               BG_Color - Foreground color                             *
;               FG_Color - Background color                             *
;************************************************************************


YSIZE           EQU     480
CUR_HEIGHT      EQU     16

AND_SAVE        EQU     (YSIZE * HBYTES + 0)
XOR_SAVE        EQU     (YSIZE * HBYTES + 2)
CUR_SAVE        EQU     (YSIZE * HBYTES + 4)

AND_Mask        EQU     [BP+4]
XOR_Mask        EQU     [BP+6]
BG_Color        EQU     [BP+8]
FG_Color        EQU     [BP+10]

Last_Cursor     DW      0

        PUBLIC  _SET_CURSOR

_SET_CURSOR     PROC NEAR
        PUSH    BP
        MOV     BP,SP
        PUSH    SI
        PUSH    DI
        PUSH    ES                      ; always save seg regs
        PUSH    DS

        MOV     AX,GRAPH_SEG               ;Point ES to video ram.
        MOV     ES,AX

        ;Set EGA to use SET/RESET register to fill with background color

        MOV     DX,GRAPHICS_CTL                 ; enable use of reset register
        MOV     AL,1
        OUT     DX,AL
        INC     DX
        MOV     AL,0Fh
        OUT     DX,AL
        DEC     DX                      ; move color into reset register
        XOR     AL,AL
        OUT     DX,AL
        INC     DX
        MOV     AX,BG_Color
        OUT     DX,AL

        ; Fill with background

        MOV     DI,AND_SAVE             ;Pointer to save area (AND and XOR)
        MOV     CX,CUR_HEIGHT           ;Number of scanlines to do
Back_Loop1:
        STOSW                           ;16 bits for AND mask
        STOSW                           ;16 bits for XOR mask
        ADD     DI,HBYTES-4             ;Point to next scanline
        LOOP    Back_Loop1

        ; Change foreground bits for the AND mask save area

        MOV     AX,FG_Color             ;Load SET/RESET with foreground color
        OUT     DX,AL
        DEC     DX                      ;Select BIT MASK register
        MOV     AL,8
        OUT     DX,AL
        INC     DX
        MOV     CX,CUR_HEIGHT           ;Initialize counter
        MOV     DI,AND_Save             ;Get pointer to AND save area
        MOV     SI,AND_Mask             ;Get pointer to AND mask

Fore_Loop1:
        LODSB                           ;Fetch next byte from the mask
        OUT     DX,AL                   ;Set BIT MASK REGISTER using cursor mas
        MOV     AH,ES:[DI]              ;Latch data
        STOSB                           ;Write next 8 bits
        LODSB                           ;Fetch next byte from the mask
        OUT     DX,AL                   ;Set BIT MASK REGISTER using cursor mas
        MOV     AH,ES:[DI]              ;Latch data
        STOSB                           ;Write next 8 bits
        ADD     DI,HBYTES-2
        LOOP    Fore_Loop1

        ; Change foreground bits for the XOR mask save area

        MOV     CX,CUR_HEIGHT           ;Initialize counter
        MOV     DI,XOR_Save             ;Get pointer to XOR save area
        MOV     SI,XOR_Mask             ;Get pointer to XOR mask

Fore_Loop2:
        LODSB                           ;Fetch next byte from the mask
        OUT     DX,AL                   ;Set BIT MASK REGISTER using cursor mas
        MOV     AH,ES:[DI]              ;Latch data
        STOSB                           ;Write next 8 bits
        LODSB                           ;Fetch next byte from the mask
        OUT     DX,AL                   ;Set BIT MASK REGISTER using cursor mas
        MOV     AH,ES:[DI]              ;Latch data
        STOSB                           ;Write next 8 bits
        ADD     DI,HBYTES-2
        LOOP    Fore_Loop2

        ;Setup EGA registers for data copy (WRITE LATCH write mode)

        MOV     DX,GRAPHICS_CTL                 ;Restore graphics controller
        MOV     AL,1                    ;Disable use of SET/RESET
        OUT     DX,AL
        INC     DX
        XOR     AL,AL
        OUT     DX,AL
        DEC     DX
        MOV     AL,8                    ;Enable all 8 bits for write
        OUT     DX,AL
        INC     DX
        MOV     AL,0FFH
        OUT     DX,AL
        DEC     DX
        MOV     AL,5                    ;Select WRITE LATCH write mode
        OUT     DX,AL
        INC     DX
        MOV     AL,1
        OUT     DX,AL

        ;Copy upper left corner into save area (this is needed for first
        ;call to Move_Cursor procedure, because it always restores and
        ;we need meaningfull data for the first restore).

        MOV     SI,0                    ;Copy from upper left
        MOV     CS:Last_Cursor,SI       ;Keep where it came from
        MOV     DI,CUR_SAVE             ;Copy to cursor save area
        MOV     AX,ES                   ;Point DS to display buffer
        MOV     DS,AX
        MOV     CX,CUR_HEIGHT           ;Number of lines to copy
Set_Copy_Loop:
        MOVSB                           ;Copy 24 bits from next raster
        MOVSB
        MOVSB
        ADD     DI,HBYTES-3             ;Update pointers to next raster
        ADD     SI,HBYTES-3
        LOOP    Set_Copy_Loop

        ;Restore normal write mode

        XOR     AL,AL                   ;WRITE MODE register is still selected
        OUT     DX,AL                   ;so load it with value 0

        ;Clean up and return

        POP     DS                      ;Restore segment registers
        POP     ES
        POP     DI
        POP     SI
        MOV     SP,BP
        POP     BP
        RET
_SET_CURSOR     ENDP

;************************************************************************
; Move_Cursor:                                                          *
; This procedure is used to move the cursor from one                    *
; location to another.                                                  *
; Entry:        Curs_X - Position of the new cursor                     *
;               Curs_Y                                                  *
;************************************************************************

FUNC_AND        EQU     1
FUNC_XOR        EQU     3

Curs_X          EQU     [BP+4]
Curs_Y          EQU     [BP+6]

        PUBLIC  _MOVE_CURSOR

_MOVE_CURSOR    PROC    NEAR
        PUSH    BP
        MOV     BP,SP
        PUSH    SI
        PUSH    DI
        PUSH    ES                      ; always save seg regs
        PUSH    DS

        CALL    _REMOVE_CURSOR          ;Restore last location

        ;Setup EGA registers for data copy (WRITE LATCH write mode)

        MOV     DX,GRAPHICS_CTL                ;Address of GRAPICS controller
        MOV     AL,5                    ;Select WRITE LATCH write mode
        OUT     DX,AL
        INC     DX
        MOV     AL,1
        OUT     DX,AL

        ;Copy next location of the cursor to the save area

        MOV     AX,Curs_Y               ;Convert cursor X,Y to offset
        MOV     BX,HBYTES
        MUL     BX
        MOV     SI,Curs_X
        SHR     SI,1
        SHR     SI,1
        SHR     SI,1
        ADD     SI,AX
        MOV     CS:Last_Cursor,SI       ;Keep location so we can restore later
        MOV     AX,GRAPH_SEG               ;Point ES and DS to display buffer
        MOV     DS,AX
        MOV     ES,AX
        MOV     DI,CUR_SAVE             ;Pointer to save area
        MOV     CX,CUR_HEIGHT           ;Number of lines to copy
Move_Copy_Loop:
        MOVSB                           ;Copy 16 bits from next raster
        MOVSB
        MOVSB
        ADD     DI,HBYTES-3             ;Update pointers to next raster
        ADD     SI,HBYTES-3
        LOOP    Move_Copy_Loop

        ;Restore normal write mode

        MOV     DX,3CFH
        MOV     AL,0                    ;WRITE MODE register is still selected
        OUT     DX,AL                   ;so load it with value 0

        ;Use BITBLT procedure to copy AND and XOR masks of the cursor

        MOV     AX,FUNC_AND             ;Push function on the stack
        PUSH    AX
        MOV     AX,CUR_HEIGHT           ;Push width and height
        PUSH    AX
        PUSH    AX
        PUSH    WORD PTR Curs_Y         ;Push x and y of destination
        PUSH    WORD PTR Curs_X
        MOV     AX,YSIZE                ;Push x and y of source
        PUSH    AX
        MOV     AX,0
        PUSH    AX
        CALL    _BITBLT
        ADD     SP,14

        MOV     AX,FUNC_XOR             ;Push function on the stack
        PUSH    AX
        MOV     AX,CUR_HEIGHT           ;Push width and height
        PUSH    AX
        PUSH    AX
        PUSH    WORD PTR Curs_Y         ;Push x and y of destination
        PUSH    WORD PTR Curs_X
        MOV     AX,YSIZE                ;Push x and y of source
        PUSH    AX
        MOV     AX,16
        PUSH    AX
        CALL    _BITBLT
        ADD     SP,14

        ;Clean up and return

        POP     DS                      ;Restore segment registers
        POP     ES
        POP     DI
        POP     SI
        MOV     SP,BP
        POP     BP
        RET
_MOVE_CURSOR    ENDP

;************************************************************************
; Remove_Cursor:                                                        *
; This procedure is used to remove the cursor from the screen           *
; and to restore the screen to its original appearance                  *
;************************************************************************

        PUBLIC  _REMOVE_CURSOR

_REMOVE_CURSOR  PROC NEAR
        PUSH    BP
        MOV     BP,SP
        PUSH    SI
        PUSH    DI
        PUSH    ES                      ; always save seg regs
        PUSH    DS

        ;Setup EGA registers for data copy (WRITE LATCH write mode)

        MOV     DX,GRAPHICS_CTL                ;Address of GRAPICS controller
        MOV     AL,5                    ;Select WRITE LATCH write mode
        OUT     DX,AL
        INC     DX
        MOV     AL,1
        OUT     DX,AL

        ;Copy save area back to last location of the cursor

        MOV     AX,0A000H               ;Set up segment pointers
        MOV     ES,AX
        MOV     DS,AX
        MOV     DI,CS:Last_Cursor       ;Copy to last X,Y of cursor
        MOV     SI,CUR_SAVE             ;Copy form cursor save area
        MOV     CX,CUR_HEIGHT           ;Number of lines to copy
Rest_Copy_Loop:
        MOVSB                           ;Copy 16 bits from next raster
        MOVSB
        MOVSB
        ADD     DI,HBYTES-3             ;Update pointers to next raster
        ADD     SI,HBYTES-3
        LOOP    Rest_Copy_Loop

        ;Restore normal write mode

        MOV     AL,0                    ;WRITE MODE register is still selected
        OUT     DX,AL                   ;so load it with value 0

        ;Clean up and return

        POP     DS                      ;Restore segment registers
        POP     ES
        POP     DI
        POP     SI
        MOV     SP,BP
        POP     BP
        RET
_REMOVE_CURSOR  ENDP

;************************************************************************
; Transfer Bit Aligned Block of dimensions Width x Height, from         *
; source, with upper left corner at (X_Src,Y_Src), to destination with  *
; upper left corner at (X_Dst,Y_Dst), using function fn.                *
; Entry:        X_Srs, Y_Src  - Source upper left                       *
;               X_Dst, Y_Dst  - Destination upper left                  *
;               BWidth,BHeight- Dimensions of the block                 *
;               Fn            - Logical function (Copy, AND, OR, XOR)   *
;************************************************************************

;xxxx add later traversing x in reverse for overlapped src and dst

X_Src   EQU     [BP+4]                  ;Formal parameters on the stack
Y_Src   EQU     [BP+6]
X_Dst   EQU     [BP+8]
Y_Dst   EQU     [BP+10]
BWidth  EQU     [BP+12]
BHeight EQU     [BP+14]
Fn      EQU     [BP+16]

Y_Incr          EQU     WORD PTR [BP-2] ;Local variables
First_Mask      EQU     BYTE PTR [BP-4]
InFirst         EQU     WORD PTR [BP-6]
Last_Mask       EQU     BYTE PTR [BP-8]
Read_Plane      EQU     BYTE PTR [BP-10]
Lower_Mask      EQU     BYTE PTR [BP-12]
Upper_Mask      EQU     BYTE PTR [BP-14]
Src_Address     EQU     WORD PTR [BP-16]
Dst_Address     EQU     WORD PTR [BP-18]
Block_Dx        EQU     WORD PTR [BP-20]
Block_Dy        EQU     WORD PTR [BP-22]

	PUBLIC  _BITBLT

_BITBLT PROC    NEAR
	PUSH    BP
	MOV     BP,SP

	SUB     SP,24                   ;Setup local variables

	PUSH    DS                      ;Presrve segment registers
	PUSH    ES
	PUSH    DI
	PUSH    SI

	;-------------------------------------------------------------
	;--- COMPUTE MASKS FOR FIRST AND LAST BYTE WITHIN A RASTER
	;    ROTATION FACTOR, AND MASKS FOR HI AND LOW BITS AFTER ROTATION
	;    ENABLE ROTATION AND LOGICAL FUNCTIONS IN GRAPHICS CONTROLLER
	;-------------------------------------------------------------

	MOV     CX,X_Dst                ; NUMBER OF CLEAR BITS IN FIRST BYTE
	AND     CX,7
	MOV     First_Mask,0FFH         ; COMPUTE MASK TO CLEAR LEADING BITS
	SHR     First_Mask,CL
	MOV     AX,8
	SUB     AX,CX
	MOV     InFirst,AX

	;MASK TO KEEP BITS IN LAST BYTE (PARTIAL BYTE)

	MOV     CX,X_Dst                ; GET ADDRESS OF LAST BIT
	ADD     CX,BWidth
	AND     CX,7                    ; POSITION JUST AFTER LAST BIT
	MOV     Last_Mask,0FFH          ; COMPUTE MASK TO KEEP BITS AFTER
	SHR     Last_Mask,CL
	NOT     Last_Mask               ; COMPLEMENT TO KEEP LEADING BITS

	;BITS FROM SOURCE BYTE MUST BE ALLIGNED WITH BITS IN THE
	;DESTINATION BYTE.  HERE IS COMPUTED THE ROTATION AND THE
	;TWO MASKS NEEDED TO ISOLOTATE TWO HALVES OF THE BYTE

	; COMPUTE ROTATION

	MOV     CX,X_Dst                ; COMPUTE BIT DISTANCE BETWEEN
	SUB     CX,X_Src                ;... BIT POSITIONS IN FIRST BYTES
	AND     CX,7                    ; ... OF SOURCE AND DESTINATION BYTES
					; ... AS (SRC-DST)&7
	MOV     AX,00FFH                ;Compute masks for non-zero rotation
	ROR     AX,CL
	MOV     Lower_Mask,AL           ;Save the masks
	MOV     Upper_Mask,AH

	MOV     DX,GRAPHICS_CTL                 ;Address of GRAPHICS controller
	MOV     AL,3                    ;Index for DATA ROTATE & FN SELECT
	OUT     DX,AL                   ;Select DATA ROTATE & FN SELECT REG
	INC     DX
	MOV     AL,CL                   ;Fetch rotate count
	MOV     AH,Fn                   ;Fetch logical function
	AND     AH,3                    ;Move logical function into
	SHL     AH,1                    ;bits 3 and 4
	SHL     AH,1
	SHL     AH,1
	OR      AL,AH
	OUT     DX,AL                   ;Write value into DATA ROTATE... reg

	;-------------------------------------------------------------
	;--- COMPUTE ABSOLUTE ADDRESS OF SOURCE AND DESTINATION
	;-------------------------------------------------------------

	MOV     WORD PTR Y_Incr,HBYTES  ;Initialize y traversal as normal

	;Because of possible overlap we must adjust direction of traversal
	;If source block is above the destination then reverse Y traversal
	;If source block is to the left, then reverse X traversal

	MOV     AX,Y_Src                ;Compare source and destination Y
	MOV     BX,Y_Dst
	CMP     AX,BX
	JNLE    Compute_Address         ;...Leave alone if src <= dst APICHART
	ADD     AX,BHeight              ;Begin with last raster in block
	DEC     AX
	ADD     BX,BHeight
	DEC     BX
	MOV     Y_Src,AX
	MOV     Y_Dst,BX
	NEG     WORD PTR Y_Incr         ;And traverse backwards
Compute_Address:
	MOV     AX,Y_Src                ;Compute offset for source
	MOV     BX,HBYTES               ;as offset = y * 80 + x/8
	MUL     BX
	MOV     SI,X_Src
	SHR     SI,1
	SHR     SI,1
	SHR     SI,1
	ADD     SI,AX
        MOV     Src_Address,SI

        MOV     AX,Y_Dst                ;Compute offset for destination
        MOV     BX,HBYTES               ;as offset = y * 80 + x/8
        MUL     BX
	MOV     DI,X_Dst
        SHR     DI,1
        SHR     DI,1
        SHR     DI,1
        ADD     DI,AX
        MOV     Dst_Address,DI

        MOV     AX,GRAPH_SEG              ;Setup segment
        MOV     ES,AX
        MOV     DS,AX

        ;-------------------------------------------------------------
        ;--- ENABLE NEXT PLANE FOR READ AND WRITE
        ;-------------------------------------------------------------

        MOV     AL,3                    ;Initialize counter of planes
        MOV     Read_Plane,AL
Plane_Loop:
        MOV     AX,BWidth               ;Copy dimensions of the block
        MOV     Block_Dx,AX
        MOV     AX,BHeight
        MOV     Block_Dy,AX
        MOV     SI,Src_Address          ;Fetch source address
        MOV     DI,Dst_Address          ;Fetch destination address
        MOV     CL,Read_Plane           ;Fetch next plane to do
        MOV     AH,1                    ;Use plane number to setup value
        SHL     AH,CL                   ;for WRITE PLANE SELECT register
        MOV     DX,3C4H                 ;Fetch address of SEQUENCER
        MOV     AL,2                    ;Index for WRITE PLANE SELECT register
        OUT     DX,AL                   ;Select register
        INC     DX
        MOV     AL,AH                   ;Fetch value
        OUT     DX,AL                   ;Write value into register
        MOV     DX,GRAPHICS_CTL                 ;Fetch address of GRAPHICS controller
        MOV     AL,4                    ;Index for READ PLANE SELECT register
        OUT     DX,AL                   ;Select register
        INC     DX
        MOV     AL,CL                   ;Fetch plane number
        OUT     DX,AL                   ;Write plane number into register
	DEC     DX
        MOV     AL,8                    ;Index for BIT MASK register
        OUT     DX,AL                   ;Select BIT MASK register
        INC     DX

        ;-------------------------------------------------------------
	; RASTER LOOP IS DONE IN FOUR STEPS
        ; (1) MASKS AND ROTATION FACTOR IS COMPUTED TO ALIGN SRC AND DEST
        ; (2) LEADIING PARTIAL BYTE OF DESTINATION IS MODIFIED
        ; (3) FULL BYTES OF DESTINATION ARE MODIFIED
        ; (4) TRAILLING PARTIAL BYTE OF DESTINATION IS MODIFIED
        ;-------------------------------------------------------------

        ;-------------------------------------------------------------
        ;--- GET ENOUGH BITS FROM SOUCE TO CONSTRUCT FIRST PARTIAL BYTE OF DST
        ;-------------------------------------------------------------

Raster_Loop:
        MOV     CX,Block_Dx             ;Number of bits to copy
        PUSH    SI                      ;Preserve addresses for next raster loo
        PUSH    DI

        MOV     AX,X_Src                ;Check if need one or two bytes
        AND     AX,7                    ;for first byte of destination
        MOV     BX,X_Dst
        AND     BX,7
        CMP     AX,BX
        JG      Get_2_Bytes
Get_1_Byte:
        MOV     AL,First_Mask           ;Fetch mask for first partial byte
        AND     AL,Lower_Mask           ;Combine with 'lower mask'
        OUT     DX,AL                   ;Write mask into BIT MASK register
        LODSB                           ;Fetch first source byte
        MOV     AH,[DI]                 ;Latch destination
        STOSB                           ;Write first byte of destination
        MOV     AH,AL                   ;Save AL since it will be trashed by OU
        JMP     Leading_Done

Get_2_Bytes:
        MOV     AL,First_Mask           ;Fetch mask for first partial byte
        AND     AL,Upper_Mask           ;Combine with 'upper mask'
        OUT     DX,AL                   ;Write mask into BIT MASK register
        LODSB
        MOV     AH,[DI]                 ;Latch destination bits
        MOV     ES:[DI],AL              ;Write first byte

        MOV     AL,First_Mask           ;Fetch mask for first partial byte
        AND     AL,Lower_Mask           ;Combine with 'lower mask'
        OUT     DX,AL                   ;Write mask into BIT MASK register
        LODSB
        MOV     AH,[DI]                 ;Latch destination bits
	STOSB                           ;Write first byte
        MOV     AH,AL                   ;Save AL since it will be trashed by OU

Leading_Done:
        SUB     CX,InFirst              ;Update number of bits to transfer
        JLE     Raster_Done

        ;-------------------------------------------------------------
        ;--- LOOP OVER COMPLETE BYTES WITHIN A SINGLE SOURCE RASTER
        ;-------------------------------------------------------------

Full_Loop:
        CMP     CX,8                    ;If less then 8 bits to do
        JL      Full_Done               ;quit this loop

        MOV     AL,Upper_Mask           ;Fetch the 'upper mask'
        OUT     DX,AL                   ;Write mask into BIT MASK register
        MOV     AL,[DI]                 ;Latch destination bits
        MOV     [DI],AH                 ;Write half 2 for src to half 1 of dst

        MOV     AL,Lower_Mask           ;Fetch the lower mask
        OUT     DX,AL                   ;Write mask into BIT MASK register
        LODSB                           ;Fetch next source byte
        MOV     AH,[DI]                 ;Latch destination bits
        STOSB                           ;Write half 1 of src to half 2 of dst
        MOV     AH,AL                   ;Save AL since it will be trashed by OU

        SUB     CX,8                    ; DECREMENT WIDTH BY BITS TO BE DONE
        JMP     Full_Loop
Full_Done:

        ;-------------------------------------------------------------
        ;--- TRANSFER THE LAST PARTIAL BYTE
        ;-------------------------------------------------------------

Last_Partial:
        CMP     CX,0                    ;Any more bits to transfer?
        JLE     Raster_Done             ;...No, quit this raster

        MOV     AL,Last_Mask            ;Fetch mask for last partial byte
        AND     AL,Upper_Mask           ;Combine with 'upper mask'
        OUT     DX,AL                   ;Write mask into BIT MASK register
        MOV     AL,[DI]                 ;Latch destination bits
        MOV     [DI],AH                 ;Write half 2 for src to half 1 of dst

	MOV     AL,Last_Mask            ;Fetch mask for last partial byte
        AND     AL,Lower_Mask           ;Combine with 'upper mask'
        OUT     DX,AL                   ;Write mask into BIT MASK register
        LODSB                           ;Fetch next source byte
        MOV     AH,[DI]                 ;Latch destination bits
        STOSB                           ;Write half 1 of src to half 2 of dst

        ;--- ADVANCE TO THE NEXT RASTER

Raster_Done:
        POP     DI                      ;Pointers to current raster
        POP     SI
        ADD     SI,Y_Incr               ;Update pointer to point to the next
        ADD     DI,Y_Incr               ;raster
        DEC     WORD PTR Block_Dy       ;Update number of rasters to do
        JLE     Test_Plane
        JMP     Raster_Loop             ;And repeat if not all done

Test_Plane:
        DEC     Read_Plane              ;Update number of planes to do
        JL      Blit_Done
        JMP     Plane_Loop              ;And do next plane if any left to do

        ;-------------------------------------------------------------
        ;--- Clean up and exit
        ;-------------------------------------------------------------

Blit_Done:
        ;restore rotate value
        MOV     DX,GRAPHICS_CTL                 ;Address of GRAPHICS controller
        MOV     AL,3                    ;Index for ROTATE & FN SELECT
        OUT     DX,AL                   ;Select ROTATE & FN SELECT register
        INC     DX
        XOR     AL,AL                   ;Disable rotate and set fn=copy
        OUT     DX,AL                   ;Write value into register
        ;restore bit mask
        DEC     DX
        MOV     AL,8                    ;Index for BIT MASK register
        OUT     DX,AL                   ;Select BIT MASK register
        INC     DX
        MOV     AL,0FFH                 ;Enable all 8 bits for write
        OUT     DX,AL                   ;Write value into register
        ;enable all four planes for write
        MOV     DX,SEQUENCE_CTL                 ;Address of SEQUENCER
        MOV     AL,2                    ;Index for PLANE ENABLE register
	OUT     DX,AL                   ;Select register
        INC     DX
        MOV     AL,0FH                  ;Value to enable all four planes
        OUT     DX,AL                   ;Write value into register

        POP     SI                      ;Restore segment registers
        POP     DI
        POP     ES
        POP     DS
        MOV     SP,BP
        POP     BP
        RET
_BITBLT ENDP

;************************************************************************
; Get current video mode from segment zero                              *
;************************************************************************

BIOS_Mode       EQU     449H            ;Location of current mode

        PUBLIC  _Get_Mode

_Get_Mode       PROC NEAR
        PUSH    ES                      ;Point ES to segment 0
        XOR     AX,AX
        MOV     ES,AX
        MOV     AL,ES:[BIOS_Mode]       ;Fetch current video mode
        XOR     AH,AH                   ;Clear rest of AX for return to C
        POP     ES
        RET
_Get_Mode       ENDP

;************************************************************************
; Use BIOS function to get the current video mode                       *
; Exit: AX - Current operation mode                                     *
;************************************************************************

;        PUBLIC  _Get_Mode

;_Get_Mode  PROC NEAR
;        MOV     AH,0FH                  ;Function = GET DISPLAY MODE
;        INT     10H                     ;Ask BIOS to fetch mode
;        XOR     AH,AH                   ;Return mode in AX
;        RET
;_Get_Mode  ENDP

_TEXT    ENDS
         END
