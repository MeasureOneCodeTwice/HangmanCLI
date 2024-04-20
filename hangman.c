#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printOutput(char output[50][100]);
void resetOutput(char output[50][100]);
void drawBorder(char output[50][100]);
void drawGallows(char output[50][100], int paddingX, int paddingY);
void drawHangman(char output[50][100], int stage, int paddingX, int paddingY);
void drawArray(char output[50][100], char* array, int size, int startY, int startX);
void drawWord(int x, int y, char output[50][100], int guessedLetters[26], char secretWord[], int sizeOfWord);

int* encodeSecretWord(char[], int);
int getWrongGuesses(int guesses[], int encodedWord[]);
void drawGuesses(int y, int x, char output[50][100], int guessedLetters[26], int lettersPerRow);
int hasWon(int encodedArray[], int guesses[]);

char getUserLetter();

void newScreen();

int main(int argc, char** argv)
{

    if(argc < 2)
    {
        printf("Usage: %s word\n", argv[0]); 
        return 0;
    }

    const int LETTERS_IN_ALPHABET = 26;

    char *secretWord = malloc( (strlen(argv[1]) - 4 + 1) * sizeof(char) );
    secretWord[strlen(argv[1]) - 4] = '\0'; 
    for(int i = 0; i < strlen(argv[1]) - 4; i++)
    {
        secretWord[i] = argv[1][i + 2];
    }
    
    int numChars = strlen(secretWord);

    //turn secret word to an array of 1s and 0s
    int* secretArray = encodeSecretWord(secretWord, numChars);
    int guesses[LETTERS_IN_ALPHABET];
    //init guesses. 
    for(int i = 0; i < LETTERS_IN_ALPHABET; i++)
        guesses[i] = 0;

    int wrongGuesses;
    char output[50][100];
    while(1)
    {
        
        resetOutput(output);
        int guess = getUserLetter();
        //make sure no a duplicate guess.
        while(guesses['a' - guess] == 1)
        {
            printf("letter already guessed\n"); 
            guess = getUserLetter();
        }

        //write guess.
        guesses[guess - 'a'] = 1;

        wrongGuesses = getWrongGuesses(guesses, secretArray);
      
        drawHangman(output, wrongGuesses, 5, 60);
        drawWord(7, 14, output, guesses, secretWord, numChars);
        drawGuesses(7, 32, output, guesses, 6);
        
        drawBorder(output);
        drawGallows(output, 3, 60);
        char WORD[4] = {'W', 'O', 'R', 'D'};
        char GUESSES[7] = { 'G', 'U', 'E', 'S', 'S', 'E', 'S'};
        drawArray(output, WORD, 4 , 5 , 10);
        drawArray(output, GUESSES, 7 , 5 ,34); 
        printOutput(output);

        if(wrongGuesses >= 6)
        {
            newScreen();
            printf("YOU LOSE :( \nTHE WORD WAS %s\n\n\n", secretWord);
            break;
        }

        if(hasWon(secretArray, guesses))
        {
            newScreen();
            printf("YOU WON!\nTHE WORD WAS %s\n\n\n", secretWord);
            break;
        }
    }



    free(secretArray);



}

void printOutput(char output[50][100])
{
    for(int i = 0; i < 50; i++)
    {
        for(int j = 0; j < 100; j++)
        {
                printf("%c", output[i][j]);
        }
        printf("\n");
    }
}

void resetOutput(char output[50][100])
{
    for(int i = 0; i < 50; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            output[i][j] = ' ';
        }
    }

}

void drawBorder(char output[50][100])
{
    for(int i = 0; i < 50; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            output[0][j] = '-';
            output[49][j] = '-';
            output[i][0] = '|';
            output[i][99] = '|';
        }

        //pluses in corners.
        output[0][0] = '+';
        output[0][99] = '+';
        output[49][0] = '+';
        output[49][99] = '+';
    }
}

