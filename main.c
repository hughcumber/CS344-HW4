#include <stdio.h>
#include <pthread.h>          //  included for pthread
#include <stdlib.h>
#include <string.h>           //  included for string functionaity
#include <stdbool.h>          //  included for bool functionality


#define MAX_LINES 50                                                            //  max amount of lines that will be read in ie size of buffers
#define MAX_LINE_LEN 1000                                                       //  max amount of chars per line


pthread_mutex_t buff_1 = PTHREAD_MUTEX_INITIALIZER;                             //  Initialize buffers for proucer/consumer method of multithreaded prog
pthread_mutex_t buff_2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t buff_3 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond_1 = PTHREAD_COND_INITIALIZER;                               // Initialize the condition variables for buffers
pthread_cond_t cond_2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_3 = PTHREAD_COND_INITIALIZER;


int buff_1_idx = 0;                                                             //  Initialize index counters for buffers
int buff_2_idx = 0;
int buff_3_idx = 0;
int buff_1_get = 0;                                                             //  Initialize index counters for buffers
int buff_2_get = 0;
int buff_3_get = 0;

int buff_1_count = 0;
int buff_2_count = 0;
int buff_3_count = 0;

bool stpline = false;
int linesread = 0;
int thread_tracker = 0;
char buffer_1[MAX_LINES][MAX_LINE_LEN] = {{0}};
char buffer_2[MAX_LINES][MAX_LINE_LEN] = {{0}};
char buffer_3[MAX_LINES][MAX_LINE_LEN] = {{0}};

char print_buffer[13][80] = {{0}};

void *thread_1(void* args);
void *thread_2(void* args);
void *thread_3(void* args);
void *thread_4(void* args);

char* newl_to_space(char* input);                                               //  takes in a string, replaces newline chars with spaces
char* plus_to_carrot(char* input);                                              //  takes in a string, changes out ++ for ^
void put_buff_1(char* line);
char* get_buff_1();
void put_buff_2(char* line);
char* get_buff_2();
void print_buffers();
char* string_chopper(int n, char* input);

int main(){

  pthread_t t1, t2, t3, t4;
  // Create the threads
  pthread_create(&t1, NULL, thread_1, NULL);
  pthread_create(&t2, NULL, thread_2, NULL);
  pthread_create(&t3, NULL, thread_3, NULL);
  pthread_create(&t4, NULL, thread_4, NULL);

  // Wait for the threads to terminate
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  pthread_join(t4, NULL);


  // print_buffers();

  return 0;
}



// ********************************    BUFFER 1  *************************
// ***********************************************************************
// Function: put_buff_1
// Purpose: puts string in buff1
// input: string
// output: NA
// ***********************************************************************
void put_buff_1(char* line){

  pthread_mutex_lock(&buff_1);                                                  //  locking mutex for data entry
  strcpy(buffer_1[buff_1_idx],line);                                            //  copies input to index of buffer_1
  buff_1_idx++;
  buff_1_count++;
  pthread_cond_signal(&cond_1);

  pthread_mutex_unlock(&buff_1);

}

// ***********************************************************************
// Function: get_buff_1
// Purpose: gets string from buff1
// input: string
// output: NA
// ***********************************************************************
char* get_buff_1(){


  pthread_mutex_lock(&buff_1);                                                  // Lock the mutex before checking if the buffer has data
  while (buff_1_count == 0){                                                    // waits until buffer has data to pull data
    pthread_cond_wait(&cond_1, &buff_1);
  }

  buff_1_get++;                                                                 // Increment the index from which the string will be gotten from buffer
  buff_1_count--;                                                               // decrement the count of items in the buffer

  pthread_mutex_unlock(&buff_1);                                                //  unlock mutex for next thread_1() call

  return buffer_1[buff_1_get - 1];                                              //  returns next string from buffer

}


// ********************************    BUFFER 2  *************************
// ***********************************************************************
// Function: put_buff_2
// Purpose: puts string in buff2
// input: string
// output: NA
// ***********************************************************************
void put_buff_2(char* line){

  strcpy(buffer_2[buff_2_idx],line);                                            //  copies input to index of buffer_2

  buff_2_idx++;
  buff_2_count++;
  pthread_cond_signal(&cond_2);

  pthread_mutex_unlock(&buff_2);

}

