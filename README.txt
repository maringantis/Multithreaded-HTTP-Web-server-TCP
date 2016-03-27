===Multithreaded-HTTP-Web-server-TCP===

Author: Srinivasa Maringanti
Tags: Multithreaded HTTP web server

== Description ==
*A multi-threaded, simple web server, which can serve static HTML files as well as jpeg/gif image files, and can map URL paths to different filesystem paths.

*The URL mapping is determined by three rules:
o All URL paths should be mapped to filenames under DocumentRoot,
which is specified as a command line argument. For example, if
DocumentRoot is set to /home/htdoc and the client is requesting “GET
/httpd.html HTTP/1.1”, the requested filename should then be
/home/htdoc/httpd.html.
o If a URL ends with /, then index.html should be appended to the
filename. For example, if the client request is “GET / HTTP/1.1”, the
mapped filename should be /home/htdoc/index.html.
o The absolute path /images in each URL should be mapped to the img
directory under DocumentRoot. For example, if the client request is
“GET /images/sjsu.jpg HTTP/1.1”, the mapped filename should
be /home/htdoc/img/sjsu.jpg.

*The root of the folder contains one 'C' programs(207httpd.c) ,one object file(207httpd.c),results documents(Results.odt,.pdf,.docx), makefile, one image(favicon.ico),one html page(index.html),four folders(Documents,html,img and Screenshots) and this README file.

*Server.c is a simple HTTP multithreaded Web server built on TCP protocol.


== How to compile and run the files ==

------TCP code running and execution------

** After the code is ready first compile the files using make file that is already in the folder.
** Command for make is  use gcc -pthread -o server server.c 
** Check for errors.If compiled the program is ready to run.
** Now on the server side of the terminal use the following command
	./server 2006 /home/sunsanikates/Desktop/2/
where 2006 is the port number which we are giving using command line argument and /home/sunsanikates/Desktop/2/ is the root folder we need to set.
** Server starts running and is waiting for the client.
** Now on the client side of the terminal use the following command
	wget http://localhost:2006/img/home.gif
where http://localhost:2006/img/home.gif is the address where the files exist in the server.
** Now on the client side in browser enter the URL
	http://localhost:2006/

== Documents ==

*In the documents folder you will find the results in different formats and also a copy of this README file in the document folder.

== Screenshots ==

*In the screenshot folder you will find a few screenshots of both the program executions and outputs.

== img and html ==

*These folders have the html files and images to run the html pages.
**Dont move the files if you cannot understand the relavtive addressing in html.

== References ==

Most of the code structure and creation of bugs was done using the class slides. But errors and bugs where refered from the following websites.

*Stack overflow- 		http://stackoverflow.com/
*Stack Exchange- 		http://codereview.stackexchange.com/
*Rutgers University,CS Dept-	https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/gettime.html
*Wordpress- 			https://wordpress.org/plugins/about/readme.txt (Reffered to create this README file)
