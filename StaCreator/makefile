#program name
prog =StaCreator.exe

#directories
srcDir =src
incDir =inc
bldDir =bld
binDir =bin

cc =g++
incl = -I$(incDir)

classes =$(shell find $(srcDir) -name "*.cpp")
objects =$(patsubst $(srcDir)/%, $(bldDir)/%, $(classes:.cpp=.o))

all:	directories $(prog)

directories:
	@mkdir -p $(binDir)
	@mkdir -p $(bldDir)

$(prog): $(objects)
	$(cc) -o $(binDir)/$@ $^

$(bldDir)/%.o:	$(srcDir)/%.cpp
	$(cc) -o $@ -c $< $(incl)

clean:
	@rm -rf $(bldDir)

purge:	clean
	@rm -rf $(binDir)
