$NOMOD51
;------------------------------------------------------------------------------
;  This file is part of the C51 Compiler package
;  Copyright (c) 1988-2005 Keil Elektronik GmbH and Keil Software, Inc.
;  Version 8.01
;
;  *** <<< Use Configuration Wizard in Context Menu >>> ***
;------------------------------------------------------------------------------
;  STARTUP.A51:  This code is executed after processor reset.
;
;  To translate this file use A51 with the following invocation:
;
;     A51 STARTUP.A51
;
;  To link the modified STARTUP.OBJ file to your application use the following
;  Lx51 invocation:
;
;     Lx51 your object file list, STARTUP.OBJ  controls
;
;------------------------------------------------------------------------------
;
;  �û��������ϵ��ʼ�����ڴ�ռ�//User-defined <h> Power-On Initialization of Memory
;
;  ʹ������EQU����ɶ�����CPU��λʱ����0���г�ʼ�����ڴ�ռ�
;  With the following EQU statements the initialization of memory at processor reset can be defined:
;
; <o> IDATALEN: IDATA memory size <0x0-0x100>
;     <i> Note: The absolute start-address of IDATA memory is always 0//IDATA �洢���Ŀռ�ľ�����ʼ��ַ����0
;     <i>       The IDATA space overlaps physically the DATA and BIT areas.// IDATA �洢���Ŀռ��������ϰ�����8051��Ƭ����DATA��BIT�洢���ռ�.
IDATALEN        EQU     80H ; ����0���г�ʼ����IDATA�洢���ռ���ֽ���
;
; <o> XDATASTART: XDATA memory start address <0x0-0xFFFF> 
;     <i> The absolute start address of XDATA memory
XDATASTART      EQU     0   ; XDATA�洢���ռ�ľ�����ʼ��ַ  
;
; <o> XDATALEN: XDATA memory size <0x0-0xFFFF> 
;     <i> The length of XDATA memory in bytes.
XDATALEN        EQU     0   ; ����0���г�ʼ����XDATA�洢���Ŀռ��ֽ���.   
;
; <o> PDATASTART: PDATA memory start address <0x0-0xFFFF> 
;     <i> The absolute start address of PDATA memory
PDATASTART      EQU     0H  ; PDATA�洢���Ŀռ�ľ�����ʼ��ַ
;
; <o> PDATALEN: PDATA memory size <0x0-0xFF> 
;     <i> The length of PDATA memory in bytes.
PDATALEN        EQU     0H  ; ����0���г�ʼ����PDATA�洢���Ŀռ��ֽ���
;
;</h>
;------------------------------------------------------------------------------
;
;<h> ���뺯��<reentrant>ģ���ʼ��//Reentrant Stack Initialization
;
;  The following EQU statements define the stack pointer for reentrant
;  functions and initialized it://������EQUָ��������뺯��ģ���ջָ��ĳ�ʼ��
;
; <h> ʹ��SMALL�洢��ģʽʱ���뺯���Ķ�ջ�ռ�//Stack Space for reentrant functions in the SMALL model.
;  <q> IBPSTACK: Enable SMALL model reentrant stack
;     <i> Stack space for reentrant functions in the SMALL model.
IBPSTACK        EQU     0       ;ʹ��SMALL�洢��ģʽ���뺯��ʱ�������ó�1//set to 1 if small reentrant is used.
;  <o> IBPSTACKTOP: End address of SMALL model stack <0x0-0xFF>
;     <i> Set the top of the stack to the highest location.
IBPSTACKTOP     EQU     0xFF +1     ; default 0FFH+1// ����ջ������Ϊ��ߵ�ַ+1.
; </h>
;
; <h> ʹ��LARGE�洢��ģʽʱ���뺯���Ķ�ջ�ռ�//Stack Space for reentrant functions in the LARGE model.      
;  <q> XBPSTACK: Enable LARGE model reentrant stack
;     <i> Stack space for reentrant functions in the LARGE model.
XBPSTACK        EQU     1       ; ʹ��LARGE�洢��ģʽ���뺯��ʱ�������ó�1//set to 1 if large reentrant is used.;by me 0,
;  <o> XBPSTACKTOP: End address of LARGE model stack <0x0-0xFFFF>
;     <i> Set the top of the stack to the highest location.
XBPSTACKTOP     EQU     0xF7FF + 1;IAP15W4K58S4 ; 0x03FF +1;STC12C5A60S2   ; ����ջ������Ϊ��ߵ�ַ+1//default 0FFFFH+1 ;by me
; </h>
;
; <h> ʹ��COMPACT�洢��ģʽʱ���뺯���Ķ�ջ�ռ�//Stack Space for reentrant functions in the COMPACT model.    
;  <q> PBPSTACK: Enable COMPACT model reentrant stack
;     <i> Stack space for reentrant functions in the COMPACT model.
PBPSTACK        EQU     0       ; ʹ��COMPACT�洢��ģʽ���뺯��ʱ�������ó�1//set to 1 if compact reentrant is used.
;
;   <o> PBPSTACKTOP: End address of COMPACT model stack <0x0-0xFFFF>
;     <i> Set the top of the stack to the highest location.
PBPSTACKTOP     EQU     0xF7FF + 1;IAP15W4K58S4 ; 0x03FF +1;STC12C5A60S2    ; ����ջ������Ϊ��ߵ�ַ+1//default 0FFH+1  ;by me
; </h>
;</h>
;------------------------------------------------------------------------------
;
;  ʹ��COMPACT�洢��ģʽʱ64K�ֽ�XDATA�洢���ռ�ķ�ҳ����//Memory Page for Using the Compact Model with 64 KByte xdata RAM
;  <e>Compact Model Page Definition
;
;  <i>������EQUָ���PDATA���ͱ�����XDATA�洢���ռ��ҳ��ַ//Define the XDATA page used for PDATA variables. 
;  <i>ʹ��EQUָ���PFAGEʱ������L51���Ӷ�λ��PDATAָ��Ŀ��Ʋ���һ��//PPAGE must conform with the PPAGE set in the linker invocation.
;
; Enable pdata memory page initalization
PPAGEENABLE     EQU     0       ; ʹ��PDATA���ͱ���ʱ�������ó�1//set to 1 if pdata object are used.
;
; <o> PPAGE number <0x0-0xFF> 
; <i> uppermost 256-byte address of the page used for PDATA variables.
PPAGE           EQU     0		; ����ҳ��
;
; <o> SFR address which supplies uppermost address byte <0x0-0xFF> 
; <i> most 8051 variants use P2 as uppermost address byte
PPAGE_SFR       DATA    0A0H
;
; </e>
;------------------------------------------------------------------------------