// ***********************************************************************
// Function: get_buff_2
// Purpose: gets string from buff2
// input: string
// output: NA
// ***********************************************************************
char* get_buff_2(){

  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&buff_2);
  while (buff_2_count == 0){                                                    // waits until buffer has data to pull data
    pthread_cond_wait(&cond_2, &buff_2);
  }

  buff_2_get++;                                                                 // Increment the index from which the string will be gotten from buffer
  buff_2_count--;                                                               // decrement the count of items in the buffer

  pthread_mutex_unlock(&buff_2);                                                //  unlock mutex for next thread_1() call

  return buffer_2[buff_2_get - 1];                                              //  returns next string from buffer

}


// ********************************    BUFFER 3  *************************
// ***********************************************************************
// Function: put_buff_3
// Purpose: puts string in buff3
// input: string
// output: NA
// ***********************************************************************
void put_buff_3(char* line){

  pthread_mutex_lock(&buff_3);                                                  //  locking mutex for data entry
  strcpy(buffer_3[buff_3_idx],line);                                            //  copies input to index of buffer_3
  buff_3_count++;
  buff_3_idx++;
  pthread_cond_signal(&cond_3);

  pthread_mutex_unlock(&buff_3);

}


// ***********************************************************************
// Function: get_buff_3
// Purpose: gets string from buff3
// input: string
// output: NA
// ***********************************************************************
char* get_buff_3(){

  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&buff_3);
  while (buff_3_count == 0){                                                    // waits until buffer has data to pull data
    pthread_cond_wait(&cond_3, &buff_3);
  }

  buff_3_get++;                                                                 // Increment the index from which the string will be gotten from buffer
  buff_3_count--;                                                               // decrement the count of items in the buffer

  pthread_mutex_unlock(&buff_3);                                                //  unlock mutex for next thread_2() call

  return buffer_3[buff_3_get - 1];                                                  //  returns next string from buffer

}




// ***********************************************************************
// Function: thread_1
// Purpose: gets input line from stdin, puts in buff1
// input: args
// output: NA
// ***********************************************************************
void *thread_1(void* args){

  int k = 0;
  for(k = 0; k < 50; k++){
  // while (stpline == false){
    char line[1000];
    fgets(line, 1000, stdin);


    if(strcmp(line, "STOP\n") == 0){
      put_buff_1(line);
      return NULL;
    }

    put_buff_1(line);

  }
  // printf(" THREAD 1 returning \n");
  // fflush(stdout);

  return NULL;
}


// ***********************************************************************
// Function: thread_2
// Purpose: gets input line buff1, changes newline to space, puts in buff2
// input: args
// output: NA
// ***********************************************************************
void *thread_2(void* args){

  int k = 0;
  for(k = 0; k < 50; k++){
  // while (stpline == false){
    char input[1000];                                                         //  string that will be put in buffer 2
    char* output;
    strcpy(input, get_buff_1());                                                  //  copying input from buff1 to string

    newl_to_space(input);

    // printf(" buff2 --> |%s|\n", input);
    put_buff_2(input);                                                            //  putting output into buff2
    // printf(" t2 got : %s\n", input);

    if(strcmp(input, "STOP ") == 0){
      break;
    }
  }

  // printf(" THREAD 2 returning \n");
  fflush(stdout);


  return NULL;

}


// ***********************************************************************
// Function: thread_3
// Purpose: gets input line buff2, changes ++ to ^ and puts line in buff3
// input: args
// output: NA
// ***********************************************************************
void *thread_3(void* args){

  int k = 0;
  for(k = 0; k < 50; k++){
  // while (stpline == false){
    char input[1000];                                                             //  string that will be put in buffer 3
    strcpy(input, get_buff_2());                                                  //  copying input from buff2 to string

    plus_to_carrot(input);                                                        //  ++ to ^

    put_buff_3(input);                                                            //  putting output into buff3
    void print_buffers();
    if(strcmp(input, "STOP ") == 0){
      break;
    }
  }

  // printf(" THREAD 3 returning \n");
  fflush(stdout);


  return NULL;

}

