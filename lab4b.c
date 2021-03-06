#include <mraa.h>
#include <mraa/aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <poll.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <math.h>
#include <sys/errno.h>
#include <stdbool.h>

// Global Flags
int periodFlag = 0;
int scaleFlag = 0;
int logFlag = 0;
bool isReporting = true;

// Global Variables
int period = 0;
char scale = 0;
FILE *logFile = 0;

// Hardware Implementations
mraa_aio_context tempSensor;
mraa_gpio_context button;

void getOptions(int argc, char **argv);
void initializeHardware();
double getTemp(double rTemp);
void writeReport(double temp);
void getTime(char *mTime, int size);
void parseCommands(const char *commands);
void shutdown();

int main(int argc, char **argv)
{
  getOptions(argc, argv);
  atexit(shutdown);
  initializeHardware();

  struct pollfd pollfds[1];
  pollfds[0].fd = STDIN_FILENO;
  pollfds[0].events = POLL_IN | POLL_HUP | POLL_ERR;

  time_t pStart, pEnd;
  time(&pStart);

  while (1)
  {
    time(&pEnd);
    // Exit on button click.
    if (mraa_gpio_read(button))
      exit(0);

    poll(pollfds, 1, 0); // 1 or 2
    if (pollfds[0].revents & POLLIN)
    {
      char commands[64];
      fgets(commands, 64, stdin);
      parseCommands(commands);
    }

    if (isReporting && difftime(pEnd, pStart) >= period)
    {
      double rTemp = mraa_aio_read(tempSensor);
      double temp = getTemp(rTemp);
      writeReport(temp);
      pStart = pEnd;
    }
  }

  return 0;
}

// Parse Options
void getOptions(int argc, char **argv)
{
  struct option long_options[] = {
      {"period", required_argument, NULL, 'p'},
      {"scale", required_argument, NULL, 's'},
      {"log", required_argument, NULL, 'l'},
      {0, 0, 0, 0},
  };
  int c;
  while (1)
  {
    c = getopt_long(argc, argv, "p:s:l:", long_options, NULL);

    if (c == -1)
      break;
    switch (c)
    {
    case 'p':
      period = atoi(optarg);
      periodFlag = true;
      break;
    case 's':
      if ((strlen(optarg) != 1))
      {
        fprintf(stderr, "Scale not recognized.");
        exit(1);
      }
      else
        scale = optarg[0];
      scaleFlag = true;
      break;
    case 'l':
      logFile = fopen(optarg, "w");
      logFlag = true;
      break;
    default:
      fprintf(stderr, "Unrecognized Arguments.");
      exit(1);
    }
  }
}

double getTemp(double rTemp)
{
  double celsius = 1.0 / (log(1023 / (rTemp)-1) / 4275 + 1 / 298.15) - 273.15;
  if (scale == 'F')
    return celsius * 9.0 / 5.0 + 32;
  return celsius;
}

void initializeHardware()
{
  tempSensor = mraa_aio_init(1);
  button = mraa_gpio_init(60);
  mraa_gpio_dir(button, MRAA_GPIO_IN);
}

void writeReport(double temp)
{
  // Get time for Report
  char sTime[16];
  getTime(sTime, 16);
  printf("%s %.1f\n", sTime, temp);
  if (logFlag)
  {
    fprintf(logFile, "%s %.1f\n", sTime, temp);
  }
}

void getTime(char *mTime, int size)
{
  time_t rawtime;
  struct tm *localTime;
  time(&rawtime);
  localTime = localtime(&rawtime);
  strftime(mTime, size, "%H:%M:%S", localTime);
}

void parseCommands(const char *commands)
{
  if (logFlag)
  {
    fprintf(logFile, "%s\n", commands);
  }
  if (strncmp(commands, "STOP", strlen("STOP")) == 0)
    isReporting = false;
  else if (strncmp(commands, "START", strlen("START")) == 0)
    isReporting = true;
  else if (strncmp(commands, "PERIOD=", strlen("PERIOD=")) == 0)
    period = strtol(commands + 7, NULL, 10);
  else if (strncmp(commands, "OFF", strlen("OFF")) == 0)
    exit(0);
  else if (strncmp(commands, "SCALE=", strlen("SCALE=")) == 0)
  {
    if (commands[6] == 'F')
      scale = 'F';
    else if (commands[6] == 'C')
      scale = 'C';
    else
      fprintf(stderr, "Unrecognized scale character");
  }
  else if (strncmp(commands, "LOG ", strlen("LOG ")) == 0)
  { // do nothing
  }
  else
  {
    fprintf(stderr, "Unrecognized arguments.\n");
    exit(1);
  }
}

void shutdown()
{
  char sTime[15];
  getTime(sTime, 15);
  printf("%s SHUTDOWN\n", sTime);
  if (logFlag)
  {
    fprintf(logFile, "%s SHUTDOWN\n", sTime);
    fclose(logFile);
  }
  mraa_aio_close(tempSensor);
  mraa_gpio_close(button);
}