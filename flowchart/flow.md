
# UART FLOW CHART



~~~mermaid
graph TD
k64[[ \n\n\n MK64FN1LL12 Microcontroller \n\n\n\n ]]
	l1(Logic level converter \n3.3v <> 5v )--PTB16_RxD---> k64
	k64 --PTB17_TxD------->l2(Logic level converter \n3.3v <> 5v )
	k64 --PTD4_RTS-------> l3(Logic level converter \n3.3v <> 5v )
	l4(Logic level converter \n3.3v <> 5v )--PTD5_CTS---> k64  
	
	ftdi( \n\n USB to UART converter \n\n\n )
	ftdi --> usb_con((USB CONNECTOR))

	ftdi --TxD---->l1
	ftdi --RTS---->l4
	l2--RxD---->ftdi 
	l3--CTS---->ftdi 
	
	usb_con --usb_cable---> host[[ \n\n\n\n   HOST       PC   \n\n\n\n\n ]]
	host --virtual_COMport-->  mini[( minicom )]
	

	info([ \n\n FTDI adapter 5v logic level is converted to 3.3v logic to connect k64 \n CTS and RTS pins are only used when Hardware Flow control enabled \n UART0 is used \n\n ])
~~~
