// // #include <mraa.h>
// // #include <mraa/aio.h>
// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>
// // #include <getopt.h>
// // #include <poll.h>
// // #include <signal.h>
// // #include <time.h>
// // #include <sys/time.h>
// // #include <fcntl.h>
// // #include <math.h>
// // #include <sys/errno.h>
// // #include <stdbool.h>
// // // TODO: Add all necessary libraries.

// // // Global Flags
// // int periodFlag = 0;
// // int scaleFlag = 0;
// // int logFlag = 0;
// // bool isReporting = true;

// // // Global Variables
// // int period = 0;
// // // TODO: May want to change initialization.
// // char scale = 0;
// // FILE *logFile = 0;

// // // Hardware Implementations
// // mraa_aio_context tempSensor;
// // mraa_gpio_context button;

// // void getOptions(int argc, char **argv);
// // void initializeHardware();
// // double getTemp(double rTemp);
// // void writeReport(double temp);
// // void getTime(char *mTime, int size);
// // void parseCommands(const char *commands);
// // void shutdown();

// // int main(int argc, char **argv)
// // {
// //   getOptions(argc, argv);
// //   atexit(shutdown);
// //   initializeHardware();

// //   struct pollfd pollfds[1];
// //   pollfds[0].fd = STDIN_FILENO;
// //   pollfds[0].events = POLL_IN | POLL_HUP | POLL_ERR;

// //   time_t pStart, pEnd;
// //   time(&pStart);

// //   while (1)
// //   {
// //     time(&pEnd);
// //     // Exit on button click.
// //     // if (mraa_gpio_read(button))
// //     //   exit(0);

// //     poll(pollfds, 1, 0); // 1 or 2
// //     if (pollfds[0].revents & POLLIN)
// //     {
// //       char commands[64];
// //       fgets(commands, 64, stdin);
// //       parseCommands(commands);
// //     }

// //     if (isReporting && difftime(pEnd, pStart) >= period)
// //     {
// //       double rTemp = mraa_aio_read(tempSensor);
// //       double temp = getTemp(rTemp);
// //       writeReport(temp);
// //       pStart = pEnd;
// //     }
// //   }

// //   return 0;
// // }

// // // Parse Options
// // void getOptions(int argc, char **argv)
// // {
// //   struct option long_options[] = {
// //       {"period", required_argument, NULL, 'p'},
// //       {"scale", required_argument, NULL, 's'},
// //       {"log", required_argument, NULL, 'l'},
// //       {0, 0, 0, 0},
// //   };
// //   int c;
// //   while (1)
// //   {
// //     c = getopt_long(argc, argv, "p:s:l:", long_options, NULL);
// //     // if (optarg == NULL)
// //     //   perror("NULL argument supplied.");
// //     if (c == -1)
// //       break;
// //     switch (c)
// //     {
// //     case 'p':
// //       period = atoi(optarg);
// //       periodFlag = true;
// //       break;
// //     case 's':
// //       if ((strlen(optarg) != 1))
// //       {
// //         fprintf(stderr, "Scale not recognized.");
// //         exit(1);
// //       }
// //       else
// //         scale = optarg[0];
// //       scaleFlag = true;
// //       break;
// //     case 'l':
// //       logFile = fopen(optarg, "w");
// //       logFlag = true;
// //       break;
// //     default:
// //       fprintf(stderr, "Unrecognized Arguments.");
// //       exit(1);
// //     }
// //   }
// // }

// // double getTemp(double rTemp)
// // {
// //   double celsius = 1.0 / (log(1023 / (rTemp)-1) / 4275 + 1 / 298.15) - 273.15;
// //   if (scale == 'F')
// //     return celsius * 9.0 / 5.0 + 32;
// //   return celsius;
// // }

// // void initializeHardware()
// // {
// //   tempSensor = mraa_aio_init(1);
// //   button = mraa_gpio_init(60);
// //   mraa_gpio_dir(button, MRAA_GPIO_IN);
// // }

