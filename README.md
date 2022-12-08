# esp8266_doorLock
an ESP8266 digital door lock project

An Arduino project, using an ESP8266 programming board.
A 0,91" display for status, 3x4 keypad for code imput and door lock were used as modules.
The board is connecting to wifi and after the connection is established it accesses an api.
The source code of the api that it is accessing is: https://github.com/octavi42/smartHomeSystemApi
It listens for data changes and if a specific variable has changed the lock module deactivates.
