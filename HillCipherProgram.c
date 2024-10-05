#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 10000

// reads file's key matrix
void readKey(const char* keyFile, int keyMatrix[][9], int* size) {
  FILE* file = fopen(keyFile, "r");
  if (file == NULL)
    return;
  
  fscanf(file, "%d", size);
  for (int i = 0; i < *size; i++) 
  {
    for (int j = 0; j < *size; j++) {
      fscanf(file, "%d", &keyMatrix[i][j]);
    }
  }
    
  fclose(file);
}

// reads file's plaintext
void readPlaintext(const char* plaintextFile, char* plaintext) {
  FILE* file = fopen(plaintextFile, "r");

  if (file == NULL)
    return;
  
  char ltr;
  int i = 0;
  while ((ltr = fgetc(file)) != EOF) {
    if (isalpha(ltr)) {
      plaintext[i++] = tolower(ltr);
    }
  }
  plaintext[i] = '\0';
    
  fclose(file);
}

// encrypts plaintext
void encryptPlaintext(int keyMatrix[][9], int size, char* plaintext, char* ciphertext) 
{
  int plaintext_len = strlen(plaintext);
  int block_size = size;
  int ciphertext_idx = 0;
  int block_count = (plaintext_len + block_size - 1) / block_size; 

  // pads plaintext by checking if there is a remainder and then adding x to end of string
  int remainder = plaintext_len % block_size;
  if (remainder != 0) 
  {
    int padding_len = block_size - remainder;
    for (int i = 0; i < padding_len; i++) {
      plaintext[plaintext_len + i] = 'x';
    }
    plaintext_len += padding_len;
    plaintext[plaintext_len] = '\0';
  }

  //performs hill cipher calculation
  for (int i = 0; i < block_count; i++) 
  {
    for (int j = 0; j < size; j++) {
      int sum = 0;
      for (int k = 0; k < size; k++) 
        sum += keyMatrix[j][k] * (plaintext[i * block_size + k] - 'a');
        // converts index to number letter
        ciphertext[ciphertext_idx++] = (sum % 26) + 'a';
        
      if (ciphertext_idx % 80 == 0 && ciphertext_idx < MAX_SIZE - 1)
          ciphertext[ciphertext_idx++] = '\n';
            
    }
  }
  ciphertext[ciphertext_idx] = '\0';        
}

// prints key matrix, plaintext, and ciphertext
void printOutput(int keyMatrix[][9], int size, char* plaintext, char* ciphertext)
{
    printf("Key matrix:\n");
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        printf("%d ", keyMatrix[i][j]);
      }
      printf("\n");
    }
    printf("Plaintext:\n%s\n", plaintext);
    printf("Ciphertext:\n%s\n", ciphertext);
}

int main(int argc, char* argv[]) 
{
  char plaintext[MAX_SIZE];
  char ciphertext[MAX_SIZE];
  int keyMatrix[9][9];
  int size;
    
  const char* keyFile = argv[1];
  const char* plaintextFile = argv[2];
    
  readKey(keyFile, keyMatrix, &size);
    
  readPlaintext(plaintextFile, plaintext);

  encryptPlaintext(keyMatrix, size, plaintext, ciphertext);

  printOutput(keyMatrix, size, plaintext, ciphertext);
    
  return 0;
}