// // void writeReport(double temp)
// // {
// //   // Get time for Report
// //   char sTime[16];
// //   getTime(sTime, 16);
// //   printf("%s %.1f\n", sTime, temp);
// //   if (logFlag)
// //   {
// //     fprintf(logFile, "%s %.1f\n", sTime, temp);
// //   }
// // }

// // void getTime(char *mTime, int size)
// // {
// //   time_t rawtime;
// //   struct tm *localTime;
// //   time(&rawtime);
// //   localTime = localtime(&rawtime);
// //   strftime(mTime, size, "%H:%M:%S", localTime);
// // }

// // void parseCommands(const char *commands)
// // {
// //   if (logFlag)
// //   {
// //     fprintf(logFile, "%s\n", commands);
// //   }
// //   if (strncmp(commands, "STOP", strlen("STOP")) == 0)
// //     isReporting = false;
// //   else if (strncmp(commands, "START", strlen("START")) == 0)
// //     isReporting = true;
// //   else if (strncmp(commands, "PERIOD=", strlen("PERIOD=")) == 0)
// //     period = strtol(commands + 7, NULL, 10);
// //   else if (strncmp(commands, "OFF", strlen("OFF")) == 0)
// //     exit(0);
// //   else if (strncmp(commands, "SCALE=", strlen("SCALE=")) == 0)
// //   {
// //     if (commands[6] == 'F')
// //       scale = 'F';
// //     else if (commands[6] == 'C')
// //       scale = 'C';
// //     else
// //       fprintf(stderr, "Unrecognized scale character");
// //   }
// //   else if (strncmp(commands, "LOG ", strlen("LOG ")) == 0)
// //   { // do nothing
// //   }
// //   else
// //   {
// //     fprintf(stderr, "Unrecognized arguments.\n");
// //     exit(1);
// //   }
// // }

// // void shutdown()
// // {
// //   char sTime[15];
// //   getTime(sTime, 15);
// //   printf("%s SHUTDOWN\n", sTime);
// //   if (logFile)
// //   {
// //     fprintf(logFile, "%s SHUTDOWN\n", sTime);
// //     fclose(logFile);
// //   }
// //   mraa_aio_close(tempSensor);
// //   mraa_gpio_close(button);
// // }

// #include <unistd.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <ctype.h>
// #include <getopt.h>
// #include <poll.h>
// #include <signal.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/errno.h>
// #include <time.h>
// #include <sys/time.h>
// #include <math.h>
// #include <mraa.h>
// #include <mraa/aio.h>

// int period_flag;
// int scale_flag;
// int log_flag;

// int period = 1;
// char scale = 'F';
// FILE *log_file = 0;

// int paused = 0; //we're outputting time and temperature

// //declare inputs
// mraa_aio_context tempSensor;
// mraa_gpio_context button;

// double getTemp()
// {
//   int reading = mraa_aio_read(tempSensor);
//   //int reading = 5;
//   int B = 4275; //thermistor value
//   double temp = 1023.0 / ((double)reading) - 1.0;
//   temp *= 100000.0;
//   double C = 1.0 / (log(temp / 100000.0) / B + 1 / 298.15) - 273.15;
//   switch (scale)
//   {
//   case 'F':
//     return C * 9 / 5 + 32;
//   default:
//     return C;
//   }
// }

// int checkMatch(const char *str1, const char *str2)
// {
//   int k = 0;
//   if (strlen(str1) != strlen(str2) + 1)
//   {
//     return 0;
//   }
//   int len = strlen(str2);
//   for (; k < len; k++)
//   {
//     if (str1[k] == '\n')
//       break;
//     if (str1[k] != str2[k])
//       return 0;
//   }
//   return 1;
// }

// void printCmd(const char *cmd)
// {
//   if (log_flag)
//     fprintf(log_file, "%s\n", cmd);
//   fflush(log_file);
// }

// void shutDown()
// {
//   struct timeval clk;
//   struct tm *now;
//   gettimeofday(&clk, 0);
//   now = localtime(&(clk.tv_sec));
//   char buf_out[128];
//   sprintf(buf_out, "%02d:%02d:%02d SHUTDOWN\n", now->tm_hour, now->tm_min, now->tm_sec);
//   fputs(buf_out, stdout);
//   if (log_flag)
//     fputs(buf_out, log_file);
//   exit(0);
// }

