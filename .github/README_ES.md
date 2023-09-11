# Módulo de experiencia individual

[English](README.md) | [Español](README_ES.md)

- Permite a los jugadores Visualizar, Asignar or Activar/ Desactivar los rates de experiencia individual.

## Como instalar

1. Clonar el modulo dentro de la carpeta **mods** del core
2. Aplicar los archivos SQL que hacen parte de este módulo.
3. Correr Cmake
4. Compilar.

## Configuración 

Existen dos variables de configuración para este módulo:

1. Max XP Rate
2. Default XP Rate

La variable "IndividualXP.MaxRate" permite modificar el tope maximo de rates permitido por el módulo.
La variable "IndividualXP.DefaultRate" permite asignar el valor por defecto con el que todo jugador inicia sus rates cuando se instala por primera vez el módulo.

## Comandos disponibles

| Command     | Description                                                  |
|-------------|--------------------------------------------------------------|
| .XP         | Comando principal                                            |
| .XP View    | Muestra los rates de experiencia actuales                    |
| .XP Set #   | Cambia el rate de experiencia al seleccionado                |
| .XP Default | Reinicia los rates de experiencia al valor original          |
| .XP Disable | Desactiva toda la experiencia ganada hasta usar .xp enable   |
| .XP Enable  | Activa la experincia ganada depsues de haber deshabilitado   |