// ***********************************************************************
// Function: thread_4
// Purpose: gets input line buff3, prints to stdout when 80 chars are reached
// input: args
// output: NA
// ***********************************************************************
void *thread_4(void* args){                                                     //  output thread that prints the output


  char output[81] = {0};
  char* output_buff = (char*)malloc(MAX_LINES * MAX_LINE_LEN * sizeof(char));
  int charcount = 0;
  int linecount = 0;
  int last_idx = 0;
  char input[1000];


  int i, k = 0;
  // while (stpline == false){
  for(k = 0; k < 49; k++){

    strcpy(input, get_buff_3());                                                // copy buffer to input string

    // printf(" t4: got |%s| \n", input);
    if(strcmp(input, "STOP ") == 0){
      // printf(" STOPLINE REACHED\n");
      fflush(stdout);
      break;
      // stpline = true;
      // exit(0);
    }

    strcat(output_buff, input);                                                 // concat string to output_buff

    charcount = strlen(output_buff);                                            //  change charcount to len(output_buff) to see if we need to print more lines

    linecount = charcount / 80;

    int j = 0;
    for(i = last_idx; i < linecount; i++){
      memset(input, 0, sizeof(char) * 80);
      // strncpy(print_buffer[j], output_buff + (i * 80), 80);
      strcpy(print_buffer[j], string_chopper((i * 80), output_buff));
      printf("%s\n", print_buffer[j]);
      fflush(stdout);
      j++;
    }

    last_idx = linecount;



    fflush(stdout);

    if(stpline == true){
      break;
    }


  }

  // printf(" THREAD 4 returning \n");
  fflush(stdout);

  free(output_buff);

  return NULL;

}

// ***********************************************************************
// Function: string_chopper
// Purpose: formats a string to 80 chars
// input: int indice, string input
// output: formatted string
// ***********************************************************************
char* string_chopper(int n, char* input){



  char * output = (char*)malloc(80 * sizeof(char));

  int i, j = 0;
  for(i = n; j < 80; i++){

    output[j] = input[i];
    j++;


  }

  return output;


}

// ***********************************************************************
// Function: newl_to_space
// Purpose: changes newline char to space
// input: string
// output: formatted string
// ***********************************************************************
char* newl_to_space(char* input){                                               //  finds newline char in a string and replaces it with a space

  int i;
  for(i = 0; i < strlen(input); i++){

    if(input[i] == '\n'){
      break;
    }

  }
  input[i] = ' ';
  return input;

}

// ***********************************************************************
// Function: plus_to_carrot
// Purpose: changes "++" to "^"
// input: string
// output: formatted string
// ***********************************************************************
char* plus_to_carrot(char* input){                                              //  replaces '++' with '^', overwriting chars and reducing strlen by # of '++'

  int i, j;

  while(strstr(input, "++") != NULL){

    for(i = 0; i < strlen(input); i++){

      if( (input[i] == '+') && (input[i + 1] == '+') ){                         //  n^3 ooooofff

        input[i] = '^';
        for(j = i + 1; j < strlen(input); j++){

          input[j] = input[j + 1];

        }

      }

    }
  }

  return input;

}


// ***********************************************************************
// Function: print_buffers
// Purpose: printing f'n for debugging
// input: NA
// output: NA
// ***********************************************************************
void print_buffers(){
  int i;

  for(i = 0; i < buff_1_get; i++){
    printf(" Buffer 1[%d]: |%s|\n", i, buffer_1[i]);
    fflush(stdout);

  }

  for(i = 0; i < buff_2_get; i++){
     printf(" Buffer 2[%d]: |%s|\n", i, buffer_2[i]);
     fflush(stdout);

  }

  for(i = 0; i < buff_3_get; i++){
     printf(" Buffer 3[%d]: |%s|\n", i, buffer_3[i]);
     fflush(stdout);

  }


  printf(" buff_1_idx: %d \n", buff_1_idx);
  printf(" buff_2_idx: %d \n", buff_2_idx);
  printf(" buff_3_idx: %d \n", buff_3_idx);
  printf(" buff_1_get: %d \n",buff_1_get);
  printf(" buff_2_get: %d \n",buff_2_get);
  printf(" buff_3_get: %d \n",buff_1_get);
  printf(" buff_1_count: %d \n",buff_1_count);
  printf(" buff_2_count: %d \n",buff_2_count);
  printf(" buff_3_count: %d \n",buff_3_count);


}











//
