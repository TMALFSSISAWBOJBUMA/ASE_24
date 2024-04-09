# Setup
Clone this repository using `git clone` command

```git clone https://github.com/TMALFSSISAWBOJBUMA/ASE_24.git [path]```
or use Github Desktop app.

## Install ESP-IDF extension in VS Code.
[Installation tutorial](https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/install.md)

Open VS Code and open the Extensions tab (`Ctrl+Shift+X`), search for ESP-IDF and install it.
A setup window should open, if not run *ESP-IDF: Configure ESP-IDF Extension* command (`Ctrl+Shit+P` to open command window).

Choose *Express Setup* and choose v5.2.1(release version) as IDF version. Change directories if desired and press *Install*.

# Programming
## Writing code
[FreeRTOS basics](https://www.freertos.org/implementation/a00002.html) -
[Tasks](https://www.freertos.org/implementing-a-FreeRTOS-task.html) - [Components library](https://github.com/UncleRus/esp-idf-lib/tree/master/components)

Before commiting format the files!

## Firmware download
 On the bottom bar choose the correct COM port and run *Build, Flash and Monitor* command. For the flash method choose *UART*.