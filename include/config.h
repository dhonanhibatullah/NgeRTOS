#ifndef __CONFIG_H
#define __CONFIG_H

//______________[ GLOBAL ]______________

//______________[ APPS::CORE ]______________
#define APPS_CORE_NAME "CORE"
#define APPS_CORE_STACK_SIZE 2400
#define APPS_CORE_PRIORITY 255
#define APPS_CORE_SLEEP_TICK 500
#define APPS_CORE_CORE_ID 0

//______________[ STORAGE ]______________
#define APPS_STORAGE_NAME "STORAGE"
#define APPS_STORAGE_STACK_SIZE 6400
#define APPS_STORAGE_PRIORITY 1
#define APPS_STORAGE_SLEEP_TICK 0
#define APPS_STORAGE_CORE_ID 0
#define APPS_STORAGE_BUFFER 8

//______________[  ]______________

#endif