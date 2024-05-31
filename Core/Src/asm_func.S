.text

.syntax unified
.thumb

.global asm_pack32to16
.global asm_max
.global asm_downSample
.global asm_invertir

.thumb_func

/*
*	r0 = vectorIn
*	r1 = vectorOut
*	r2 = longitud
*	r3 = vectorOut temporal
*/

	asm_pack32to16:
			CBZ r0, _exit	//Guarda
			CBZ r1, _exit
			CBZ r2, _exit

		_loop:
			SUBS r2, #1
			LDR	 r3, [r0, r2, LSL #2] // Actualiza el APSR
			ASR  r3, r3, #16		  // Guarda en r3 lo que estaba en r3 desplazado 16 bits (con signo)
			STRH r3, [r1, r2, LSL #1] // Almacena en r1 lo que está en r0 en la direcciñon almacenada en r2 y lo desplaza 2 bytes
			BNE	 _loop				  // Branch If Non-Equal (Z != 0) - APSR Actualizado por SUBS

		_exit:
			BX lr
