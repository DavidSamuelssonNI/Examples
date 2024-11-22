
LD = link 
CC = cl
CFLAGS = /c /I. /I..\..\..
LFLAGS = /DLL /INCREMENTAL:NO
CFLAGS = $(CFLAGS) /MD

EXPORTS : delay.dll
		
delay.dll : delay.obj ni_modelframework.obj
	$(LD) $(LFLAGS) $** 
	@if exist $@.manifest mt.exe -manifest $@.manifest -outputresource:$@;2

delay.obj : delay.c
	$(CC) $(CFLAGS) $**

ni_modelframework.obj : ..\..\src\ni_modelframework.c
	$(CC) $(CFLAGS) $**
	
clean :
	del *.dll *.obj *.lib *.exp *.manifest