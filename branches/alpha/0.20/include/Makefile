all:	CoreGraphics CarbonCore/MachineExceptions.h CarbonCore/fp.h

CoreGraphics:
	cp -r /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers CoreGraphics

CarbonCore/%.h: CarbonCore/%.h.diff
	cp /System/Library/Frameworks/CoreServices.framework/Frameworks/CarbonCore.framework/Headers/$*.h CarbonCore/$*.h
	cd CarbonCore && patch -p0 < $*.h.diff 

clean:
	rm -fr CoreGraphics
	rm -f CarbonCore/MachineExceptions.h
	rm -f CarbonCore/fp.h

install:
	install -d -v architecture/arm /usr/include/architecture/arm
	install -b -v architecture/arm/*.h /usr/include/architecture/arm
	install -d -v arm /usr/include/arm
	install -b -v arm/*.h /usr/include/arm
	install -b -v fenv.h math.h TargetConditionals.h /usr/include
	install -b -v machine/*.h /usr/include/machine
	install -b -v mach/machine/*.h /usr/include/mach/machine
	install -b -v sys/cdefs.h /usr/include/sys

