# Individual XP Module

[English](README.md) | [Español](README_ES.md)

- Allows players to View, Set, or Toggle the rate at which they gain experience individually.

## How to install

1. clone this module into the **mods** directory of the main source
2. apply the provided sql in the character database
3. re-run cmake
4. compile.

## Configuration 

There are two variables to configure in the Config:

1. Max XP Rate
2. Default XP Rate

The "IndividualXP.MaxRate" variable dictates how high a player can set their XP rate.
While the "IndividualXP.DefaultRate" variable dictates what XP rate players start with and the rate will be set to if the user does '.XP Default'

## Player Commands

| Command     | Description                                       |
|-------------|---------------------------------------------------|
| .xp         | Main Module Command                               |
| .xp view    | Displays the current XP rate                      |
| .xp set #   | Changes the XP rate to the value specified        |
| .xp default | Returns the XP rate to the default value          |
| .xp disable | Disables all XP gain until user does '.xp enable' |
| .xp enable  | Enables all XP gain if it was disabled            |
