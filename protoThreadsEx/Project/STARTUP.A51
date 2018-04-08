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
;  用户定义需上电初始化的内存空间//User-defined <h> Power-On Initialization of Memory
;
;  使用以下EQU命令可定义在CPU复位时需用0进行初始化的内存空间
;  With the following EQU statements the initialization of memory at processor reset can be defined:
;
; <o> IDATALEN: IDATA memory size <0x0-0x100>
;     <i> Note: The absolute start-address of IDATA memory is always 0//IDATA 存储器的空间的绝对起始地址总是0
;     <i>       The IDATA space overlaps physically the DATA and BIT areas.// IDATA 存储器的空间在物理上包括了8051单片机的DATA和BIT存储器空间.
IDATALEN        EQU     80H ; 需用0进行初始化的IDATA存储器空间的字节数
;
; <o> XDATASTART: XDATA memory start address <0x0-0xFFFF> 
;     <i> The absolute start address of XDATA memory
XDATASTART      EQU     0   ; XDATA存储器空间的绝对起始地址  
;
; <o> XDATALEN: XDATA memory size <0x0-0xFFFF> 
;     <i> The length of XDATA memory in bytes.
XDATALEN        EQU     0   ; 需用0进行初始化的XDATA存储器的空间字节数.   
;
; <o> PDATASTART: PDATA memory start address <0x0-0xFFFF> 
;     <i> The absolute start address of PDATA memory
PDATASTART      EQU     0H  ; PDATA存储器的空间的绝对起始地址
;
; <o> PDATALEN: PDATA memory size <0x0-0xFF> 
;     <i> The length of PDATA memory in bytes.
PDATALEN        EQU     0H  ; 需用0进行初始化的PDATA存储器的空间字节数
;
;</h>
;------------------------------------------------------------------------------
;
;<h> 再入函数<reentrant>模拟初始化//Reentrant Stack Initialization
;
;  The following EQU statements define the stack pointer for reentrant
;  functions and initialized it://以下用EQU指令定义了再入函数模拟堆栈指针的初始化
;
; <h> 使用SMALL存储器模式时再入函数的堆栈空间//Stack Space for reentrant functions in the SMALL model.
;  <q> IBPSTACK: Enable SMALL model reentrant stack
;     <i> Stack space for reentrant functions in the SMALL model.
IBPSTACK        EQU     0       ;使用SMALL存储器模式再入函数时将其设置成1//set to 1 if small reentrant is used.
;  <o> IBPSTACKTOP: End address of SMALL model stack <0x0-0xFF>
;     <i> Set the top of the stack to the highest location.
IBPSTACKTOP     EQU     0xFF +1     ; default 0FFH+1// 将堆栈顶设置为最高地址+1.
; </h>
;
; <h> 使用LARGE存储器模式时再入函数的堆栈空间//Stack Space for reentrant functions in the LARGE model.      
;  <q> XBPSTACK: Enable LARGE model reentrant stack
;     <i> Stack space for reentrant functions in the LARGE model.
XBPSTACK        EQU     1       ; 使用LARGE存储器模式再入函数时将其设置成1//set to 1 if large reentrant is used.;by me 0,
;  <o> XBPSTACKTOP: End address of LARGE model stack <0x0-0xFFFF>
;     <i> Set the top of the stack to the highest location.
XBPSTACKTOP     EQU     0xF7FF + 1;IAP15W4K58S4 ; 0x03FF +1;STC12C5A60S2   ; 将堆栈顶设置为最高地址+1//default 0FFFFH+1 ;by me
; </h>
;
; <h> 使用COMPACT存储器模式时再入函数的堆栈空间//Stack Space for reentrant functions in the COMPACT model.    
;  <q> PBPSTACK: Enable COMPACT model reentrant stack
;     <i> Stack space for reentrant functions in the COMPACT model.
PBPSTACK        EQU     0       ; 使用COMPACT存储器模式再入函数时将其设置成1//set to 1 if compact reentrant is used.
;
;   <o> PBPSTACKTOP: End address of COMPACT model stack <0x0-0xFFFF>
;     <i> Set the top of the stack to the highest location.
PBPSTACKTOP     EQU     0xF7FF + 1;IAP15W4K58S4 ; 0x03FF +1;STC12C5A60S2    ; 将堆栈顶设置为最高地址+1//default 0FFH+1  ;by me
; </h>
;</h>
;------------------------------------------------------------------------------
;
;  使用COMPACT存储器模式时64K字节XDATA存储器空间的分页定义//Memory Page for Using the Compact Model with 64 KByte xdata RAM
;  <e>Compact Model Page Definition
;
;  <i>以下用EQU指令定义PDATA类型变量在XDATA存储器空间的页地址//Define the XDATA page used for PDATA variables. 
;  <i>使用EQU指令定义PFAGE时必须与L51连接定位器PDATA指令的控制参数一致//PPAGE must conform with the PPAGE set in the linker invocation.
;
; Enable pdata memory page initalization
PPAGEENABLE     EQU     0       ; 使用PDATA类型变量时将其设置成1//set to 1 if pdata object are used.
;
; <o> PPAGE number <0x0-0xFF> 
; <i> uppermost 256-byte address of the page used for PDATA variables.
PPAGE           EQU     0		; 定义页号
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

                NAME    ?C_STARTUP; 模块名为 ?C_STAUTUP


