#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORYACLLOCERROR "Ошибка выделения памяти"
#define TRUE 1
#define FALSE 0

FILE *INPUTFILE = NULL;
FILE *OUTPUTFILE = NULL;
FILE *KEYFILE = NULL;
char *INPUT = NULL;
char *KEY = NULL;
char *OUTPUT = NULL;

void EncodingDecoding(FILE *output);
int RedirectionStreamToFile(FILE **stream);
char *WriteReadFromFile();
void Menu();

void ClearMemory();
void CloseFiles();

void PrintStartMessage();
void MenuItemChangeStream();
void ChangeStream(FILE **fp);
void PrintChangeStreamOptions();
void PrintMenuOptions();

// return TRUE if memory alloced else FALSE
int StringInput(char **str, FILE *fp);

// return TRUE if memory alloced else FALSE
int EncodeDecodeInput(char **cryptoInput, const char *input, const char *key);

int main() {
  Menu();
  return 0;
}

void Menu() {
  PrintStartMessage();

  char input;
  char safetyChar;

  while (1) {
    PrintMenuOptions();
    if (scanf("%c%c", &input, &safetyChar) == 2 && safetyChar == '\n') {
      if (input == '1') {
        //
        if (INPUT != NULL) free(INPUT);
        if (INPUTFILE != NULL) {
          StringInput(&INPUT, INPUTFILE);
        } else {
          printf("i an here\n");
          StringInput(&INPUT, stdin);
        }

        //
      } else if (input == '2') {
        //
        if (KEY != NULL) free(KEY);
        if (KEYFILE != NULL)
          StringInput(&KEY, KEYFILE);
        else
          StringInput(&KEY, stdin);
        //
      } else if (input == '3') {
        //
        if (KEY != NULL && INPUT != NULL) {
          if (OUTPUTFILE == NULL)
            EncodingDecoding(stdout);
          else
            EncodingDecoding(OUTPUTFILE);

          ClearMemory();
          CloseFiles();
        } else {
          if (INPUT == NULL) printf("Поле сообщение не было заполнено\n");
          if (KEY == NULL) printf("Поле ключа не было заполнено\n");
          // printf("Не все поля: сообщение, ключ шифрации\nбыли заполнены\n");
        }

        //
      } else if (input == '4') {
        //

        MenuItemChangeStream();

        //
      } else if (input == 'q') {
        //
        break;
        //
      } else {
        printf(" '%c' - неизвестная команда\n", input);
      }
    } else {
      printf("Ошибка ввода\n");
    }
  }
  ClearMemory();
  CloseFiles();
}

void ClearMemory() {
  if (INPUT != NULL) free(INPUT);
  if (KEY != NULL) free(KEY);
  INPUT = NULL;
  KEY = NULL;
}
void CloseFiles() {
  if (INPUTFILE != NULL) fclose(INPUTFILE);
  if (OUTPUTFILE != NULL) fclose(OUTPUTFILE);
  if (KEYFILE != NULL) fclose(KEYFILE);
  INPUTFILE = NULL;
  OUTPUTFILE = NULL;
  KEYFILE = NULL;
}
void PrintStartMessage() {
  printf("Вас приветсвует программа для шифрации сообщений.\n");
  printf(
      "Суть шифрации заключается в использовании операции XOR между "
      "элементами "
      "сообщения и шифрующего ключа.\n");
  printf(
      "В программе можно выбрать файл, откуда будет происходить считывание "
      "или "
      "же файл, куда будет записываться результат шифрации.\n");
  printf(
      "В случае бездействия программа ожидает консольного ввода и имеет "
      "соответственно консольный вывод.\n");
  printf(
      "После успешной шифрации или дешифрации нужно заново выбирать файлы "
      "для "
      "ввода или вывода");
}

void PrintMenuOptions() {
  printf("Меню программы:\n");
  printf("1 - Ввод сообщения для шифрации\n");
  printf("2 - Ввод ключа шифрации\n");
  printf("3 - Зашифровать и затем дешифровать сообщение\n");
  printf("4 - Выбор файлов\n");
  printf("q - выход\n");
}

