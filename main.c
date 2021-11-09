#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <ff/ff.h>

#define D if(0)

char *argv0;

int i, x, y;
int len;
u32 w, h;
char *stdfgstr = "#000000ff" ;
char *stdbgstr = "#00000000" ;
FFPixel stdfg, stdbg;
FFPixel *outbuf;
char inbuf[512];
const char *seps = "\n\t " ;

void
usage(void)
{
	fprintf(stderr, "usage: %s <width> <height>\n", argv0);
	exit(1);
}


char *
rdline(void)
{
	char *s = fgets(inbuf, sizeof(inbuf), stdin) ;
	return s;
}

char *
strchp(char *s)
{
	if(!*s) return s ;
	s[strlen(s)-1] = 0 ;
	return s ;
}

char *
strchm(char *s, char c)
{
	if(s[strlen(s)-1] == c)
		strchp(s);
	return s ;
}

int
main(int argc, char *argv[])
{
	FFPixel *pix;
	char *tok;
	argv0 = argv[0] ;
	if(argc < 3)
		usage();

	w = atoi(argv[1]) ;
	h = atoi(argv[2]) ;

	ff_colorname_to_pixel(&stdfg, stdfgstr);
	ff_colorname_to_pixel(&stdbg, stdbgstr);

	if(!w || !h)
		usage();

	len = w*h ;
	outbuf = malloc(len * sizeof(FFPixel)) ;
	for(i = 0 ; i<len ; ++i){
		outbuf[i] = stdbg;
	}

	while(rdline()){
		if(!(tok = strtok(inbuf, seps)))
			continue ;
		x = atoi(tok) ;

		D printf("%d %d %d\n", x, y, pix) ;
		if(!(tok = strtok(0, seps)))
			continue ;
		y = atoi(tok) ;

		D printf("%d %d %d\n", x, y, pix) ;
		pix = outbuf + w*y + x ;
		if(!(tok = strtok(0, seps))){
			memcpy(pix, &stdfg, sizeof(stdfg));
		} else {
			ff_colorname_to_pixel(pix, tok);
		}
	}

	if(ff_is_little_endian){
		ff_swap_endian(&w, 4);
		ff_swap_endian(&h, 4);
		ff_swap_pixels_endian(outbuf, len);
	}

	printf("farbfeld");
	fwrite(&w, 4, 1, stdout);
	fwrite(&h, 4, 1, stdout);
	fwrite(outbuf, sizeof(*outbuf), len, stdout);

	return 0 ;
}
