/*===  Multithreaded HTTP web server ===

Author: Srinivasa Maringanti

Tags: Multithreaded HTTP web server
Course: CMPE 207


*/
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <sys/wait.h> 
#include <signal.h> 
#include <pthread.h>
#include <fcntl.h>

//Global variables used only for setiing the command line root from the user.
char demoRpath[100];
char *rootPath;


/*=========================== Error handling-Start ===========================*/
void errormsg(const char *msg)// Method used to call errors
	{
	    perror(msg);
	    exit(0);
	}
/*=========================== Error handling-End ===========================*/


/*=========================== Readline- Start ===========================*/
 #if 1
 int http_readline(int sock, char *buf, int maxlen)// Function to read each line of the GET reuqest
	{
		int n = 0;
		char *p = buf;
		while (n < maxlen - 1) {
		char c;
		int rc = read(sock, &c, 1);// Reading from a socket and copying it to a integer
		
		if (rc == 1) {
		/* Stop at \n and strip away \r and \n. */
		if (c == '\n') {
		*p = 0; /* null-terminated */
		return n;
		} else if (c != '\r') {
		*p++ = c;
		n++;
		}
		} else if (rc == 0) {
		return -1;
		/* EOF */
		} else if (errno == EINTR) {
		continue;
		/* retry */
		} else {
		return -1;
		/* error */
		}
		}
		return -1; /* buffer too small */
	}
#endif	
/*=========================== Readline- End ===========================*/

/*=========================== Calculating Filesize- Start ===========================*/	
 int http_get_filesize(FILE *fp)// // Method given in assignment 2 to calculate filesize.
	{
	int fsize;
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	rewind(fp);
	return fsize;
	}       
        
/*=========================== Calculating Filesize- End ===========================*/	        
        
