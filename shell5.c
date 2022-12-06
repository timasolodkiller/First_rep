#define MYSHELL "./a.out"
#define parse_calloc_er "parse: Memory callocation failed"
#define parse_malloc_er "parse: Memory allocation failed"
#define parse_realloc_er "parse: Memory reallocation failed"
#define parse_quotes_er "parse: Double quotes not closed"
#define skobki_bracket_er "bracket: Wrong bracket order"
#define get_s_malloc_er "get_s: Memory allocation failed"
#define get_s_realloc_er "get_s: Memory reallocation failed"
#define exec_command_exec_er "exec_command: Execution error"
#define exec_command_fork_er "exec_command: Error in fork()"
#define exec_bracket_bracket_er "exec_bracket: Invalid bracket usage"
#define exec_pipe_exec_er "exec_pipe: Execution error"
#define exec_pipe_fork_er "exec_pipe: Error in fork()"
#define exec_redir_filename_er "exec_redir: Empty file name"
#define execute_fork_er "execute: Error in fork()"
#define main_inval_param_er "main: Invalid parameters"
#define main_file_er "main: Invalid parameters"
#define cd_access_er "cd: Failed to access directory"
#define RED "\033[1;31m\0"
#define BLUE "\033[1;36m\0"
#define WHITE "\033[;37m\0"
#define YELLOW "\033[;33m\0"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#define M 8
void print_err(const char * str_er) {
	printf("%s(# > <) %s%s\n", RED, str_er, WHITE);
}
char * get_s(FILE * fp){
	int len, k = 0;
	char *str = malloc(sizeof(char)*M);
	if (!str) {
		print_err(get_s_malloc_er); 
		return NULL; 
		}
	while(fgets(str + k, M, fp)){
		len = strlen(str);
		if (str[len - 1] != '\n') {
			k = k + M - 1;
			str = realloc(str, sizeof(char)*(k + M));
			if (!str) {print_err(get_s_realloc_er); return NULL; };
		} else {
			str[len-1] = '\0';
			return str;
		}
	}
	free(str);
	return NULL;
}
void show(char ** arr);
void print_sys(const char * str_sys) {
	printf("%s%s%s\n", YELLOW, str_sys, WHITE);
}
int len(char ** arr) {
	int count;
	for (count = 0; arr[count]; count ++);
	return count;
}
char * join(char ** arr){
	char * buf_str = calloc(sizeof(char), 256 * len(arr) + 1);
	int j_buf = 0;
	for (int i = 0; arr[i]; i ++)
		for (int j = 0; j < strlen(arr[i]); j ++){
			buf_str[j_buf] = arr[i][j];
			j_buf++;
		}
	return buf_str;
}
int check(char * elem, char ** source) {
	for (int i = 0; source[i]; i ++) if (!strcmp(elem, source[i])) return i;
	return -1;
}
void show(char ** arr) {
	if (!arr) {
		printf("empty array\n"); 
		return;
	}
	for (int i = 0; arr[i]; i ++)
    if (arr[i][0]) printf("%s\n", arr[i]);
		else printf("\\0\n");
	printf("NULL\n");
}
void free_string(char ** arr){
	if (!arr) return;
	for (int i = 0; arr[i]; i ++) 
		free(arr[i]);
	free(arr);
}
char ** parse(char * str) {
	char * buffer = calloc(strlen(str) + 1, sizeof(char));
	char ** stroka = malloc(sizeof(char *) * 1);
	if (!stroka) { print_err(parse_malloc_er); return NULL; }
	int c_arr = 0, flag = 0, dq_flag = 0, i_buffer = 0;
	if (!buffer) { print_err(parse_calloc_er); free(stroka); return NULL; }
	for (int i = 0; i < strlen(str); ++i) {
		if (str[i] == '"') {                              
			if (dq_flag) dq_flag = 0;
			else dq_flag = 1;
			i_buffer ++;
			flag = 0;
		} else if (dq_flag) {                               
			buffer[i - i_buffer] = str[i];
		}	else if (str[i] == ' ') {                                                          
			flag = 0;
			if ((i > 0) && (str[i - 1] != ' ') && (buffer[0])) {
				stroka[c_arr] = buffer;
				buffer = calloc(strlen(str) + 1, sizeof(char));
				if (!buffer) { print_err(parse_calloc_er); free(stroka); return NULL;}
				c_arr ++;
				stroka = realloc(stroka, sizeof(char *) * (c_arr + 1));
				if (!stroka) { print_err(parse_realloc_er); free(stroka); return NULL;}

			}
			i_buffer = i + 1;
		} else if ((str[i] == '&') || (str[i] == '|') || (str[i] == '>')) {       
			if (flag) {
				flag = 0;
				if (str[i] == str[i - 1]) {
					if ((i < strlen(str) - 1) && (str[i] == '|') && (str[i + 1] == '|')) {
						i_buffer = i;
						stroka[c_arr] = buffer;
						buffer = calloc(strlen(str) + 1, sizeof(char));
						if (!buffer) { print_err(parse_calloc_er); free(stroka); return NULL;}
						buffer[i - i_buffer] = str[i];
						flag = 1;
					} else {
						buffer[i - i_buffer] = str[i];
						i_buffer = i + 1;
						stroka[c_arr] = buffer;
						buffer = calloc(strlen(str) + 1, sizeof(char));
						if (!buffer) { print_err(parse_calloc_er); free(stroka); return NULL;}
					}
				} else {
					i_buffer = i;
					stroka[c_arr] = buffer;
					buffer = calloc(strlen(str) + 1, sizeof(char));
					if (!buffer) { print_err(parse_calloc_er); free(stroka); return NULL;}
					buffer[i - i_buffer] = str[i];
					buffer[i - i_buffer + 1] = '\0';
					flag = 1;
				}
				c_arr ++;
				stroka = realloc(stroka, sizeof(char*) * (c_arr + 1));
				if (!stroka) { print_err(parse_realloc_er); free(stroka); return NULL;}
			} else {
				if (buffer[0] != '\0') {
					i_buffer = i;
					stroka[c_arr] = buffer;
					buffer = calloc(strlen(str) + 1, sizeof(char));
					if (!buffer) { print_err(parse_calloc_er); free(stroka); return NULL;}
					c_arr ++;
					stroka = realloc(stroka, sizeof(char*) * (c_arr + 1));
					if (!stroka) { print_err(parse_realloc_er); free(stroka); return NULL;}
				}
				buffer[i - i_buffer] = str[i];
				flag = 1;
			}
		} else if ((str[i] == ';') || (str[i] == '<') || (str[i] == '(') || (str[i] == ')')) {  
			if (flag || buffer[0]) {
				flag = 0;
				i_buffer = i;
				stroka[c_arr] = buffer;
				buffer = calloc(strlen(str) + 1, sizeof(char));
				if (!buffer) { print_err(parse_calloc_er); free(stroka); return NULL;}
				c_arr ++;
				stroka = realloc(stroka, sizeof(char*) * (c_arr + 1));
				if (!stroka) { print_err(parse_realloc_er); free(stroka); return NULL;}
			}
			buffer[i - i_buffer] = str[i];
			buffer[i - i_buffer + 1] = '\0';
			i_buffer = i + 1;
			stroka[c_arr] = buffer;
			buffer = calloc(strlen(str) + 1, sizeof(char));
			if (!buffer) { print_err(parse_calloc_er); free(stroka); return NULL;}
			c_arr ++;
			stroka = realloc(stroka, sizeof(char*) * (c_arr + 1));
			if (!stroka) { print_err(parse_realloc_er); free(stroka); return NULL;}
		} else {                                                                  
			if (flag) {
				flag = 0;
				i_buffer = i;
				stroka[c_arr] = buffer;
				buffer = calloc(strlen(str) + 1, sizeof(char));
				if (!buffer) { print_err(parse_calloc_er); free(stroka); return NULL;}
				c_arr ++;
				stroka = realloc(stroka, sizeof(char*) * (c_arr + 1));
				if (!buffer) { print_err(parse_realloc_er); free(stroka); return NULL;}
			}
			buffer[i - i_buffer] = str[i];
		}
	}
	if (buffer)
		if (buffer[0] != '\0') {
			stroka[c_arr] = buffer;
			c_arr ++;
			stroka = realloc(stroka, sizeof(char*) * (c_arr + 1));
			if (!buffer) { print_err(parse_realloc_er); free(stroka); return NULL; }
		} else free(buffer);
	if (dq_flag) { print_err(parse_quotes_er); free_string(stroka); return NULL;	}
	stroka[c_arr] = NULL;
	return stroka;
}
char ** skobki(char * str) {
  char * buffer = calloc(strlen(str) + 1, sizeof(char));
  char ** new_arr, ** skobki_arr = malloc(sizeof(char *) * 1);
  if (!skobki_arr) { print_err(parse_malloc_er); return NULL; }
  int c_arr = 0, i_buffer = 0, bracket_count = 0, bracket_flag = 0;
  if (!buffer) { print_err(parse_calloc_er); free(skobki_arr); return NULL; }
  for (int i = 0; i < strlen(str); ++i) {
    if (str[i] == '(') {                            
      if ((!bracket_flag) && buffer[0]) {
        i_buffer = i;
        skobki_arr[c_arr] = buffer;
        buffer = calloc(strlen(str) + 1, sizeof(char));
        if (!buffer) { print_err(parse_calloc_er); free(skobki_arr); return NULL;}
        c_arr ++;
        skobki_arr = realloc(skobki_arr, sizeof(char*) * (c_arr + 1));
        if (!skobki_arr) { print_err(parse_realloc_er); free(skobki_arr); return NULL;}
      }
      bracket_count ++;
      bracket_flag = 1;
      buffer[i - i_buffer] = str[i];
    } else if (str[i] == ')') {      
      bracket_count --;
      if (!bracket_flag) {
        print_err(skobki_bracket_er);
        free(buffer);
        free_string(skobki_arr);
        return NULL;
      }
      buffer[i - i_buffer] = str[i];
      if (!bracket_count) {
        bracket_flag = 0;
        i_buffer = i + 1;
        skobki_arr[c_arr] = buffer;
        buffer = calloc(strlen(str) + 1, sizeof(char));
        if (!buffer) { print_err(parse_calloc_er); free(skobki_arr); return NULL;}
        c_arr ++;
        skobki_arr = realloc(skobki_arr, sizeof(char*) * (c_arr + 1));
        if (!skobki_arr) { print_err(parse_realloc_er); free(skobki_arr); return NULL;}
      }
    } else {
      buffer[i - i_buffer] = str[i];
    }
  }
  if (buffer)
    if (buffer[0]) {
      skobki_arr[c_arr] = buffer;
      c_arr ++;
      skobki_arr = realloc(skobki_arr, sizeof(char*) * (c_arr + 1));
      if (!buffer) {
        print_err(parse_realloc_er);
        free(skobki_arr);
        return NULL;
      }
    } else free(buffer);
  if (bracket_count) {
    print_err(skobki_bracket_er);
    free_string(skobki_arr);
    return NULL;
  }
  skobki_arr[c_arr] = NULL;
  int arr_count = 0, newlen = 0;;
  char ** stroka;
  for (int i = 0; skobki_arr[i]; i ++){
    if (skobki_arr[i][0] != '(') {
      new_arr = parse(skobki_arr[i]);
      newlen = len(new_arr);
      if (!arr_count) stroka = malloc(sizeof(char *) * (arr_count + newlen + 1));
      else stroka = realloc(stroka, sizeof(char *) * (arr_count + newlen + 1));
      memcpy(stroka + arr_count, new_arr, (newlen + 1) * sizeof(char *));
      arr_count += newlen;
    } else {

      if (!arr_count) stroka = malloc(sizeof(char *) * (arr_count + 2));
      else stroka = realloc(stroka, sizeof(char *) * (arr_count + 2));
      stroka[arr_count] = skobki_arr[i];
      arr_count ++;
      stroka[arr_count] = NULL;
    }
  }
  return stroka;
}
int exec_command(char ** arr) {
  switch (fork()){
    case 0:
      execvp(arr[0], arr);
      print_err(exec_command_exec_er);
      exit(EXIT_FAILURE);
    case -1:
      print_err(exec_command_fork_er);
      return 1;
    default:
      return 0;
  }
}
int exec_pipe(char ** pipe_arr, int background_flag) {
	char ** cmd_arr = calloc(sizeof(char *), len(pipe_arr) + 1);
	int j_buf = 0, cmd = 0, first_flag = 1, status, return_status = 0;
	int fd[2], stand_output = dup(1);
	for (int i = 0; pipe_arr[i]; i ++) {
		if (strcmp(pipe_arr[i], "|")) {
			cmd_arr[j_buf] = pipe_arr[i];
			j_buf++;
		} else {
			pipe(fd);
			switch (fork()){
		    case 0:
					dup2(fd[1], 1);
					close(fd[0]);
					close(fd[1]);
		      execvp(cmd_arr[0], cmd_arr);
		      print_err(exec_pipe_exec_er);
		      exit(EXIT_FAILURE);
		    case -1:
		      print_err(exec_pipe_fork_er);
		      return_status = 1;
					goto metka;
		  }
      dup2(fd[0], 0);
			close(fd[0]);
			close(fd[1]);

			j_buf = 0;
      free(cmd_arr);
			cmd_arr = calloc(sizeof(char *), len(pipe_arr));
		}
	}
	switch (fork()){
		case 0:
			execvp(cmd_arr[0], cmd_arr);
			print_err(exec_pipe_exec_er);
			exit(EXIT_FAILURE);
		case -1:
			print_err(exec_pipe_fork_er);
			return_status = 1;
	}
  metka:
  free(cmd_arr);
	close(fd[0]);
	close(fd[1]);
	if (background_flag) {
		return return_status;
	}
	while(wait(&status) != -1);
	return return_status || status;
}
int execute(char ** arr, int background_flag, char * input, char * output, int output_flag){
  int status;
	int fp[2], redir_fp[2];
	switch (fork()){
		case 0:
			if (input) {
				redir_fp[0] = open(input, O_RDONLY);
				dup2(redir_fp[0], 0);
				close(redir_fp[0]);
			}
			if (output) {
				if (output_flag) redir_fp[1] = open(output, O_WRONLY | O_CREAT | O_APPEND, 0666);
				else redir_fp[1] = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0666);
				dup2(redir_fp[1], 1);
				close(redir_fp[1]);
			}

			if (background_flag) {
				fp[0] = open("/dev/null", O_RDONLY);
				dup2(fp[0], 0);
				close(fp[0]);
				close(fp[1]);
			}
			if (exec_pipe(arr, background_flag)) exit(EXIT_FAILURE);
      exit(EXIT_SUCCESS);
		case -1:
			print_err(execute_fork_er);
			return 1;
		default:
			wait(&status);
	}
	return status;
}
int exec_redir(char ** redir_arr, int background_flag){
	char ** pipe_arr = calloc(sizeof(char*), len(redir_arr) + 1);
	char ** redir_symbols = calloc(sizeof(char *), 4);
	redir_symbols[0] = "<";
	redir_symbols[1] = ">";
	redir_symbols[2] = ">>";
	int output_flag = 0, i = 0, return_status = 0;
	char * input = NULL, * output = NULL;
	while (redir_arr[i] && (check(redir_arr[i], redir_symbols) == -1)) {
		pipe_arr[i] = redir_arr[i];
		i ++;
	}

	if (redir_arr[i] && (check(redir_arr[i], redir_symbols) == 0)) {
		if (redir_arr[i + 1]) input = redir_arr[i + 1];
		else {
			print_err(exec_redir_filename_er);
			return_status = 1;
			goto metka;
		}
		i += 2;
	}
	if (redir_arr[i] && (check(redir_arr[i], redir_symbols) >= 1)) {
		if (redir_arr[i + 1]) output = redir_arr[i + 1];
		else {
			print_err(exec_redir_filename_er);
			return_status = 1;
			goto metka;
		}
		if (check(redir_arr[i], redir_symbols) == 2) output_flag = 1;
	}
	if (pipe_arr[0])
		return_status = return_status || execute(pipe_arr, background_flag, input, output, output_flag);
	metka:
	free(pipe_arr);
	free(redir_symbols);
	return return_status;
}
int exec_bracket(char ** skobki_arr, int background_flag){
  int len;
  char * new_command, ** buf_arr;

  if (skobki_arr && skobki_arr[0] && skobki_arr[0][0] == '(') {
    if (skobki_arr[1]) {
      print_err(exec_bracket_bracket_er);
      return 1;
    }
    new_command = skobki_arr[0] + 1;
    new_command[strlen(skobki_arr[0]) - 2] = '\0';
    buf_arr = calloc(sizeof(char *), 4);
    buf_arr[0] = MYSHELL;
    buf_arr[1] = "stdin";
    buf_arr[2] = new_command;
    return execute(buf_arr, background_flag, NULL, NULL, 0);
  } else return exec_redir(skobki_arr, background_flag);
}
int exec_cond(char ** cond_arr, int background_flag) {
	char ** redir_arr = calloc(sizeof(char*), len(cond_arr) + 1);
	int j_buf = 0, return_status = 0, sym_num = -1, i_arr;
	char ** condit_symbols = calloc(sizeof(char *), 3);
	condit_symbols[0] = "&&";
	condit_symbols[1] = "||";
	for (i_arr = 0; cond_arr[i_arr]; i_arr ++) {
		if ((sym_num = check(cond_arr[i_arr], condit_symbols)) != -1) {
			return_status = exec_bracket(redir_arr, background_flag);
			free(redir_arr);
			redir_arr = NULL;
			break;
		} else {
			redir_arr[j_buf] = cond_arr[i_arr];
			j_buf ++;
		}
	}
	if (redir_arr && redir_arr[0] && redir_arr[0][0])
		return_status = exec_bracket(redir_arr, background_flag);

	if (cond_arr[i_arr] && cond_arr[i_arr + 1])
    if (sym_num == 1)
      return_status = return_status && exec_cond(cond_arr + i_arr + 1, background_flag);
    else
      return_status = return_status || exec_cond(cond_arr + i_arr + 1, background_flag);
	metka:
	if (redir_arr) free(redir_arr);
	free(condit_symbols);
	return return_status;
}
int exec_background(char ** stroka) {
	char ** redir_arr = calloc(sizeof(char *), len(stroka) + 1);
	int j_buf = 0, return_status = 0, stat;
	for (int i = 0; stroka[i]; i ++) {
		if (!strcmp(stroka[i], "&")) {
			return_status = return_status || exec_cond(redir_arr, 1);
			j_buf = 0;
			free(redir_arr);
			redir_arr = calloc(sizeof(char *), len(stroka));
		} else if (!strcmp(stroka[i], ";")) {
			return_status = return_status || exec_cond(redir_arr, 0);
			j_buf = 0;
			free(redir_arr);
			redir_arr = calloc(sizeof(char *), len(stroka));
		} else {
			redir_arr[j_buf] = stroka[i];
			j_buf ++;

		}
	}
	if (redir_arr[0]) {
		stat = exec_cond(redir_arr, 0);
		return_status = return_status || stat;
	}
	free(redir_arr);
	return return_status;
}
void cd(char ** arr){
	char * target = arr[1];
	if (!target) target = getenv("HOME");
	if (chdir(target)) print_err(cd_access_er);
}
int main(int argc, char ** argv) {
	char ** stroka, * str;
	int return_status = 0;
	FILE * fp, * buffer_fp = NULL;
	if (argc == 1 || (!strcmp(argv[1], "stdin")))
    fp = stdin;
	else if (argc >= 2) {
    buffer_fp = fopen(argv[1], "r");
    fp = buffer_fp;
	} else {
    print_err(main_inval_param_er);
    return 0;
  }
	if (!fp) { 
		print_err(main_file_er); 
		return 0; 
		}
  	if (argc == 3) {
    	stroka = skobki(argv[2]);
    	return_status = exec_background(stroka);
		free_string(stroka);
    if (buffer_fp) fclose(buffer_fp);
  	return return_status;
  }
	for(;;) {
		printf("%sВВОДИТЕ, ПОЖАЛУЙСТА: %s", BLUE, WHITE);
		str = get_s(fp);
		if (!str) {
      return_status = 1;
      break;
    }
		stroka = skobki(str);
		if (!stroka) continue;
		if (stroka[0] && strcmp(stroka[0], "cd") == 0) {
			cd(stroka);
		}
		return_status = exec_background(stroka);
		free(str);
		free_string(stroka);
	}
	printf("%s", "\n");
	if (buffer_fp){
		fclose(buffer_fp);
	}
	return return_status;
}
