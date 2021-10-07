int fun (int arg) {
	while ( arg > 0 ) {
		arg = arg % 1000000;
		arg++;
	}
	return 1;
}
     
int main (void) {
        int arg = 1; 
        return fun (arg);
}