/*=========================== Handling HTTP Requests- Start ===========================*/    
void* http_req(void* data)
{
	
	int th,r,b=0,fileOK=0,f_size,ifp,z='.',dot;
	char *req,tmp_var[50],doc_root[100],line[65000],header[6000],demoRpath[100],buf[6500], *filePath,*ftype,*type,status1[100]="200 OK",status[100]="404 FILE NOT FOUND",*tmp_ptr1,*tmp_ptr2,tmp_ptr3[100],*tmp_ptr4,*tmp_ptr5,tmp_ptr6[100]="/",tmp_ptr7[100],tmp_ptr8[100],*tmp_ptr9[100]; 
	
		
	/*
	
	Int variables
	-------------
	int r			-----> Used to save the readline return value.
	int b			-----> Used in iteration in while loop.
	
	Char variables/arrays description:
	----------------------------------
	
	char *req 		-----> Used to save the returned buffer line.
	char tmp_var[] 		-----> Used to check empty space and empty space.
	char doc_root 		-----> Used to save the root location taken from the user.
	char line[] 		-----> Used to save the data from the file opened.
	char header[] 		-----> Used to save the header that has to be sent.
	char demoRpath[]	-----> Used to commandline arugument passed for root address 
	char buf[] 		-----> Used to save the GET request from client.
	char *filePath 		-----> Used to save the file path requested in root folder.
	char *ftype 		-----> Used to save the file file.
	char *type 		-----> Used to return MIME type.
	char *status1[] 	-----> Used to return status when file exist.
	char *status[]	 	-----> Used to return status when file does not exist.
	char *tmp_ptr1 		-----> Used to break the line and processes the string which has "/".
	char *tmp_ptr2 		-----> Used to removes the  " " in string.
	char *tmp_ptr3 		-----> Used to save file loaction and also used as tmp array to perform string operations.
	char *tmp_ptr4 		-----> Used to remove "/" from the and save the one before seond "/".
	char *tmp_ptr5 		-----> Used to save the filename and removes the string before when encounters "/" for the last time.
	char *tmp_ptr6 		-----> Used to contatinate img to initial existing "/" in its array.
	char *tmp_ptr7 		-----> Used to save the root path and add the requesting file after Image URL mapping.
	char *tmp_ptr8 		-----> Used to save the contents of tmp_ptr5 temporarily.
	
	*/

	
	th=*((int *)data); //Type casting void pointer to integer pointer
	int a=0;
	//int rc = read(th, buf, sizeof(buf));
	
 	r=http_readline(th,buf,sizeof(buf));// 
	

 	 	
 	if(r>0)
		{ 	

	 	req=buf;
	 	
	 	//puts(req);
	 	
		}
	else
		{
		errormsg("[ERROR:] Failed to read.");
		}

	
	while(req[b]!=' ')// Condition runs till it encounters an empty space
		{
			tmp_var[b]=req[b++];
		}

	strcpy(demoRpath,rootPath);//copying the command line entered root location to rootPath to make it rootlocation

 	if((strcmp(tmp_var,"GET"))==0);// Comparing the temporary array to check the string
	 	{
	 	
	 	 // filePath is the name of the file to map the URL tmp_ptr1 and 2 are used to perform string operation
	 	  
		        strcpy(doc_root,demoRpath);//copying the command line entered root location to rootPath to make it rootlocation
		        fprintf(stderr,"\nDocument Root\n\n\t");
		        puts(doc_root);
		  
		        tmp_ptr1=strpbrk(req,"/"); // Breaks the line and processes the string which has "/".
		        tmp_ptr2=strtok(tmp_ptr1," "); // Removes blankspces if any
    		        filePath=strpbrk(tmp_ptr2,"/");// Copying the file loaction that the client want to access and break at / and copy after that
		        strcpy(tmp_ptr3,tmp_ptr2);//coping tmp_ptr2 value to tmp_ptr3
		       	       

			if (strcmp(filePath,"/")==0)// When GET request sends "/" we map to "/index.html"
		 	{
		 		strcpy(filePath,strcat(doc_root,"/index.html"));
		 	
		 	}//strcmp if
	 	
	 	else
	 	{
	 		 
		        tmp_ptr5=strrchr(tmp_ptr3,'/');//Copies the file name reuested.
		        strcpy(tmp_ptr8,tmp_ptr5);//Copying the value of tmp_ptr5 to tmp_ptr8.
		        tmp_ptr4=strtok(tmp_ptr3,"/");//Removes any "/".
		        strcpy(filePath,strcat(doc_root,filePath));//Adding the requested file.
		       
			printf("\nRequest File path\n\n\t");
	 		puts(filePath);
	 		
	 		if((strcmp(tmp_ptr4,"images"))==0)
				{
				      
			               strcat(tmp_ptr6,"img");//Adds the location name to "/" for URL mapping.
				       strcpy(tmp_ptr7,demoRpath);// Making a copy of root location.
				       strcat(tmp_ptr7,tmp_ptr6);// Adding the folder name to map to.
				       strcat(tmp_ptr7,tmp_ptr8);// Adding the file name. 
				       strcpy(filePath,tmp_ptr7);// Passing the location and setting it as filePath.
				       fprintf(stderr,"\nImage URL Mapped File Path\n\n\t");
				       puts(filePath);
				
				}
	 	}//else strcmp
	 	

	 	FILE *fp;
	 	fp=fopen(filePath,"r");
	 	ifp=open(filePath,O_RDONLY);
	 	
	 	printf("\nRequested File status\n\n\t");
	 	
	 	if(fp!=NULL)
	 	{
	 		fileOK=1;
	 		printf("Requested file exist in the server.\n");
	 	}//if
	 	else 
	 	{
	 		printf("Requested file does exist in the server.\n");
	 	}
	 	
	 	
		
		
		if(fileOK==1)
		{
		
		f_size= http_get_filesize(fp);// Calculating the Filesize of the requested file.
	 	fclose (fp);
		
	
		ftype=strrchr(filePath,z);
		printf("\nRequested file is of type:\t");
		puts(ftype);
		
		// MIME recognition 
		if (ftype == NULL) 
		{
			type= "application/octet-stream";
		} 
		else if((strcmp(ftype,".htm"))==0 || (strcmp(ftype,".html"))==0)
		{
			type="text/html";
		} 
		else if (strcmp(ftype, ".jpg") == 0)
		{
			type= "image/jpeg";
		} 
		else if (strcmp(ftype, ".gif") == 0) 
		{
			type= "image/gif";
		} 
		else 
		{
			type= "application/octet-stream";
		}
		
		
		
		sprintf(header,"HTTP/1.1 %s\r\nServer:207httpd/0.0.1\r\nConnention:Close\r\nContent-Type:%s\r\nContent-Length:%d\r\n\n",status1,type,f_size);
		printf("\nHeader sent to client as response:\n\n");		
		puts(header);
		
		send(th,header,strlen(header),0);
		printf("Body sent to client as response:\n\n");		
		while((dot = read(ifp,line,sizeof(line)))>0)
		{		send(th,line,dot,0);}
		read(ifp,line,sizeof(line));
		puts(line);
		}
		
		else if(fileOK==0)
		{

		
		sprintf(header,"HTTP/1.1 %s\r\nServer:207httpd/0.0.1\r\nConnention:Close\r\n\n<html><head><title>404 Page Not Found</title></head><body><h1>404 Page Not Found</h1><p>Snap! We lost the page somewhere.We sent the batallion out to search for the page. Meanwhile go to homepage and look around other pages, we will find the page you wanted ASAP!</p>   </body></html>\r\n",status);
		send(th,header,strlen(header),0);

	 		
		}
		
printf("\n\nAll requests handled and responses sent and waiting for client Requests.\n");		


close(ifp);
}

}//http_req

