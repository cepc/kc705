# Nmake macros for building Windows 32-Bit apps

!include <Win32.Mak>

all: $(OUTDIR) $(OUTDIR)\streamread.exe $(OUTDIR)\streamwrite.exe $(OUTDIR)\memread.exe $(OUTDIR)\memwrite.exe $(OUTDIR)\winstreamread.exe $(OUTDIR)\fifo.exe

#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

$(OUTDIR)\streamread.obj: streamread.c
    $(cc) -D_CRT_SECURE_NO_WARNINGS $(cflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" streamread.c

$(OUTDIR)\streamread.exe: $(OUTDIR)\streamread.obj
    $(link) $(conflags) -out:$(OUTDIR)\streamread.exe $(OUTDIR)\streamread.obj $(conlibs)

$(OUTDIR)\streamwrite.obj: streamwrite.c
    $(cc) -D_CRT_SECURE_NO_WARNINGS $(cflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" streamwrite.c

$(OUTDIR)\streamwrite.exe: $(OUTDIR)\streamwrite.obj
    $(link) $(conflags) -out:$(OUTDIR)\streamwrite.exe $(OUTDIR)\streamwrite.obj $(conlibs)

$(OUTDIR)\memread.obj: memread.c
    $(cc) -D_CRT_SECURE_NO_WARNINGS $(cflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" memread.c

$(OUTDIR)\memread.exe: $(OUTDIR)\memread.obj
    $(link) $(conflags) -out:$(OUTDIR)\memread.exe $(OUTDIR)\memread.obj $(conlibs)

$(OUTDIR)\memwrite.obj: memwrite.c
    $(cc) -D_CRT_SECURE_NO_WARNINGS $(cflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" memwrite.c

$(OUTDIR)\memwrite.exe: $(OUTDIR)\memwrite.obj
    $(link) $(conflags) -out:$(OUTDIR)\memwrite.exe $(OUTDIR)\memwrite.obj $(conlibs)

$(OUTDIR)\winstreamread.obj: winstreamread.c
    $(cc) -D_CRT_SECURE_NO_WARNINGS $(cflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" winstreamread.c

$(OUTDIR)\winstreamread.exe: $(OUTDIR)\winstreamread.obj
    $(link) $(conflags) -out:$(OUTDIR)\winstreamread.exe $(OUTDIR)\winstreamread.obj $(conlibs)

$(OUTDIR)\fifo.obj: fifo.c
    $(cc) -D_CRT_SECURE_NO_WARNINGS $(cflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" fifo.c

$(OUTDIR)\fifo.exe: $(OUTDIR)\fifo.obj
    $(link) $(conflags) -out:$(OUTDIR)\fifo.exe $(OUTDIR)\fifo.obj $(conlibs)

#--------------------- Clean Rule --------------------------------------------------------
# Rules for cleaning out those old files
clean:
        $(CLEANUP)