// void handleLog(const char *cmd)
// {
//   if (!log_flag)
//     return;
//   fputs(cmd, log_file);
// }

// int handlePeriod(const char *cmd)
// {
//   char per_str[] = "PERIOD=";
//   int i = 0;
//   while (cmd[i] != '\0' && per_str[i] != '\0')
//   {
//     if (cmd[i] != per_str[i])
//       return 0;
//     i++;
//   }
//   int len = strlen(cmd) - 1;
//   if (i != 7)
//     return 0;
//   while (i < len)
//   {
//     if (!isdigit(cmd[i]))
//     {
//       return 0;
//     }
//     i++;
//   }
//   return atoi(&cmd[7]);
// }

// void cmdHandler(const char *cmd)
// {
//   if (checkMatch(cmd, "OFF"))
//   {
//     printCmd("OFF");
//     shutDown();
//   }
//   else if (checkMatch(cmd, "STOP"))
//   {
//     paused = 1;
//     printCmd("STOP");
//   }
//   else if (checkMatch(cmd, "START"))
//   {
//     paused = 0;
//     printCmd("START");
//   }
//   else if (checkMatch(cmd, "SCALE=F"))
//   {
//     scale = 'F';
//     printCmd("SCALE=F");
//   }
//   else if (checkMatch(cmd, "SCALE=C"))
//   {
//     scale = 'C';
//     printCmd("SCALE=C");
//   }
//   else if (strlen(cmd) > 4 && cmd[0] == 'L' && cmd[1] == 'O' && cmd[2] == 'G' && cmd[3] == ' ')
//   {
//     handleLog(cmd);
//   }
//   else
//   {
//     int new_period = handlePeriod(cmd);
//     if (new_period == 0)
//     {
//       printCmd(cmd);
//       printCmd("error: invalid command\n");
//       exit(1);
//     }
//     printCmd(cmd);
//     period = new_period;
//   }
// }

// int main(int argc, char **argv)
// {
//   //process options with getopt_long
//   static struct option long_options[] = {
//       {"period", required_argument, &period_flag, 1},
//       {"scale", required_argument, &scale_flag, 1},
//       {"log", required_argument, &log_flag, 1},
//       {0, 0, 0, 0}};
//   //var to store index of command that getopt finds
//   int opt_index;
//   int c;
//   while (1)
//   {
//     c = getopt_long(argc, argv, "", long_options, &opt_index);
//     if (c == -1)
//       break;
//     if (c != 0)
//     {
//       fprintf(stderr, "error: invalid argument\n");
//       exit(1);
//     }

//     if (opt_index == 0)
//     {
//       period = atoi(optarg);
//     }
//     if (opt_index == 1)
//     {
//       if (strlen(optarg) != 1 || (optarg[0] != 'C' && optarg[0] != 'F'))
//       {
//         fprintf(stderr, "unrecognized scale argument\n");
//         exit(1);
//       }
//       scale = optarg[0];
//     }
//     if (opt_index == 2)
//     {
//       log_file = fopen(optarg, "w");
//       ;
//     }
//   }

//   //printf("point 1\n"
//   //initialize I/O pins
//   tempSensor = mraa_aio_init(1);
//   button = mraa_gpio_init(62);
//   mraa_gpio_dir(button, MRAA_GPIO_IN);

//   //initialize poll stuff
//   struct pollfd pollfds[1];
//   pollfds[0].fd = STDIN_FILENO;
//   pollfds[0].events = POLLIN | POLLHUP | POLLERR;

//   //struct pollfd poll_stdin = { 0, POLLIN, 0 };
//   char buf_out[128];
//   struct timeval clk;
//   time_t next = 0;
//   struct tm *now;
//   while (1)
//   {
//     //get the time
//     gettimeofday(&clk, 0);

//     double temperature = getTemp();