void drawGallows(char output[50][100], int paddingX, int paddingY)
{
    const int VER_PADDING = paddingX;
    const int HOR_PADDING = paddingY;

    //layer 16
    for(int i = 0; i < 19; i++)
        output[15 + VER_PADDING][i + HOR_PADDING + 15 ] = '-';
    output[15 + VER_PADDING][HOR_PADDING + 15] = '+';
    output[15 + VER_PADDING][HOR_PADDING + 15 + 19] = '+';
    //layer 15
    output[14 + VER_PADDING][15 + HOR_PADDING] = '/';
    output[14 + VER_PADDING][15 + 18 + HOR_PADDING] = '\\';
    //layer 14
    for(int i = 0; i < 9; i++)
        output[13 +VER_PADDING][16 + i + HOR_PADDING] = '_';
    output[13 + VER_PADDING][25 + HOR_PADDING] = '['; 
    output[13 + VER_PADDING][32 + HOR_PADDING] = ']'; 
    //layer 13
    output[12 + VER_PADDING][26 + HOR_PADDING] = '/';
    output[12 + VER_PADDING][31 + HOR_PADDING] = '\\';
    //layers 12 - 7 
    for(int i = 0; i < 6; i++)
    {
        output[11 - i + VER_PADDING][27 + HOR_PADDING] = '|';
        output[11 - i + VER_PADDING][30 + HOR_PADDING] = '|';
    }
    //layer 6 
    output[5 + VER_PADDING][27 + HOR_PADDING] = '\\';
    output[5 + VER_PADDING][30 + HOR_PADDING] = '|';
    //layer 5
    output[4 + VER_PADDING][26 + HOR_PADDING] = '|';
    output[4 + VER_PADDING][30 + HOR_PADDING] = '|';
    //layer 4
    output[3 + VER_PADDING][25 + HOR_PADDING] = '\\';
    output[3 + VER_PADDING][30 + HOR_PADDING] = '\\';
    //layer 3
    output[2 + VER_PADDING][4 + HOR_PADDING] = '|';
    output[2 + VER_PADDING][24 + HOR_PADDING] = '|';
    output[2 + VER_PADDING][25 + HOR_PADDING] = '|';
    output[2 + VER_PADDING][29 + HOR_PADDING] = ']';
    //layer 2
    output[1 + VER_PADDING][4 + HOR_PADDING] = '|';
    for(int i = 0; i < 19; i++)
        output[1 + VER_PADDING][5 + i + HOR_PADDING] = '-';
    output[1 + VER_PADDING][24 + HOR_PADDING] = '+';
    output[1 + VER_PADDING][29 + HOR_PADDING] = '|';
    //layer 1
    output[0 + VER_PADDING][4 + HOR_PADDING] = '|';
    for(int i = 0; i < 24; i++)
    {
        output[0 + VER_PADDING][5 + i + HOR_PADDING] = '-';
    }
    output[0 + VER_PADDING][29 + HOR_PADDING] = '+';
}

