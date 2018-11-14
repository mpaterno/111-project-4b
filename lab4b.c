// #include <mraa.h>
// #include <mraa/aio.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <getopt.h>
// #include <poll.h>
// #include <signal.h>
// #include <time.h>
// #include <sys/time.h>
// #include <fcntl.h>
// #include <math.h>
// #include <sys/errno.h>
// #include <stdbool.h>
// // TODO: Add all necessary libraries.

// // Global Flags
// int periodFlag = 0;
// int scaleFlag = 0;
// int logFlag = 0;
// bool isReporting = false;

// // Global Variables
// int period = 0;
// // TODO: May want to change initialization.
// char scale = 0;
// FILE *logFile = 0;

// // Hardware Implementations
// mraa_aio_context tempSensor;
// mraa_gpio_context button;

// void getOptions(int argc, char **argv);
// void initializeHardware();
// double getTemp(double rTemp);
// void writeReport(double temp);
// void getTime(char *mTime, int size);
// void parseCommands(const char *commands);
// void shutdown();

// int main(int argc, char **argv)
// {
//   atexit(shutdown);
//   getOptions(argc, argv);
//   initializeHardware();

//   struct pollfd pollfds[1];
//   pollfds[0].fd = STDIN_FILENO;
//   pollfds[0].events = POLL_IN | POLL_HUP | POLL_ERR;

//   time_t pStart, pEnd;
//   time(&pStart);

//   while (1)
//   {
//     time(&pEnd);
//     // Exit on button click.
//     if (mraa_gpio_read(button))
//       exit(0);

//     poll(pollfds, 1, 0); // 1 or 2
//     if (pollfds[0].revents & POLLIN)
//     {
//       char commands[64];
//       fgets(commands, 64, stdin);
//       parseCommands(commands);
//     }

//     if (isReporting && difftime(pStart, pEnd) >= period)
//     {
//       double rTemp = mraa_aio_read(tempSensor);
//       double temp = getTemp(rTemp);
//       writeReport(temp);
//       pStart = pEnd;
//     }
//   }

//   return 0;
// }

// // Parse Options
// void getOptions(int argc, char **argv)
// {
//   struct option long_options[] = {
//       {"period", required_argument, &periodFlag, 'p'},
//       {"scale", required_argument, &scaleFlag, 's'},
//       {"log", required_argument, &logFlag, 'l'},
//       {0, 0, 0, 0},
//   };
//   int c;
//   while (1)
//   {
//     c = getopt_long(argc, argv, "p:s:l:", long_options, NULL);
//     if (optarg == NULL)
//       perror("NULL argument supplied.");
//     if (c == -1)
//       break;
//     switch (c)
//     {
//     case 'p':
//       period = atoi(optarg);
//       break;
//     case 's':
//       if (strlen(optarg) != 1 || optarg[0] != 'C' || optarg[0] != 'F')
//       {
//         fprintf(stderr, "Scale not recognized.");
//         exit(1);
//       }
//       else
//         scale = optarg[0];
//       break;
//     case 'l':
//       logFile = fopen(optarg, "w");
//       printf("Line 114");
//       break;
//     default:
//       fprintf(stderr, "Unrecognized Arguments.");
//       exit(1);
//     }
//   }
// }

// double getTemp(double rTemp)
// {
//   double celsius = 1.0 / (log(1023 / (rTemp)-1) / 4275 + 1 / 298.15) - 273.15;
//   if (scale == 'F')
//     return celsius * 9.0 / 5.0 + 32;
//   return celsius;
// }

// void initializeHardware()
// {
//   tempSensor = mraa_aio_init(1);
//   button = mraa_gpio_init(60);
//   mraa_gpio_dir(button, MRAA_GPIO_IN);
// }

// void writeReport(double temp)
// {
//   // Get time for Report
//   char sTime[16];
//   getTime(sTime, 16);
//   printf("%s %.1f\n", sTime, temp);
//   if (logFlag)
//   {
//     fprintf(logFile, "%s %.1f\n", sTime, temp);
//   }
// }

// void getTime(char *mTime, int size)
// {
//   time_t rawtime;
//   struct tm *localTime;
//   time(&rawtime);
//   localTime = localtime(&rawtime);
//   strftime(mTime, size, "%H:%M:%S", localTime);
// }

// void parseCommands(const char *commands)
// {
//   if (logFlag)
//   {
//     fprintf(logFile, "%s\n", commands);
//   }
//   if (strncmp(commands, "STOP", strlen("STOP")) == 0)
//     isReporting = false;
//   else if (strncmp(commands, "START", strlen("START")) == 0)
//     isReporting = true;
//   else if (strncmp(commands, "PERIOD=", strlen("PERIOD=")) == 0)
//     period = strtol(commands + 7, NULL, 10);
//   else if (strncmp(commands, "OFF", strlen("OFF")) == 0)
//     exit(0);
//   else if (strncmp(commands, "SCALE=", strlen("SCALE=")) == 0)
//   {
//     if (commands[6] == 'F')
//       scale = 'F';
//     else if (commands[6] == 'C')
//       scale = 'C';
//     else
//       fprintf(stderr, "Unrecognized scale character");
//   }
//   else if (strncmp(commands, "LOG ", strlen("LOG ")) == 0)
//   { // do nothing
//   }
//   else
//   {
//     fprintf(stderr, "Unrecognized arguments.\n");
//     exit(1);
//   }
// }

