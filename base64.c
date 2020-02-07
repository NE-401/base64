// command line base64 encoder/decoder

// header file
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// character of base64
const char frm_b64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// output help
void print_help(void) {
	fprintf(stderr,"base64 encoder/decoder\n");
	fprintf(stderr,"format ->\n");
	fprintf(stderr,"base64 -e {string}\n");
	fprintf(stderr,"base64 --encode {string}\n");
	fprintf(stderr,"base64 -d {base64 string}\n");
	fprintf(stderr,"base64 --decode {base64 string}\n");
	fprintf(stderr,"-e/--encode : string to base64\n");
	fprintf(stderr,"-d/--decode : base64 to string\n");
}

// get index of character
char get_b64idx(const char c) {
	int i;
	if(c=='=') return 64;
	for(i=0;i<64;i++) {
		if(c==frm_b64[i]) return i;
	}
	return -1;
}

// encode : string to base64
void encode(const char *s,int len) {
	int clen=len;
	int i;
	unsigned char b[4]={0};
	while(1) {
		for(i=0;i<4;i++) b[i]&=0x00;
		if(clen>=3) {
			b[0]|=(0xFC&s[len-clen])>>2;	// 1Œê–Ú‚Ì7~2bit
			b[1]|=(0x03&s[len-clen])<<4;	// 1Œê–Ú‚Ì1~0bit
			b[1]|=(0xF0&s[len-clen+1])>>4;	// 2Œê–Ú‚Ì7~4bit
			b[2]|=(0x0F&s[len-clen+1])<<2;	// 2Œê–Ú‚Ì3~0bit
			b[2]|=(0xC0&s[len-clen+2])>>6;	// 3Œê–Ú‚Ì7~6bit
			b[3]|=(0x3F&s[len-clen+2]);	// 3Œê–Ú‚Ì5~0bit
			printf("%c%c%c%c",frm_b64[b[0]],frm_b64[b[1]],frm_b64[b[2]],frm_b64[b[3]]);
			clen-=3;
		}
		else if(clen==2) {
			b[0]|=(0xFC&s[len-clen])>>2;
			b[1]|=(0x03&s[len-clen])<<4;
			b[1]|=(0xF0&s[len-clen+1])>>4;
			b[2]|=(0x0F&s[len-clen+1])<<2;
			printf("%c%c%c%c",frm_b64[b[0]],frm_b64[b[1]],frm_b64[b[2]],'=');
			break;
		}
		else if(clen==1) {
			b[0]|=(0xFC&s[len-clen])>>2;
			b[1]|=(0x03&s[len-clen])<<4;
			printf("%c%c%c%c",frm_b64[b[0]],frm_b64[b[1]],'=','=');
			break;
		}
		else break;
	}
	putchar('\n');
}

// decode : base64 to string
void decode(const char *s,int len) {
	int clen=len,i;
	unsigned char b[3];
	if(clen%4) {
		fprintf(stderr,"\aIllegal string!\n");
		return;
	}
	for(i=0;i<clen;i++) {
		if(get_b64idx(s[i])==-1) {
			fprintf(stderr,"\aIllegal character!\n");
			return;
		}
	}
	while(clen>0) {
		for(i=0;i<4;i++) b[i]&=0x00;
		// character to index(0~64)
		b[0]|=(0x3F&get_b64idx(s[len-clen]))<<2;
		b[0]|=(0x30&get_b64idx(s[len-clen+1]))>>4;
		b[1]|=(0x0F&get_b64idx(s[len-clen+1]))<<4;
		b[1]|=(0x3C&get_b64idx(s[len-clen+2]))>>2;
		b[2]|=(0x03&get_b64idx(s[len-clen+2]))<<6;
		b[2]|=(0x3F&get_b64idx(s[len-clen+3]));
		printf("%c%c%c",b[0],b[1],b[2]);
		clen-=4;
	}
	putchar('\n');
}

// main function
main(int argc,char* argv[]) {
	if(argc==2&&(!strcmp(argv[1],"-h")||!strcmp(argv[1],"--help"))) {
		print_help();
	}
	else if(argc==3&&(!strcmp(argv[1],"-e")||!strcmp(argv[1],"--encode"))) {
		encode(argv[2],strlen(argv[2]));
	}
	else if(argc==3&&(!strcmp(argv[1],"-d")||!strcmp(argv[1],"--decode"))) {
		decode(argv[2],strlen(argv[2]));
	}
	else {
		fprintf(stderr,"\asyntax error!\n");
		print_help();
	}
	return 0;
}