/*=========================== Handling HTTP Requests- Start===========================*/


/*=========================== Main function- Start ===========================*/
int main(int argc, char *argv[])
{

	int fd,nfd,th_id,portnum;// fd will be used as a socket descriptor
	struct sockaddr_in client,server;
	socklen_t c_len;
	pthread_t p_thread[3000];// Number of slaves threads
	pthread_attr_t attr_t;// Attritube to thread
	pthread_attr_init(&attr_t);// Initilizing the attribute
	pthread_attr_setdetachstate(&attr_t, PTHREAD_CREATE_DETACHED); //Creating the tread in detach state
	int i=0;
	portnum = atoi(argv[1]);
	rootPath= argv[2];
	
	// Creating socket and socket descriptor
	fd=socket(AF_INET,SOCK_STREAM,0);// TCP uses stream so SOCK_STREAM

	// Try opening the socket
	if(fd<0)
	{
		
		errormsg("[ERROR:] Cannot open the socket: ");	
	}
	int enable=1;
	//Assigning the PORT IP and FAMILY
	if(setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int))==-1){
	errormsg("[ERROR:]");
	exit(1);
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;// IP
	server.sin_port=htons(portnum);//PORT
	if (bind(fd, (struct sockaddr *) &server,sizeof(server)) < 0) 


              errormsg("[ERROR:] Binding");
	// Listen 
     	listen(fd,5);
	c_len = sizeof(client);
	char cpt[c_len];


	//Accept the connection from client     
	while(1)
	    {
	     nfd = accept(fd,(struct sockaddr *) &client,&c_len);
		inet_ntop(AF_INET, &(client.sin_addr.s_addr),cpt, c_len);// to display the client details we convert Network to Presntation 
		printf("\nConnected client details\n\n\tClient Address: %s\n",cpt);
	
		fprintf(stderr,"\tClient Port: %i \n\n",client.sin_port);
	     if (nfd < 0) 
		  {
		errormsg("[ERROR:] Accepting client");
		continue;
     		  }
	
     	th_id = pthread_create(&p_thread[i++],&attr_t,http_req,(void*)&nfd);
     	pthread_attr_destroy(&attr_t);// Destroying the thread attribute
     	

             }
        }
 /*=========================== Main function- End ===========================*/      
 	/*=========================== END ===========================*/
       
