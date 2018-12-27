//Ran Shem Tov  -   206059586
//Hodaya Simhi  -   313613226
//bonus history by Hodaya
//bonus redirect stdin by Ran
#include "utils.h"
//
//
int isAppCommand(const char *code){
    if(strcmp(code, "l") == 0 || strcmp(code, "&") == 0 || strcmp(code, ">") == 0 || strcmp(code, "<") == 0)
        return 1;
    return 0;
}
//
//
int betterAtoi(char *s){
	if(s == NULL){
		fprintf(stderr, "betterAtoi() empty string\n");
		return 0;
	}
	int n = 0;
	while(*s != '\0' && *s != '\n'){
		if((!(*s <= '9' && *s>= '0'))){
			return 0;
		}
		n = (*s - '0') + (n * 10);
		s++;
	}
	return n;
}
//
//
int argsCount(char **args){
    if(args == NULL)    return 0;
    int c = 0;
    while(*args++ != NULL) c++;
    return c;
}
//
//
char *readLine(void)
{
    char *line = NULL;
    ssize_t bufsize = 0; // have getline allocate a buffer for us
    getline(&line, &bufsize, stdin);
    return line;
}
//
//splitLine()
char **splitLine(char *line)
{
	int position = 0, isEqueal = 0, isAx = 0;
	char **tokens = (char**)malloc(tknsNum * sizeof(char *));
	char *token, *lineCpy;
	lineCpy = (char *)malloc(sizeof(char) * (strlen(line) + 1));
	if(lineCpy == NULL){
		perror("malloc err in splitLine()");
		return NULL;
	}
	strcpy(lineCpy, line);
	if (!tokens)
	{
		fprintf(stderr, "splitLine: malloc error\n");
		return NULL;
	}
	//
	//key=value
	while(*lineCpy != '\0'){
		// if(strcmp(*lineCpy, " ") == 0){
		// 	break;
		// }
		if(*lineCpy == '='){
			isEqueal = 1;
		}
		if(*lineCpy == '!')
			isAx = 1;
		lineCpy++;
	}
	//
	//split line
	token = strtok(line, tokDelim);
	while (token != NULL)
	{
		//printf("token: %s\n", token);
		tokens[position] = (char*)malloc(sizeof(char) * (strlen(token) + 1));
		if(tokens[position] == NULL){
			perror("malloc err in splitLine()");
			return NULL;
		}
		strcpy(tokens[position], token);
		position++;

		if (position > tknsNum - 1) {
			fprintf(stderr, "too many words in line: %s\n", line);
			break;
		}
		token = strtok(NULL, tokDelim);
	}
    //terminate
	tokens[position] = NULL;
	//
	//simple commands
	if (strcmp(tokens[0], "myMan") == 0 || strcmp(tokens[0], "tasks") == 0 || strcmp(tokens[0], "return") == 0 || strcmp(tokens[0], "print_env") == 0 || strcmp(tokens[0], "show_history") == 0 || strcmp(tokens[0], "exit") == 0) {
		return tokens;
	}
	//
	//!
	if(argsCount(tokens) == 1 && isAx == 1){
		tokens[1] = tokens[0];
		//tokens[0] = "!";
		tokens[0] = (char*)malloc(sizeof(char) * 2);
        if (!tokens[0])
        {
            fprintf(stderr, "splitLine: malloc error\n");
            return NULL;
        }
		strcpy(tokens[0], "!");
		tokens[2] = NULL;
		return tokens;
	}
	//
	//key=value
	if (argsCount(tokens) == 2 && isEqueal == 1) {
		//push
        tokens[2] = tokens[1];
		tokens[1] = tokens[0];
		//tokens[0] = '=';
		tokens[0] = (char*)malloc(sizeof(char) * 2);
        if (!tokens[0])
        {
            fprintf(stderr, "splitLine: malloc error\n");
            return NULL;
        }
		strcpy(tokens[0], "=");
		//terminate
		tokens[3] = NULL;
		return tokens;
	}
	int isAmper = 0, isOutput = 0, isIn = 0, i;
	//
	//&
	if (argsCount(tokens) >= 2 && strcmp(tokens[argsCount(tokens) - 1], "&") == 0) {
		isAmper = 1;
		//push "&" at the beginning of tokens && remove it from tokens[last]
		for (i = argsCount(tokens) - 1; i >= 1; i--) {
			tokens[i] = tokens[i - 1];
		}
		//tokens[0] = myAmper;
		tokens[0] = (char*)malloc(sizeof(char) * 2);
        if (!tokens[0])
        {
            fprintf(stderr, "splitLine: malloc error\n");
            return NULL;
        }
		strcpy(tokens[0], "&");
		return tokens;
	}
	int tmp;
	//
	//In/Out
	if(argsCount(tokens) >= 3 && strcmp(tokens[argsCount(tokens) - 2], ">") == 0)	isOutput = 1;
	if (argsCount(tokens) >= 3 && strcmp(tokens[argsCount(tokens) - 2], "<") == 0) isIn = 1;
	if(isOutput == 1 || isIn == 1){
		isOutput = 1;
		//push ">" at the beginning of tokens && remove ">" from tokens[second last]
		tmp = argsCount(tokens);
		for (i = tmp - 2; i >= 1; i--) {
			tokens[i] = tokens[i - 1];
		}
		//tokens[0] = myOut;
		tokens[0] = (char*)malloc(sizeof(char) * 2);
        if (!tokens[0])
        {
            fprintf(stderr, "splitLine: malloc error\n");
            return NULL;
        }
		if(isOutput == 1)	strcpy(tokens[0], ">");
		if(isIn == 1)	strcpy(tokens[0], "<");
		return tokens;
	}
	//
	//l
	if (isAmper == 0 && isOutput == 0 && isIn == 0) {
		//push "l" at the beginning of tokens
		tmp = argsCount(tokens);
		for (i = tmp; i >= 1; i--) {
			tokens[i] = tokens[i - 1];
		}
		//tokens[0] = myL;
		tokens[0] = (char*)malloc(sizeof(char) * 2);
        if (!tokens[0])
        {
            fprintf(stderr, "splitLine: malloc error\n");
            return NULL;
        }
		strcpy(tokens[0], "l");
		//terminate
		tokens[++tmp] = NULL;
		return tokens;
	}
    printf("end of splitLine! tokens:\n");
	if(lineCpy != NULL)	free(lineCpy);
    printArgs(tokens);
}
//
//
void printArgs(char **args){
    if(args == NULL){
        fprintf(stderr, "empty args in printArgs()\n");
        return;
    }
    while(*args != NULL) printf("%s", *args++);

}
//
//
int freeArgs(char **args){
	if(args == NULL){
		return 1;
	}
	int s = 0, i;
	s = argsCount(args);
	for(i = 0; i < s; i++){
		if(args[i] != NULL)	free(args[i]);
	}
	if(args != NULL)	free(args);
	return 1;
}