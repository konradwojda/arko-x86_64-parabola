# arko-x86_64-parabola
Drawing a quadratic function, approximated by segments of equal length.  
Params: a,b,c - coefficients of quadratic function  
S - length of approximation segment  
To get a ∆x I move along the tangent to the parabola using the formula ∆x = S * cos(atan(f'(x)))  
# Sources
* http://www.ue.eti.pg.gda.pl/fpgalab/zadania.spartan3/zad_vga_struktura_pliku_bmp_en.html
* https://en.wikipedia.org/wiki/Streaming_SIMD_Extensions
* https://en.wikibooks.org/wiki/X86_Assembly/SSE
* https://www.nasm.us/doc/nasmdo12.html#section-12.3