?C_C51STARTUP   SEGMENT   CODE		; 代码
?STACK          SEGMENT   IDATA		; 堆栈

                RSEG    ?STACK		; 堆栈
                DS      1

                EXTRN CODE (?C_START)	; 程序开始地址
                PUBLIC  ?C_STARTUP

                CSEG    AT      0		;  定义用户程序的起始地址,用MON51仿真器时可能有用
?C_STARTUP:     LJMP    STARTUP1

                RSEG    ?C_C51STARTUP

STARTUP1:

IF IDATALEN <> 0	;  单片机上电IDATA内存清零
                MOV     R0,#IDATALEN - 1
                CLR     A
IDATALOOP:      MOV     @R0,A
                DJNZ    R0,IDATALOOP
ENDIF

IF XDATALEN <> 0	;  单片机上电XDATA内存清零
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

IF PPAGEENABLE <> 0	;  送PDATA存储器页面高位地址
                MOV     PPAGE_SFR,#PPAGE
ENDIF

IF PDATALEN <> 0	;  单片机上电PDATA内存清零
                MOV     R0,#LOW (PDATASTART)
                MOV     R7,#LOW (PDATALEN)
                CLR     A
PDATALOOP:      MOVX    @R0,A
                INC     R0
                DJNZ    R7,PDATALOOP
ENDIF

IF IBPSTACK <> 0	;  设置使用SMALL存储器模式时再入函数的堆栈空间
EXTRN DATA (?C_IBP)

                MOV     ?C_IBP,#LOW IBPSTACKTOP
ENDIF

IF XBPSTACK <> 0	;  设置使用LARGE存储器模式时再入函数的堆栈空间
EXTRN DATA (?C_XBP)

                MOV     ?C_XBP,#HIGH XBPSTACKTOP
                MOV     ?C_XBP+1,#LOW XBPSTACKTOP
ENDIF

IF PBPSTACK <> 0	;  设置使用COMPACT存储器模式时再入函数的堆栈空间.
EXTRN DATA (?C_PBP)
                MOV     ?C_PBP,#LOW PBPSTACKTOP
ENDIF
;  设置堆栈的起始地址
                MOV     SP,#?STACK-1	; 例如 MOV  SP,#4FH;

; 如果你的程序使用了Mode4程序分组技术 请启动下面的程序(_(:зゝ∠)_不会吧 你的程序超过64K 利害)This code is required if you use L51_BANK.A51 with Banking Mode 4
;<h> Code Banking
; <q> Select Bank 0 for L51_BANK.A51 Mode 4
#if 0   
;     <i> Initialize bank mechanism to code bank 0 when using L51_BANK.A51 with Banking Mode 4.
EXTRN CODE (?B_SWITCH0)
                CALL    ?B_SWITCH0      ; 程序从第一组bank 0 块开始执行//init bank mechanism to code bank 0
#endif
;</h> 跳转到用户程序MAIN函数
                LJMP    ?C_START

                END