//     if (!paused && clk.tv_sec >= next)
//     {
//       now = localtime(&(clk.tv_sec));
//       sprintf(buf_out, "%02d:%02d:%02d %.1f\n", now->tm_hour, now->tm_min, now->tm_sec, temperature);
//       fputs(buf_out, stdout);
//       if (log_flag)
//         fputs(buf_out, log_file);
//       next = clk.tv_sec + period;
//     }
//     //poll for command
//     int retval = poll(pollfds, 1, 0);
//     if (retval < 0)
//     {
//       fprintf(stderr, "error polling\n");
//       exit(1);
//     }

//     if ((pollfds[0].revents & POLLIN))
//     {
//       char cmd[50];
//       fgets(cmd, 50, stdin);
//       cmdHandler(cmd);
//     }

//     if (mraa_gpio_read(button))
//     {
//       shutDown();
//       //printf("button pressed\n");
//     }
//   }

//   mraa_aio_close(tempSensor);
//   mraa_gpio_close(button);
//   exit(0);
// }

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <ctype.h>
#include <math.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <time.h>
#include <mraa.h>
#include <mraa/aio.h>

#define PERIOD 'd'
#define SCALE 's'
#define LOG 'l'

mraa_aio_context sensor;
mraa_gpio_context button;

struct pollfd polls[1];
struct timeval tv;
struct tm *currentTime;
struct timezone timeZone;
time_t periodEnd;
int samplesPerSecond = 1;
int logging = 0;
FILE *logFd = NULL;

//void buttonPressed()
//{
//
//}

int findCommand(const char *input, char *command, int periodFlag, int logFlag)
{
  int retVal = 1;

  if (logFlag == 1)
  {
    int i = 0;
    for (; i < 4; i++)
    {
      if (input[i] != command[i])
      {
        retVal = 0;
      }
    }

    if (retVal == 1)
    {
      logging = 1;
    }
  }
  else if (periodFlag == 1)
  {
    int newPeriod = 0;

    int i = 7;
    while (input[i] != '\n')
    {
      if (isdigit(input[i])) // do we need to take decimals?
      {
        newPeriod *= 10;
        newPeriod += input[i] - '0';
        i++;
      }
      else
      {
        fprintf(stderr, "Error: invalid period");
        exit(1);
      }
    }

    samplesPerSecond = newPeriod;
  }
  else if (strcmp(input, command) != 0) // it is a command
  {
    retVal = 0;
  }

  return retVal;
}

void offHandler()
{
  gettimeofday(&tv, &timeZone);
  currentTime = localtime(&(tv.tv_sec));

  char outputBuffer[256];
  sprintf(outputBuffer, "%02d:%02d:%02d SHUTDOWN\n", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);

  if (logging == 1)
  {
    fputs(outputBuffer, logFd);
  }

  fputs(outputBuffer, stdout);

  exit(0);
}

void handleInput(const char *input, int *pauseReading, char *tempScale)
{
  if (findCommand(input, "START\n", 0, 0) == 1)
  {
    *pauseReading = 0;
    if (logging == 1)
    {
      fprintf(logFd, "%s\n", input);
    }
    fflush(logFd);
  }
  else if (findCommand(input, "STOP\n", 0, 0) == 1)
  {
    *pauseReading = 1;
    if (logging == 1)
    {
      fprintf(logFd, "%s\n", input);
    }
    fflush(logFd);
  }
  else if (findCommand(input, "SCALE=C\n", 0, 0) == 1)
  {
    *tempScale = 'C';
    if (logging == 1)
    {
      fprintf(logFd, "%s\n", input);
    }
    fflush(logFd);
  }
  else if (findCommand(input, "SCALE=F\n", 0, 0) == 1)
  {
    *tempScale = 'F';
    if (logging == 1)
    {
      fprintf(logFd, "%s\n", input);
    }
    fflush(logFd);
  }
  else if (findCommand(input, "LOG ", 0, 1) == 1)
  {
    if (logging == 1)
    {
      fprintf(logFd, "%s\n", input);
    } // double check this
    fflush(logFd);
  }
  else if (findCommand(input, "PERIOD=", 1, 0) == 1)
  {
    if (logging == 0)
    {
      fprintf(logFd, "%s%d\n", input, samplesPerSecond);
    }
    fflush(logFd);
  }
  else if (findCommand(input, "OFF\n", 0, 0) == 1)
  {
    if (logging == 0)
    {
      fprintf(logFd, "%s\n", input);
    }
    fflush(logFd);
    offHandler();
  }
  else
  {
    fprintf(stderr, "Error: not one of the avaialble commands"); // GET ERROR HERE TOO
    exit(1);
  }
}

