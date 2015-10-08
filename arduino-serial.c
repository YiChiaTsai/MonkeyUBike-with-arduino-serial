/*
 * arduino-serial
 * --------------
 *
 * A simple command-line example program showing how a computer can
 * communicate with an Arduino board. Works on any POSIX system (Mac/Unix/PC)
 *
 *
 * Compile with something like:
 *   gcc -o arduino-serial arduino-serial-lib.c arduino-serial.c
 * or use the included Makefile
 *
 * Mac: make sure you have Xcode installed
 * Windows: try MinGW to get GCC
 *
 *
 * Originally created 5 December 2006
 * 2006-2013, Tod E. Kurt, http://todbot.com/blog/
 *
 *
 * Updated 8 December 2006:
 *  Justin McBride discovered B14400 & B28800 aren't in Linux's termios.h.
 *  I've included his patch, but commented out for now.  One really needs a
 *  real make system when doing cross-platform C and I wanted to avoid that
 *  for this little program. Those baudrates aren't used much anyway. :)
 *
 * Updated 26 December 2007:
 *  Added ability to specify a delay (so you can wait for Arduino Diecimila)
 *  Added ability to send a binary byte number
 *
 * Update 31 August 2008:
 *  Added patch to clean up odd baudrates from Andy at hexapodia.org
 *
 * Update 6 April 2012:
 *  Split into a library and app parts, put on github
 *
 * Update 20 Apr 2013:
 *  Small updates to deal with flushing and read backs
 *  Fixed re-opens
 *  Added --flush option
 *  Added --sendline option
 *  Added --eolchar option
 *  Added --timeout option
 *  Added -q/-quiet option
 *
 */

#include <stdio.h>    // Standard input/output definitions
#include <stdlib.h>
#include <string.h>   // String function definitions
#include <unistd.h>   // for usleep() and for fork()
#include <getopt.h>

#include "arduino-serial-lib.h"

/*Written by Richard Tsai*/
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h>  /* for wait */
#include <time.h>      /* clock_t, clock, CLOCKS_PER_SEC */

/*Written by Richard Tsai*/
int nearStatus = 1;
int direction = 1; // 1 means same direction, 0 means adversity.
int degree1 = 160;
int degree2 = 0;
const int thresholdFar = 75; //Reality: 70, Laboratory: 70.
const int thresholdNear = 65; //Reality: 60, Laboratory: 55.
const int thresholdStore = 100; //Reality: 60, Laboratory: 55.
enum picOption { black, near1p, far1p, near0p, far0p, monkeylightpro, ncculogo, cslogo, DCT, zoopanda, apple, starbucks, TaylorSwift, TomCruise, ttsai, RichardTsai, cat, dog, natalie_fish, cslogov, zoologov, siubike, taipei101old, taipei101new, near1v, far1v, near0v, far0v, cinemav, readingv, Taiwanflagv };
//
void usage(void)
{
    printf("Usage: arduino-serial -b <bps> -p <serialport> [OPTIONS]\n"
    "\n"
    "Options:\n"
    "  -h, --help                 Print this help message\n"
    "  -b, --baud=baudrate        Baudrate (bps) of Arduino (default 9600)\n"
    "  -p, --port=serialport      Serial port Arduino is connected to\n"
    "  -s, --send=string          Send string to Arduino\n"
    "  -S, --sendline=string      Send string with newline to Arduino\n"
    "  -i  --stdinput             Use standard input\n"
    "  -r, --receive              Receive string from Arduino & print it out\n"
    "  -n  --num=num              Send a number as a single byte\n"
    "  -F  --flush                Flush serial port buffers for fresh reading\n"
    "  -d  --delay=millis         Delay for specified milliseconds\n"
    "  -e  --eolchar=char         Specify EOL char for reads (default '\\n')\n"
    "  -t  --timeout=millis       Timeout for reads in millisecs (default 5000)\n"
    "  -q  --quiet                Don't print out as much info\n"
    "\n"
    "Note: Order is important. Set '-b' baudrate before opening port'-p'. \n"
    "      Used to make series of actions: '-d 2000 -s hello -d 100 -r' \n"
    "      means 'wait 2secs, send 'hello', wait 100msec, get reply'\n"
    "\n");
    exit(EXIT_SUCCESS);
}

//
void error(char* msg)
{
    fprintf(stderr, "%s\n",msg);
    exit(EXIT_FAILURE);
}

/*Written by Richard Tsai*/
int MLP_picture(char* objectType, int objectRSSI, enum picOption picChosen, time_t* picTimer);
const char* getPicChosenName(enum picOption picChosen);

