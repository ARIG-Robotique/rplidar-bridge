/* RPLIDAR Bridge Application */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <rplidar.h> //RPLIDAR standard sdk, all-in-one header

using namespace rp::standalone::rplidar;

bool ctrl_c_pressed;
void ctrlc(int) {
  ctrl_c_pressed = true;
}

int main(int argc, const char * argv[]) {
  const char * opt_com_path = NULL;
  _u32         opt_com_baudrate = 115200;
  u_result     op_result;

  printf("RPLidar bridge with socket.\n"
         "Version: "RPLIDAR_SDK_VERSION"\n");

  // read serial port from the command line...
  if (argc>1) opt_com_path = argv[1]; // or set to a fixed value: e.g. "com3"

  // read baud rate from the command line if specified...
  if (argc>2) opt_com_baudrate = strtoul(argv[2], NULL, 10);


  if (!opt_com_path) {
      opt_com_path = "/dev/ttyUSB0";
  }

	signal(SIGINT, ctrlc);

  // fetech result and print it out...
  while (1) {
    if (ctrl_c_pressed){
      break;
    }
  }

  return 0;
}
