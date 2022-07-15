## Automation of building executable using Nmake with the MSVC compiler [v2.1] by Oscar Milstein.

## Path variables.
INCLUDES = includes\\
LIBPATH = lib\\
BUILD = build\\
BIN = bin\\
DEBUG = debug\\
EXECUTABLE = $(BIN)GMTK.exe
SOURCE = source\\

## Compile command variables.
INCLUDEPATHS = -I$(INCLUDES) -I$(INCLUDES)glad -I$(INCLUDES)KHR -I$(INCLUDES)sdl -I$(INCLUDES)stb -Icglm\\include
CFLAGS =-c -permissive- -std:c17 -TC -MP -W4 -nologo $(INCLUDEPATHS) -Fd$(DEBUG) -D"_CRT_SECURE_NO_WARNINGS" -D"HAVE_M_PI"
LFLAGS =-nologo -LIBPATH:$(LIBPATH) $(LIBS) 
LIBS = raylibdll.lib

debug: $(EXECUTABLE)

$(EXECUTABLE): $(BUILD)*.obj
   LINK $(LFLAGS) -DEBUG:FASTLINK  $** -OUT:$@

{$(SOURCE)}.c{$(BUILD)}.obj::
   $(CC) $(CFLAGS) -Zi -MTd -Od -FC -D"DEBUG" $< -Fo:$(BUILD)

## Builds an optimized release build of the entire project in parallel using max avaliable cores on the system.
release:
   $(CC) $(CFLAGS) -MT -O2 -favor:blend -Qfast_transcendentals -fp:fast -D"NDEBUG" $(SOURCE)*.c -Fo:$(BUILD)
   LINK $(LFLAGS) $(BUILD)*.obj -OUT:$(EXECUTABLE)
   @echo Build complete!

## Cleans the project folder.
clean:
   -del $(BIN)*.ilk $(BIN)*.pdb $(BIN)*.exe $(BUILD)*.obj $(DEBUG)*.pdb
   @echo Clean complete!

rebuild:
   nmake clean -nologo
   nmake -nologo

init:
   mkdir bin build debug includes lib source
