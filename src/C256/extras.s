				.public restart_cli

				.extern cli_rerepl
            	.extern _Vfp
            	.extern _DirectPageStart

#ifndef __CALYPSI_DATA_MODEL_SMALL__
              	.extern _NearBaseAddress
#endif

              	.section stack
				.section code

;
; Reset the stack to the initial value.
; Reset the direct page and data bank registers
; Restart the CLI
;
restart_cli:	
				rep     #0x38         ; 16-bit registers, no decimal mode
				ldx     ##.sectionEnd stack
				txs                   ; set stack
				lda     ##_DirectPageStart
				tcd                   ; set direct page
#ifdef __CALYPSI_DATA_MODEL_SMALL__
				lda     ##0
#else
				lda     ##.word2 _NearBaseAddress
#endif
				stz     dp:.tiny(_Vfp+2)
				xba                   ; A upper half = data bank
				pha
				plb                   ; pop 8 dummy
				plb                   ; set data bank

				; jsl cli_rerepl
				bra restart_cli
