#include "include/main.h"

int wifi_flag = 1;

void app_main(void)
{
	wifi_flag = wifi_initialize_station();

}