int main(int argc, char *argv[])
{
    const int buf_max = 256;
    const int object_max = 8;

    int fd = -1;
    char serialport[buf_max];
    int baudrate = 9600;  // default
    char quiet=0;
    char eolchar = '\n';
    int timeout = 5000;
    char buf[buf_max];
    int rc,n;

    char objectType[object_max];
    char stringValue[object_max];
    int objectRSSI = 0;

    enum picOption picChosen = black;
    time_t picTimer = time(NULL);

    if (argc==1) {
        usage();
    }

    /* parse options */
    int option_index = 0, opt;
    static struct option loptions[] = {
        {"help",       no_argument,       0, 'h'},
        {"port",       required_argument, 0, 'p'},
        {"baud",       required_argument, 0, 'b'},
        {"send",       required_argument, 0, 's'},
        {"sendline",   required_argument, 0, 'S'},
        {"stdinput",   no_argument,       0, 'i'},
        {"receive",    no_argument,       0, 'r'},
        {"flush",      no_argument,       0, 'F'},
        {"num",        required_argument, 0, 'n'},
        {"delay",      required_argument, 0, 'd'},
        {"eolchar",    required_argument, 0, 'e'},
        {"timeout",    required_argument, 0, 't'},
        {"quiet",      no_argument,       0, 'q'},
        {NULL,         0,                 0, 0}
    };

    while(1) {
        opt = getopt_long (argc, argv, "hp:b:s:S:i:rFn:d:qe:t:",
                           loptions, &option_index);
        if (opt==-1) break;
        switch (opt) {
        case '0': break;
        case 'q':
            quiet = 1;
            break;
        case 'e':
            eolchar = optarg[0];
            if(!quiet) printf("eolchar set to '%c'\n",eolchar);
            break;
        case 't':
            timeout = strtol(optarg,NULL,10);
            if( !quiet ) printf("timeout set to %d millisecs\n",timeout);
            break;
        case 'd':
            n = strtol(optarg,NULL,10);
            if( !quiet ) printf("sleep %d millisecs\n",n);
            usleep(n * 1000 ); // sleep milliseconds
            break;
        case 'h':
            usage();
            break;
        case 'b':
            baudrate = strtol(optarg,NULL,10);
            break;
        case 'p':
            if( fd!=-1 ) {
                serialport_close(fd);
                if(!quiet) printf("closed port %s\n",serialport);
            }
            strcpy(serialport,optarg);
            fd = serialport_init(optarg, baudrate);
            if( fd==-1 ) error("couldn't open port");
            if(!quiet) printf("opened port %s\n",serialport);
            serialport_flush(fd);
            break;
        case 'n':
            if( fd == -1 ) error("serial port not opened");
            n = strtol(optarg, NULL, 10); // convert string to number
            rc = serialport_writebyte(fd, (uint8_t)n);
            if(rc==-1) error("error writing");
            break;
        case 'S':
        case 's':
            if( fd == -1 ) error("serial port not opened");
            sprintf(buf, (opt=='S' ? "%s\n" : "%s"), optarg);

            if( !quiet ) printf("send string:%s\n", buf);
            rc = serialport_write(fd, buf);
            if(rc==-1) error("error writing");
            break;
        case 'i':
            rc=-1;
            if( fd == -1) error("serial port not opened");
            while(fgets(buf, buf_max, stdin)) {
                if( !quiet ) printf("send string:%s\n", buf);
                rc = serialport_write(fd, buf);
            }
            if(rc==-1) error("error writing");
            break;
        case 'r':
            if( fd == -1 ) error("serial port not opened");
            do {
              memset(buf,0,buf_max);  //
              memset(objectType,0,object_max);  //
              memset(stringValue,0,object_max);  //
              serialport_read_until(fd, buf, '!', buf_max, timeout/5);
              if( !quiet ) printf("read string: ");

              // printf("%s\n", buf);
              printf("picChosen: %s, picTimer: %f\n", getPicChosenName(picChosen), difftime(time(NULL), picTimer));

              memcpy( objectType, &buf[0], 4 );
              memcpy( stringValue, &buf[11], 3 );

              if(!strcmp (objectType,"Bike")) {
                objectRSSI = atoi(stringValue);
                printf("%s: %d\n\n", objectType, objectRSSI);
                // printf("Fuck0!\n");
              }
              else if(!strcmp (objectType,"Deg1")) {
                degree1 = atoi(stringValue);
                printf("%s: %d\n\n", objectType, degree1);
                // printf("Fuck1!\n");
              }
              else if(!strcmp (objectType,"Deg2")) {
                degree2 = atoi(stringValue);
                printf("%s: %d\n\n", objectType, degree2);
                // printf("Fuck2!\n");
              }
              else {
                objectRSSI = atoi(stringValue);
                printf("%s: %d\n\n", objectType, objectRSSI);
                // printf("FuckFuck!\n");
              }

              if( abs(degree1-degree2) > 90 && abs(degree1-degree2) < 270 )
                direction = 0;
              else
                direction = 1;

              // printf("buf: %s - objectType: %s - stringValue: %s\n\n", buf, objectType, stringValue);

              printf("Direction: %d\n\n", direction);

              // printf("%s\n\n", buf);
              picChosen = MLP_picture(objectType, objectRSSI, picChosen, &picTimer);

              // Mac_music
              // ./arduino-serial -b 115200 -p /dev/tty.usbmodem1411 -q -r -F

            } while(strcmp (buf,"MLPEnd") != 0);
            break;
        case 'F':
            if( fd == -1 ) error("serial port not opened");
            if( !quiet ) printf("flushing receive buffer\n");
            serialport_flush(fd);
            break;

        }
    }

    exit(EXIT_SUCCESS);
} // end main

