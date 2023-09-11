# Individual XP Module

[English](README.md) | [Español (Soon...)]()

- Allows players to View, Set, or Toggle the rate at which they gain experience individually.

## How to install

1. clone this module into the modules directory of the main source
2. apply the provided sql in the character database
3. re-run cmake
4. compile.

## Configuration 

There are two variables to configure in the Config:

1. Max XP Rate
2. Default XP Rate

The Max XP Rate variable dictates how high a player can set their XP rate.
While the Default XP Rate variable dictates what XP rate players start with and the rate will be set to if the user does '.XP Default'

## Player Commands

| Command     | Description                                       |
|-------------|---------------------------------------------------|
| .XP         | Main Module Command                               |
| .XP View    | Displays the current XP rate                      |
| .XP Set #   | Changes the XP rate to the value specified        |
| .XP Default | Returns the XP rate to the default value          |
| .XP Disable | Disables all XP gain until user does '.XP Enable' |
| .XP Enable  | Enables all XP gain if it was disabled            |
