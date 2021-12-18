;******************************************************************************
;* C) Copyright HTL - HOLLABRUNN  2020 									      * 
;*                                                                            * 
;* File Name:   pplcnt.s					                                  *
;* Autor: 		Maximilian Roll & Martin Platajs                              *
;* Version: 	V1.0                                                          *
;* Date: 		08.01.2020                                                    *
;* Description: Personenzähleinrichtung mithilfe von Lichtschranken           *
;******************************************************************************

        		AREA PPLCNT, CODE, READONLY
				INCLUDE STM32_F103RB_MEM_MAP.INC
				EXPORT __main
					
;******************************** Definitionen ********************************

;R9 ...counter
;R10...state
;R11...time
;R12...direction(00...outside/01...goingIn/10...goingOut)

;Definition Gates:
Gate1			EQU		PERIPH_BB_BASE+(GPIOA_IDR-PERIPH_BASE)*0x20+0*4
Gate2			EQU 	PERIPH_BB_BASE+(GPIOA_IDR-PERIPH_BASE)*0x20+7*4

;Definition LEDs:
LED0 			EQU 	PERIPH_BB_BASE+(GPIOB_ODR-PERIPH_BASE)*0x20+8*4
LED1 			EQU 	PERIPH_BB_BASE+(GPIOB_ODR-PERIPH_BASE)*0x20+9*4
LED2 			EQU 	PERIPH_BB_BASE+(GPIOB_ODR-PERIPH_BASE)*0x20+10*4
LED3 			EQU 	PERIPH_BB_BASE+(GPIOB_ODR-PERIPH_BASE)*0x20+11*4
LED4	 		EQU 	PERIPH_BB_BASE+(GPIOB_ODR-PERIPH_BASE)*0x20+12*4
LED5	 		EQU 	PERIPH_BB_BASE+(GPIOB_ODR-PERIPH_BASE)*0x20+13*4
LED6			EQU 	PERIPH_BB_BASE+(GPIOB_ODR-PERIPH_BASE)*0x20+14*4
LED7			EQU 	PERIPH_BB_BASE+(GPIOB_ODR-PERIPH_BASE)*0x20+15*4

;Definition Piezo:
PIEZO			EQU		PERIPH_BB_BASE+(GPIOB_ODR-PERIPH_BASE)*0x20+0*4

					
;******************************************************************************
;*                        M A I N  P r o g r a m m:                           *
;******************************************************************************

__main			PROC
				BL	 	init_ports
				BL		init_variables
				
main_loop		LDR R0, =GPIOA_IDR ;Alle Schalter einlesen
				LDR R1, [R0]
				AND R10, R1, #0x3 ;Ersten 2 Schalter abmaskieren
				
				BL check_error
				
				BL check_double
				
				BL checkin
				
				BL checkout
				
				BL check_timeout
				
				BL inc_time
				
				BL wait_500ms
				
				B main_loop
				
				ENDP


;******************************************************************************
;*            U N T E R P R O G R A M M:    init_ports                        *
;*                                                                            *
;* Aufgabe:   Initialisiert Portleitungen für LED / Schalterplatine           *
;* Input:     keine                                                           *
;* return:	  keine                                                           *
;******************************************************************************

init_ports		PROC
				push {R0,R1,R2,LR}	   	; save link register to Stack
				
				MOV	R2, #0x4 	  		; enable clock for GPIOA (APB2 Peripheral clock enable register)
				LDR R1,	=RCC_APB2ENR 
				LDR	R0, [R1]
				ORR	R0,	R0, R2
				STR R0, [R1]
				
				MOV	R2, #0x8  	  		; enable clock for GPIOB (APB2 Peripheral clock enable register)
				LDR R1,	=RCC_APB2ENR
				LDR	R0, [R1]
				ORR	R0,	R0, R2
				STR R0, [R1]

				LDR R1,	=GPIOA_CRL	 	; set Port Pins PA0 to Pull Up/Down Input mode (50MHz) - Schalter S0
				LDR R0,	[R1]	   
				LDR R2, =0xFFFFFF00  
				AND	R0,	R0, R2
				LDR R2, =0x00000088
				ORR	R0,	R0, R2
				STR R0, [R1]
				LDR R1,	=GPIOA_ODR	 	; GPIOA Output Register Bit 0 auf "1" sodass Input Pull Up aktiviert ist!!	
				LDR R0,	[R1]	
				LDR R2, =0x03  
				ORR	R0,	R0, R2
				STR R0, [R1]

				LDR R1,	=GPIOB_CRH	 	; set Port Pin PB8 to Push Pull Output Mode (50MHz) - LED0
				LDR R0, [R1]	   				
				LDR R2, =0x00000000  
				AND	R0,	R0, R2		
				LDR R2, =0x33333333
				ORR	R0,	R0, R2
				STR R0, [R1]

				POP {R0,R1,R2,PC}	   	;restore link register to Programm Counter and return
				ENDP
				
					
;******************************************************************************
;*            U N T E R P R O G R A M M:    init_variales                     *
;*                                                                            *
;* Aufgabe:   Initialisiert Portleitungen für LED / Schalterplatine           *
;* Input:     keine                                                           *
;* return:	  keine                                                           *
;******************************************************************************

init_variables	PROC
				PUSH{LR}

				LDR R9, =0x0
				BL reset
				
				POP{PC}
				
				ENDP

;******************************************************************************
;*            U N T E R P R O G R A M M:    wait_500ms                        *
;*                                                                            *
;* Aufgabe:   Wartet 500ms                                                    *
;* Input:     keine                                                           *
;* return:	  keine	                                                          *
;******************************************************************************

wait_500ms		PROC
				push 	{R0-R2,LR}	   ; save link register to Stack
                MOV     R0,#0x1F4	   ; wait 500ms
                MOV     R1,#0
wait_ms_loop	MOV		R2,#0x63B			
wait_ms_loop1	SUB 	R2,R2,#1
				CMP		R2,R1
				BNE		wait_ms_loop1
				SUB 	R0,R0,#1
				CMP		R0,R1
				BNE		wait_ms_loop
				POP 	{R0-R2,PC}	   ;restore link register to Programm Counter and return
				ENDP
				ALIGN				

				END