/*Written by Richard Tsai*/
int MLP_picture(char* objectType, int objectRSSI, enum picOption picChosen, time_t* picTimer)
{
  int status;

  if ( objectRSSI == 0 ) {

  }
  else if ( picChosen != black && (int)(difftime(time(NULL), *picTimer)) >= 60) {
    picChosen = black;
    *picTimer = time(NULL);
    // status = system("../download-playlist exhibition.script");
  }
  else if ( !strcmp (objectType,"Bike") && picChosen != near1p && objectRSSI < thresholdNear && direction == 1) {
    // nearStatus++;
    // if(nearStatus>1){
        picChosen = near1p;
        *picTimer = time(NULL);
        // status = system("./dokermit1");
    // }
  }
  else if ( !strcmp (objectType,"Bike") && picChosen != far1p && objectRSSI > thresholdFar && direction == 1) {
    // nearStatus++;
    // if(nearStatus>1){
        picChosen = far1p;
        *picTimer = time(NULL);
        // status = system("./dokermit2");
    // }
  }
  else if ( !strcmp (objectType,"Bike") && picChosen != near0p && objectRSSI < thresholdNear && direction == 1) {
    // nearStatus--;
    // if(nearStatus<1){
      picChosen = near0p;
      *picTimer = time(NULL);
      // status = system("./dokermit3");
    // }
  }
  else if ( !strcmp (objectType,"Bike") && picChosen != far0p && objectRSSI > thresholdFar && direction == 1) {
    // nearStatus--;
    // if(nearStatus<1){
      picChosen = far0p;
      *picTimer = time(NULL);
      // status = system("./dokermit4");
    // }
  }
  else if ( !strcmp (objectType,"Sto1") && picChosen != taipei101new && objectRSSI < thresholdStore && (int)(difftime(time(NULL), *picTimer)) >= 8 ) {
    picChosen = taipei101new;
    *picTimer = time(NULL);
    status = system("../download-playlist exhibition23.script");
  }
  else if ( !strcmp (objectType,"Sto2") && picChosen != cslogov && objectRSSI < thresholdStore && (int)(difftime(time(NULL), *picTimer)) >= 8 ) {
    picChosen = cslogov;
    *picTimer = time(NULL);
    status = system("../download-playlist exhibition19.script");
  }
  else if ( !strcmp (objectType,"Sto3") && picChosen != siubike && objectRSSI < thresholdStore && (int)(difftime(time(NULL), *picTimer)) >= 8 ) {
    picChosen = siubike;
    *picTimer = time(NULL);
    status = system("../download-playlist exhibition21.script");
  }
  else if ( !strcmp (objectType,"Sto4") && picChosen != Taiwanflagv && objectRSSI < thresholdStore && (int)(difftime(time(NULL), *picTimer)) >= 8 ) {
    picChosen = Taiwanflagv;
    *picTimer = time(NULL);
    status = system("../download-playlist exhibition30.script");
  }
  //   // status = system("./dokermit8");
  //   // /*Spawn a child to run the program.*/
  //   // pid_t pid=fork();
  //   // if (pid==0) { /* pid==0; child process */
  //   //   status = system("../download-playlist playscript2.script");
  //   //   exit(127); /* only if execv fails */
  //   // }
  //   // else { /* pid!=0; parent process */
  //   //   waitpid(pid,0,0); /* wait for child to exit */
  //   // }
  // }

  // MLP_print(picChosen);

  return picChosen;
}

const char* getPicChosenName(enum picOption picChosen)
{
     switch (picChosen)
   {
      case black: return "black";
      case near1p: return "near1p";
      case far1p: return "far1p";
      case near0p: return "near0p";
      case far0p: return "far0p";
      case monkeylightpro: return "monkeylightpro";
      case ncculogo: return "ncculogo";
      case cslogo: return "cslogo";
      case DCT: return "DCT";
      case zoopanda: return "zoopanda";
      case apple: return "apple";
      case starbucks: return "starbucks";
      case TaylorSwift: return "TaylorSwift";
      case TomCruise: return "TomCruise";
      case ttsai: return "ttsai";
      case RichardTsai: return "RichardTsai";
      case cat: return "cat";
      case dog: return "dog";
      case natalie_fish: return "natalie_fish";
      case cslogov: return "cslogov";
      case zoologov: return "zoologov";
      case siubike: return "siubike";
      case taipei101old: return "taipei101old";
      case taipei101new: return "taipei101new";
      case near1v: return "near1v";
      case far1v: return "far1v";
      case near0v: return "near0v";
      case far0v: return "far0v";
      case cinemav: return "cinemav";
      case readingv: return "readingv";
      case Taiwanflagv: return "Taiwanflagv";
      /* etc... */
   }
}
