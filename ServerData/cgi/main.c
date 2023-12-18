#include "stdio.h"
// #include "web.h"
#include "unistd.h" 
#include <fcntl.h>
int main(int argc, char **argv, char **env){

	// web_print_header();
    // printf("Content-Type: text/html; charset=utf-8\r\n\r\n");

	printf("<!DOCTYPE html>\n<html lang=\"en\">\n<html>\n<head><title>hello</title></head>\n<body>\n");


	while(*env){
		printf("%s<br>", *env);
		env += 1;
	
	}
	printf("@<br>");
	char buffer[3009];
	while(1)
	{
		int a = read(0, buffer, 3000);
		if (a == 0)
			break;
		printf("%s", buffer);
	}
	// int o = open("README.md", O_RDONLY);
	// while(1)
	// {
	// 	int p = read(o, buffer, 3000);
	// 	if(p == 0 || p < 0)
	// 	{
	// 		break;
	// 	}
	// 	printf("%s", buffer);
	// }
	if (argc <= 1 )
	{
    printf("</body>\n</html>");
		return 0;
	}
	printf("hi<br>");
	while(*argv)
	{

		printf("%s<br>", *argv);
		argv += 1;
	}
    printf("</body>\n</html>");

	return 0;

}//main*/