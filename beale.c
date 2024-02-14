#include "beale.h"
#include "rothelper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SENTINEL (0)
#define BUF_SIZE 255

int count_words(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Failed to open file for reading in count_words().");
    exit(1);
  }

  char *format_string = malloc(sizeof(char) * 6);
  if (format_string == NULL) {
    perror("Failed to allocate memory for new format string in count_words().");
    free(format_string);
    exit(1);
  }
  // create a format string to limit the size written into buf
  sprintf(format_string, "%%%ds", BUF_SIZE - 1);

  char buf[BUF_SIZE];
  int c = fscanf(fp, format_string, buf);
  int count = 0;
  while (c != EOF) {
    count++;
    c = fscanf(fp, format_string, buf);
  }

  free(format_string);
  fclose(fp);
  return count;
}

char *first_letters(char *filename) {
  char buf[BUF_SIZE];

  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Failed to open file for reading in first_letters().");
    exit(1);
  }

  int len = count_words(filename) + 1;
  char *letters = (char *)malloc(sizeof(char) * len);
  if (letters == NULL) {
    perror("Failed to allocate memory for letter array in first_letters()!");
    exit(1);
  }

  char *format_string = malloc(sizeof(char) * 6);
  if (format_string == NULL) {
    perror(
        "Failed to allocate memory for new format string in first_letters().");
    free(letters);
    exit(1);
  }
  // create a format string to limit the size written into buf
  sprintf(format_string, "%%%ds", BUF_SIZE - 1);

  int c = fscanf(fp, format_string, buf);
  int index = 1;
  while (c != EOF) {
    letters[index] = buf[0];
    c = fscanf(fp, format_string, buf);
    index++;
  }
  letters[index] = '\0';

  free(format_string);
  fclose(fp);
  return letters;
}

int *encipher(char *str, Beale b) {
  int len = stringlength(str);

  int *cipher = (int *)malloc(sizeof(int) * len + 1);
  if (cipher == NULL) {
    perror("Failed to allocate memory for enciphered text.");
    exit(1);
  }

  // initialize the cipher to -1
  for (int i = 0; i < len + 1; i++) {
    cipher[i] = -1;
  }

  // construct the cipher text
  int j;
  int i;
  for (i = 0; i < len; i++) {
    for (j = 1; j < b.length; j++) {
      if (b.letters[j] == str[i]) {
        cipher[i] = j;
        break;
      }
    }
  }
  // set a sentinel value
  cipher[i] = SENTINEL;

  return cipher;
}

char *decipher(int *msg, Beale b) {
  // get the length of the message
  int len = 0;
  for (int k = 0; msg[k] != SENTINEL; k++) {
    len++;
  }

  char *deciphered_text = (char *)malloc(sizeof(char) * len);
  if (deciphered_text == NULL) {
    perror("Failed to allocate memory for deciphered text.");
    exit(1);
  }

  // decipher the message
  for (int i = 0; i < len; i++) {
    int index = msg[i];
    if (index != -1) {
      deciphered_text[i] = b.letters[index];
    } else {
      deciphered_text[i] = '*';
    }
  }
  deciphered_text[len] = '\0';

  return deciphered_text;
}

Beale create_cipher(char filename[]) {
  if (access(filename, R_OK) != F_OK) {
    perror("Unable to read file while creating cipher!");
    exit(1);
  }

  char *letters = first_letters(filename);
  int count = count_words(filename);

  Beale b;
  b.length = count;
  b.letters = letters;
  b.letters[0] = -1;

  return b;
}
