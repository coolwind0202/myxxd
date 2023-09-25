#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 1024
#define COLUMN_SIZE 16

void byte2hex(unsigned char c, char s[2])
{
  unsigned char first = c / 16, next = c % 16;

  if (first <= 9)
  {
    s[0] = '0' + first;
  }
  else
  {
    s[0] = 'A' + (first - 10);
  }

  if (next <= 9)
  {
    s[1] = '0' + next;
  }
  else
  {
    s[1] = 'A' + (next - 10);
  }
}

void print(const unsigned char s[BUF_SIZE], int length, int start)
{
  char byteBuf[2];

  for (int i = 0; i < (length / COLUMN_SIZE + (length % COLUMN_SIZE != 0)); i++)
  {
    printf("0x%08x: ", i * COLUMN_SIZE + start);
    for (int j = 0; j < COLUMN_SIZE && i * COLUMN_SIZE + j < length; j++)
    {
      byte2hex(s[i * COLUMN_SIZE + j], byteBuf);
      printf("%c%c", byteBuf[0], byteBuf[1]);
      if (j + 1 != COLUMN_SIZE)
        fputs(" ", stdout);
    }
    fputs("\n", stdout);
  }
}

int main(int argc, char *argv[])
{
  FILE *fp = NULL;
  unsigned char buf[BUF_SIZE];
  size_t read_size;
  int read_point = 0;

  if (argc < 2)
  {
    puts("Please specify a filename.");
    return EXIT_FAILURE;
  }

  fp = fopen(argv[1], "rb");

  /*
    1024バイトずつ読み出す
  */

  if (!fp)
  {
    perror("fopen");
    return EXIT_FAILURE;
  }

  while (!feof(fp))
  {
    read_size = fread(buf, 1, BUF_SIZE, fp);
    if (read_size < BUF_SIZE)
    {
      if (ferror(fp))
      {
        puts("Failed to read file.");
        return EXIT_FAILURE;
      }
    }
    print(buf, read_size, read_point);
    read_point += read_size;
  }

  puts("end of file.");
  fclose(fp);

  return 0;
}