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
int buff_1_count = 0;
int buff_2_count = 0;
int buff_3_count = 0;

bool stpline = false;
int linesread = 0;

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


  print_buffers();

  return 0;
}



//  ********************************    BUFFER 1  ******************************
void put_buff_1(char* line){

  pthread_mutex_lock(&buff_1);                                                  //  locking mutex for data entry
  strcpy(buffer_1[buff_1_idx],line);                                            //  copies input to index of buffer_1

  free(line);                                                                   //  frees input
  buff_1_count++;
  pthread_cond_signal(&cond_1);

  pthread_mutex_unlock(&buff_1);

}

char* get_buff_1(){


  pthread_mutex_lock(&buff_1);                                                  // Lock the mutex before checking if the buffer has data
  while (buff_1_count == 0){                                                    // waits until buffer has data to pull data
    pthread_cond_wait(&cond_1, &buff_1);
  }

  buff_1_idx++;                                                                 // Increment the index from which the string will be gotten from buffer
  buff_1_count--;                                                               // decrement the count of items in the buffer
  printf(" buff1 index: %d\n", buff_3_idx);

  pthread_mutex_unlock(&buff_1);                                                //  unlock mutex for next thread_1() call

  return buffer_1[buff_1_idx - 1];                                              //  returns next string from buffer

}


//  ********************************    BUFFER 2  ******************************
void put_buff_2(char* line){

  pthread_mutex_lock(&buff_2);                                                  //  locking mutex for data entry
  strcpy(buffer_2[buff_2_idx],line);                                            //  copies input to index of buffer_2
  buff_2_count++;
  pthread_cond_signal(&cond_2);

  pthread_mutex_unlock(&buff_2);

}

char* get_buff_2(){

  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&buff_2);
  while (buff_2_count == 0){                                                    // waits until buffer has data to pull data
    pthread_cond_wait(&cond_2, &buff_2);
  }

  buff_2_idx++;                                                                 // Increment the index from which the string will be gotten from buffer
  buff_2_count--;                                                               // decrement the count of items in the buffer
  printf(" buff2 index: %d\n", buff_3_idx);

  pthread_mutex_unlock(&buff_2);                                                //  unlock mutex for next thread_1() call

  return buffer_2[buff_2_idx - 1];                                              //  returns next string from buffer

}


//  ********************************    BUFFER 3  ******************************
void put_buff_3(char* line){

  pthread_mutex_lock(&buff_3);                                                  //  locking mutex for data entry
  strcpy(buffer_3[buff_3_idx],line);                                            //  copies input to index of buffer_3
  buff_3_count++;
  pthread_cond_signal(&cond_3);

  pthread_mutex_unlock(&buff_3);

}

char* get_buff_3(){

  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&buff_3);
  while (buff_3_count == 0){                                                    // waits until buffer has data to pull data
    pthread_cond_wait(&cond_3, &buff_3);
  }

  buff_3_idx++;                                                                 // Increment the index from which the string will be gotten from buffer
  printf(" buff3 index: %d\n", buff_3_idx);
  buff_3_count--;                                                               // decrement the count of items in the buffer

  pthread_mutex_unlock(&buff_3);                                                //  unlock mutex for next thread_2() call

  return buffer_3[buff_3_idx - 1];                                              //  returns next string from buffer

}





void *thread_1(void* args){

  int k = 0;
  while (stpline == false){
    char* line = NULL;                                                            //  the string var that getline puts input in
    size_t len = 0;                                                               //  tells getline how far to go
    ssize_t char_count = 0;                                                       //  getline changes this to # of chars gotten, including \n

    char_count = getline(&line, &len, stdin);                                     //  gets line delimited by "\n" from stdin, alloc and stores it in line, must free line after use
    if(strcmp("STOP\n", line) == 0){
      stpline = true;
    }

    put_buff_1(line);
    k++;
    void print_buffers();

  }
  printf(" THREAD 1 returning \n");

  return NULL;
}

void *thread_2(void* args){

  int k = 0;
  while (stpline == false){
    char input[1000];                                                             //  string that will be put in buffer 2
    strcpy(input, get_buff_1());                                                  //  copying input from buff1 to string

    newl_to_space(input);                                                         //  changing newl to space

    put_buff_2(input);                                                            //  putting output into buff2
    void print_buffers();

  }

  printf(" THREAD 2 returning \n");

  return NULL;

}

void *thread_3(void* args){

  int k = 0;
  while (stpline == false){
    char input[1000];                                                             //  string that will be put in buffer 3
    strcpy(input, get_buff_2());                                                  //  copying input from buff2 to string

    plus_to_carrot(input);                                                        //  ++ to ^

    put_buff_3(input);                                                            //  putting output into buff3
    void print_buffers();

  }

  printf(" THREAD 3 returning \n");


  return NULL;

}

void *thread_4(void* args){                                                     //  output thread that prints the output


  char output[81] = {0};
  char* output_buff = (char*)malloc(MAX_LINES * MAX_LINE_LEN * sizeof(char));
  int charcount = 0;
  int linecount = 0;
  int last_idx = 0;
  char input[1000];

  int i, k = 0;
  while (stpline == false){

  // copy buffer to input string
  // concat string to output_buff
  // change charcount to strlen(output_buff)
  // change linecount to charcount / 80
  // see how many lines need to be printed
  // clear output buffer
  // put lines in output buffer
  // print lines




    strcpy(input, get_buff_3());                                                // copy buffer to input string

    if(strcmp(input, "STOP ") == 0){
      stpline = true;
      // exit(0);
    }

    strcat(output_buff, input);                                                 // concat string to output_buff

    charcount = strlen(output_buff);                                            //  change charcount to len(output_buff) to see if we need to print more lines

    linecount = charcount / 80;

    int j = 0;
    for(i = last_idx; i < linecount; i++){
      memset(input, 0, sizeof(char) * 80);
      strncpy(print_buffer[j], output_buff + (i * 80), 80);
      j++;
    }

    for(i = 0; i < linecount - last_idx; i++){
      printf(" len: %lu line: |%s\n|", strlen(print_buffer[i]), print_buffer[i]);
      fflush(stdout);
    }

    last_idx = linecount;



    fflush(stdout);


    void print_buffers();


  }

  printf(" THREAD 4 returning \n");

  free(output_buff);

  return NULL;

}



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



void print_buffers(){
  int i;

  for(i = 0; i < buff_1_idx; i++){
    printf(" Buffer 1[%d]: |%s|\n", i, buffer_1[i]);

  }

  for(i = 0; i < buff_2_idx; i++){
     printf(" Buffer 2[%d]: |%s|\n", i, buffer_2[i]);

  }

  for(i = 0; i < buff_3_idx; i++){
     printf(" Buffer 3[%d]: |%s|\n", i, buffer_3[i]);

  }


}







//
