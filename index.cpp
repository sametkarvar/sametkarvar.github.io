#pragma hdrstop
#pragma argsused

#include <snorkel.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

call_status_t index_htm(snorkel_obj_t http,     /*read environment from this object */
			snorkel_obj_t outstream)  /* write data to the output stream   */
  {
   if (snorkel_printf (outstream, "<html><body><img src=QR4.jpg alt=QRcode width=178 height=178>" //width=100% height=100% alt=QRcode>" // alt=QRcode width=178 height=178>"
								   "</img></body></html>\r\n")== SNORKEL_ERROR)
		 return HTTP_ERROR;

   return HTTP_SUCCESS;
   }

void syntax (char *pszProg)
	{
	 fprintf (stderr, "syntax error:\n");
	 fprintf (stderr, "%s [-p <port>]\n", pszProg);
	 exit (1);
	 }

void main (int argc, char *argv[])
 {
  int i = 1;
  int port = 8888;
  snorkel_obj_t http = 0;
  char szExit[10];
  for (; i < argc; i++)
  {
   if (argv[i][0] == '-' || argv[i][0] == '/')
	 {
	  char carg = argv[i][1];
	  switch (carg)
	   {
		case 'p':          /* port number */
		port = atoi (argv[i + 1]);
		i++;
		break;
		default:
		syntax (argv[0]);
		break;
		}
	  }
   }

/*
 *
 * initialize API
 *
 */
 if (snorkel_init () != SNORKEL_SUCCESS)
  {
	perror ("could not initialize snorkel\n");
	//exit (1);
   }
/*
 *
 * create a server object
 *
 */
 http = snorkel_obj_create (snorkel_obj_server, 5,     /* number of handler threads to create */
								 NULL       /* directory containing index.html     */);
 //or i can put "/Users/rania/Documents/images"
  if (!http)
   {
	 perror ("could not create http server\n");
	 //exit (1);
   }

/*
 *
 * create a listener
 *
 */
 if (snorkel_obj_set (http,    /* server object */
					  snorkel_attrib_listener, /* attribute   */
					  port,    /* port number */
					  0 /* SSL support */ )
					  != SNORKEL_SUCCESS)
	  {
	   fprintf (stderr, "could not create listener\n");
	   snorkel_obj_destroy (http);
	   //exit (1);
	  }

 /*
  *
  * overload the URL index.html
  *
  */
	 //THIS GENERATES AN EMPTY BOX WITH NO QR CODE
	 //============================================
 if (snorkel_obj_set (http,    // server object
					 snorkel_attrib_uri,      // attribute type
					 GET,     // method
					 "/index.html",   // url
					 encodingtype_text, index_htm) != SNORKEL_SUCCESS)

		 {
		   perror ("could not overload index.html");
		   snorkel_obj_destroy (http);
		   //exit (1);
		  }
if (snorkel_obj_set(http, snorkel_attrib_ipvers, IPVERS_IPV4, SOCK_SET) != SNORKEL_SUCCESS)
		{
		  fprintf (stderr, "error could not set ip version\n");
		  //exit (1);
		 }

 fprintf (stderr, "\n\n[HTTP] starting embedded server\n");
  if (snorkel_obj_start (http) != SNORKEL_SUCCESS)
   {
	perror ("could not start server\n");
	snorkel_obj_destroy (http);
	//exit (1);
	}

/*
 *
 * do something while server runs
 * as a separate thread
 *
 */
  fprintf (stderr, "\n[HTTP] started.\n\n"
					 "--hit enter to terminate--\n");
  fgets (szExit, sizeof (szExit), stdin);

  fprintf (stderr, "[HTTP] bye\n");

 /*
  *
  * graceful clean up
  *
  */
  snorkel_obj_destroy (http);
  exit (0);
  }