void MenuItemChangeStream() {
  // rewind(stdin);
  fseek(stdin, 0, SEEK_END);
  char input = 'a';
  char safetyChar;
  while (input != 'q') {
    PrintChangeStreamOptions();
    scanf("%c%c", &input, &safetyChar);

    if (input == '1') {
      ChangeStream(&INPUTFILE);
    } else if (input == '2') {
      ChangeStream(&KEYFILE);
    } else if (input == '3') {
      ChangeStream(&OUTPUTFILE);
    } else if (input == '4') {
      CloseFiles();
    } else if (input != 'q') {
      printf("'%c' - Неизвестная команда\n", input);
    }
  }
}
void ChangeStream(FILE **fp) {
  if (RedirectionStreamToFile(fp))
    printf("Выбор файла произошел успешно\n");
  else
    printf("Ошибка открытия файла\n");
}

void PrintChangeStreamOptions() {
  printf("1 - Выбор файла для считывания сообщения\n");
  printf("2 - Выбор файла для считывания ключа шифрации\n");
  printf("3 - Выбор файла для вывода результатов шифрации\n");
  printf("4 - Сброс до базовых настроек\n");
  printf("q - выход из пункта меню\n");
}

int RedirectionStreamToFile(FILE **stream) {
  printf("Введите путь до файла\nВвод:");

  char *pathToFile = NULL;
  if (!StringInput(&pathToFile, stdin)) return FALSE;

  (*stream) = fopen(pathToFile, "r+");

  if (pathToFile != NULL) free(pathToFile);

  if (*stream == NULL) return FALSE;

  return TRUE;
}

void EncodingDecoding(FILE *output) {
  char *encodedInput = NULL;
  char *decodedInput = NULL;

  EncodeDecodeInput(&encodedInput, INPUT, KEY);

  if (encodedInput != NULL) {
    fprintf(output,
            "Введенная строка '%s' после шифрации строкой '%s' "
            "получилась:'%s'\n",
            INPUT, KEY, encodedInput);

    EncodeDecodeInput(&decodedInput, encodedInput, KEY);

    if (decodedInput != NULL)
      fprintf(output, "Обратная дешифрация строки:'%s'\n", decodedInput);
    else
      printf(MEMORYACLLOCERROR);
  } else {
    printf(MEMORYACLLOCERROR);
  }

  if (encodedInput != NULL) free(encodedInput);
  if (decodedInput != NULL) free(decodedInput);
}

int StringInput(char **str, FILE *fp) {
  rewind(fp);
  int len = 0;
  if (*str != NULL) free(*str);
  (*str) = (char *)calloc(1, sizeof(char));
  if ((*str) == NULL) return FALSE;

  char *saftyChar = NULL;

  char symbol;
  while (1) {
    fscanf(fp, "%c", &symbol);

    if (symbol == '\n' || symbol == EOF || symbol == '\0') break;

    saftyChar = (char *)realloc(*str, (len + 1) * sizeof(char));

    if (saftyChar != NULL) {
      (*str)[len] = (char)symbol;
      len++;
    } else {
      printf(MEMORYACLLOCERROR);
      free(*str);
      return FALSE;
    }
  }
  // printf("input == !%s! !", *str);
  return TRUE;
}

int EncodeDecodeInput(char **cryptoInput, const char *input, const char *key) {
  int inputLen = strlen(input);
  int keyLen = strlen(key);

  int isMemoryAlloced = TRUE;
  (*cryptoInput) = (char *)malloc(inputLen * sizeof(char));
  if (*cryptoInput != NULL)
    for (int i = 0; i < inputLen; i++)
      (*cryptoInput)[i] = (input[i] ^ key[i % keyLen]);
  else
    isMemoryAlloced = FALSE;

  return isMemoryAlloced;
}
