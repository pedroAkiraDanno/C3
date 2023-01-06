int* get_input(const char* ip , int* num)
{
	FILE* fp = fopen(ip , "r") ;
	int val , j = 0 ;
	fscanf(fp , "%d" , &val) ;
	int* res = (int*) malloc((sizeof (int))*val) ;
	while(fscanf(fp , "%d" , &val) != EOF)
		res[j++] = val ;
	*num = j ;
	fclose(fp) ;
	return res ;
}