int write_file(char name[256],char key[1024]){
	FILE *file;
	char *buffer;
	char ch;
	int fsize;
	file = fopen(name,"w");
	if (!file){
		printf("\nFile not found!\n");
		printf("Program terminated.");
		//return 1;
	}
	fprintf(file,"%s", key);
	fclose(file);
    return 0;
}