void drawHangman(char output[50][100], int stage, int paddingX, int paddingY)
{
    int VER_PADDING = paddingX;
    int HOR_PADDING = paddingY;
    if(stage >= 1)
    {
        output[VER_PADDING][4 + HOR_PADDING] = '|';
        output[1 + VER_PADDING][4 + HOR_PADDING] = '|';
        output[1 + VER_PADDING][3 + HOR_PADDING] = '(';
        output[1 + VER_PADDING][6 + HOR_PADDING] = ')';
        output[2 + VER_PADDING][3 + HOR_PADDING] = '-';
        output[2 + VER_PADDING][4 + HOR_PADDING] = '-';
        output[2 + VER_PADDING][5 + HOR_PADDING] = '-';
    }
    if(stage >= 2)
    {
        output[3 + VER_PADDING][4+ HOR_PADDING] = '|';
        output[3 + VER_PADDING][6+ HOR_PADDING] = '|';
        output[4 + VER_PADDING][4+ HOR_PADDING] = '|';
        output[4 + VER_PADDING][6+ HOR_PADDING] = '|';
        output[5 + VER_PADDING][5+ HOR_PADDING] = '-';
    }
    if(stage >= 3)
    {
        output[3 + VER_PADDING][2+ HOR_PADDING] = '/';
        output[4 + VER_PADDING][1+ HOR_PADDING] = '/';
        output[5 + VER_PADDING][1+ HOR_PADDING] = '*';
}
    if(stage >= 4)
    {
        output[3 + VER_PADDING][7+ HOR_PADDING] = '\\';
        output[4 + VER_PADDING][8+ HOR_PADDING] = '\\';
        output[5 + VER_PADDING][9+ HOR_PADDING] = '*';
    }
    if(stage >= 5)
    {
        output[5 + VER_PADDING][4+ HOR_PADDING] = '/';
        output[6 + VER_PADDING][3+ HOR_PADDING] = '/';
        output[7 + VER_PADDING][2+ HOR_PADDING] = '/';
        output[8 + VER_PADDING][1+ HOR_PADDING] = 'd';
    }
    if(stage >= 6)
    {
        output[5 + VER_PADDING][6+ HOR_PADDING] = '\\';
        output[6 + VER_PADDING][7+ HOR_PADDING] = '\\';
        output[7 + VER_PADDING][8+ HOR_PADDING] = '\\';
        output[8 + VER_PADDING][9+ HOR_PADDING] = 'b';
    }
}

void drawArray(char output[50][100], char* array, int size, int startY, int startX)
{
    for(int i = 0; i < size; i++)
    {
        output[startY][startX + i] = *(array + i);
    }
}


int* encodeSecretWord(char secretWord[], int wordLength)
{
    const int LETTERS_IN_ALPHABET = 26;
    int* encodedSecretWord = (int*)malloc(sizeof(int) * LETTERS_IN_ALPHABET);

    for(int i = 0; i < wordLength; i++)
    {
        encodedSecretWord[i] = 0;
    }

    for(int i = 0; i < wordLength; i++)
    {
        *(encodedSecretWord + (secretWord[i] - 'a')) = 1;
    }
    return encodedSecretWord;
}


char getUserLetter()
{
    const int TO_LOWER = 32;
    char letter = -1;

    while(!(letter >= 'a' && letter <= 'z'))
    {
        if(letter == -1)
        {
            printf("Guess a letter: \n");
        }

        scanf("%c", &letter);

        if(letter <= 'Z')
            letter += TO_LOWER;

    }
    return letter;
}

void drawWord(int y, int x, char output[50][100], int guessedLetters[26], char secretWord[], int sizeOfWord)
{
    for(int i = 0; i < sizeOfWord; i++)
    {
        if(guessedLetters[secretWord[i] - 'a'] == 1)
            output[y][x + i * 2 - sizeOfWord + 1] = secretWord[i];
        else
            output[y][x + i * 2 - sizeOfWord + 1] = '?';

    }
}

void drawGuesses(int y, int x, char output[50][100], int guessedLetters[26], int lettersPerRow)
{
    int rowNum = y;
    int colNum = 0;
    for(int i = 0; i < 26; i++)
    {
        if(guessedLetters[i] == 1)
        {
            output[rowNum][x + colNum * 2] = 'a' + i;
            colNum++; 
            if(colNum == lettersPerRow)
            {
                colNum = 0;
                rowNum += 2;
            }
        }
    }
}

int getWrongGuesses(int guesses[], int encodedWord[])
{
    int wrongGuesses = 0;
    for(int i = 0; i < 26; i++)
    {
        if(guesses[i] == 1 && encodedWord[i] == 0)
        {
            wrongGuesses++;
        }

    }
    return wrongGuesses;
}

int hasWon(int encodedArray[], int guesses[])
{
    for(int i = 0; i < 26; i++)
    {
        if(encodedArray[i] == 1 &&  guesses[i] == 0)
            return 0;
    }
    return 1;

}

void newScreen()
{
    for(int i = 0; i < 70; i++) { printf("\n"); }
}