; Standard SFR Symbols 
ACC     DATA    0E0H
B       DATA    0F0H
SP      DATA    81H
DPL     DATA    82H
DPH     DATA    83H

                NAME    ?C_STARTUP; ģ����Ϊ ?C_STAUTUP


?C_C51STARTUP   SEGMENT   CODE		; ����
?STACK          SEGMENT   IDATA		; ��ջ

                RSEG    ?STACK		; ��ջ
                DS      1

                EXTRN CODE (?C_START)	; ����ʼ��ַ
                PUBLIC  ?C_STARTUP

                CSEG    AT      0		;  �����û��������ʼ��ַ,��MON51������ʱ��������
?C_STARTUP:     LJMP    STARTUP1

                RSEG    ?C_C51STARTUP

STARTUP1:

IF IDATALEN <> 0	;  ��Ƭ���ϵ�IDATA�ڴ�����
                MOV     R0,#IDATALEN - 1
                CLR     A
IDATALOOP:      MOV     @R0,A
                DJNZ    R0,IDATALOOP
ENDIF

IF XDATALEN <> 0	;  ��Ƭ���ϵ�XDATA�ڴ�����
                MOV     DPTR,#XDATASTART
                MOV     R7,#LOW (XDATALEN)
  IF (LOW (XDATALEN)) <> 0
                MOV     R6,#(HIGH (XDATALEN)) +1
  ELSE
                MOV     R6,#HIGH (XDATALEN)
  ENDIF
                CLR     A
XDATALOOP:      MOVX    @DPTR,A
                INC     DPTR
                DJNZ    R7,XDATALOOP
                DJNZ    R6,XDATALOOP
ENDIF

IF PPAGEENABLE <> 0	;  ��PDATA�洢��ҳ���λ��ַ
                MOV     PPAGE_SFR,#PPAGE
ENDIF

IF PDATALEN <> 0	;  ��Ƭ���ϵ�PDATA�ڴ�����
                MOV     R0,#LOW (PDATASTART)
                MOV     R7,#LOW (PDATALEN)
                CLR     A
PDATALOOP:      MOVX    @R0,A
                INC     R0
                DJNZ    R7,PDATALOOP
ENDIF

IF IBPSTACK <> 0	;  ����ʹ��SMALL�洢��ģʽʱ���뺯���Ķ�ջ�ռ�
EXTRN DATA (?C_IBP)

                MOV     ?C_IBP,#LOW IBPSTACKTOP
ENDIF

IF XBPSTACK <> 0	;  ����ʹ��LARGE�洢��ģʽʱ���뺯���Ķ�ջ�ռ�
EXTRN DATA (?C_XBP)

                MOV     ?C_XBP,#HIGH XBPSTACKTOP
                MOV     ?C_XBP+1,#LOW XBPSTACKTOP
ENDIF

IF PBPSTACK <> 0	;  ����ʹ��COMPACT�洢��ģʽʱ���뺯���Ķ�ջ�ռ�.
EXTRN DATA (?C_PBP)
                MOV     ?C_PBP,#LOW PBPSTACKTOP
ENDIF
;  ���ö�ջ����ʼ��ַ
                MOV     SP,#?STACK-1	; ���� MOV  SP,#4FH;

; �����ĳ���ʹ����Mode4������鼼�� ����������ĳ���(_(:�٩f��)_����� ��ĳ��򳬹�64K ����)This code is required if you use L51_BANK.A51 with Banking Mode 4
;<h> Code Banking
; <q> Select Bank 0 for L51_BANK.A51 Mode 4
#if 0   
;     <i> Initialize bank mechanism to code bank 0 when using L51_BANK.A51 with Banking Mode 4.
EXTRN CODE (?B_SWITCH0)
                CALL    ?B_SWITCH0      ; ����ӵ�һ��bank 0 �鿪ʼִ��//init bank mechanism to code bank 0
#endif
;</h> ��ת���û�����MAIN����
                LJMP    ?C_START

                END