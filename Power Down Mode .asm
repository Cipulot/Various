        list       p = 16f84a
		RADIX       DEC
		#include     <P16f84.inc>
        errorlevel -302
        __CONFIG    3FF1H

SWITCH1 EQU  0
SWITCH2 EQU  1
LED1    EQU  2
        ORG  0CH
		
Count   RES  2
        ORG  00H
        goto Start
        ORG  04H
        bcf  INTCON,INTF
        retfie

Start:
        bsf     STATUS,RP0
        MOVLW   00011111B
        MOVLW   TRISA & 0x7F
		bsf     TRISB & 0x7F, SWITCH1
		bsf     TRISB & 0x7F, SWITCH2
		bsf     TRISB & 0x7F, LED1
		
		bcf     OPTION_REG & 0X7F, INTEDG
		bcf     STATUS,RP0
		bcf     INTCON,GIE
		bcf     INTCON,INTE
		bcf     PORTB,LED1
		
MainLoop

        btfss   PORTB,SWITCH2
		sleep
		call    Delay
		btfss   PORTB,LED1
		goto    TurnOnLed1
		goto    TurnOffLed1
		
TurnOnLed1
        bsf     PORTB,LED1
		goto    MAinLoop
		
TurnOffLed1
        bcf     PORTB,LED1
		goto    MAinLoop
		
Delay
        clrf    Count
		clrf    Count+1
		
DelayLoop

        decfsz  Count,1
		goto    DelayLoop
		
		decfsz  Count+1,1
		goto    DelayLoop
		
		return
		
		END