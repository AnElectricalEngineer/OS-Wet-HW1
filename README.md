# Smash
 
Smash (**Sma**ll **Sh**ell) is a new, basic shell for GNU/Linux operating systems. 

## Using Smash

Smash can execute both built-in commands, such as 'cd' or 'pwd' and external commands. For a full list of built-in commands, see the attached PDF. External commands can be executed either in the foreground by using `command [arguments]` or in the background by using `command [arguments] &`. In addition to functioning as a (basic) shell, Smash also prints to standard output a message of the form: "signal SIGNAL_NAME was sent to pid PID" whenever a signal is sent.

## Screenshot

![image](https://user-images.githubusercontent.com/47844685/130352460-629080e3-5035-466b-9ed2-1761ee457009.png)

## Build from source

To build from source, first clone the repository. Then open a terminal in the directory into which you cloned the repository and run `make`.