// void shutdown()
// {
//   char sTime[15];
//   getTime(sTime, 15);
//   printf("%s SHUTDOWN\n", sTime);
//   if (logFile)
//   {
//     fprintf(logFile, "%s SHUTDOWN\n", sTime);
//     fclose(logFile);
//   }
//   mraa_aio_close(tempSensor);
//   mraa_gpio_close(button);
// }

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <mraa.h>
#include <stdbool.h>
#include <sys/poll.h>
#include <time.h>
#include <math.h>
#include <string.h>

int period = 1;
char scale = 'F';
FILE *logfp = NULL;
mraa_aio_context temp_sensor;
mraa_gpio_context button;
bool reporting = true;
bool first_report_written = false;

void write_report(double temp);
void exit_handler(void);
void get_time(char *buffer, int size);
void process_command(const char *command);
double get_temp(int raw);

int main(int argc, char **argv)
{
  // Process input
  struct option long_options[] = {
      {"period", required_argument, NULL, 'p'},
      {"scale", required_argument, NULL, 's'},
      {"log", required_argument, NULL, 'l'},
      {0, 0, 0, 0}};
  int ch;
  while ((ch = getopt_long(argc, argv, "p:s:l:", long_options, NULL)) != -1)
  {
    switch (ch)
    {
    case 'p':
      period = strtol(optarg, NULL, 10);
      break;
    case 's':
      scale = *optarg;
      break;
    case 'l':
      logfp = fopen(optarg, "w");
      break;
    default:
      fprintf(stderr, "Invalid argument\n");
      exit(1);
    }
  }
  atexit(exit_handler);

  // initialize data sensors
  temp_sensor = mraa_aio_init(1);
  button = mraa_gpio_init(73);
  mraa_gpio_dir(button, MRAA_GPIO_IN);

  // set up polling
  struct pollfd poll_fds[1];
  poll_fds[0].fd = STDIN_FILENO;
  poll_fds[0].events = POLLIN | POLLHUP | POLLERR;

  // create interval timers for period tracking
  time_t beg, end;
  time(&beg);
  time(&end);

  while (1)
  {
    // check if button is pressed
    if (mraa_gpio_read(button))
      exit(0);

    if (first_report_written)
    {
      // poll for commands from STDIN
      poll(poll_fds, 2, 0);
      if (poll_fds[0].revents & POLLIN)
      {
        char command[100];
        fgets(command, 100, stdin);
        process_command(command);
      }
    }

    // write temperature report if period has passed
    if (reporting && difftime(end, beg) >= period)
    {
      double raw_temp = mraa_aio_read(temp_sensor);
      double temp = get_temp(raw_temp);
      write_report(temp);
      if (!first_report_written)
        first_report_written = true;
      beg = end;
    }
    time(&end);
  }
}

/*
 * Writes report to logfile if logging is enabled
 */
void write_report(double temp)
{
  char time[15];
  get_time(time, 15);
  char output[50];
  sprintf(output, "%s %.1f\n", time, temp);
  printf("%s", output);
  if (logfp)
    fprintf(logfp, "%s", output);
}

/*
 * Handles program exits by:
 *     - closing log file
 *     - writing time with SHUTDOWN
 *     - exits
 */
void exit_handler(void)
{
  char time[15];
  get_time(time, 15);
  char output[50];
  sprintf(output, "%s SHUTDOWN\n", time);
  printf("%s", output);
  if (logfp)
    fprintf(logfp, "%s", output);
  if (logfp)
    fclose(logfp);
}

/*
 * Puts the current time into buffer
 * that is assumed to be of capacity size
 */
void get_time(char *buffer, int size)
{
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer, size, "%H:%M:%S", timeinfo);
}

/*
 * Processes command by logging to file or stdout
 * then taking appropriate action based on the command.
 */
void process_command(const char *command)
{
  // log command to file or output
  if (logfp)
    fprintf(logfp, "%s", command);

  // process command
  if (strncmp(command, "SCALE=", strlen("SCALE=")) == 0)
  {
    if (command[6] == 'F')
      scale = 'F';
    else
      scale = 'C';
  }
  else if (strncmp(command, "PERIOD=", strlen("PERIOD=")) == 0)
  {
    period = strtol(command + 7, NULL, 10);
  }
  else if (strncmp(command, "STOP", strlen("STOP")) == 0)
  {
    reporting = false;
  }
  else if (strncmp(command, "START", strlen("START")) == 0)
  {
    reporting = true;
  }
  else if (strncmp(command, "LOG ", strlen("LOG ")) == 0)
  {
    // do nothing
  }
  else if (strncmp(command, "OFF", strlen("OFF")) == 0)
  {
    exit(0);
  }
  else
  {
    fprintf(stderr, "Invalid argument\n");
    exit(1);
  }
}

/*
 * Converts raw temperature readings into
 * either Celsius or Farenheit
 */
double get_temp(int raw)
{
  double celsius = 1.0 / (log(1023 / ((double)raw) - 1) / 4275 + 1 / 298.15) - 273.15;
  if (scale == 'F')
    return celsius * 9.0 / 5.0 + 32;
  return celsius;
}