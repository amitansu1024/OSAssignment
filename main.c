#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include <sys/stat.h> // for mkdir

static char Prompt[] = "[username ]"; // + The current directory
static char CurrentDirectory[100] = {0};
static char** line;

static char* Commands[] = {"cd", "exit", "remove", "pwd", "mkdir", "rmdir", "wc", "sort"}; // all the commands

// functions for respective commands
int my_cd(char**);
int my_exit();
int my_remove(char**);
int my_pwd();
int my_mkdir(char**);
int my_rmdir(char**);
int my_wc(char**);
int my_sort(char**); 

char** ParseCommand(char*);
int ExecuteCommand(char**);

void InitShell();
void DisplayPrompt();
void ShellLoop();
char* GetInput();
void PrintArgs(char**);

int (*BuiltInCommands[]) (char**) = { // the commands should be in order in which they are defined in Commands
    my_cd,   
    my_exit,
	my_remove,
	my_pwd,
	my_mkdir,
	my_rmdir,
	my_wc,
	my_sort,
};

int main(int argc, char* argv[]) {
    InitShell();
	ShellLoop();
    
	return EXIT_SUCCESS;
}

void InitShell() {
	if ((getcwd(CurrentDirectory, 100) == 0))
		fprintf(stderr, "Failed to initialize directory");
}

void DisplayPrompt() {
	printf("%s%s ~ ", Prompt, CurrentDirectory);
}

char** ParseCommand(char* line) {
    char* token = strtok(line, " ");
    char** args = malloc(1024 * sizeof(char));

    int i = 0;
    while (token != NULL) {
        args[i] =  token;
        i++;
        token = strtok(NULL, " ");
    }
    return args;
}

void ShellLoop() {
	while (1)  {
		DisplayPrompt();
		char* input = GetInput();
		char** args = ParseCommand(input);
        ExecuteCommand(args);
	}
}

char* GetInput() {
	char *input = {0};
	ssize_t bufsize = 0;

	if (getline(&input, &bufsize, stdin) == -1)  {
		fprintf(stderr, "Unable to get the input\n");
	}
	return input;
}


int ExecuteCommand(char** args) {
    for (int i = 0; i < 8; ++i) {
        if (strcmp(Commands[i], args[0]) == 0) {
            (BuiltInCommands[i])(args);
        }
    } 
}

int my_cd(char** args) {
	printf("%s", args[1]);
    if (chdir(args[1]) == 0) 
		getcwd(CurrentDirectory, 100);
	else printf("not successful\n");
	printf("%s", CurrentDirectory);
}


int my_exit() {
	printf("Exit\n");
	exit(EXIT_SUCCESS);
}

int my_remove(char** args)  {
	if (remove(args[1]) == 0)
        return 0;
    return 1;
}

int my_pwd() {
	printf("%s\n", CurrentDirectory);
	return 0;
}

int my_mkdir(char** args) {
	mkdir(args[1], 0777);
	return 0;
}

int my_rmdir(char** args) {
	rmdir(args[1]);
	return 0;
}


int my_wc(char** args) {
	FILE* file = fopen(args[1], "r");
	if (file == NULL) {
		printf("Couldn't open file\n");
		return 0;
	}

	int lineCount = 0, wordCount = 0, byteCount = 0;
	for (char c = getc(file); c != EOF; c = getc(file)) {
		byteCount++;
		if (c == '\n')
			lineCount += 1;

		if (c == ' ')
			wordCount++;
	}

	printf("The number of lines, words and bytes in the file %s are %d %d %d respectively\n", args[1], lineCount, wordCount, byteCount);
	return 1;
}

int my_sort(char** args) {
    printf("Sorting");
	FILE* file = fopen(args[1], "r");
    char line_buf[300];
    long int size = ftell(file);

    line = (char**)malloc(size);

    if (file == NULL) {
        printf("File not found");
        return 1;
    }

    int x = 0;

    while (fgets(line_buf, sizeof(line_buf), file) != NULL) {
        char* newLine = strdup(line_buf);
        line[x] = newLine;
        printf("%s", line[x]);
        x++;
    }

// x++;

    //sorting
    for (int i = 0; i < x; i++) {
       for (int j = 0; j < x; j++) {
           if (strcmp(line[i], line[j]) <= 0){
               char* temp = line[i];
               line[i] = line[j];
               line[j] = temp;
           }
       }
    }

    for (int i = 0; i < x; i++) {
        printf("%s", line[i]);
    }

    
}
