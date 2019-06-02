#include <stdio.h>

#define MAXLINE 1024

int _getline(char ln[], int ml);
int _rmtrigraphs(char in[], char out[], int in_start);

int main(void)
{
    int in_lit = 0, in_const = 0, in_com_blk = 0, in_com_ln = 0, put_space = 0;
    int i, len;
    char input[MAXLINE], line[MAXLINE];
    int stop;

    while((len = _getline(input, MAXLINE)) > 0) {
        len = _rmtrigraphs(input, line, 0);
        if(len > 2) {
            if(line[len-1] == '\n' && line[len-2] == '\\') {
                stop = 0;
                while(!stop) {
                    if((_getline(input, MAXLINE)) > 0) {
                        len = _rmtrigraphs(input, line, len - 2);
                        if(line[len-1] == '\n' && line[len-2] != '\\') {
                            stop = 1;
                        }
                    } else {
                        stop = 1;
                    }
                }
            }
        }
        i = 0;
        while(i < len) {
            if(!in_com_blk && !in_com_ln) {
                if(!in_lit && line[i] == '\'') {
                    if ((i == 0) ||
                        (i == 1 && line[i-1] != '\\') ||
                        (i > 1  && (line[i-1] != '\\' || (line[i-1] == '\\' && line[i-2] == '\\')))
                    ) {
                        in_const = !in_const;
                    }
                }
                if(!in_const && line[i] == '"') {
                    if ((i == 0) ||
                        (i == 1 && line[i-1] != '\\') ||
                        (i > 1  && (line[i-1] != '\\' || (line[i-1] == '\\' && line[i-2] == '\\')))
                    ) {
                        in_lit = !in_lit;
                    }
                }
            }
            if(!in_lit && !in_const) {
                if(!in_com_ln) {
                    if(in_com_blk == 1 && line[i] == '*' && line[i+1] == '/') {
                        in_com_blk = 0;
                        i += 2;
                    }
                    if(in_com_blk == 0 && line[i] == '/' && line[i+1] == '*') {
                        in_com_blk = 1;
                        put_space = 1;
                        i += 1;
                    }
                }
                if(!in_com_blk){
                    if(in_com_ln == 1 && line[i] == '\n') {
                        in_com_ln = 0;
                    }
                    if(in_com_ln == 0 && line[i] == '/' && line[i+1] == '/') {
                        in_com_ln = 1;
                        i += 1;
                    }
                }
            }
            if(!in_com_blk && !in_com_ln)
                putchar(line[i++]);
            else {
                if(put_space) {
                    putchar(' ');
                    put_space = 0;
                }
                ++i;
            }
        }
    }
    return 0;
}

int _getline(char ln[], int ml)
{
    int c, i;
    for(i = 0; i < ml-1 && (c = getchar()) != EOF && c != '\n'; ++i) {
        ln[i] = c;
    }
    if(c == '\n') {
        ln[i] = c;
        ++i;
    }
    ln[i] = '\0';
    return i;
}

int _rmtrigraphs(char in[], char out[], int in_start)
{
    int j = 0;
    int i = in_start;
    while(in[j] != '\0') {
        if(in[j] == '?' && in[j+1] == '?' && in[j+2] == '/') {
            out[i] = '\\';
            i += 1;
            j += 3;
        } else if(in[j] == '?' && in[j+1] == '?' && in[j+2] == '\'') {
            out[i] = '^';
            i += 1;
            j += 3;
        } else {
            out[i] = in[j];
            i += 1;
            j += 1;
        }
    }
    out[i] = '\0';
    return i;
}