float recordTemperature(char tempScaleFlag)
{
  int B = 4275;
  int R0 = 100000;

  float retTemp;

  int temp = mraa_aio_read(sensor); // reads in celsius

  float R = 1023 / temp - 1;

  R *= R0;

  float tempC = 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15;

  if (tempScaleFlag == 'C')
  {
    retTemp = tempC;
  }
  else if (tempScaleFlag == 'F')
  {
    retTemp = tempC * 1.8 + 32;
  }

  return retTemp;
}

int main(int argc, char *argv[])
{

  static struct option long_options[] = {
      {"period", optional_argument, NULL, PERIOD},
      {"scale", optional_argument, NULL, SCALE},
      {"log", required_argument, NULL, LOG}};

  int getoptReturnChar = -1;

  char tempScale = 'F';
  int pauseWriting = 0; // not paused

  while ((getoptReturnChar = getopt_long(argc, argv, "", long_options, NULL)) != -1)
  {
    if (getoptReturnChar == PERIOD)
    {
      samplesPerSecond = atoi(optarg);
      if (samplesPerSecond <= 0)
      {
        fprintf(stderr, "Error: incorrect period");
        exit(1);
      }
      // tempScale = should lower case also work?
    }
    else if (getoptReturnChar == SCALE)
    {
      if ((optarg[0] != 'C' && optarg[0] != 'F') || optarg[1] != '\0')
      {
        fprintf(stderr, "Error: not one of the specified temperature scales");
        exit(1);
      }
      tempScale = optarg[0];
    }
    else if (getoptReturnChar == LOG)
    {
      logging = 1;
      logFd = fopen(optarg, "w"); // Double check you are writing to the right place
    }
    else
    {
      fprintf(stderr, "Not one of the specified options \n");
      exit(1);
    }
  }

  sensor = mraa_aio_init(1);
  button = mraa_gpio_init(60);
  mraa_gpio_dir(button, MRAA_GPIO_IN);

  polls[0].fd = STDIN_FILENO;
  polls[0].events = POLLIN | POLLHUP | POLLERR;

  timeZone.tz_minuteswest = 200;
  //    timeZone.tz_dsttime     = DST_USA;

  char inputs[256];
  do // need to generate first report before looking at input commands
  {
    gettimeofday(&tv, &timeZone);
    double temperature = recordTemperature(tempScale);

    if (tv.tv_sec >= periodEnd)
    {

      currentTime = localtime(&(tv.tv_sec));

      char outputBuffer[256];
      sprintf(outputBuffer, "%02d:%02d:%02d %.1f\n", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, temperature);

      if (logging == 1)
      {
        fputs(outputBuffer, logFd);
      }

      if (pauseWriting != 1)
      {
        fputs(outputBuffer, stdout);
      }

      periodEnd = tv.tv_sec + samplesPerSecond;
    }

    if (poll(polls, 1, 0) < 0) // returns number of structures with nonzero revents fields
    {
      fprintf(stderr, "Error: could not poll for events on file descriptors");
      exit(1);
    }

    if (polls[0].revents & POLLIN)
    {
      fgets(inputs, 256, stdin);
      handleInput(inputs, &pauseWriting, &tempScale);
    }
    // See if any events occur i.e. inputs from stdin

    // check to see if button was pressed
    // get time of day
    // read temperature
    // create correct format
    // print it to stdout and append to logfile
    // on button press output time and SHUTDOWN
    memset(inputs, '\0', sizeof(char));

  } while (!mraa_gpio_read(button));

  offHandler();

  mraa_aio_close(sensor);
  mraa_gpio_close(button);
  exit(0);
}
