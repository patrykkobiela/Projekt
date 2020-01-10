#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <termio.h>
#include <time.h>



void title(void)   
{
      printf("\t*******************************************************\n");
      printf("\t*\x1b[40m\x1b[32m#     #     #  #   # #   #  ####  #     ####      # \x1b[0m *\n");
      printf("\t*\x1b[40m\x1b[32m#    # #    #  #  #      #  #     #     #      #    \x1b[0m *\n");
      printf("\t*\x1b[40m\x1b[32m #   # #   #   #   # #   #  ####  #     ####  #     \x1b[0m *\n");
      printf("\t*\x1b[40m\x1b[32m #  #   #  #   #      #  #  #     #     #      #    \x1b[0m *\n");
      printf("\t*\x1b[40m\x1b[32m  #      #     #   # #   #  ####  ####  ####      # \x1b[0m *\n");
      printf("\t*******************************************************\n\n");
}


static struct termios stary, nowy;

void initTermios(int echo) { 
    tcgetattr(0, &stary);
    nowy = stary;
    nowy.c_lflag &= ~ICANON;
    nowy.c_lflag &= echo ? ECHO : ~ECHO;
    tcsetattr(0, TCSANOW, &nowy);
}

void resetTermios(void) { tcsetattr(0, TCSANOW, &stary); }

char getch_(int echo) {
    initTermios(echo);
    char ch = getchar();
    resetTermios();
    return ch;
}

char getch(void) { return getch_(0); }
char getche(void) {	return getch_(1); }

typedef struct {int wg; char ws[7]; } Game;

typedef struct { size_t word, words, *index; char *buffer; } Buffer;
Buffer readwords(const char *fn){
    char buffer[512];
    char *bp, *wi, *wl;
    size_t size, elements, bytes, wlen=0, ilen, len;
    FILE *stream, *tstream;
    FILE *fp=fopen(fn, "r");
    stream=open_memstream(&bp, &size);
    do{
        bytes = fread(buffer, sizeof(char), sizeof buffer, fp);
        fwrite(buffer, sizeof(char), bytes, stream);
    } while (bytes == sizeof buffer && !feof(fp));
        fflush(stream); fclose(stream);
        stream=open_memstream(&wi, &ilen);
        tstream=open_memstream(&wl, &bytes);
            for(char *tok=strtok(bp, "\n"); tok; tok=strtok(NULL, "\n")){
                len=strlen(tok);
                if(len<3 || len>50) continue;
                for(char *p=tok; *p; ++p){
                    if(isalpha(*p)) *p=tolower(*p);
                    else goto next;
                }
                fwrite(&wlen, sizeof wlen, 1, stream);
                wlen+=fprintf(tstream, "%s", tok);
                fputc(0, tstream);
                wlen++;
                next:;
            }
    fflush(tstream); fclose(tstream);
    fflush(stream); fclose(stream);
    free(bp);
    return (Buffer){.words=ilen/sizeof ilen, .index=(size_t *)wi, .buffer=wl};
}


