Different linkers use different options to generate map file and specify its name.
For this for each compiler distinct set of options is to be specified

### AppleClang

    -Wl, -map, map-file-name

Note—if the linker is being invoked indirectly, via a compiler driver (e.g.  gcc) then all the linker command line options should be prefixed by  -Wl,  (or whatever is appropriate for the particular compiler driver) like this:

       gcc -Wl,--startgroup foo.o bar.o -Wl,--endgroup